#include <stdio.h>
#include <stdlib.h>

/*
 *  Return the sum of even tribonacci numbers between 1 and max
 */

long  even_tribonacci_sum_h(long n)
{
    // printf( "%ld\n", max);
    if (n == 1 || n == 2) {
        return 1;
    } 
    else if (n == 3) { 
        return 2; 
    } 
    else if (n == 0){ 
        return 0;
    }
    
    return even_tribonacci_sum_h(n-1) + even_tribonacci_sum_h(n-2) + even_tribonacci_sum_h(n-3);
}

long even_tribonacci_sum( long max ) {
    int sum = 0;
    long i = 0;
    int booln = 1;
    while (booln){
        int ith_term = even_tribonacci_sum_h(i++);
        // printf( "%ld\n", i);
        if (sum+ith_term > max) booln=0; 
        if (ith_term%2 == 0) sum += ith_term;
    }
    return sum;
}



int main( void )
{
    long max;

    scanf("%ld", &max);

    printf( "%ld\n", even_tribonacci_sum( max ));

    return 0;
}