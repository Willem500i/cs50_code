#include <cs50.h>
#include <stdio.h>

int main(void)
{
    // GET HEIGHT
    int height;
    do
    {
        height = get_int("Height: ");
    }
    while ((1 > height) || (8 < height));
    // SET PYRAMID SPEC INTEGERS
    int layer = 1;
    int hashes = 0;
    int spacesW = 0;
    int spacesH = 0;
    // BUILD PYRAMID
    do
    {
        //DECIDE HOW MANY SPACES NEEDED FOR LAYER
        spacesW = height - layer;
        // printf("height is %i; ",height);
        // printf("layer is %i; ",layer);
        // printf("will print %i",spacesW);
        // printf("will print %i",hashes);
        while (spacesH < spacesW)
        {
            //PRINT FIRST SPACES
            spacesH++;
            // printf("spacesH = %i",spacesH);
            printf(" ");
        }
        do
        {
            //PRINT FIRST HASHES
            printf("#");
            hashes++;
        }
        while (hashes < layer);
        hashes = 0;
        //PRINT 2 SPACES
        printf("  ");
        do
        {
            //PRINT HASHES
            printf("#");
            hashes++;
        }
        while (hashes < layer);
        //NEXT LINE

        printf("\n");

        //RESET INTEGERS FOR NEXT LAYER
        layer++;
        spacesW = 0;
        spacesH = 0;
        hashes = 0;
    }
    while (layer < (height + 1));

}
