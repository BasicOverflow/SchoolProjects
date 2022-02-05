# A node object that contains data
class Node:
    def __init__(self, data, next = None):
        self.data = data # A reference to the data 
        self.next = next # A link to the next element


class LinkedList:
    #Initialize a new linked list
    def __init__(self):    
        self._head = None # The head points to the first element

    # Add an element to the beginning
    def add_first(self, item):
        self._head = Node(item, self._head)

    # Remove and return the first element
    def remove_first(self):
        if self._head == None:
            return None
        item = self._head.data       # Extract the data
        self._head = self._head.next # Update the head
        return item                  # Return the data

    # Add an element to the end
    def add_last(self, item):
        if self._head is None:
            self.add_first(item) #identical to add_last for an empty list
        else:
            new_node = Node(item)

            # Find the end of the linked list
            node = self._head
            while node.next:
                node = node.next

            node.next = new_node # Append the new_node
        
    def remove_last(self):
        if (self._head is None) or (self._head.next is None):
            return self.remove_first()
        else:
            # Find the penultimate node
            node = self._head
            while node.next.next:
                node = node.next

            item = node.next.data # extract the data
            node.next = None      # detach the last node
            return item           # return the data


if __name__ == '__main__':
    ##########Test Node##########
    n1 = Node(1)
    assert(n1.data==1)
    n2 = Node(2, next=n1)
    assert(n2.data == 2)
    assert(n2.next == n1)

    ##########Test LinkedList##########
    ll1 = LinkedList()
    assert(ll1._head == None)

    #add_first()
    for i in range(10):
        ll1.add_first(i*3)
        assert(ll1._head.data == i*3)

    #remove_first()
    for i in range(9,-1,-1):
        assert(ll1.remove_first() == i*3)

    #add_last()
    ll1 = LinkedList()
    for i in range(10):
        ll1.add_last(i*2)

    for i in range(10):
        assert(ll1.remove_first() == i*2)

    #remove_last()
    ll1 = LinkedList()
    for i in range(10):
        ll1.add_first(i*7)

    for i in range(10):
        assert(ll1.remove_last() == i*7)
