#include "helpers.h"
#include <math.h>
#include <stdlib.h>
#define MIN(a,b) (a) < (b) ? (a) : (b)

void copy_image(int height, int width, RGBTRIPLE image[height][width], RGBTRIPLE image_[height][width])
{

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image_[i][j].rgbtBlue =  image[i][j].rgbtBlue;
            image_[i][j].rgbtGreen = image[i][j].rgbtGreen;
            image_[i][j].rgbtRed = image[i][j].rgbtRed;
        }
    }
}

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    // The greyscale filter is achieved by replacing each pixel's RGB value with the average of the 3 RGB values at that pixel
    BYTE avg;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            avg = round(((float) image[i][j].rgbtBlue + image[i][j].rgbtGreen + image[i][j].rgbtRed)/3);
            image[i][j].rgbtBlue = avg;
            image[i][j].rgbtGreen = avg;
            image[i][j].rgbtRed = avg;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    // The reflect filter will swap bytes left<->right across a row
    float middle = (float) width/2;
    BYTE tmpBlue, tmpGreen, tmpRed;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < middle; j++)
        {
            tmpBlue = image[i][j].rgbtBlue;
            tmpGreen = image[i][j].rgbtGreen;
            tmpRed = image[i][j].rgbtRed;

            image[i][j].rgbtBlue = image[i][width-j-1].rgbtBlue;
            image[i][j].rgbtGreen = image[i][width-j-1].rgbtGreen;
            image[i][j].rgbtRed = image[i][width-j-1].rgbtRed;

            image[i][width-j-1].rgbtBlue = tmpBlue;
            image[i][width-j-1].rgbtGreen = tmpGreen;
            image[i][width-j-1].rgbtRed = tmpRed;
        }
    }

    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    // The blur filter will replace each pixel's value with the average of the surrounding pixels, on each channel

    // Start by making a copy of the original image
    RGBTRIPLE(*image_)[width] = calloc(height, width * sizeof(RGBTRIPLE));
    copy_image(height, width, image, image_);

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int denominator = 9;
            float avg_b = 0;
            float avg_g = 0;
            float avg_r = 0;
            for (int i_ = -1; i_ < 2; i_++)
            {
                for (int j_ = -1; j_ < 2; j_++)
                {
                    if (i_ + i < 0 || i_ + i >=height || j_ + j < 0 || j_ + j >= width)
                    {
                        denominator--;
                    }
                    else
                    {
                        avg_b += image_[i+i_][j+j_].rgbtBlue;
                        avg_g += image_[i+i_][j+j_].rgbtGreen;
                        avg_r += image_[i+i_][j+j_].rgbtRed;
                    }
                }
            }
            image[i][j].rgbtBlue = round(avg_b / denominator);
            image[i][j].rgbtGreen = round(avg_g / denominator);
            image[i][j].rgbtRed = round(avg_r / denominator);
        }
    }

    free(image_);
    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    // The edges filter detects edges by stepping through each pixel, and passing the neighboring pixels through operators Gx & Gy, which measure the pixel color gradient
    // The output will be greyscale, with lighter colors signifying a higher color gradient

    int Gx[3][3] = {{-1, 0, 1},
                    {-2, 0, 2},
                    {-1, 0, 1}};
    int Gy[3][3] = {{-1, -2, -1},
                     {0,  0,  0},
                     {1,  2,  1}};

    // Start by making a copy of the original image
    RGBTRIPLE(*image_)[width] = calloc(height, width * sizeof(RGBTRIPLE));
    copy_image(height, width, image, image_);

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int xsum_blue  = 0;
            int ysum_blue = 0;
            int xsum_green = 0;
            int ysum_green = 0;
            int xsum_red = 0;
            int ysum_red = 0;
            for (int i_ = -1; i_ < 2; i_++)
            {
                for (int j_ = -1; j_ < 2; j_++)
                {
                    if (i_ + i >= 0 && i_ + i < height && j_ + j >= 0 && j_ + j < width)
                    {
                        xsum_blue += Gx[i_+1][j_+1] * image_[i+i_][j+j_].rgbtBlue;
                        ysum_blue += Gy[i_+1][j_+1] * image_[i+i_][j+j_].rgbtBlue;
                        xsum_green += Gx[i_+1][j_+1] * image_[i+i_][j+j_].rgbtGreen;
                        ysum_green += Gy[i_+1][j_+1] * image_[i+i_][j+j_].rgbtGreen;
                        xsum_red += Gx[i_+1][j_+1] * image_[i+i_][j+j_].rgbtRed;
                        ysum_red += Gy[i_+1][j_+1] * image_[i+i_][j+j_].rgbtRed;
                    }
                }
            }
            image[i][j].rgbtBlue = MIN(round(sqrt((float) xsum_blue * xsum_blue + ysum_blue * ysum_blue)), 255);
            image[i][j].rgbtGreen = MIN(round(sqrt((float) xsum_green * xsum_green + ysum_green * ysum_green)), 255);
            image[i][j].rgbtRed = MIN(round(sqrt((float) xsum_red * xsum_red + ysum_red * ysum_red)), 255);
        }
    }

    free(image_);
    return;
}
