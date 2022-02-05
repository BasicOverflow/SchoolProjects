#       CSE 3666 Lab 1

        .globl  main

        .text

main:   
        addi s4 s4 2048
        # read two positive integers from the console and 
        # save them in s1 and s2 
        # compute the GCD of the two numbers with Euclidean algorithm 
        #     while a != b:
        #         if a > b:
        #             a = a - b
        #         else:
        #             b = b - a
        # print the GCD

        # GCD examples:
        #     gcd(11, 121) = 11
        #     gcd(24, 60) = 12
        #     gcd(192, 270) = 6
        #     gcd(14, 97) = 1

        # use system call 5 to read integers
        addi    a7, x0, 5
        ecall
        addi    s1, a0, 0       # a in s1

        # using pseudoinstructions
        li      a7, 5
        ecall
        mv      s2, a0          # b in s2

        # TODO
        # Add you code here
        # compute GCD(a, b) and print it

        #initialize 'while loop'
        beq zero zero Result
        
        
IfClause:	
	sub s1 s1 s2                      # a = a - b
	beq zero zero Result              # Go back to loop
ElseClause:
	sub s2 s2 s1                      # b = b - a
	beq zero zero Result              # Go back to loop
	
	
Loop:
	bgt s1 s2 IfClause                # if a > b, goto IfClause
	beq zero zero ElseClause          # Else, goto ElseClause
Result:
	bne s1 s2 Loop                    # Loop until a == b 

        
        # load result into t0
        add t0, s1, zero
        
        # print result 
        li  a7, 1          # service 1 is print integer
    	add a0, t0, zero   # load desired value into argument register a0, using pseudo-op 
    	ecall
        
        # sys call to exit
exit:   addi    a7, x0, 10      
        ecall
