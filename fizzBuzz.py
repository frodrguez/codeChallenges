# Problem:
# Given a number x (integer and positive), print all numbers from 1 to x, but when it is divisible by 3 print "Fizz", when by 5 print "Buzz", when by 3 and 5 print "FizzBuzz".

def fizzBuzz(x):
    for i in range(1, x + 1):
        if i % 15 == 0:
            print("FizzBuzz")
        elif i % 3 == 0:
            print("Fizz")
        elif i % 5 == 0:
            print("Buzz")
        else:
            print(str(i))


# Testing:
fizzBuzz(16)

# Output:

# 1
# 2
# Fizz
# 4
# Buzz
# Fizz
# 7
# 8
# Fizz
# Buzz
# 11
# Fizz
# 13
# 14
# FizzBuzz
# 16
