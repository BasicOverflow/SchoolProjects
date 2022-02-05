
class Entry:
    def __init__(self, key, value):
        self.key = key
        self.value = value


class Mapping:
    def __init__(self):
        self.n_buckets = 100
        self._L = [[] for i in range(self.n_buckets)]
        self._len = 0
        self._max_buckets = 1600

    def remove(self, key):
        bucket = self.hash(key) #returns index

        # Case 1: item in mapping
        for entry in self._L[bucket]:
            if entry.key == key:
                #set the buckets value to the value passed 
                self._L[bucket].remove(entry)

                self._len -= 1

    def __setitem__(self, key, value):
        bucket = self.hash(key) #returns index

        # Case 1: item in mapping
        for entry in self._L[bucket]:
            if entry.key == key:
                #set the buckets value to the value passed 
                entry.value = value
                return

        # Case 2: item not in mapping
        self._L[bucket].append(Entry(key, value)) #add new entry at that index's bucket
        self._len += 1

        #must rehash once new index is added to hash table
        if (self.n_buckets < self._max_buckets and len(self) > self.n_buckets):
            self.rehash()

    def hash(self, key):
        # return key % self.n_buckets
        return hash(key) % self.n_buckets

    def __getitem__(self, key):
        bucket = self.hash(key)
        for entry in self._L[bucket]:
            if entry.key == key: return entry.value
        raise KeyError("key {} not in Mapping".format(key))

    # Increase the number of buckets only when the
    # increase_size argument is true
    def rehash(self, increase_size = True):
        if (increase_size):
            self.n_buckets *= 2

        new_L = [[] for i in range(self.n_buckets)]

        # move all items to new list
        for bucket in self._L:
            for entry in bucket:
                #obtain new index from rehashing, hash will be different now since hash table has grown
                new_bucket = self.hash(entry.key)
                #re add all entries in each bucket
                new_L[new_bucket].append(entry)

        self._L = new_L[:]
    
    def __len__(self):
        return (self._len)



class CustomSet(object):
    
    def __init__(self):
        self._m = Mapping()

    def __len__(self):
        return len(self._m)

    def add(self, value):
        if value in self:
            return

        self._m[value] = value

    def remove(self, value):
        if value not in self: raise ValueError

        self._m.remove(value)

    def __contains__(self, value):
        try:
            x =  self._m[value]
            return True
        except:
            return False










if __name__ == "__main__":
    test = CustomSet()

    test.add("yo")

    print("yo" in test)

    test.remove("yo")

    print("yo" in test)