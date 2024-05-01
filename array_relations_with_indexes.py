# Problem:
# Given a = [a0, a1, a2, a3, a4, .... an-1]
# Calculate the number of pairs (i,j) that satisfies: a[i] - a[j] = i - j
import random
import time


# Naive solution:
# Here it uses two nested for loops, so the time complexity is O(n^2).
# As it will be shown later, this is very slow compared to the optimized solution.
def naive_solution(array):
    n = 0
    for i in range(0, len(array)):
        for j in range(0, len(array)):
            if array[i] - array[j] == i - j:
                n += 1
    return n


# Better solution:
# This solution contains no nested for loops, so the maximum complexity is for
# running the array once (maybe more than once, but not nested this time), so now the time complexity is O(n).
def better_solution(array):
    # By creating an aux array and changing elements with the rule
    # aux[i] = array[i] - i, the problem now reads "...that satisfies aux[i] = aux[j]"
    aux = []
    for i in range(0, len(array)):
        aux.append(array[i] - i)
    # A dictionary is going to store the frequencies of aux.
    # So if aux = [8, 8, 3, 3 9, 0] the dictionary should be:
    # frequencies = {8:2, 3:2, 9:1, 0:1}
    frequencies = {}
    for i in range(0, len(aux)):
        if aux[i] not in frequencies.keys():
            frequencies[aux[i]] = 1
        else:
            increment = frequencies.get(aux[i]) + 1
            frequencies[aux[i]] = increment
    # The final answer will be the number of elements in aux plus the repetitive combinations.
    # If aux = [1,4,4], three (i,j)s from the final answer come from the trivial (0,0) (1,1) (2,2) cases, this is the len(array) parcel on the return.
    # It is left to include the repetitive combinations: in [1,4,4] they will be (1,2) and (2,1). It is always the frequency n times (n-1) of each item.
    repetitive_combinations = 0
    for value in frequencies.values():
        repetitive_combinations += value * (value - 1)
    return len(array) + repetitive_combinations


# Testing:
array_test = []
for i in range(0, 20000):
    array_test.append(random.randint(0, 30))
    
start_time = time.time()
print(naive_solution(array_test))
print(time.time() - start_time)  # It took over 36 seconds

start_time = time.time()
print(better_solution(array_test))
print(time.time() - start_time)  # It took only 0.006 seconds
