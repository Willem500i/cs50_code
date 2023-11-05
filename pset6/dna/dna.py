# Read DNA sequences and match them to people

import csv
import sys


def main():

    # Ensure correct command line argument
    if len(sys.argv) != 3:
        sys.exit("Usage: python dna.py data.csv sequence.txt")
        exit()

    # Open DNA sequence, read into memory
    with open(sys.argv[1]) as file:
        reader = csv.DictReader(file)
        data = list(reader)

    # Open csv and read into memory
    with open(sys.argv[2]) as file:
        sequence = file.read()

    # Create list counting the max times each STR shows up
    STR_counts = []

    # Loop count for each STR, not including 0 because that says "name"
    for i in range(1, len(reader.fieldnames)):
        # Sets current STR we are looking for
        STR = reader.fieldnames[i]
        # Sets count for that STR to 0
        STR_counts.append(0)

        # for each STR, compute the longest run of consecutive repeats in the DNA sequence
        for j in range(len(sequence)):
            currentMaxSTR = 0

            # If sequence length of STR is the STR, start checking for more
            if sequence[j:(j+len(STR))] == STR:
                k = 0
                # Loop through until it doesn't match
                while sequence[(j+k):(j+k+len(STR))] == STR:
                    k += len(STR)
                    currentMaxSTR += 1
                # After all the sequence occurances have been logged, update new max if it is bigger
                if currentMaxSTR > STR_counts[i-1]:
                    STR_counts[i-1] = currentMaxSTR

    # compare STR counts against each row in CSV
    for i in range(len(data)):
        # counts # of STR count matches
        matches = 0
        # repeat for every STR
        for j in range(1, len(reader.fieldnames)):
            # if the STR count of a certain STR from the csv as an int match the STR count for row i and str count i as an int
            if int(STR_counts[j-1]) == int(data[i][reader.fieldnames[j]]):
                # add 1 to the # of matches
                matches += 1
        # if the number of count matches (len(reader.fieldnames) - 1) is the same for each, they are the same person
        if matches == (len(reader.fieldnames) - 1):
            print(data[i]["name"])
            exit()

    print("No match")


main()