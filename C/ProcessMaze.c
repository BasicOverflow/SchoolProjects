#define _POSIX_SOURCE
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/select.h>
#include <signal.h>
#include <stdlib.h>
#include "maze.h"

// Exercise for creating processes & establishing communication w/ pipes
// Given an input file, create a conceptual 'maze' of cells where each cell
// is a child process. Input file also specifies what cells are connected
// to which surrounding ones and in what cardinal direction. The build_maze() function
// reads the input file and creates all maze cell processes, as well as 
// set up communication between them & the parent process

// all additional macros/datatypes/functions are omitted


void initialize_pipes(FILE* maze_file, int pipe_arr[], int num_cells) {
   // Goes through maze file and creates pipes for all cells in appropriate cardinal directions, 
   // Stores them in flat array in the following format:
      // [inpipe,outpipe, inpipe,outpipe, inpipe,outpipe, inpipe,outpipe, ...]
      // [north_end1,north_end2, south1,south2, east1,east2, west1,west2, ...]
      //                         ^represents 1 cell^

   int curr_cell, north_cell, south_cell, east_cell, west_cell; 
   int curr_to_other[2], other_to_curr[2];
   int curr_cell_i;
   
   while (feof(maze_file) == 0) {
      // store contents of line in vars
      fscanf(maze_file, "%d %d %d %d %d\n", &curr_cell, &north_cell, &south_cell, &east_cell, &west_cell);
      // north1[0], north2[1], south1[2], south2[3], east1[4], east2[5], west1[6], west[7]

      // other_to_curr[0] == input
      // curr_to_other[1] == output

      // curr_to_other[0] == input
      // other_to_curr[1] == output
      
      if (north_cell != -1) {
         curr_cell_i = curr_cell*8;
         if (pipe_arr[curr_cell_i] == -1) {
            pipe(curr_to_other);
            pipe(other_to_curr);
            pipe_arr[curr_cell_i] = other_to_curr[0];
            pipe_arr[++curr_cell_i] = curr_to_other[1];
            // go to other cell's south fields and configure its pipes
            curr_cell_i = north_cell*8+2;
            pipe_arr[curr_cell_i] = curr_to_other[0];
            pipe_arr[++curr_cell_i] = other_to_curr[1]; 
         }
      }

      if (south_cell != -1) {
         curr_cell_i = curr_cell*8 + 2;
         if (pipe_arr[curr_cell_i] == -1) { 
            pipe(curr_to_other);
            pipe(other_to_curr);
            pipe_arr[curr_cell_i] = other_to_curr[0];
            pipe_arr[++curr_cell_i] = curr_to_other[1];
            // go to other cell's north fields and configure its pipes
            curr_cell_i = south_cell*8;
            pipe_arr[curr_cell_i] = curr_to_other[0];
            pipe_arr[++curr_cell_i] = other_to_curr[1];
         }
      }

      if (east_cell != -1) {
         curr_cell_i = curr_cell*8 + 4;
         if (pipe_arr[curr_cell_i] == -1) {
            pipe(curr_to_other);
            pipe(other_to_curr);
            pipe_arr[curr_cell_i] = other_to_curr[0];
            pipe_arr[++curr_cell_i] = curr_to_other[1];
            // go to other cell's west fields and configure its pipes
            curr_cell_i = east_cell*8+6;
            pipe_arr[curr_cell_i] = curr_to_other[0];
            pipe_arr[++curr_cell_i] = other_to_curr[1];
         }
      }

      if (west_cell != -1) {
         curr_cell_i = curr_cell*8 + 6;
         if (pipe_arr[curr_cell_i] == -1) {
            pipe(curr_to_other);
            pipe(other_to_curr);
            pipe_arr[curr_cell_i] = other_to_curr[0];
            pipe_arr[++curr_cell_i] = curr_to_other[1];
            // go to other cell's east fields and configure its pipes
            curr_cell_i = west_cell*8+4;
            pipe_arr[curr_cell_i] = curr_to_other[0];
            pipe_arr[++curr_cell_i] = other_to_curr[1];
         }
      }
   }
   // Reset file pointer to where it was before func call
   fseek(maze_file, 0, SEEK_SET); 
   char junk[18];
   fgets(junk, 18, maze_file); fgets(junk, 18, maze_file);
   fgets(junk, 18, maze_file); fgets(junk, 18, maze_file);
} 



int build_maze( FILE* maze_file, char* cell_executable, int num_cells, int start_cell, int treasure_cell, char* treasure_amount,
                int to_start[2],
                int from_start[2],
                pid_t* pid)
{
   // [input,output, input,output, input,output, ...]
   int pipe_arr[num_cells*8]; for (int i=0; i<num_cells*8; i++) {pipe_arr[i]=-1;} //init all values to -1
   // Create pipes and store them in pipe array
   initialize_pipes(maze_file, pipe_arr, num_cells);
   // for (int i=0; i<num_cells*8; i++) { printf("%d\t", pipe_arr[i]); } //debugging

   int curr_cell, north_cell, south_cell, east_cell, west_cell; 

   // Loop through file again, create child cells
   while (feof(maze_file) == 0) {
      fscanf(maze_file, "%d %d %d %d %d\n", &curr_cell, &north_cell, &south_cell, &east_cell, &west_cell);
      // printf("%d %d %d %d %d\n", curr_cell, north_cell, south_cell, east_cell, west_cell);

      // Source Cell
      if (curr_cell == start_cell) {
         // fork
         pid_t cell_pid = fork();
         // printf("fork source");
         if (cell_pid == 0) {   // In child

            // Configure parent pipes
            dup2(from_start[1], 1); // from_start write end becomes stdout
            dup2(to_start[0], 0); // to_start read end becomes stdin
            // Close unused parent pipes
            close(to_start[1]);
            close(from_start[0]);

            // Configure cardinal pipes
            if (north_cell != -1) {
               dup2(pipe_arr[curr_cell*8], INPIPE_FD(NORTH)); // other_to_curr[0]
               dup2(pipe_arr[curr_cell*8+1], OUTPIPE_FD(NORTH)); // curr_to_other[1]
               // Close unused pipes: north cell's south
               close(pipe_arr[north_cell*8+2]); // other_to_curr[1]
               close(pipe_arr[north_cell*8+3]); // curr_to_other[0]
            } 
            if (south_cell != -1) {
               dup2(pipe_arr[curr_cell*8+2], INPIPE_FD(SOUTH)); // other_to_curr[0]
               dup2(pipe_arr[curr_cell*8+3], OUTPIPE_FD(SOUTH)); // curr_to_other[1]
               // Close unused pipes: south cell's north
               close(pipe_arr[south_cell*8]); // other_to_curr[1]
               close(pipe_arr[south_cell*8+1]); // curr_to_other[0]
            }
            if (east_cell != -1) {
               dup2(pipe_arr[curr_cell*8+4], INPIPE_FD(EAST)); // other_to_curr[0]
               dup2(pipe_arr[curr_cell*8+5], OUTPIPE_FD(EAST)); // curr_to_other[1]
               // Close unused pipes: east cell's west
               close(pipe_arr[east_cell*8+6]); // other_to_curr[1]
               close(pipe_arr[east_cell*8+7]); // curr_to_other[0]
            }
            if (west_cell != -1) {
               dup2(pipe_arr[curr_cell*8+6], INPIPE_FD(WEST)); // other_to_curr[0]
               dup2(pipe_arr[curr_cell*8+7], OUTPIPE_FD(WEST)); // curr_to_other[1]
               // Close unused pipes: west cell's east
               close(pipe_arr[west_cell*8+4]); // other_to_curr[1]
               close(pipe_arr[west_cell*8+5]); // curr_to_other[0]
            }
                        
            // Close rest of pipes in pipe_arr
            if (north_cell == -1) { close(INPIPE_FD(NORTH)); close(OUTPIPE_FD(NORTH));}
            if (south_cell == -1) { close(INPIPE_FD(SOUTH)); close(OUTPIPE_FD(SOUTH));}
            if (east_cell == -1) { close(INPIPE_FD(EAST)); close(OUTPIPE_FD(EAST));}
            if (west_cell == -1) { close(INPIPE_FD(WEST)); close(OUTPIPE_FD(WEST));}
            
            // Upgrade process
            if (curr_cell == treasure_cell) {
               execl(cell_executable, cell_executable, treasure_amount, NULL);
            }
            execl(cell_executable, cell_executable, NULL);
            printf("Unable to Upgrade Child\n");
         }
         // Back in parent: close unused pipe ends
         if (curr_cell == start_cell)
         {
            close(to_start[0]);
            close(from_start[1]);
         }
         // Append process ID 
         pid[curr_cell] = cell_pid;
      } else {

         // Other Cells
         pid_t cell_pid = fork();
         // printf("fork other");
         if (cell_pid == 0) {  // In child
            // Close std in/out
            close(0); close(1);

            // Configure cardinal pipes
            if (north_cell != -1) {
               dup2(pipe_arr[curr_cell*8], INPIPE_FD(NORTH)); // other_to_curr[0]
               dup2(pipe_arr[curr_cell*8+1], OUTPIPE_FD(NORTH)); // curr_to_other[1]
               // Close unused pipes: north cell's south
               close(pipe_arr[north_cell*8+2]); // other_to_curr[1]
               close(pipe_arr[north_cell*8+3]); // curr_to_other[0]
            } 
            if (south_cell != -1) {
               dup2(pipe_arr[curr_cell*8+2], INPIPE_FD(SOUTH)); // other_to_curr[0]
               dup2(pipe_arr[curr_cell*8+3], OUTPIPE_FD(SOUTH)); // curr_to_other[1]
               // Close unused pipes: south cell's north
               close(pipe_arr[south_cell*8]); // other_to_curr[1]
               close(pipe_arr[south_cell*8+1]); // curr_to_other[0]
            }
            if (east_cell != -1) {
               dup2(pipe_arr[curr_cell*8+4], INPIPE_FD(EAST)); // other_to_curr[0]
               dup2(pipe_arr[curr_cell*8+5], OUTPIPE_FD(EAST)); // curr_to_other[1]
               // Close unused pipes: east cell's west
               close(pipe_arr[east_cell*8+6]); // other_to_curr[1]
               close(pipe_arr[east_cell*8+7]); // curr_to_other[0]
            }
            if (west_cell != -1) {
               dup2(pipe_arr[curr_cell*8+6], INPIPE_FD(WEST)); // other_to_curr[0]
               dup2(pipe_arr[curr_cell*8+7], OUTPIPE_FD(WEST)); // curr_to_other[1]
               // Close unused pipes: west cell's east
               close(pipe_arr[west_cell*8+4]); // other_to_curr[1]
               close(pipe_arr[west_cell*8+5]); // curr_to_other[0]
            }
                        
            // Close rest of pipes in pipe_arr
            if (north_cell == -1) { close(INPIPE_FD(NORTH)); close(OUTPIPE_FD(NORTH));}
            if (south_cell == -1) { close(INPIPE_FD(SOUTH)); close(OUTPIPE_FD(SOUTH));}
            if (east_cell == -1) { close(INPIPE_FD(EAST)); close(OUTPIPE_FD(EAST));}
            if (west_cell == -1) { close(INPIPE_FD(WEST)); close(OUTPIPE_FD(WEST));}

            // Upgrade process
            if (curr_cell == treasure_cell) {
               execl(cell_executable, cell_executable, treasure_amount, NULL);
            }
            execl(cell_executable, cell_executable, NULL);
            printf("Unable to Upgrade Child\n");
         }
         // Back in parent: close all pipes
         
         // Append process ID 
         pid[curr_cell] = cell_pid;
      }

   }   
   return 0;
}
