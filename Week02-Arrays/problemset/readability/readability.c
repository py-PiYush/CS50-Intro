#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

int count_letters(string text);     // Function to count letters in the text
int count_words(string text);       // Function to count words in the text
int count_sentences(string text);   // Function to count sentences in the text
int coleman_liau(float l, float s); // Function to compute coleman-liau index
void print_result(int index);       // Function to print grade based on c-l index

int main(void)
{
    // Get user input
    string text = get_string("Enter Text: ");

    // Count letters, words and sentences
    int letters = count_letters(text);
    int words = count_words(text);
    int sentences = count_sentences(text);

    // Calculate Coleman-Liau index
    float L = (letters * 100) / (float)words;
    float S = (sentences * 100) / (float)words;
    int index = coleman_liau(L, S);

    // Print Result
    print_result(index);
    // printf("Text: %s\nLetters: %i\nWords: %i\nSentences: %i\n", text, letters, words, sentences);
}

int count_letters(string text)
{
    int letter_count = 0;
    for (int i = 0; i < strlen(text); i++)
    {
        // If char is a alphanumeric
        if (isalnum(text[i]))
        {
            letter_count++;
        }
    }
    return letter_count;
}

int count_words(string text)
{
    int word_count = 1;
    for (int i = 0; i < strlen(text); i++)
    {
        // Check for spaces
        // Assumptions
        // will contain at least one word;
        // will not start or end with a space; and
        // will not have multiple spaces in a row.
        if (isspace(text[i]))
        {
            word_count++;
        }
    }
    return word_count;
}

int count_sentences(string text)
{
    // Assumptions
    // consider any sequence of characters that ends with a . or a ! or a ? to be a sentence
    int sentence_count = 0;
    for (int i = 0; i < strlen(text); i++)
    {
        if (text[i] == '.' || text[i] == '!' || text[i] == '?')
        {
            sentence_count++;
        }
    }
    return sentence_count;
}

int coleman_liau(float l, float s)
{
    int index = round(0.0588 * l - 0.296 * s - 15.8);
    return index;
}

void print_result(int index)
{
    if (index >= 16)
    {
        printf("Grade 16+\n");
    }
    else if (index < 1)
    {
        printf("Before Grade 1\n");
    }
    else
    {
        printf("Grade %i\n", index);
    }
}