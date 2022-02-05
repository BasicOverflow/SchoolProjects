import math
import random

def createQubit(alpha, beta):
    """
    Parameters:
    Probability amplitutde = alpha -> float
    Probability amplitutde = beta  -> float

    Return:
    A qubit state -> list [x, y], which represents x|0> + y|1>
    """

    N = math.sqrt(alpha**2+beta**2)
    x = alpha/N
    y = beta/N

    return [x,y]



def stateFromProbabilities(p0, p1):
    """
    Parameters:
    Probability of Zero = p0 -> float
    Probability of One  = p1 -> float

    Return:
    Measured state from the probabilities
    """
    r = random.random()

    if r < p0:
        return 0
    else:
        return 1


def ZBasisMeasure(state):
    """
    This function performs a simulated Z Basis measurement.

    Parameters:
    A qubit state = state -> list [x, y]

    Return:
    0 or 1 depending on the simulated measurement result
    """
    
    # p0 = probability of reading 0
    p0 = state[0]**2 # calculate p0
    
    # p1 = probability of reading 1
    p1 = state[1]**2 # calculate p1

    return stateFromProbabilities(p0, p1)



def XBasisMeasure(state):
    """
    This function performs a simulated X Basis measurement

    Parameters:
    A qubit state = state -> list [x, y]

    Return:
    0 or 1 depending on the simulated measurement result
    """
    #0.5 * (x^2+ y^2) + xy
    #0.5 * (x^2+ y^2) - xy
    p0 = 0.5* (state[0]**2 + state[1]**2) + (state[0]*state[1])
    p1 = 0.5* (state[0]**2 + state[1]**2) - (state[0]*state[1])

    return stateFromProbabilities(p0,p1)


def tomographyExperiment(alpha, beta, numTrials):
    """
    This funciton performs a simple quantum tomography experiment.

    Parameters:
    Probability amplitutde = alpha     -> float
    Probability amplitutde = beta      -> float
    Number of trials       = numTrials -> int
    
    Return:
    A tuple: (guess, E)    
    
    where:
    Estimated qubit state  = guess     -> list [guess_x, guess_y]
    Error                  = E         -> float
    """

    n0 = 0
    n1 = 0
    for _ in range(numTrials):
        state = createQubit(alpha, beta)    # creates a qubit representing |q> = x|0> + y|0>
        m = ZBasisMeasure(state)            # Measures outcome in Z basis

        if m == 0:
          n0 += 1
        else:
          n1 += 1

    guess_x = math.sqrt(n0/numTrials)
    guess_y = math.sqrt(n1/numTrials)

    print('Estimated x =', guess_x)
    print('Estimated y =', guess_y)

    E = (guess_x - state[0])**2 + (guess_y - state[1])**2 # calculate error, E 

    print('The error is', E)
    return ([guess_x, guess_y], E)


######### Quantum Key Distribution #########
# TASK5

def AlicePrepare():
    """
    Alice prepares a qubit using this function.
    First, she chooses a random key and a random basis.
    Next, she prepares an actual qubit 'state'.

    Return:
    A tuple: (key, basis, state)
    """
    key = random.randint(0, 1)

    # For Z Basis Measure, basis = 0
    # For X Basis Measure, basis = 1
    basis = random.randint(0, 1)

    if (key == 0) and (basis == 0):
        # A |0> bit is created when alpha = 1 and beta = 0.
        state = createQubit(1, 0)
    elif (key == 1) and (basis == 0):
        # A |1> bit is created when alpha = 0 and beta = 1.
        state = createQubit(0, 1)
    elif (key == 0) and (basis == 1):
        state = createQubit(1. / math.sqrt(2.), 1. / math.sqrt(2.))
    else:
        state = createQubit(1. / math.sqrt(2.), -1. / math.sqrt(2.))

    return (key, basis, state)


def BobMeasure(state):
    """
    Bob measures the qubit using this function.
    First, he chooses a random basis (Z or X). Hint: Notice the function above. How basis was generated?
    Next, he measures in that basis
    This measurement value is his key

    Parameters:
    A qubit state = state -> list [x, y]

    Return:
    A tuple: (basis, key) Here, basis = 0 for ZBasisMeasure and basis = 1 for XBasisMeasure
    """
    # 0 for Z | 1 for X
    basis = random.randint(0, 1)

    if basis == 0:
      #perform Z basis measurement 
      key = ZBasisMeasure(state)
    else:
      key = XBasisMeasure(state)
    
    return (basis,key)


def QKDSimulation(numTrials):
    """
    This function simulates the BB84 QKD protocol for "numTrials" times
    In each trial:
        First, run Alice's preparation
        Next, Bob's measurement
        Finally, perform reconciliation (step 5 in the writeup)
        Append to 'aliceKey' and 'bobKey'

    Parameters:
    Number of trials = numTrials -> int

    Return:
    Shared kay       = aliceKey  -> list
    """
    aliceKey = []
    bobKey = []
    for _ in range(numTrials):
        # Use AlicePrepare() and BobMeasure(...)
        # Be sure to only add the returned keys to the lists 'aliceKey' and 'bobKey' if they choose the same basis
        alicePrep = AlicePrepare()
        bobResult = BobMeasure(alicePrep[2])

        if alicePrep[1] == bobResults[0]: #if both basis are identical
          aliceKey.append(alicePrep[0])
          bobKey.append(bobResult[1])

    print('Size of key is', len(aliceKey))
    return aliceKey




























