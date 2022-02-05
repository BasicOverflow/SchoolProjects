from LinkedList import LinkedList

class Stack_L:
    def __init__(self):
        self._L = list()        # Composition: the Stack_L class has a List

    def push(self,item):
        self._L.append(item)

    def pop(self):
        popped = self._L[-1]
        self._L = self._L[:-1]
        return popped

class Stack_LL:
    def __init__(self):
        self._LL = LinkedList() # Composition: the Stack_LL class has a Linked List

    def push(self,item):
        self._LL.add_first(item)

    def pop(self):
        return self._LL.remove_first()

class Queue_L:
    def __init__(self): 
        self.head = 0
        self._L = list()

    def enqueue(self,item):
        self._L.append(item)

    def dequeue(self):
        if len(self._L) == 0:
            return self._L
        dq = self._L[0]
        self._L = self._L[1:]
        return dq

class Queue_LL:
    def __init__(self): 
        self._LL = LinkedList()

    def enqueue(self,item):
        self._LL.add_first(item)

    def dequeue(self):
        return self._LL.remove_last()



if __name__ == '__main__':
    ##########Test Stack_L##########
    s1 = Stack_L()
    for i in range(10): s1.push(i*3)

    for i in range(9,-1,-1): assert(s1.pop() == i*3)

    ##########Test Stack_LL#########
    s2 = Stack_LL()
    for i in range(10): s2.push(i*4)

    for i in range(9,-1,-1): assert(s2.pop() == i*4)

    ##########Test Queue_L##########
    q1 = Queue_L()
    for i in range(5): 
        q1.enqueue(i)

    assert(q1.dequeue() == 0)
    assert(q1.dequeue() == 1)
    assert(q1.dequeue() == 2)
    assert(q1.dequeue() == 3)
    assert(q1.dequeue() == 4)

    ##########Test Queue_LL#########
    q2 = Queue_LL()
    for i in range(5): 
        q2.enqueue(i)

    assert(q2.dequeue() == 0)
    assert(q2.dequeue() == 1)
    assert(q2.dequeue() == 2)
    assert(q2.dequeue() == 3)
    assert(q2.dequeue() == 4)

