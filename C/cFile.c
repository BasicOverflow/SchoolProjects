#define _DEFAULT_SOURCE
#include <stdio.h>
#include <stdlib.h>

// play around with FILE api

void print_lines_from_file(FILE* f, char* str) {
    // printf("%lu\n", ftell(f));
    int i, c, count;

    while (ftell(f) > 1) { 
        //Step back two bytes
        fseek(f, -2, SEEK_CUR);
        if (ftell(f) <= 2) { break; }
        // Read back one of those bytes
        c = fgetc(f);

        count = 0;
        do {
            str[count++] = c;
            if (ftell(f) < 2) { break; }
            //Step back two bytes
            fseek(f, -2, SEEK_CUR);
            // Read back one of those bytes
            c = fgetc(f);
        } while(c != '\n');

        // Print line
        for (i=count-1; i>=0 && count>0; i--) { printf("%c", str[i]); }  
        printf("\n");
    }
    fclose(f);
}

void print_lines_stdin(FILE* f) {
    char** lines = malloc(sizeof(char*)); //Initialize array of pointers to size of one pointer
    lines[0] = malloc(sizeof(char)*255); //Pointer for one line with max size of 255
    int lines_i = 0; // Keep track of current line
    int line_i = 0; //Keep track of current position in current line
    int c;

    do {
        c = fgetc(f);

        if (c == '\n') { 
            line_i=0; 
            lines = realloc(lines, sizeof(char)*255*(lines_i+1));
            lines[++lines_i] = malloc(sizeof(char)*255);
            lines[lines_i-1][line_i+1] = '\n';
        } else {
            lines[lines_i][line_i++] = c; 
        }        
    } while (c != EOF);

     for (int j=lines_i; j>-1; j--) { 
        if (lines[j][0] == '\xff') { 
            // Get rid of that character, which is in 0'th index of curr string, shift rest of characters
            for (int i=0; i<line_i; i++) { lines[j][i] = lines[j][i+1]; }
            printf("%s", lines[j]);
            }
        printf("%s", lines[j]);
        // for (int i=0; i<255; i++) { printf("%c", lines[j][i]); }
     }
    free(lines);
}
 

int main(int argc, char *argv[])
{
    char *str = malloc(sizeof(char)*255);
    FILE *f;

    if (argc > 1) {
        f = fopen(argv[1], "r");
        fseek(f, 1, SEEK_END);
        print_lines_from_file(f, str);
    } 
    else {
        print_lines_stdin(stdin);
    }
    free(str);
}
