#       CSE 3666 Lab 4: Remove spaces

        .data

        # allocating space for str
str:    .space  128

        .globl  main

        .text
main:   
        # read a string into str
        # use pseudoinstruction la to load address into register
        la      a0, str

main_loop:
        # read a string
        addi    a1, x0, 100
        addi    a7, x0, 8
        ecall

        # check if the line is empty (has only the newline) 
        lb      t0, 0(a0)
        addi    t1, x0, '\n'
        beq     t0, t1, exit

	# a0 is already set. does not change during ecall
	jal	ra, print_ns

        # the address of str should be in a0 
        beq     x0, x0, main_loop
 
exit:   addi    a7, x0, 10
        ecall


# function 
print_ns:
        addi sp sp -128 # allocate a byte array of 128 bytes on stack to save result  
        add a1 sp x0 # save address of allocared space to a1
        
        add t4 ra x0 # preserve ra, since it will be changed by remove_Spaces call
        add t3 a1 x0 # preserve a1, since it will be changed by 
        
        jal ra remove_spaces # call remove_spaces
        
        add a1 t3 x0 # restore a1 to beginning of allocated space
        
        # print the result string
        add      a0 a1 x0        # load the address of the string
        li      a7 4             # set the system call number. 4 for printing a string
        ecall                    # system call
        
        # identify the registers that need to be preserved, but changed
        # save/restore registers
        add ra t4 x0 # restore ra to return to main

	jr      ra

# function remove_spaces
remove_spaces:
	addi t2 x0 32 # s2 = 32
	
loop: 
	lb t0, 0(a0) # c = str[i]
	addi a0, a0, 1 # i += 1 (i = index for str)
	beq t0 t2 cond # if c == 0, restart loop
	# else, continue
	sb t0 0(a1) # res[j] = c
	addi a1 a1 1 # j += 1 (j = index for res)
cond: 
	bne t0 x0 loop # loop until c == 0
	# return 
	jr      ra