# PROBLEM:
# Suppose a value x is in register s1. Use a minimum number of RISC-V instructions to
# compute 24x (i.e., the product of 24 and x) and save the result in register s2. The instructions
# are shift, ADD, or SUB instructions. You do not need to use all of them. Assume the result
# has only 32 bits so it can be saved in a register without overflow. Explain your method.

# Solution: shift x left 4 bits then add 8x 
	# compute 8x by shifting x left 3 bits

addi s1 s1 3                              # load s1 and assign it x (EX: x=2)
add s2 s2 zero                            # load s2, where the result will be stored
slli s3 s1 3                              # store the product of 8 & x in register s3

addi s4 s4 999

slli s2 s1 4                              # shift s1 left by 4 bits, store it in s2
add s2 s2 s3                              # add 8*x to s2, which has already been stored in s3
                                          
li  a7, 1                                 # print result 
add a0, s2, zero                          
ecall                                     
