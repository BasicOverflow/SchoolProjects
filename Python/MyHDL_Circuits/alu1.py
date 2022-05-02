from myhdl import block, always_comb

@block
def ALU1bit(a, b, carryin, binvert, operation, result, carryout):

    """ 1-bit ALU

    result and carrayout are output

    all other signals are input

    operation, the select signal to 4-input Mux, has two bits.
    We can compare operation directly integers, for example, 
        if operation == 0:

    """

    # MyHDL analyzes code and adds Signals (of MyHDL Signal type) that appear
    # on the right hand side of any statements to a sentitivity list. 

    # The function is called if any Signal on the sentitivity list has a new value
    @always_comb 
    def alu1_logic():

        # notb is an internal signal. we do not need define a MyHDL Signal
        notb = not b
        mux1_out = notb if binvert else b
        and_out = mux1_out and a
        or_out = mux1_out or a
        adder_sum = (mux1_out ^ a) ^ carryin
        carryout.next = (mux1_out and a) or (a and carryin) or (mux1_out and carryin)
        
        if operation == 0: 
            result.next = and_out
        elif operation == 1: 
            result.next = or_out
        elif operation == 2: 
            result.next = adder_sum
        elif operation == 3: 
            result.next = 0

    # return the logic  
    return alu1_logic


# provided code for testing:

if __name__ == "__main__":
    from myhdl import intbv, delay, instance, Signal, StopSimulation, bin
    import argparse

    # testbench itself is a block
    @block
    def test_comb(args):

        # create signals
        result = Signal(bool(0))
        carryout = Signal(bool(0))

        a, b, carryin, binvert = [Signal(bool(0)) for i in range(4)]

        # operation has two bits
        operation = Signal(intbv(0)[2:])

        # instantiating a block
        alu1 = ALU1bit(a, b, carryin, binvert, operation, result, carryout)

        @instance
        def stimulus():
            print("op a b cin bneg | cout res")
            for op in args.op:
                assert 0 <= op <= 3
                for i in range(16):
                    # use MyHDL intbv to split bits, instead of shift and AND
                    bi = intbv(i)
                    a.next, b.next, carryin.next, binvert.next = \
                        bi[0], bi[1], bi[2], bi[3]
                    operation.next = op
                    yield delay(10)
                    print("{} {} {} {}   {}    | {}    {}".format(
                        bin(op, 2), 
                        int(a), int(b), int(carryin), int(binvert), 
                        int(carryout), int(result)))

            # stop simulation
            raise StopSimulation()

        return alu1, stimulus

    parser = argparse.ArgumentParser(description='Testing 1-bit ALU')
    parser.add_argument('op', type=int, nargs='*', 
            default=[0, 1, 2, 3], help='operation')
    parser.add_argument('--trace', action='store_true', help='generate trace')
    parser.add_argument('--verbose', '-v', action='store_true', help='verbose')

    args = parser.parse_args()
    if args.verbose:
        print(args)

    tb = test_comb(args)
    tb.config_sim(trace=args.trace)
    tb.run_sim()