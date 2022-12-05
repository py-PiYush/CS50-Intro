#include <math.h>
#include <stdio.h>
#include "helpers.h"

RGBTRIPLE surrounding_avg(int height, int width, RGBTRIPLE arr[height][width], int row, int col);
RGBTRIPLE sobel(int height, int width, int row, int col, RGBTRIPLE image[height][width]);
void make_color_matrix(int height, int width, int row, int col, RGBTRIPLE image[height][width], RGBTRIPLE color_matrix[3][3]);
int cap255(int value);

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

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    // Make a copy array to fill it with new calculated values
    RGBTRIPLE copy[height][width];

    // For every pixel, calculate sobel val
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            copy[i][j] = sobel(height, width, i, j, image);
        }
    }

    // Change original image
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = copy[i][j];
        }
    }
    return;
}

// Caculate and return Sobel value of a pixel
RGBTRIPLE sobel(int height, int width, int row, int col, RGBTRIPLE image[height][width])
{
    RGBTRIPLE color_matrix[3][3] =
        {
            {{0, 0, 0},
             {0, 0, 0},
             {0, 0, 0}},
            {{0, 0, 0},
             {0, 0, 0},
             {0, 0, 0}},
            {{0, 0, 0},
             {0, 0, 0},
             {0, 0, 0}}};
    make_color_matrix(height, width, row, col, image, color_matrix);

    int gx_kernel[3][3] =
        {
            {-1, 0, 1},
            {-2, 0, 2},
            {-1, 0, 1}};
    int gy_kernel[3][3] =
        {
            {-1, -2, -1},
            {0, 0, 0},
            {1, 2, 1}};
    int gx_red = 0, gy_red = 0, gx_green = 0, gy_green = 0, gx_blue = 0, gy_blue = 0;

    // Multiple corresponding values of color_matrix and gx_kernel and take sum
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            // RED
            gx_red += color_matrix[i][j].rgbtRed * gx_kernel[i][j];
            gy_red += color_matrix[i][j].rgbtRed * gy_kernel[i][j];

            // GREEN
            gx_green += color_matrix[i][j].rgbtGreen * gx_kernel[i][j];
            gy_green += color_matrix[i][j].rgbtGreen * gy_kernel[i][j];

            // BLUE
            gx_blue += color_matrix[i][j].rgbtBlue * gx_kernel[i][j];
            gy_blue += color_matrix[i][j].rgbtBlue * gy_kernel[i][j];
        }
    }

    RGBTRIPLE new_pixel;
    new_pixel.rgbtRed = cap255(round(sqrt((double)(gx_red * gx_red + gy_red * gy_red))));
    new_pixel.rgbtGreen = cap255(round(sqrt((double)(gx_green * gx_green + gy_green * gy_green))));
    new_pixel.rgbtBlue = cap255(round(sqrt((double)(gx_blue * gx_blue + gy_blue * gy_blue))));
    return new_pixel;
}

// make color_matrix of neighbouring 3x3 values of a particular pixel
void make_color_matrix(int height, int width, int row, int col, RGBTRIPLE image[height][width], RGBTRIPLE color_matrix[3][3])
{
    // Check for valid rows and cols
    int start_row = row > 0 ? row - 1 : 0;
    int end_row = row < height - 2 ? row + 1 : height - 1;

    int start_col = col > 0 ? col - 1 : 0;
    int end_col = col < width - 2 ? col + 1 : width - 1;

    // Fill the color_matrix with corresponding values from image matrix
    for (int i = start_row; i <= end_row; i++)
    {
        for (int j = start_col; j <= end_col; j++)
        {
            color_matrix[i - row + 1][j - col + 1] = image[i][j];
        }
    }
}

// Cap pixel color value at 255
int cap255(int value)
{
    return value < 255 ? value : 255;
}
