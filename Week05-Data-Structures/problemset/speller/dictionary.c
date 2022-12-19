// Implements a dictionary's functionality

#include <ctype.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <strings.h>
#include <stdlib.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
} node;

// TODO: Choose number of buckets in hash table
const unsigned int N = 26 + 26 * 26 + 26 * 26 * 26;

// Hash table
node *table[N];

int num_words = 0;

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // TODO

    // Compute hash value of the word and compare it with every collision
    node *key = table[hash(word)];
    while (key)
    {
        if (strcasecmp(key->word, word) == 0)
        {
            return true;
        }
        key = key->next;
    }

    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // TODO: Improve this hash function
    // return toupper(word[0]) - 'A';

    if (strlen(word) == 1)
    {
        return toupper(word[0]) - 'A';
    }
    else if (strlen(word) == 2)
    {
        // Instead of 1, takes two letters of the word and calculate index of table
        return 26 + (26 * (toupper(word[0])) - 'A') + (toupper(word[1]) - 'A');
    }
    else
    {
        // Now takes 3 :)
        return 26 + (26 * 26) + (26 * 26 * ((toupper(word[0])) - 'A')) + (26 * (toupper(word[1]) - 'A')) + toupper(word[2]) - 'A';
    }
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // TODO
    // Open file and validate
    FILE *file = fopen(dictionary, "r");
    if (file == NULL)
    {
        return false;
    }

    // Memory to store dictionary word
    char *dict_word = malloc(LENGTH + 1);
    if (dict_word == NULL)
    {
        fclose(file);
        return false;
    }

    // Read and Store every dictionary word into hash table
    while (fscanf(file, "%s", dict_word) != EOF)
    {
        // Declare a node to add in hash table
        node *n = malloc(sizeof(node));
        if (n == NULL)
        {
            free(dict_word);
            fclose(file);
            return false;
        }

        // Store dict_word in n->word
        strcpy(n->word, dict_word);
        num_words++;

        // Get index of hash table
        int index = hash(dict_word);

        // Add word to beginning
        n->next = table[index];
        table[index] = n;
    }

    free(dict_word);
    fclose(file);
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    // TODO
    return num_words;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    node *tmp, *cur;

    // Delete every collisions for each table index
    for (int i = 0; i < N; i++)
    {
        if (table[i] == NULL) // No word
        {
            continue;
        }

        cur = table[i];
        while (cur != NULL)
        {
            tmp = cur;
            cur = cur->next;
            free(tmp);
        }
    }
    return true;
}
