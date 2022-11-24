#include <cs50.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

void cipher(int k, string text); // Function to convert plaintext to ciphertext

int main(int argc, string argv[])
{

    // Taking user input
    // Checking for correct number of command line arguments
    if (argc != 2)
    {
        printf("Please enter correct number of arguments i.e. 1.\n");
        return 1;
    }

    // Checking for correct form of command line argument
    for (int i = 0; i < strlen(argv[1]); i++)
    {
        if (!isdigit(argv[1][i]))
        {
            printf("Usage: ./caesar key\n");
            return 1;
        }
    }
    // If all good
    int k = atoi(argv[1]) % 26;
    // printf("key: %i\n", k);
    string text = get_string("Enter Text: ");

    // Printing plain text
    printf("plaintext:  %s", text);
    printf("\n");

    // Computing cipher text
    cipher(k, text);
}

void cipher(int k, string text)
{
    printf("ciphertext: ");
    for (int i = 0; i < strlen(text); i++)
    {
        char cur_char = text[i];
        // Check if it's a alphabet
        if (isalpha(cur_char))
        {
            // encrypt it
            int encrypted = (int)cur_char + k;

            // If char goes out of case-sensitive alphabet bound, bring it back
            if ((encrypted > 122) || (isupper(cur_char) && encrypted > 90))
            {
                printf("%c", (char)(encrypted - 26));
            }
            else
            {
                printf("%c", (char)encrypted);
            }
        }
        else
        {
            printf("%c", cur_char);
        }
    }
    printf("\n");
}
