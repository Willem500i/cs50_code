import cs50 as cs
#import ctype as ctype
#import stdio as stdio
#import string as string
#import math as math

sentence = cs.get_string("Text: ")

sentence = sentence.upper()

# sentences
S = 0
# letters
L = 0
# words
W = 1

# loops through every letter in sentence
for i in range(len(sentence)):

    # if it is uppercase (if its a letter), add 1 to letter count
    if(sentence[i].isupper()):
        L += 1
    # if there is a space, add 1 to word count
    if(sentence[i] == ' '):
        W += 1
    # if there is punctuation, add 1 to sentence count
    if(sentence[i] == '.' or sentence[i] == '!' or sentence[i] == '?'):
        S += 1

# set coefficient
coefficient = 100 / W

# set letters per 100 using coefficient
L *= coefficient
S *= coefficient

# set grade as rounded int of grade formula
grade = int(round(0.0588 * L - 0.296 * S - 15.8))

if grade < 1:
    print("Before Grade 1")
    exit()
if grade > 16:
    print("Grade 16+")
    exit()
else:
    grade = str(grade)
    print("Grade " + grade)