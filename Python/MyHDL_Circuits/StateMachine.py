from myhdl import block, always_comb, always_seq, Signal, intbv, instances, modbv

@block 
def Register(dout, din, clock, reset):
    """ 
    A register that always saves din to dout on positive edges
    """

    @always_seq(clock.posedge, reset=reset)
    def seq_reg():
        dout.next = din

    return seq_reg

# HW4Q5
@block
def Detect3x(z, b, clock, reset):
    """ 
    Input:  b
    Output: z

    b is the input bit
    z is 1 if and only if the current state is 0
    """
    # 2 bits are enough to encode 3 states
    # initial state is 0
    state = Signal(intbv(0)[2:])
    next_state = Signal(intbv(0)[2:])

    # TODO
    # instantiate a register here.
    # next_state is the input and state is the output
    reg1 = Register(state, next_state, clock, reset) 

    # generate next_state, based on state and b
    @always_comb
    def next_state_logic():
        # We can use if-elif-else statements in Python
        if (state == 1 and b == 1) or (state == 0 and b == 0):
            next_state.next = 0
        else: 
            next_state.next = 1

    # generate output
    @always_comb
    def z_logic():
        # TODO
        # generate z from state
        if state == 0:
            z.next = 1
        else:
            z.next = 0

    # return all logic  
    return instances()
