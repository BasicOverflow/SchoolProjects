#include <stdio.h>
#include <ctype.h>
#include <string.h>

/* Input:
 *    NUL terminated string 'phrase' which consists of lower-case letters,
 *    spaces, and punctuation marks
 *
 * Return value:
 *    1 if 'phrase' is an isogram, and 0 otherwise.
 */

// prev count; count cuur character in phrase
// while true:
    // calc count of next char, if this cound and prev count are different, break  

int is_isogram( char phrase[] ) {
    int p_len = strlen(phrase);
    int prev_count = 0;
    char curr_char;
    // get initial count for first char
    curr_char = phrase[0];
    for (int i=0;i<p_len; i++) { if (phrase[i]==curr_char) prev_count++;}
    
    // Loop through rest of chars, get and compare their counts
    for (int j=1; j<p_len; j++) {
        int curr_count = 0;
        char curr_char = phrase[j];

        // get count
        for (int k=0;k<p_len; k++) { if (phrase[k]==curr_char) curr_count++;}
        
        // Compare counts
        if (curr_count != prev_count) {
            return 0;
        } else { prev_count = curr_count; }

    } return 1;
}



#define MAXLEN 1000

int main( void )
{
  int i;

  char buff[MAXLEN], phrase[MAXLEN];

  fgets(buff, sizeof buff, stdin);         // reads at most MAXLEN-1 chars to avoid buffer overflow

  for(i=0; buff[i] != '\n' && buff[i] != '\0'; i++) {
    if( isupper(buff[i]) )
      phrase[i] = tolower( buff[i] ); // convert uppercase letters to lowercase
    else
      phrase[i] = buff[i];            // copy everything else unchanged
  }

  phrase[i] = '\0';                   // add NUL terminator

  if( is_isogram(phrase) )
    printf("isogram\n");
  else
    printf("not isogram\n");

  return 0;
}