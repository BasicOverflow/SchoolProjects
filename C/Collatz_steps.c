#include <stdio.h>


// Recursively go through each step, incrementing i at the same time 

// If n is even, divide n by 2 to get n / 2. If n is odd, multiply n by 3 and add 1 to get 3n + 1
int collatz_steps_helper(int n, int i) {
    //printf("%d\n", n);
    // base case
    if (n == 1 | n == 0) { return i; }
    // update # of steps
    i++;
    // compute next step
    if (n % 2 == 0) { return collatz_steps_helper(n/2, i); } else { return collatz_steps_helper((3*n) + 1, i); }   
}

int collatz_steps(int n) {
    return collatz_steps_helper(n, 0);
}


int main() {
    printf("%d\n", collatz_steps(12));

    return 0;
}