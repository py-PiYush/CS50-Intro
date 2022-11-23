#include <cs50.h>
#include <stdio.h>

int get_height(void);  // Function to get user input
void print_tower(int); // Function to print the tower

int main(void)
{
    // Get height from user
    int n = get_height();

    // Print result
    print_tower(n);
}

int get_height(void)
{
    int n;
    do
    {
        n = get_int("Enter height of the tower: ");
    } while (n < 1 || n > 8);
    return n;
}

void print_tower(int n)
{
    for (int i = 1; i <= n; i++)
    {

        // For spaces left of the tower
        for (int k = n - i; k > 0; k--)
        {
            printf(" ");
        }

        // Left tower
        for (int j = 0; j < i; j++)
        {
            printf("#");
        }

        printf("  ");

        // Right tower
        for (int m = 0; m < i; m++)
        {
            printf("#");
        }

        printf("\n");
    }
}