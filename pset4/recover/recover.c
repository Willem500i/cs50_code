#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
typedef uint8_t BYTE;

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Program must be exceucuted with exactly one command line argument.\n");
        return 1;
    }

    // remember filename
    char *infile = argv[1];

    // open and verify file
    FILE *input = fopen(infile, "r");

    if (input == NULL)
    {
        printf("Forensic image can not be opened for reading\n");
        return 1;
    }

    // initialise # of photo, buffer as a byte, the file img to work with, and the filename to be 8 long.
    int j = 0;
    BYTE buffer[512];
    FILE *img = NULL;
    char filename[8];

    // while there is still data to read, continue
    while (fread(&buffer, 512, 1, input) == 1)
    {
        // if it is the start of a jew jpg
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {
            // if it is not the first jpg, close the previous one
            if (!(j == 0))
            {
                fclose(img);
            }
            
            // create current file and move file count up one
            sprintf(filename, "%03i.jpg", j);
            img = fopen(filename, "w");
            j++;
        }
        
        // if it isn't the first file or the start of a jpg, continue writing to image.
        if (!(j == 0))
        {
            // write 512 bytes
            fwrite(&buffer, 512, 1, img);
        }

    }
    // close input file
    fclose(input);
    
    // close image we are working with
    fclose(img);
    
    // finished
    return 0;
}