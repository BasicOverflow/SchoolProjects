#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#define DEFAULT_START_PORT 1
#define DEFAULT_END_PORT   100
#define DEFAULT_HOST       "localhost"
#define BUF_LEN            100

#define handle_error(msg) \
           do { perror(msg); exit(-1); } while (0)


int isValidResponse(char serverResposne[]) {
    // check if response begins with: `HTTP`
    if ( strstr(serverResposne, "HTTP") != NULL ) { return 1; }
    else { return 0; }
}

int main(int argc, char* argv[])
{
    char* hostname;
    int start_port;
    int end_port;

    // Find out host and port range to scan
    if (argc < 3) { 
        hostname = DEFAULT_HOST;
        start_port = DEFAULT_START_PORT;
        end_port = DEFAULT_END_PORT;
    } else {
        hostname = argv[1];
        start_port = atoi(argv[2]);
        end_port = atoi(argv[3]);
    }

    // prepare hints for getaddrinfo
    struct addrinfo hints;
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    // Used for stablishing connection/communication to server
    struct addrinfo *servinfo;
    int res;
    int sock_fd;
    char servermsg[28] = "GET /index.html HTTP/1.0\r\n\r\n";

    // loop through each port, attempt connections
    for (int portint=start_port; portint<end_port+1; portint++) {
        // Convert port to type string
        char port[6];
        sprintf(port, "%d", portint);
        // printf("port: %s\n", port);

        // get server address info
        res = getaddrinfo(hostname, port, &hints, &servinfo);
        if(res != 0) { handle_error("client: getaddrinfo error"); }

        // create socket
        sock_fd = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol);
        if(sock_fd == -1) { handle_error("client: socket error"); } 

        // connect to the server 
        res = connect(sock_fd, servinfo->ai_addr, servinfo->ai_addrlen);
        if(res == -1) { close(sock_fd); freeaddrinfo(servinfo); continue; } //Port not active at all, skip it
        else {
            char recvline[BUF_LEN];
            memset(recvline, 0, sizeof(recvline)); // clear buffer
            
            // write message to the socket
            write(sock_fd, servermsg, sizeof(servermsg)); 
            shutdown(sock_fd, SHUT_WR);

            // read and print server's response
            read(sock_fd, recvline, BUF_LEN);

            // Check if response is valid http response 
            if (isValidResponse(recvline)) { printf("found active HTTP port: %s\n", port); }
            else { printf("found active non-HTTP port: %s\n", port); }      
            // close socket
            close(sock_fd);
            // cleanup
            freeaddrinfo(servinfo); // done with this struct 
        }      
    }
    exit(0);    
}
