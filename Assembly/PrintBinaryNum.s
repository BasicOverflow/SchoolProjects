#       CSE 3666 Lab 2

        .globl  main
        .text

main:   
        
        # read one integer from the console and 
        # print the number in binary 
 
        # use system call 5 to read integer
        addi    a7, x0, 5
        ecall
        addi    s1, a0, 0

        # use system call 35 to print a0 in binary
        # a0 has the integer we want to print
        addi    a7, x0, 35
        ecall
        
        # TODO
        # Add you code here
        
        # print newline
        addi a0 x0 10
        addi    a7, x0, 11
        ecall 
        
        #Store mask in s0
        lui s0, 0x80000
	addi s0, s0, 0x000 
        
        # Init loop
        addi s3 x0 0 # s3 = i = 0
        addi s4 x0 32 # s4 used for loop's stop condition
        beq x0 x0 condition # Start loop

print1: 
	addi a7 x0 1         
    	addi a0 x0 1   
    	ecall
    	beq x0 x0 condition # goto end of loop
    	
print0: 
	addi a7 x0 1         
    	addi a0 x0 0  
    	ecall 
        beq x0 x0 condition # goto end of loop
loop: 
	addi s3 s3 1 # increment i
	and s2 s1 s0 # obtain MSB, store in s2
	slli s1 s1 1 # shift bits
	beq s2 x0 print0 # print 0 then reset loop
	beq x0 x0 print1 # print 1 if above branch not executed
	
condition: 
	bne s3 s4 loop # loop until i == 32
        
        #   print newline
        addi a0 x0 10
        addi    a7, x0, 11
        ecall 
    	
        # exit
exit:   addi    a7, x0, 10      
        ecall
