#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <assert.h>
#include <string.h>

// Uses multiprocessing with fork() and interprocess communication to create 
// a child process, upgrade it to a live python interpreter, and establish bidirectional pipes
// with parent. User is then able to input numbers and receive their exponentiation from the
// python process.


int main(int argc, char ** argv)
{
   char *cmd = "python3";
   int status;

   // Create pipes for communication
   int parent_to_python[2];
   int python_to_parent[2];

   pipe(parent_to_python);
   pipe(python_to_parent);

   // Fork into child 
   pid_t pid = fork();

   if (pid == 0) { // In Child 'python'
      // child's stdin becomes parent_to_python read end
      dup2(parent_to_python[0], 0);

      // Close other unecessary descriptors
      close(parent_to_python[1]);
      close(parent_to_python[0]);
      
      // Child's stdout becomes python_to_parent write end
      dup2(python_to_parent[1], 1);

      // Close other unecessary descriptors
      close(python_to_parent[1]);
      close(python_to_parent[0]);

      //The stderr of the child python process should be redirected 
      // to the special file /dev/null so the user does not see the banner and prompt.
      int dev_ull = open("/dev/null",0);
      dup2(dev_ull, 2);

      // Upgrade child
      execl("/bin/python3", cmd, "-i", (char *)NULL);
      exit(-1); // terminate child if execv fails

   } else  {
      // Close unecessary pipe ends
      close(parent_to_python[0]);
      close(python_to_parent[1]);
   }
   // In Parent
   int userint1, userint2;
   char pythonout[1024];
   char userinpt[1024];
   
   while (1) {
      // Clear buffers
      memset(userinpt,0,sizeof(userinpt));
      memset(pythonout,0,sizeof(pythonout));

      // Gather input and format expression 
      if (scanf("%d %d", &userint1, &userint2) != 2) {break;}
      sprintf(userinpt, "(%d) ** (%d)\n", userint1, userint2);

      // Find amount of bytes in expression
      int inptt_sizee = strlen(userinpt); 
      
      // Write to parent_to_python write end
      write( parent_to_python[1], userinpt, inptt_sizee );   

      //Read results from child 'python' process
      int outt_sizee=0;
      for (int i=0; i<1024; i++) {
         if (pythonout[i] == '\n') {
            outt_sizee += 1;
            break;
         } else {
            outt_sizee += 1;
         }
      }
      read( python_to_parent[0], pythonout, outt_sizee );

      // Print Results 
      // dynamically allocate memory?
      char result[outt_sizee];
      sprintf(result, "(%d) ** (%d) = %s", userint1, userint2, pythonout);
      printf("%s", result);    
      // printf("(%d) ** (%d) = %s", userint1, userint2, pythonout);
   }

   close(parent_to_python[1]);
   close(python_to_parent[0]);
   
   waitpid(pid, &status, 0);
   status = 0;
   printf("Child exit Status: %d\n", status);
   
   return 0;
}


