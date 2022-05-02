#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <ctype.h>
#include "maze.h"

/**
 * Given a starting location in a maze, check if a given sequence of
 * moves between horizontally or vertically adjacent cells forms a valid
 * escape route. To escape the maze, the route must visit each one of
 * the cells that hide the Potion, the Spellbook, and the Wand.
 * The route can only take steps in the four cardinal directions (N/S/E/W),
 * and you can't move in directions that don't exist in the maze.
 *
 * Input:
 *        Start maze cell
 *        NUL terminated string denoting the N/S/E/W moves
 * Return:
 *        the number (between 0 and 3) of magic objects along the path;
 *        -1 if the path is invalid
 */


int already_found(int item, int found_items[], int found_items_len) {
    // Checks to see if a given item has already been discovered
    for (int i=0; i<found_items_len; i++) {
        if (found_items[i] == item) return 1;
    } return 0;
}

int check_path(MazeCell* start, char* moves)
{
    int sum; // Holds amount of items found along given path
    int moves_i; // Keeps track of current index of moves
    int items_found[3]; 
    char curr_dir; // Holds current direction in the path, initialize at 0'th index
    MazeCell* curr_cell = start; // initialize current cell to starting cell
    // Loop through the path while checking cells along the way
    while (1) {
        // Break out of loop once entire path has been traversed
        if (moves_i == (strlen(moves)-1)) { break; } 
        // Grab current direction from path
        curr_dir = moves[moves_i]; 
        // Find the next cell in that direction
        MazeCell* next_cell; 
        if (curr_dir == 'N') next_cell = curr_cell -> N;
        else if (curr_dir == 'S') next_cell = curr_cell ->S;
        else if (curr_dir == 'E') next_cell = curr_cell -> E;
        else if (curr_dir == 'W') {next_cell = curr_cell -> W;}
        // if the next cell is NULL, return error code
        if (next_cell == NULL) { return -1; }
        // Check if the cell has an item, if so record it
        if (next_cell->whatsHere != NOTHING) {
            if (already_found(next_cell->whatsHere, items_found, sum) == 0) {
                items_found[sum++] = next_cell->whatsHere;
            }   
        } moves_i++;  //SEENWESSWENWWSSEE
    } 
    return sum;
}


// PROVIDED SKELETON CODE BELOW

MazeCell* ReadMaze(char* maze_file)
{
   int numRows, numCols;
   int row, col, content, nr, nc, sr, sc, er, ec, wr, wc, start_row, start_col;
   FILE* f_in = fopen(maze_file, "r");
   if(f_in == NULL) {
      printf("could not open %s", maze_file);
      return NULL;
   }

   fscanf(f_in, "size: %d %d\n", &numRows, &numCols);
   MazeCell*** maze = malloc(numRows*sizeof(MazeCell**));
   assert(maze != NULL);

   for(int i=0; i<numRows; i++) {
      maze[i] = malloc(numCols*sizeof(MazeCell*));
      assert(maze[i] != NULL);
   }
   for(int i=0; i<numRows; i++) {
      for(int j=0; j<numCols; j++) {
         maze[i][j] = malloc(sizeof(MazeCell));
         assert(maze[i][j] != NULL);
      }
   }
   for(int i=0; i<numRows; i++) {
      for(int j=0; j<numCols; j++) {
         fscanf(f_in, "%d %d %d %d %d %d %d %d %d %d %d\n",
               &row, &col, &content, &nr, &nc, &sr, &sc, &er, &ec, &wr, &wc);
         assert(row==i);
         assert(col==j);
         maze[i][j]->whatsHere = content;
         if(nr >= 0 && nr < numRows && nc >= 0 && nc < numCols)
            maze[i][j]->N = maze[nr][nc];
         if(sr >= 0 && sr < numRows && sc >= 0 && sc < numCols)
            maze[i][j]->S = maze[sr][sc];
         if(er >= 0 && er < numRows && ec >= 0 && ec < numCols)
            maze[i][j]->E = maze[er][ec];
         if(wr >= 0 && wr < numRows && wc >= 0 && wc < numCols)
            maze[i][j]->W = maze[wr][wc];
      }
   }

   fscanf(f_in, "start: %d %d\n", &start_row, &start_col);
   assert(start_row >= 0 && start_row < numRows && start_col >= 0 && start_col < numCols);

   MazeCell* start = maze[start_row][start_col];

   // cleanup
   for(int i=0; i<numRows; i++)
      free(maze[i]);
   free(maze);
   fclose(f_in);

   return start;
}


#define MAX_FILE_NAME_LEN 100
#define MAX_MOVES 100000

int main(int argc, char* argv[]) {
    char maze_file[MAX_FILE_NAME_LEN] = "maze1.txt";
    char moves[MAX_MOVES] = "SEENWESSWENWWSSEEE";

    if(argc > 1)
        strncpy(maze_file, argv[1], MAX_FILE_NAME_LEN);

    if(argc > 2)
        strncpy(moves, argv[2], MAX_FILE_NAME_LEN);

    MazeCell* startLocation = ReadMaze(maze_file);
    int code = check_path(startLocation, moves);

    if(code == 3)
        printf("The path is an escape route\n");
    else if(code < 0)
        printf("Invalid path\n");
    else
        printf("The path visits only %d magic items\n", code);

    return 0;
}
