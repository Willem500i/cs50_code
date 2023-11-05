import cs50 as cs

n = int(cs.get_string("Number: "))

digits = 0
number = n

# count digits
while number > 0:
    number = int(number/10)
    digits += 1

# check for right number of digits
if digits != 13 and digits != 15 and digits != 16:
    print("INVALID")
    exit()

# introduce variables we will be using
number = n
sum1 = 0
sum2 = 0
lastdigit1 = 0
lastdigit2 = 0

# repeat until we are through with number
while number > 0:
    # add to sum 1
    lastdigit1 = number % 10
    number = int(number / 10)
    sum1 += lastdigit1

    # get 2nd to last digit to double
    lastdigit2 = number % 10
    number = int(number / 10)

    # add other digits
    lastdigit2 = lastdigit2 * 2
    digit1 = int(lastdigit2 % 10)
    digit2 = int(lastdigit2 / 10)
    sum2 = sum2 + digit1 + digit2

sum3 = sum1 + sum2

if (sum3 % 10) != 0:
    print("INVALID")
    exit()

firsttwo = n

# find first two digits
while firsttwo > 100:
    firsttwo = int(firsttwo / 10)

# tests for all three cards, prints invalid if none work
if firsttwo >= 40 and firsttwo <= 49:
    print("VISA")
if firsttwo == 37 or firsttwo == 34:
    print("AMEX")
if firsttwo >= 51 and firsttwo <= 55:
    print("MASTERCARD")
else:
    print("INVALID")
