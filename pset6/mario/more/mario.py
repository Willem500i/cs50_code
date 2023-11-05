import cs50 as cs

height = 0

while height > 8 or height < 1:
    height = cs.get_int("Height: ")

completed_layers = 1

while completed_layers < height + 1:

    # initalize variables we will be using
    spacesH = 0
    hashes = 0
    spacesW = height - completed_layers
    
    # print first spaces
    while spacesH < spacesW:
        print(" ", end='')
        spacesH += 1
    # print first hashes
    while hashes != completed_layers:
        print("#", end='')
        hashes += 1
    # add two spaces
    print("  ", end='')
    # count down the right amount of hashes back to 0
    while hashes != 0:
        print("#", end='')
        hashes -= 1
    print("")

    completed_layers += 1

