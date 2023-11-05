#include <cs50.h>
#include <stdio.h>
#include <math.h>

int main(void)
{

//get card number
    long n = get_long("Number: ");

//count digits
    int digits = 0;
    long number = n;
    while (number > 0)
    {
        number = number / 10;
        digits++;
    }

//check for correct number of digits
    if (digits != 13 && digits != 15 && digits != 16)
    {
        printf("INVALID\n");
        return 0;
    }

//create integers
    number = n;
    int sum1 = 0;
    int sum2 = 0;
    int sum3 = 0;
    int digit1 = 0;
    int digit2 = 0;
    int lastdigit1 = 0;
    int lastdigit2 = 0;
//add up sum
    do
    {
//add to sum1
        lastdigit1 = (number % 10);
        number = (number / 10);
        sum1 = (lastdigit1 + sum1);

//get last digit to double
        lastdigit2 = (number % 10);
        number = (number / 10);

//double last digit and add digits
        lastdigit2 = (lastdigit2 * 2);
        digit1 = (lastdigit2 % 10);
        digit2 = (lastdigit2 / 10);
        sum2 = (sum2 + digit1 + digit2);

//add to digit count
        digits++;
    }
    while (number > 0);

//add sums
    sum3 = (sum1 + sum2);

//check for valid card number and correct # of digits
    if (sum3 % 10 != 0)
    {
        printf("INVALID\n");
        return 0;
    }

    number = n;
    long firsttwo = number;
//get first two digits
    do
    {
        firsttwo = firsttwo / 10;
    }
    while (firsttwo > 100);

//check what brand card it is
    if ((firsttwo == 40) || (firsttwo == 41) || (firsttwo == 42) || (firsttwo == 43) || (firsttwo == 44) || (firsttwo == 45)
        || (firsttwo == 46) || (firsttwo == 47) || (firsttwo == 48) || (firsttwo == 49))
    {
        printf("VISA\n");
    }
    else if ((firsttwo == 37) || (firsttwo == 34))
    {
        printf("AMEX\n");
    }
    else if ((firsttwo == 51) || (firsttwo == 52) || (firsttwo == 53) || (firsttwo == 54) || (firsttwo == 55))
    {
        printf("MASTERCARD\n");
    }
    else
    {
        printf("INVALID\n");
        return 0;
    }
    return 0;
}