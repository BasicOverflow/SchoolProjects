
class Entry:

    def __init__(self, val, priority):
        self.val = val
        self.priority = priority
    
    def __lt__(self, other):
        return self.priority < other.priority

    def __eq__(self, other):
        return self.priority == other.priority and self.val == other.val




class PQ_UL:

    def __init__(self):
        self._length = 0
        self._L = []

    def insert(self, item, p):
        self._L.append(Entry(item, p))
        self._length += 1

    def find_min(self):
        min_i = 0
        for i,entry in enumerate(self._L):
            if entry.priority < self._L[min_i].priority:
                min_i = i

        return self._L[min_i]

    def remove_min(self):
        item = self.find_min()
        self._L.remove(item)
        self._length -= 1
        return item

    def __len__(self):
        return self._length




class PQ_OL:
    
    def __init__(self):
        self._L = []
        self._length = 0

    def insert(self, item, p):
        self._L.append(Entry(item, p))
        self._L.sort(reverse=True)
        self._length += 1

    def find_min(self):
        return self._L[-1]

    def remove_min(self):
        item = self._L[-1]
        self._L = self._L[:-1]
        self._length -= 1
        return item
    
    def __len__(self):
        return self._length















if __name__ == "__main__":
    x = [1,2,3,4,5]
    print(x[:-1])