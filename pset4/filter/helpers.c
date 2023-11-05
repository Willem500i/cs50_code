
#include "helpers.h"
#include <math.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // add RGB values
            int temp3 = (image[i][j].rgbtRed + image[i][j].rgbtGreen + image[i][j].rgbtBlue);
            // turn to float
            float tempF = temp3;
            // average by dividing by 3
            tempF /= 3;
            // round float
            float tempR = roundf(tempF);
            // turn float to int
            int temp = tempR;

            image[i][j].rgbtRed = temp;
            image[i][j].rgbtGreen = temp;
            image[i][j].rgbtBlue = temp;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    // create reference image to copy from
    RGBTRIPLE ref[height][width];
    {
        for (int i = 0; i < height; i++)
        {
            for (int j = 0; j < width; j++)
            {
                ref[i][j] = image[i][j];
            }
        }
    }

    for (int i = 0; i < height; i++)
    {
        int temp = width - 1;

        for (int j = 0; j < width; j++)
        {
            // set left pixel to right pixel (temp changes how far to the right)
            image[i][j] = ref[i][temp];
            // set previous right pixel to what left pixel was before, using reference image
            image [i][temp] = ref[i][j];

            // decrease temp to go to the next pixel
            temp--;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    // copy everything from image into a reference image to get averages from
    RGBTRIPLE ref[height][width];
    {
        for (int i = 0; i < height; i++)
        {
            for (int j = 0; j < width; j++)
            {
                ref[i][j] = image[i][j];
            }
        }
    }

    // get averages and put them into image
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int pixelcount = 0;
            int tempRed = 0;
            int tempGreen = 0;
            int tempBlue = 0;

            // clycle through height 1 above and 1 below
            for (int h = i - 1; h < (i + 2); h++)
            {
                // cycle through width 1 to the left and 1 to the right
                for (int w = j - 1; w < (j + 2); w++)
                {
                    // ignore edge pixels
                    if (h < 0 || w < 0 || h >= height || w >= width)
                    {
                        continue;
                    }
                    else
                    {
                        // add RGB values
                        tempRed += ref[h][w].rgbtRed;
                        tempGreen += ref[h][w].rgbtGreen;
                        tempBlue += ref[h][w].rgbtBlue;

                        // add 1 to pixel count to use to average
                        pixelcount++;
                    }
                }
            }
            // turn into floats
            float tempRedF = tempRed;
            float tempGreenF = tempGreen;
            float tempBlueF = tempBlue;
            float pixelcountF = pixelcount;

            // average
            tempRedF /= pixelcountF;
            tempGreenF /= pixelcountF;
            tempBlueF /= pixelcountF;

            // round average
            tempRedF = roundf(tempRedF);
            tempGreenF = roundf(tempGreenF);
            tempBlueF = roundf(tempBlueF);

            // turn average into an int
            int tempRed1 = tempRedF;
            int tempGreen1 = tempGreenF;
            int tempBlue1 = tempBlueF;

            // set average as RGB value
            image[i][j].rgbtRed = tempRed1;
            image[i][j].rgbtGreen = tempGreen1;
            image[i][j].rgbtBlue = tempBlue1;
        }
    }
    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    // make two 3x3 arrays of Gx and Gy values to multiply by
    int Gx[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
    int Gy[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};

    // make a reference to use that remains unchanged
    RGBTRIPLE ref[height][width];

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            ref[i][j] = image[i][j];
        }
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // x and y values for RGB
            int Rx = 0;
            int GxG = 0;
            int Bx = 0;
            int Ry = 0;
            int GyG = 0;
            int By = 0;

            for (int h = 0; h < 3; h++)
            {
                for (int w = 0; w < 3; w++)
                {
                    // skip through the edges
                    if (i - 1 + h < 0 || j - 1 + w < 0 || i - 1 + h >= height || j - 1 + w >= width)
                    {
                        continue;
                    }
                    else
                    {
                        // add to Gx values
                        Rx += ref[i - 1 + h][j - 1 + w].rgbtRed * Gx[h][w];
                        GxG += ref[i - 1 + h][j - 1 + w].rgbtGreen * Gx[h][w];
                        Bx += ref[i - 1 + h][j - 1 + w].rgbtBlue * Gx[h][w];

                        // add to Gy values
                        Ry += ref[i - 1 + h][j - 1 + w].rgbtRed * Gy[h][w];
                        GyG += ref[i - 1 + h][j - 1 + w].rgbtGreen * Gy[h][w];
                        By += ref[i - 1 + h][j - 1 + w].rgbtBlue * Gy[h][w];

                    }
                }
            }

            // square values, get square root, make sure it is not above 255, set image value.
            int Red = round(sqrt((Rx * Rx) + (Ry * Ry)));
            if (Red > 255)
            {
                Red = 255;
            }

            image[i][j].rgbtRed = Red;

            int Green = round(sqrt((GxG * GxG) + (GyG * GyG)));
            if (Green > 255)
            {
                Green = 255;
            }

            image[i][j].rgbtGreen = Green;

            int Blue = round(sqrt((Bx * Bx) + (By * By)));
            if (Blue > 255)
            {
                Blue = 255;
            }

            image[i][j].rgbtBlue = Blue;
        }
    }
    return;
}
