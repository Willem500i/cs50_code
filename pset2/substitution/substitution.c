#include <ctype.h>
#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

int main(int argc, string argv[])
{

    // must be 2
    if (argc != 2)
    {
        printf("Usage: ./substitution key\n");
        return 1;
    }
    // must be 26 long
    if (strlen(argv[1]) != 26)
    {
        printf("Key must contain 26 characters.\n");
        return 1;
    }

    // create string out of code
    string code = argv[1];
    char key[26];

    // create a bool marking all of the letters as unused
    bool used[26];
    for (int i = 0; i < 26; i++)
    {
        used[i] = false;
    }

    // must be all letters
    for (int i = 0; i < 26; i++)
    {
        if (isalpha(code[i]))
        {
            //convert all of them to uppercase to make it simpler
            code[i] = toupper(code[i]);

            if (used[code[i] - 'A'] == true)
            {
                // check for repeated letters
                printf("Key must not contain repeated characters.\n");
                return 1;
            }
            else
            {
                // set a letter to used (true) so it is not repeated, and assign letter to place in code
                used[code[i] - 'A'] = true;
                key[i] = code[i] - 'A';
            }
        }
        else
        {
            // if it contains a non-letter the code is bad
            printf("Key must only contain alphabetic characters.\n");
            return 1;
        }
    }

    // get string
    string plaintext = get_string("plaintext: ");

    // create cypher text
    for (int i = 0; i < strlen(plaintext); i++)
    {
        if (isalpha(plaintext[i]))
        {
            if isupper(plaintext[i])
            {
                plaintext[i] = (key[plaintext[i] - 'A'] + 'A');
            }
            else
            {
                plaintext[i] = (key[plaintext[i] - 'a'] + 'a');
            }
        }
    }

    // print cyphertext
    printf("ciphertext: %s\n", plaintext);
    return 0;
}