#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

/* 
 *  Compute Pascal's triangle up to a given number of rows
 *  Input:
 *      Number of triangle rows to be computed
 *  Return:
 *      Pointer to a dynamically allocated array of 'rows' pointers to ints
 *      The i-th pointer in this array should point to a dynamically allocated array of
 *      i+1 integers, which should be filled in with the integers in the i-th row of 
 *      Pascal's triangle (where rows are numbered from 0)
 */

int** pascal(int rows) {
    // Allocate rows
    int **pointer_rows, i, j;
    pointer_rows = malloc(sizeof(int *) * rows);

    // Allocate number of elements for each row in Pascal's triangle
    for (i=0; i<rows; i++) {
        pointer_rows[i] = malloc(sizeof(int) * (i+1));
        // insert 1's in appropriate spots
        pointer_rows[i][0] = 1;
        pointer_rows[i][i] = 1;
    }
    // Calculate rest of values 
    for (i=2; i<rows; i++) { 
        int *curr_row = pointer_rows[i];
        int *prev_row = pointer_rows[i-1];
        // Go through values in each row
        for (j=1; j<i; j++) {
            curr_row[j] = prev_row[j] + prev_row[j-1];
            // printf("%d = %d + %d\n", curr_row[j], prev_row[j], prev_row[j-1]);
        }
    }
    return pointer_rows;
}

/* 
 *  Free dynamically allocated memory
 *  Input:
 *      A pointer to a pointer to an int returned by calling the pascal() function
 *      The 'rows' argument used when calling the pascal() function
 *  Return:
 *      None
 *  Side effect:
 *      The function should free all memory allocated by the pascal() function 
 *      that returned the given 'triangle' pointer
 */

void free_memory( int** triangle, int rows ) {
   for (int i=0; i<rows; i++) {
       free(triangle[i]);
   } 
   free(triangle);
}


// DO NOT MODIFY CODE BELOW

/*
 * compute polynomial hash code with base 257 for a given array of unsigned chars
 */
#define PRIME_BASE 257u
unsigned int hash( unsigned char s[], int size )
{  
   unsigned int result = 0;
   
   // compute polynomial hash code using Horner's rule
   for( int i=size-1; i>=0; i-- )
       result = s[i] + PRIME_BASE * result;
   
   return result;
}

int main(void)
{
   int rows;
   scanf("%d", &rows);
   if(rows<0) {
      fprintf(stderr, "Number of rows should be positive!\n");
      return -1;
   }

   int** triangle = pascal(rows);

   if(rows <= 5) {
      for(int i=0; i<rows; i++) {
         for(int k=0; k< rows-i; k++)
            printf(" ");
         for(int j=0; j<i; j++)
            printf("%1d ", triangle[i][j]);
         printf("%1d\n", triangle[i][i]);
      }
   }
   else if (rows <= 10 ) {
      printf("Row checksums: ");
      for(int i=0; i<rows; i++)
         printf("%u ", hash( (unsigned char*) triangle[i], (i+1)*sizeof(int) ) );
      printf("\n");
   }
   else {
      unsigned int checksum[rows];
      for(int i=0; i<rows; i++)
         checksum[i] = hash( (unsigned char*) triangle[i], (i+1)*sizeof(int) );
      
      printf("Checksum of row checksums: %u\n", hash( (unsigned char*) checksum, rows*sizeof(unsigned int) ) );
   }

   // free the triangle
   free_memory( triangle, rows );

   return 0;
}