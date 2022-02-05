#include <stdio.h>
#include <ctype.h>
#include <string.h>

/* Input:
 *    s - NUL terminated string consisting of lower-case letters
 *    b - non-negative integer base
 *
 * Return:
 *    unsigned int polynomial hash code
 *
 *       hash_code(s) = d[0] + d[1]*b + ... + d[n-1]*b^{n-1}
 *
 *    where n is the number of letters in s and d[i] is the position
 *    of letter s[i] in the English alphabet ('a' has position 1,
 *    'b' has position 2, and so on).  Overflows, if any, are ignored.
 */

int expn(unsigned int a, int pow) {
    int res = 1;
    int i = 0;
    while (i!=pow) {
        res *= a;
        i++;
    }
    return res;
}

int return_alpha_number(char a) {
    char d[] = {'0','a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z'};
    int d_len = sizeof(d) / sizeof(char); // Obtain number of elements in array: https://stackoverflow.com/questions/37538/how-do-i-determine-the-size-of-my-array-in-c
    int ans;
    // printf("%d\n", s_len);
    for (int i=0; i<d_len; i++) {
        if (d[i] == a) ans = i;
    }
    return ans;
}

unsigned int hash_code( char s[], unsigned int b )
{
    // int s_len = sizeof(s) / sizeof(char); 
    int s_len = strlen(s); // for some reason the above method for getting # of elements in the array doesnt work here, using this instead: http://www.cplusplus.com/reference/cstring/strlen/
    int sum = 0;
    for (int i=0; i<s_len; i++) {
        sum = sum + (return_alpha_number(s[i]) * (expn(b, i)));
    }
    return sum;
}


#define MAXLEN 1000

int main()
{
   char buff[MAXLEN], s[MAXLEN];
   unsigned int b;
   int i, j;

   fgets(buff, sizeof buff, stdin);         // reads at most MAXLEN-1 chars to avoid buffer overflow

   for(i=j=0; buff[i] != '\n' && buff[i] != '\0'; i++) {
      if( isupper(buff[i]) )
          s[j++] = tolower( buff[i] ); // convert uppercase letters to lowercase
      else if( islower(buff[i]) )
          s[j++] = buff[i];
      else ;                           // ignore non-letters
   }
   s[j] = '\0';                   // add NUL terminator

   scanf("%u", &b);               // read the value of the base

   printf("hash_code(%s)=%u\n", s, hash_code(s, b) );

   return 0;
}

