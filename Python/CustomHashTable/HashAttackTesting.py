from HashAttack import Mapping, hash_attack

if __name__ == '__main__':

    m = Mapping()
    n = 4800

    # Normal operation
    for i in range(n):
        m[i] = str(i)

    # HASH ATTACK 1: a group of hackers overload a single bucket with index 7
    hash_attack(m, 500, bucket_index = 7)

    max_idx2, max_load2 = m.max_load_bucket()
    _, min_load2 = m.min_load_bucket()

    assert max_idx2 == 7; 
    assert max_load2 == 500 # Lots of collisions
    assert min_load2 == 3;

    a_prime_number = 1601 # > m.n_buckets = 1600

    #  use a new hash function to re-balance  Mapping
    prime_hash = lambda x: m.prime_hash( x, a_prime_number)


    m.balance(prime_hash)

    _, max_load3 = m.max_load_bucket()
    _, min_load3 = m.min_load_bucket()

    assert max_load3 == 7
    assert min_load3 == 3

#######################################################################################################


    # HASH ATTACK 2: The perpitrators discovered that
    # we used a hash function based on the prime number 1601
    # and attacked again
    for i in range(500):
        j = 1601*i + 1; #  These will hash to the same value but
                        #  not necessarily 1
        m[j] = str(j)

    _, max_load4 = m.max_load_bucket()
    _, min_load4 = m.min_load_bucket()

    assert max_load4 == 500 # Lots of collisions
    assert min_load4 == 3;

    # rebalance again based on the prime number 3191
    a_prime_number = 3191
    prime_hash = lambda x: m.prime_hash( x, a_prime_number)
    m.balance(prime_hash)

    _, max_load5 = m.max_load_bucket()
    _, min_load5 = m.min_load_bucket()

    assert max_load5 == 6
    assert min_load5 == 2;

    # rebalance based on a highly composite number 2000
    a_composite_number = 2000
    composite_hash = lambda x: m.prime_hash( x, a_composite_number)
    m.balance(composite_hash)

    _, max_load6 = m.max_load_bucket()
    _, min_load6 = m.min_load_bucket()

    assert max_load6 == 204 # Lots of collisions!
    assert min_load6 == 2;
