#include <cs50.h>
#include <stdio.h>
#include <stdbool.h>

long get_credit_card_number(void); // Function to get user's credit card number
bool check_validity(long);         // Function to check whether the number is valid or not
void print_result(long);           // Function to print the card company
int count_digits(long);            // Function to count digits
int add_digits(int);               // Function to add digits of a number
int find_first_two(long);          // Function to find first two digits of the number

int main(void)
{
    // Get Card Number
    long card_no = get_credit_card_number();

    // If valid
    if (check_validity(card_no))
    {
        // Print result
        print_result(card_no);
    }
    else
    {
        printf("INVALID\n");
    }
}

long get_credit_card_number()
{
    long number = get_long("Enter Card Number: ");
    return number;
}

bool check_validity(long n)
{
    // If number of digits is not one of 13, 15, 16, it is invalid
    int no_of_digits = count_digits(n);
    if (no_of_digits != 15 && no_of_digits != 16 && no_of_digits != 13)
    {
        return false;
    }

    // Luhn's Algorithm
    int sum_of_mul = 0;
    int sum_of_not_mul = 0;
    while (n > 0)
    {
        sum_of_mul += add_digits(2 * ((n % 100) / 10)); // Adding sum of digits of every second digit multiplied by 2
        sum_of_not_mul += n % 10;                       // Adding every other digit
        n /= 100;
    }

    // Check if last digit is 0
    int check_sum = sum_of_mul + sum_of_not_mul;
    if (check_sum % 10 != 0)
    {
        return false;
    }
    return true;
}

void print_result(long n)
{
    int no_of_digits = count_digits(n);
    int first_two = find_first_two(n);

    // Checking conditions for Amex
    if (no_of_digits == 15 && (first_two == 34 || first_two == 37))
    {
        printf("AMEX\n");
    }
    // Checking conditions for Visa
    else if ((no_of_digits == 13 || no_of_digits == 16) && (first_two / 10 == 4))
    {
        printf("VISA\n");
    }
    // Checking conditions for Mastercard
    else if (no_of_digits == 16 && first_two / 10 == 5 && first_two % 10 >= 1 && first_two % 10 <= 5)
    {
        printf("MASTERCARD\n");
    }
    else
    {
        printf("INVALID\n");
    }
}

int count_digits(long n)
{
    int res = 0;
    while (n > 0)
    {
        n /= 10;
        res++;
    }
    return res;
}

int add_digits(int n)
{
    int res = 0;
    while (n > 0)
    {
        res += n % 10;
        n /= 10;
    }
    return res;
}

int find_first_two(long n)
{
    while (n > 100)
    {
        n /= 10;
    }
    return n;
}