#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <math.h>
#include <float.h>
#include <sys/select.h>
#include <assert.h>

#define DEFAULT_PORT        "3100"
#define DEFAULT_META_PORT   "3101"
#define BACKLOG 10
#define BUF_LEN 80
#define MAX_REPLY_LEN 1000


// Socket server that accepts connections on two ports.
// Primary port executes commands that perform basic math/statistics & returns results to client
// Secondary port gives Meta statistics to the clientabout how many primary commands were executed during the 
// server's lifetime up until that point.



#define handle_error(msg) \
           do { perror(msg); exit(-1); } while (0)

double compute_stddev( int count, double sum, double sumsqrd) {
   // computes standard deviation and writes it to store
    double countN = (double)count;
    double result = (double)sqrt( (1/(countN-1)) * (sumsqrd - ((sum*sum) / (countN))) );
    return result;
}

void load_msg(char buffer[], char* msg, int msglen) {
    // clear buffer
    memset(buffer,0,BUF_LEN);
    // write msg into buffer[]
    for (int i=0; i<msglen; i++) {
        buffer[i] = msg[i];
    }
} 


int main(int argc, char* argv[])
{
    char* port      = DEFAULT_PORT;
    char* meta_port = DEFAULT_META_PORT;
    if(argc > 1)
        port = argv[1];
    if(argc > 2)
        meta_port = argv[2];

    // prepare hints for getaddrinfo
    struct addrinfo hints;
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;  // Use wildcard IP address

    // PRIMARY PORT CREATION: 

    // get server address info
    struct addrinfo *servinfo;
    int res = getaddrinfo(NULL, port, &hints, &servinfo);
    if(res != 0)
        handle_error("server: getaddrinfo error");
    
    // create socket
    int listen_fd = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol);
    if(listen_fd == -1)
        handle_error("server: socket error");

    // set socket options to allow port reuse
    int yes = 1;
    res = setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));
    if(res == -1)
        handle_error("server: setsockopt error");

    // bind socket to server address
    res = bind(listen_fd, servinfo->ai_addr, servinfo->ai_addrlen);
    if(res == -1)
        handle_error("server: bind error");


    // SECONDARY (META) PORT CREATION:

    // clear struct from primary socket's info
    freeaddrinfo(servinfo);

    // get server address info
    res = getaddrinfo(NULL, meta_port, &hints, &servinfo);
    if(res != 0)
        handle_error("server: getaddrinfo error");

    // create secondary (meta) socket
    int meta_listen_fd = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol);
    if(listen_fd == -1)
        handle_error("server: socket error");

    // set socket options to allow port reuse (for meta)
    yes = 1;
    res = setsockopt(meta_listen_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));
    if(res == -1)
        handle_error("server: setsockopt error");

    // bind (meta) socket to server address
    res = bind(meta_listen_fd, servinfo->ai_addr, servinfo->ai_addrlen);
    if(res == -1)
        handle_error("server: bind error");

    // cleanup
    freeaddrinfo(servinfo); // done with this struct

    // START LISTENING FOR CONNECTIONS

    // prepare socket for accepting incoming connections
    res = listen(listen_fd, BACKLOG);
    if(res == -1)
        handle_error("server: listen error");

    // prepare (meta) socket for accepting incoming connections
    res = listen(meta_listen_fd, BACKLOG);
    if(res == -1)
        handle_error("server: listen error");
    
    // For multiplexed listening:
    // Initialize the file descriptor set; must be done in each iteration since select changes it in place
    fd_set set;
    FD_ZERO(&set);  

    // create pipe for communicating meta statistics
    int pd[2]; //pd[0] = read, pd[1] = write
    pipe(pd);

    // determine bigger FD
    int max;
    if (pd[0] > meta_listen_fd && pd[0] > listen_fd) { max=pd[0]; }
    else if (listen_fd > pd[0] && listen_fd > meta_listen_fd) { max=listen_fd; }
    else { max=meta_listen_fd; } 

    // init metastatistics
    double curr_mean=0;
    int mean_count=0;
    double mean_sum=0;
    double mean_of_means=0;
    double means_sqrd=0;
    double means_stddev=0;
    char meta_buf[BUF_LEN];
    // write default msg with 0s
    mean_sum+=curr_mean; 
    means_sqrd+=(curr_mean*curr_mean); 
    mean_of_means = (double)mean_sum/mean_count;
    means_stddev = compute_stddev( mean_count, mean_sum, means_sqrd );
    sprintf(meta_buf, "means_count %d mean_of_means %.2lf stddev_of_means %.2lf\n", mean_count, mean_of_means, means_stddev);

    // main server loop
    while (1) 
    {
        // accept incoming connections from both ports with select()
        FD_ZERO(&set); // clear set
        FD_SET(listen_fd, &set); 
        FD_SET(meta_listen_fd, &set);
        FD_SET(pd[0], &set);
        // wait for input on any of the file descriptors 
        select(max+1, &set, NULL, NULL, NULL);

        // find fd where input is coming from
        int comm_fd = -1;
        int is_meta = 0; // Inidcated if incoming conn is on secondary port
        if (FD_ISSET(listen_fd, &set)) { comm_fd = accept(listen_fd, NULL, NULL); }
        else if (FD_ISSET(meta_listen_fd, &set)) { comm_fd = accept(meta_listen_fd, NULL, NULL); is_meta=1; }
        else if (FD_ISSET(pd[0], &set)) {
            printf("Reading pipe\n"); 
            read(pd[0], &curr_mean, sizeof(double)); 
            mean_count++;
            mean_sum+=curr_mean; 
            means_sqrd+=(curr_mean*curr_mean); 
            mean_of_means = (double)mean_sum/mean_count;
            means_stddev = compute_stddev( mean_count, mean_sum, means_sqrd );
            sprintf(meta_buf, "means_count %d mean_of_means %.2lf stddev_of_means %.2lf\n", mean_count, mean_of_means, means_stddev);
            continue;
         }
        if(comm_fd == -1)
	       handle_error("server: accept error");

        fprintf(stderr, "Accepted connection, communicating using FD# %d\n", comm_fd);
        // if (is_meta) { printf("Meta connection\n"); } else { printf("Primary connection\n"); }

        if (is_meta) { // If secondary conn, send single msg and exit
            printf("Sending metadata\n");
            
            int sent = 0;
            while (sent < strlen(meta_buf)) {
                int r = send(comm_fd, meta_buf + sent, strlen(meta_buf) - sent, 0);
                if (r < 0) handle_error("Sending error");
                sent += r;
            }
        } else { // If primary conn, fork
            pid_t pid = fork();
            if(pid == 0) { // child
                close(listen_fd);
                close(meta_listen_fd);
                close(pd[0]);

                char client_str[BUF_LEN]; // init buffer to hold client response
                char server_resp[BUF_LEN]; // init buffer to hold server's response to client

                // init vars for holding client data
                int count = 0;
                double sum = 0.00;
                double sumsqrd = 0.00;
                double mean= 0.00; 
                double stddev = 0.00; 

                // When the server accepts a connection it should send the following greeting message to the client: Stats server ready
                load_msg(server_resp, "Stats server ready\n", strlen("Stats server ready\n"));
                int sent = 0;
                while (sent < strlen(server_resp)) {
                    int r = send(comm_fd, server_resp + sent, strlen(server_resp) - sent, 0);
                    if (r < 0) handle_error("Sending error");
                    sent += r;
                }

                // Enter infinte loop of communication
                while(1)
                {   
                    // Clear buffers before using
                    memset(client_str,0,strlen(client_str));
                    memset(server_resp,0,strlen(server_resp));

                    // receive client command into read buffer
                    int received = 0; 
                    while (received < BUF_LEN) {
                        int r = recv(comm_fd, client_str + received, BUF_LEN - received, 0);
                        if (r < 0) handle_error("Error Receiving bytes");
                        received += r;
                        // If '\0' received, break loop
                        if ( strchr(client_str, '\0') != NULL ) { break; }
                        //Indicates nothing being received from client anymore
                        if ( received==0 ) { break; } 
                    }

                    // If a longer line is received then the server should ignore that line and send back the message - Error: long line
                    if ( strchr(client_str, '\n') == NULL ) { // Check this by seeing if '\n' in buffer currently. If not:

                        // clear the read buffer
                        memset(client_str,0,strlen(client_str));

                        // read out rest of too long message 
                        while ( strchr(client_str, '\n') == NULL ) { recv(comm_fd, client_str, BUF_LEN, 0); } 

                        // send error msg to client
                        load_msg(server_resp, "Error: long line\n", strlen("Error: long line\n"));
                        int sent = 0; 
                        while (sent < strlen(server_resp)) {
                            int r = send(comm_fd, server_resp + sent, strlen(server_resp) - sent, 0);
                            if (r < 0) handle_error("Error Sedning bytes");
                            sent += r;
                        }
                    } else { //If msg fits correctly:

                        // debugging
                        // printf("Received: %s", client_str);

                        double user_float; // holds user-inputted float
                        int exit_time = 0; // triggered when exit command is received
                        int send_resp = 1; // triggered when user sends float, allows server to send empty string to client

                        // Pull out user's command/input data
                        // count  - the server sends back the number of floating point values received from the client
                        if ( strstr(client_str, "count") != NULL ) {
                            char prep[BUF_LEN];
                            sprintf(prep, "%d\n", count);
                            load_msg(server_resp, prep, strlen(prep));
                        } 
                        // sum - the server sends back the sum of the floating point values received from the client
                        else if ( strstr(client_str, "sum") != NULL ) {
                            char prep[BUF_LEN];
                            sprintf(prep, "%.2lf\n", sum);
                            load_msg(server_resp, prep, strlen(prep));
                        }
                        // mean - the server sends back the mean of the floating point values received from the client
                        else if ( strstr(client_str, "mean") != NULL ) {
                            char prep[BUF_LEN];
                            sprintf(prep, "%.2lf\n", mean);
                            load_msg(server_resp, prep, strlen(prep));
                        }
                        // stddev - the server sends back the standard deviation of the floating point values received from the client
                        else if ( strstr(client_str, "stddev") != NULL ) {
                            char prep[BUF_LEN];
                            sprintf(prep, "%.2lf\n", stddev);
                            load_msg(server_resp, prep, strlen(prep));
                        }
                        // exit - the server sends back a line with the final count, mean, and standard deviation, in the format seen in the examples below.
                        else if ( strstr(client_str, "exit") != NULL ) {
                            // send mean for meta statistics
                            write(pd[1], &mean, sizeof(double));
                            exit_time=1;
                            char prep[BUF_LEN];
                            sprintf(prep, "EXIT STATS: count %d mean %.2lf stddev %.2lf\n", count, mean, stddev);
                            load_msg(server_resp, prep, strlen(prep));
                        }
                        // user inputs single float value
                        else if ( sscanf(client_str,"%lf\n", &user_float) == 1) {
                            // re-compute all values upon receiving new float 
                            count++;
                            sum += user_float;
                            mean = sum/(double)count;
                            sumsqrd += user_float*user_float;
                            stddev = compute_stddev( count, sum, sumsqrd );
                            send_resp = 0;
                        }
                        // if not valid input the server must send to the client the message - Error: unrecognized command
                        else {
                            load_msg(server_resp, "Error: unrecognized command\n", strlen("Error: unrecognized command\n"));
                        }

                        // Send client response back to client
                        if (send_resp) {
                            int sent = 0; 
                            while (sent < strlen(server_resp)) {
                                int r = send(comm_fd, server_resp + sent, strlen(server_resp) - sent, 0);
                                if (r < 0) handle_error("Error Sedning bytes");
                                sent += r;
                            }
                        }
                        // break if exit command was called
                        if (exit_time) { break; }
                }
            }
            close(comm_fd);
            exit(0);
        }
        else { // parent
            fprintf(stderr, "Forked %d\n", pid);
            close(comm_fd);

            // reap dead children, if any
            int status;
            pid_t deadChild;
            do {
                deadChild = waitpid(-1, &status, WNOHANG);
                if(deadChild == -1)
                    handle_error("server: waitpid");
                if(deadChild > 0)
                    fprintf(stderr, "Reaped child with PID %d\n", deadChild);
            } while(deadChild > 0);
        }

        }
        close(comm_fd);
    }
   exit(0);
}

