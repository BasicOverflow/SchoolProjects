import time

root = "CollectionNumbers"

### Algorithms ###

def brute_force(num, L):
    for i in L:
        for j in L:
            if i+j == num: return True
    return False


def binary_search(num, L):
    L = sorted(L)
    left_bound = 0
    right_bound = len(L) - 1
    while (left_bound < right_bound):
        if (left_bound*2) == num or (right_bound*2) == num: return True

        if L[left_bound] + L[right_bound] == num:
            return True
        elif L[left_bound] + L[right_bound] < num:
            left_bound += 1
        else: right_bound -= 1
    return False

### Load Dataset ###

def load_numbers(filename): #provide filename without n-sol
    '''Return list of solutions and list of numbers'''
    sols = []
    with open(f"{root}/{filename}-nsol.txt","r") as f:
        sols = f.readlines()
        sols = [i.strip("\n") for i in sols] #remove \n chars from each element
        sols = [int(i) for i in sols] #convert to ints
        f.close()
    nums = []
    with open(f"{root}/{filename}.txt","r") as f:
        nums = f.readlines()
        nums = [i.strip("\n") for i in nums] #remove \n chars from each element
        nums = [int(i) for i in nums] #convert to ints
        f.close()

    return nums, sols

### Runtime testing ###

def test_algo(algo, solution_nums, nums):
    '''Tests given algorithm on given data and returns time in seconds'''
    start = time.time()
    for sol in solution_nums:
        # run the algorithm, no need to unpack result
        algo(sol, nums)
    end = time.time()
    # return time delta
    return end - start 


def avg_algo_test(algo, solution_nums, nums):
    '''Run the time test 5 different times, return average time'''
    runtimes = []
    for _ in range(5):
        time = test_algo(algo, solution_nums, nums)
        runtimes.append(time)
    return sum(runtimes) / len(runtimes)


def main():
    results = [{"fname":f"listNumbers-{i}","brute_force":0,"binary_search":0} for i in ["10","100","1000","10000","100000","1000000"]]
    
    for result in results:
        fname = result["fname"]
        # Grab data from files
        numbers, solutions = load_numbers(fname)
        # perform time test and obtain average time for each aglorithm (takes average of 5 tests)
        brute_force_time = avg_algo_test(brute_force, numbers, solutions)
        binary_search_time = avg_algo_test(binary_search, numbers, solutions)
        # add times to results
        result["brute_force"] = brute_force_time
        result["binary_search"] = binary_search_time
    
    return results




if __name__ == "__main__":
    # print(binary_search(8,[4,5,2,3,1,1]))
    print(main()) #returns list of dict items containing the data