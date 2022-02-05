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
        return key % self.n_buckets

    #    key % 1600 = bucket_index


    def prime_hash(self, key, a_prime_number):
        # 1) Throw a ValueError exception when the prime_number is < than the number of buckets
        # (no need to check whether the a_prime_number is actually prime)
        if a_prime_number < self.n_buckets: raise ValueError
        # 2) return the hash value that is equal to
        # (key mod prime_number ) mod (number of buckets)
        return (key % a_prime_number) % self.n_buckets


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


    def balance(self, hash_func):
        self.hash = hash_func

        self.rehash(increase_size=False)


# Write a function Mapping.balance(hash_func) which sets the hash function in the 
# Mapping instance to hash_func and then simply calls the Mapping.rehash(increase_size = False). 
# In this way, a new function hash_func will place items in their new buckets and if that function is chosen well, 
# the imbalances in the Mapping will be corrected.

    # Return the index of the bucket with the largest number of items
    # and the number of items in that bucket
    def max_load_bucket(self):
        max_items = 0
        max_index = 0

        for i,bucket in enumerate(self._L):
            if (num_entries := len(bucket)) > max_items:
                max_items = num_entries
                max_index = i

        return max_index, max_items


    # Return the index of the bucket with the largest number of items
    # and the number of items in that bucket
    def min_load_bucket(self):
        min_items = None
        min_index = 0

        for i, bucket in enumerate(self._L):
            if min_items is None:
                min_items = len(bucket)
                min_index = i
            elif (num_entries := len(bucket)) < min_items:
                min_items = num_entries
                min_index = i
        
        return min_index, min_items


    def __len__(self):
        return (self._len)

"""
m is a Mapping
q is the number of key-value pairs of the form (k, str(k)) where k are integers
bucket_index is the index of the bucket to receive the q pairs mentioned above

We want all the keys to hash to the same value bucket_index assuming that the original 
hash function is in place.
"""
def hash_attack( m , q, bucket_index):
    # key = bucket_index
    for i in range(1,q):
        # print(m.n_buckets)
        m[m.n_buckets*i+bucket_index] = str(i)



# m = Mapping()
# n = 4800

# # Normal operation
# for i in range(n):
#     m[i] = str(i)

# for i,bucket in enumerate(m._L):
#     if i != 7:
#         continue
#     txt = f"Bucket {i}: "
#     for entry in bucket:
#         txt += f"({entry.key},{entry.value}) "
#     print(txt)

# hash_attack(m, 500, bucket_index = 7)


# for i,bucket in enumerate(m._L):
#     if i != 7:
#         continue
#     txt = f"Bucket {i}: "
#     for entry in bucket:
#         txt += f"({entry.key},{entry.value}) "
#     print(txt)




