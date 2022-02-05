import copy

class Task:
    def __init__(self, operation, condition):
        self.multiplier = operation
        self.multiplicity = condition

    def execute(self, L,  time):          # time is the index into L
        if time % self.multiplicity == 0:
            L[time] *= self.multiplier

    def __repr__(self):
        return "Operation: multiply by = " + str(self.multiplier) + ", " + "condition: time is a multiple of  "\
               + str(self.multiplicity)  + '\n'


class HeapPQ:
    def __init__(self):
        self._tasks = []
        self._len = 0

    def insert(self, time, task):
        self._tasks.append([time, task])
        self._len += 1
        self._upheap(len(self._tasks) - 1)

    def _parent(self, i):
        return (i - 1) // 2

    def _children(self, i):
        left = 2 * i + 1
        right = 2 * i + 2
        return range(left, min(len(self._tasks), right + 1))

    def _swap(self, a, b):
        L = self._tasks
        L[a], L[b] = L[b], L[a]

    def _upheap(self, i):
        L = self._tasks
        parent = self._parent(i)
        if i > 0 and L[i][0] < L[parent][0]:  # Comparing scheduled execution times
            self._swap(i, parent)
            self._upheap(parent)

    def findmin(self):
        if self._len:
            return self._tasks[0]
        return None, None  # Two returns are expected from this queue (time, task)

    def removemin(self):
        if self._len == 0:
            return None, None

        L = self._tasks
        time_task_pair = copy.copy(L[0])
        L[0] = L[-1]
        L.pop()
        self._len -= 1
        self._downheap(0)
        return time_task_pair

    def _downheap(self, i):
        L = self._tasks
        children = self._children(i)
        if children:
            child = min(children, key = lambda x: L[x][0])
            if L[child][0] < L[i][0]:  # Comparing scheduled execution times
                self._swap(i, child)
                self._downheap(child)

    def __len__(self):
        return self._len

    def __str__(self):
        ret_str = ''
        for [tt, task] in self._tasks:
            ret_str += "exec. time = " + str(tt) + ',  ' + str(task)

        return ret_str


if __name__ == "__main__":
    #### EXAMPLE SIMULATION 1 ####
    # Initialize the tape
    TQ = HeapPQ()           # PQ of tasks
    N = 100                 # Number of entries
    TAPE = list(range(N))   # List of entries

    t0 = Task(-1,  3)  # Multiply by `1 if t is a multiple of 3
    TQ.insert(0, t0)   # Putting t0 on the Priority queue for execution at time 0


    # Process the tape
    for t in range(N):  # Scan every entry of the TAPE one by one
        tt, task = TQ.findmin()

        # If there are still tasks queued AND
        # the minimum time queued is this time:
        while task is not None and tt == t:
            task.execute(TAPE, t)   # execute task at current time
            TQ.removemin()          # pop the task we just executed

            next_time = tt + task.multiplicity  # Re-insert the task we just executed at a future timestamp     
            TQ.insert(next_time, task)          # (this is the "rule" for this simulation")

            tt, task = TQ.findmin() # Grab the next task from the priority queue

    print(TAPE)

    print("NOW TASK QUEUE IS:")
    print(TQ)

    print("+++++++++++++++++++++++++++++++++++++++")

    #### EXAMPLE SIMULATION 2 ####
    # Initialize the tape
    TQ = HeapPQ()           # PQ of tasks
    N = 10                  # Number of entries
    TAPE = list(range(N))   # List of entries

    # Process the tape
    for t in range(N):
        if t < 1: continue  # skip the first time step

        tt, task = TQ.findmin()

        # If there are tasks to be executed at this
        # time go ahead and do the work
        while task and tt == t:
            task.execute(TAPE, tt)  # execute task
            TQ.removemin()          # remove from queue
            tt, task = TQ.findmin() # grab next task

        # Rule for this sim: Add a task to the next time step
        # Multiplier: the current time step's entry (after all processing)
        # Condition: Multiple of one (e.g. all times)
        task = Task(TAPE[t], 1)
        next_time = t + 1
        TQ.insert(next_time, task)

    print(TAPE)

    print("NOW TASK QUEUE IS:")
    print(TQ)

    print("+++++++++++++++++++++++++++++++++++++++")