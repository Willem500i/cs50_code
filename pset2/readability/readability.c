#include <ctype.h>
#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

int main(void)
{
    //get sentence
    string sentence = get_string("Text: ");

    float grade = 0;
    float S = 0;
    float L = 0;
    float W = 1;

    for (int i = 0; i < strlen(sentence); i++)
    {
        //changes letter to uppercase
        sentence[i] = toupper(sentence[i]);

        // Checks for uppercase (if its a letter it will be)
        if (isupper(sentence[i]))
        {
            // if there is a letter add +1 to letter list
            L++;
        }
        if (sentence[i] == ' ')
        {
            // if there is a space add +1 to wordslist
            W++;
        }
        if ((sentence[i] == '.') || (sentence[i] == '!') || (sentence[i] == '?'))
        {
            // if it isnt a space or a letter it is punctuation
            S++;
        }
    }
    // prints word, sentence, letter count
//        printf("L is %f, S is %f, W is %f\n", L, S, W);

    //sets coefficient out of 100
    float coefficient = 100 / W;

    //set L and S per 100 words
    L = L * coefficient;
    S = S * coefficient;

    // prints values out of 100
//        printf("L is %f, S is %f, W is %f\n", L, S, W);

    // finds grade
    grade = 0.0588 * L - 0.296 * S - 15.8;

    //rounds grade
    float grade1 = roundf(grade);

    // converts grade to int
    int gradeF = grade1;

    // prints output
    if (gradeF < 1)
    {
        printf("Before Grade 1\n");
        return 0;
    }
    if (gradeF > 16)
    {
        printf("Grade 16+\n");
        return 0;
    }
    else
    {
        printf("Grade %i\n", gradeF);
        return 0;
    }
}