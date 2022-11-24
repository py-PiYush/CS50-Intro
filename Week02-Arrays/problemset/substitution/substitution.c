#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

bool isValid(string key);             // Function to check if key contains 26 unique alphabets
void cipher(string key, string text); // Function to convert and print plaintext to ciphertext

int main(int argc, string argv[])
{
    // Check for correct number of command line arguments
    if (argc != 2)
    {
        printf("Usage: ./substitution key\n");
        return 1;
    }

    // Check if key contains 26 unique alphabets
    string key = argv[1];
    if (strlen(key) != 26)
    {
        printf("Key must contain 26 characters.\n");
        return 1;
    }
    if (!isValid(key))
    {
        printf("Key must contain unique alphabets.\n");
        return 1;
    }

    // Get user input
    string plaintext = get_string("Enter Text: ");
    printf("plaintext: %s\n", plaintext);

    // Compute and print ciphertext
    cipher(key, plaintext);
}

bool isValid(string key)
{
    int count[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    for (int i = 0; i < strlen(key); i++)
    {
        char cur_char = tolower(key[i]);
        // Check for alphabet
        if (!isalpha(cur_char))
        {
            return false;
        }

        // Check for count
        int cur_count = count[(int)cur_char - 97];
        if (cur_count != 0)
        {
            return false;
        }
        count[(int)cur_char - 97]++;
    }
    return true;
}

void cipher(string key, string text)
{
    printf("ciphertext: ");

    for (int i = 0; i < strlen(text); i++)
    {
        char cur_char = text[i];
        // Check for alphabet
        if (isalpha(cur_char))
        {
            // Convert uppercase char to its corresponding uppercase key
            if (isupper(cur_char))
            {
                char new = toupper(key[(int)cur_char - 65]);
                printf("%c", new);
            }

            // Convert lowercase char to its corresponding lowercase key
            else
            {
                char new = tolower(key[(int)cur_char - 97]);
                printf("%c", new);
            }
        }

        // If not alphabet, print as it is
        else
        {
            printf("%c", cur_char);
        }
    }
    printf("\n");
}