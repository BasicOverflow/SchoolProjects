from TaskSimulation import Task, HeapPQ


def simulation1():
       TQ = HeapPQ()           # PQ of tasks
       N = 100             # length of tape
       TAPE = list(range(N))   

       for t in range(N):  # This will look at every entry of the TAPE one by one

              tt, task = TQ.findmin() # Grab a task if any exist
              
              # while there are tasks to be executed at this
              # time, go ahead and do the work
              while task is not None and tt == t: 
                     task.execute(TAPE, tt)  # execute task
                     TQ.removemin()          # remove from PQ
                     tt, task = TQ.findmin() # grab the next task

              # Create a  new task and add it to the PQ
              task = Task(-2, 1)  
              next_time = t + 1
              TQ.insert(next_time, task)
              
       return TAPE, TQ

def simulation2():
       # The length of the tape should be 10 (entries should be 0-9).
       # Start with an empty task queue and do no processing for index 0.
       # Beginning with t=1, insert a single new task after processing all tasks for the current time step (the first task should be processed at t=2).
       # execution time: the next time step (t + 1)
       # multiplier: index of the next entry to be processed, i.e., t + 1
       # multiplicity: as above, pick a factor so every time step can execute this task
       # After processing, the initial entries on the output tape should be 0, 1, 4.

       TQ = HeapPQ()           
       N = 10
       TAPE = list(range(N)) 

       for t in range(N):
              if t == 0:
                     continue

              tt, task = TQ.findmin() # Grab a task if any exist 

              # while there are tasks to be executed at this
              # time, go ahead and do the work
              while task is not None and tt == t: 
                     task.execute(TAPE, tt)  # execute task
                     TQ.removemin()          # remove from PQ
                     tt, task = TQ.findmin() # grab the next task

              # Create a  new task and add it to the PQ
              # Note - the condition is "multiple of 1", so this task should
              # be executed at any time
              next_time = t + 1
              task = Task(next_time, 1)  
              TQ.insert(next_time, task)
       
       return TAPE, TQ



def simulation3():
       TQ = HeapPQ()           
       N = 10
       TAPE = list(range(N)) 

       for t in range(N):
              if t == 0:
                     continue

              tt, task = TQ.findmin() # Grab a task if any exist 

              # while there are tasks to be executed at this
              # time, go ahead and do the work
              while task is not None and tt == t: 
                     task.execute(TAPE, tt)  # execute task
                     TQ.removemin()          # remove from PQ
                     tt, task = TQ.findmin() # grab the next task

              # Create a  new task and add it to the PQ
              # Note - the condition is "multiple of 1", so this task should
              # be executed at any time
              next_time = t+1
              task = Task(next_time**2, 1)  
              TQ.insert(next_time, task)
       
       return TAPE, TQ

def simulation4():
       TQ = HeapPQ()           
       N = 100
       TAPE = list(range(N)) 

       for t in range(N):
              if t == 0 or t == 1:
                     continue

              tt, task = TQ.findmin() # Grab a task if any exist 

              # while there are tasks to be executed at this
              # time, go ahead and do the work
              while task is not None and tt == t: 
                     task.execute(TAPE, tt)  # execute task
                     TQ.removemin()          # remove from PQ
                     TQ.insert(tt + task.multiplicity, task)

                     tt, task = TQ.findmin() # grab the next task
              
              if TAPE[t] != 0:
                     T = Task(0, t)
                     TQ.insert(t + T.multiplicity, T)

       return [i for i in TAPE if i > 1], TQ

def simulation5():
       TQ = HeapPQ()           
       N = 100
       TAPE = list(range(N)) 

       for t in range(N):
              if t == 0 or t == 1:
                     continue

              T = Task(-1, t)
              TQ.insert(t,T)

              tt, task = TQ.findmin() # Grab a task if any exist 

              # while there are tasks to be executed at this
              # time, go ahead and do the work
              while task is not None and tt == t: 
                     task.execute(TAPE, tt)  # execute task
                     TQ.removemin()          # remove from PQ
                     TQ.insert(tt + task.multiplicity, task)

                     tt, task = TQ.findmin() # grab the next task

       return [i for i in TAPE if i > -1], TQ



if __name__ == "__main__":
       x = simulation3()

       print(x[0])