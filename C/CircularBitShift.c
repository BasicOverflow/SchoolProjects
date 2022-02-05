#include <stdio.h>


int obtain_msb(int x) {
    // Move lsb of 1 to the msb
    int msb = 1 << (31); 

    // Evaluate msb with bitwise &
    if (x & msb) { return 1; } else { return 0; }
}

int circular_shift(int a, int pos) {
    // obtain MSB, store it in temp variable 
    // then perform shift, and add that stored MSB to the end
    // repeat
    int msb;
                        
    for (int i=0; i<(pos%32); i++) {
        // store MSB
        msb = obtain_msb(a);
        // perform shift
        a = a << 1;
        // add the msb back as the new lsb. (n & ~1) turns lsb to 0, (n | 1) turns it to 1
        if (msb) { a = a | 1; } else { a = a & ~1; } 
    }
    return a;
}



int main() {
    printf("%d\n", circular_shift(1,1) ); //2
    printf("%d\n", circular_shift(1,10) ); //1024
    printf("%d\n", circular_shift(1,1024) ); //1
    printf("%d\n", circular_shift(-1,1) ); //-1
    printf("%d\n", circular_shift(246672848,864323070) );

    return 0;
}