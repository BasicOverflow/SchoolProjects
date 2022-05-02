from myhdl import *

@block 
def RegisterE(dout, din, en, clock, reset):
    """ register with enable 

        register is updated if en = 1
        Otherwise, dout is not changed

        If reset is active, dout is set to the initial value
    """

    @always_seq(clock.posedge, reset=reset)
    def reg_en():
        if en: 
            dout.next = din
        else:
            dout.next = dout

    return reg_en

@block 
def RegisterShiftLeft(dout, load_data, load, shift_en, clock, reset):
    """ shift left register with load

        register is set to load_data if load is 1
        register is shifted left by 1 if shift_en is 1 (and load is 0)
        Otherwise, register is not changed.

        The bit shifted in is always 0.

        If reset is active, dout is set to the initial value
    """

    @always_seq(clock.posedge, reset=reset)
    def logic_seq():
        if load: 
            dout.next = load_data
        elif shift_en:
            dout.next = (dout << 1)
    return logic_seq

@block 
def RegisterShiftRight(dout, load_data, load, shift_en, clock, reset):
    """ shift right register with load

        register is set to load_data if load is 1
        register is shifted right by 1 if shift_en is 1 (and load is 0)
        Otherwise, register is not changed.

        The bit shifted in is always 0.

        If reset is active, dout is set to the initial value
    """

    @always_seq(clock.posedge, reset=reset)
    def logic_seq():
        if load: 
            dout.next = load_data
        elif shift_en:
            dout.next = (dout >> 1)
    return logic_seq

@block
def Adder(result, x, y):
    """
        result = x + y
    """

    @always_comb
    def adder_logic():
        result.next = x + y

    return adder_logic

@block
def Mul2x(p, x_init, y_init, load, done, clock, reset):
    """ Multiplier

    Input:
        x_init, y_init, load, clock, reset

    Output:
        p, done

    p:  the product

        p = x_init * y_init when done is asserted

    x_init: initial value of x, the multiplicand
    y_init: initial value of y, the multiplier

    load:
        1:  load x_init and y_init into x and y registers. Clear p.
        0:  work mode

    done:
        0: not done
        1: done. result is available in p

    """
    
    # the width determined by the size of x_init
    W = len(x_init)
    W2 = W + W

    # create signals
    adder_out = Signal(modbv(0)[W2:]) # output of adder

    # x and y are the output of registers
    # p is one of arguments (ports) of the multiplier
    x    = Signal(modbv(0)[W2:]) 
    y    = Signal(intbv(0)[W:])

    # enable signal for p, x, and y registers
    p_en = Signal(bool(1))  # enable for reg_p 
    x_en = Signal(bool(1))  # always shift
    y_en = Signal(bool(1))  # always shift

    # counter is used in the control module
    counter = Signal(0)
    counter_in = Signal(0)
    counter_en = Signal(bool(1))    # always enabled
    reg_counter = RegisterE(counter, counter_in, counter_en, clock, reset) 

    # an active high reset signal for register P
    # if load is asserted, P is reset
    p_reset = ResetSignal(bool(0), active=1, isasync=False)

    # instantiate register P
    # pay attention to the signals  
    reg_p = RegisterE(p, adder_out, p_en, clock, p_reset)   

    # RegisterShift(dout, load_data, load, shift_en, clock, reset):
    # instantiate x and y registers, and adder
    reg_x = RegisterShiftLeft(x, x_init, load, x_en, clock, reset)
    reg_y = RegisterShiftRight(y, y_init, load, y_en, clock, reset)
    adder = Adder(adder_out, x, p)
    # x_init and y_init are the load_data signal to reg_x and reg_y, respectively

    # set up control signals for registers
    @always_comb
    def comb_regs():
        p_reset.next = load
        # TODO: 
        # set the p_en signal
        p_en.next = y[0] 

    # counter in the control
    @always_comb
    def comb_counter():
        if load:
            counter_in.next = 0
            done.next = 0
        elif counter == W:
            counter_in.next = counter 
            done.next = 1
        else:
            counter_in.next = counter + 1
            done.next = 0

    ##################################################
    # Provided skeleton code:
    # Monitor for testing
    # it is not really part of the circuit
    # we place it here because it is easier to monitor internal signals
    @instance
    def monitor():
        wl = [4, 3, W2, W2, max(W, 8), 4, 4, 4, 4] 
        fmt_str = ' '.join(["{:"+str(_)+"}" for _ in wl])
        print(fmt_str.format("load", "cnt", "prod", "x", 
            "y", "p_en", "x_en", "y_en", "done"))
        while 1:
            yield clock.posedge
            # wait all signals are updated after positive edge
            yield delay(2) 
            print(fmt_str.format(
                int(load), int(counter), bin(p, W2), bin(x, W2), bin(y, W), 
                int(p_en), int(x_en), int(y_en), int(done)))

    return instances()
