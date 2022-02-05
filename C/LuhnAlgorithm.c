#include <stdio.h>

/* Input:
 *    Array 'digit' of 'len' integers between 0 and 9
 *
 * Return value:
 *    1 if the digits in the array pass LUHN-10 validation, 0 otherwise
 */

// 1.) The first step of the algorithm is to double every second digit, s
    // tarting from the right, and subtracting 9 if doubling results in a number greater than 9.

// 2.) If summing up the digits after the first step results in a number 
    // divisible by 10 then the original number is valid, otherwise it is not

int check_luhn(int digit[], int len)
{
  // Step 1 
    for (int i=0; i<len; i+=2) {
        // digit[i] *= 2;
        digit[i] = digit[i] * 2;
        if (digit[i] > 9) digit[i] = digit[i]-9;
  }
  // Step 2
    int sum = 0;
    for (int i=0; i<len; i++) {
        // printf("%d\n", digit[i]);
        sum += digit[i];
    }
  // printf("%d\n", sum);
    if (sum%10 == 0) { return 1; }
    return 0;
  
}


int main( void )
{
  int testlen = 16;
  int test[] = {4,5,6,3,9,6,0,1,2,2,0,0,1,9,9,9};
  

  int result = check_luhn(test, testlen);
  if (result) { printf("Valid\n"); } else { printf("Invalid\n"); }
}