#include <math.h>
#include <stdio.h>
#include "helpers.h"

// Function to caculate avg of a pixel and its surrounding values
RGBTRIPLE surrounding_avg(int height, int width, RGBTRIPLE arr[height][width], int row, int col);

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    // For every pixel value (rgb), calculate average of all three to get new pixel value (grey)
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            BYTE red = image[i][j].rgbtRed;
            BYTE green = image[i][j].rgbtGreen;
            BYTE blue = image[i][j].rgbtBlue;
            BYTE grey_val = round((red + green + blue) / 3.0);

            // Change original pixel to this new pixel
            image[i][j].rgbtRed = grey_val;
            image[i][j].rgbtGreen = grey_val;
            image[i][j].rgbtBlue = grey_val;
        }
    }
    return;
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    // For every pixel, calculate new pixel value using sepia formula
    // Make sure to cap the pixel value at 255
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            BYTE red = image[i][j].rgbtRed;
            BYTE green = image[i][j].rgbtGreen;
            BYTE blue = image[i][j].rgbtBlue;

            int sepia_red = round((0.393 * red) + (0.769 * green) + (0.189 * blue));
            int sepia_green = round((0.349 * red) + (0.686 * green) + (0.168 * blue));
            int sepia_blue = round((0.272 * red) + (0.534 * green) + (0.131 * blue));

            BYTE new_red = sepia_red < 255 ? sepia_red : 255;
            BYTE new_green = sepia_green < 255 ? sepia_green : 255;
            BYTE new_blue = sepia_blue < 255 ? sepia_blue : 255;

            image[i][j].rgbtRed = new_red;
            image[i][j].rgbtGreen = new_green;
            image[i][j].rgbtBlue = new_blue;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    // For every row, swap its first half pixel values with second half pixel values
    for (int i = 0; i < height; i++)
    {
        for (int j = 0, half_width = width / 2; j < half_width; j++)
        {
            RGBTRIPLE pixel = image[i][j];

            image[i][j] = image[i][width - j - 1];

            image[i][width - j - 1] = pixel;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{

    // Make a copy of array and fill it with avg pixel values
    RGBTRIPLE copy[height][width];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            copy[i][j] = surrounding_avg(height, width, image, i, j);
        }
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = copy[i][j];
        }
    }

    return;
}

RGBTRIPLE surrounding_avg(int height, int width, RGBTRIPLE arr[height][width], int row, int col)
{
    // Check for valid rows and cols
    int start_row = row > 0 ? row - 1 : 0;
    int end_row = row < height - 2 ? row + 1 : height - 1;

    int start_col = col > 0 ? col - 1 : 0;
    int end_col = col < width - 2 ? col + 1 : width - 1;

    // Count number of surrounding pixels
    float sur_px_cnt = 0.0;

    // Calculate avg of a pixel value and its surrounding pixels
    int sum_red = 0;
    int sum_green = 0;
    int sum_blue = 0;
    for (int i = start_row; i <= end_row; i++)
    {
        for (int j = start_col; j <= end_col; j++)
        {
            sum_red += arr[i][j].rgbtRed;
            sum_green += arr[i][j].rgbtGreen;
            sum_blue += arr[i][j].rgbtBlue;
            sur_px_cnt++;
        }
    }

    int avg_red = round(sum_red / sur_px_cnt);
    int avg_green = round(sum_green / sur_px_cnt);
    int avg_blue = round(sum_blue / sur_px_cnt);

    // Return result
    RGBTRIPLE result;
    result.rgbtRed = avg_red;
    result.rgbtGreen = avg_green;
    result.rgbtBlue = avg_blue;

    return result;
}
