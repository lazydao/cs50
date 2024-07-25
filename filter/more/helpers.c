#include "helpers.h"
#include <math.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            double sum = image[i][j].rgbtBlue + image[i][j].rgbtGreen + image[i][j].rgbtRed;
            BYTE gray = (BYTE)round(sum / 3);
            image[i][j].rgbtBlue = gray;
            image[i][j].rgbtGreen = gray;
            image[i][j].rgbtRed = gray;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width / 2; j++)
        {
            RGBTRIPLE tmp = image[i][j];
            image[i][j] = image[i][width - 1 - j];
            image[i][width - 1 - j] = tmp;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE new_image[height][width];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            WORD sum_b = 0;
            WORD sum_g = 0;
            WORD sum_r = 0;
            int count = 0;
            for (int ni = i - 1; ni <= i + 1; ni++)
            {
                for (int nj = j - 1; nj <= j + 1; nj++)
                {
                    if (ni >= 0 && ni < height && nj >= 0 && nj < width)
                    {
                        sum_b = sum_b + image[ni][nj].rgbtBlue;
                        sum_g = sum_g + image[ni][nj].rgbtGreen;
                        sum_r = sum_r + image[ni][nj].rgbtRed;
                        count = count + 1;
                    }
                }
            }
            new_image[i][j].rgbtBlue = (BYTE)round((double)sum_b / count);
            new_image[i][j].rgbtGreen = (BYTE)round((double)sum_g / count);
            new_image[i][j].rgbtRed = (BYTE)round((double)sum_r / count);
        }
    }
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = new_image[i][j];
        }
    }
    return;
}

BYTE clamp(double gx, double gy)
{
    double value = sqrt(gx * gx + gy * gy);
    if (value > 255)
    {
        value = 255;
    }
    return (BYTE)round(value);
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    int Gx[3][3] = {
        {-1, 0, 1},
        {-2, 0, 2},
        {-1, 0, 1}};
    int Gy[3][3] = {
        {-1, -2, -1},
        {0, 0, 0},
        {1, 2, 1}};
    RGBTRIPLE new_image[height][width];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            double gx_b = 0;
            double gx_g = 0;
            double gx_r = 0;
            double gy_b = 0;
            double gy_g = 0;
            double gy_r = 0;
            for (int ni = i - 1; ni <= i + 1; ni++)
            {
                for (int nj = j - 1; nj <= j + 1; nj++)
                {
                    if (ni >= 0 && ni < height && nj >= 0 && nj < width)
                    {
                        gx_b = gx_b + image[ni][nj].rgbtBlue * Gx[ni - i + 1][nj - j + 1];
                        gx_g = gx_g + image[ni][nj].rgbtGreen * Gx[ni - i + 1][nj - j + 1];
                        gx_r = gx_r + image[ni][nj].rgbtRed * Gx[ni - i + 1][nj - j + 1];
                        gy_b = gy_b + image[ni][nj].rgbtBlue * Gy[ni - i + 1][nj - j + 1];
                        gy_g = gy_g + image[ni][nj].rgbtGreen * Gy[ni - i + 1][nj - j + 1];
                        gy_r = gy_r + image[ni][nj].rgbtRed * Gy[ni - i + 1][nj - j + 1];
                    }
                }
            }
            new_image[i][j].rgbtBlue = clamp(gx_b, gy_b);
            new_image[i][j].rgbtGreen = clamp(gx_g, gy_g);
            new_image[i][j].rgbtRed = clamp(gx_r, gy_r);
        }
    }
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = new_image[i][j];
        }
    }
    return;
}
