// Implements a dictionary's functionality

#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h>
#include "dictionary.h"
#include <stdio.h>
#include <string.h>
#include <cs50.h>
#include <strings.h>

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets in hash table (366, the sum of the ascii values of z + z + z)
const unsigned int N = 3 * 'z';

// Hash table
node *table[N];
int word_count = 0;

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // Hash word
    int ha = hash(word);

    // Use strcasecmp to compare word to each word in list, return true if it matches.
    // Start with cursor as first item in list, keep moving until get to NULL

    node *cursor = table[ha];

    while (cursor != NULL)
    {
        // if they are the same return true
        if (strcasecmp(cursor->word, word) == 0)
        {
            return true;
        }
        // if they aren't, go to next word
        cursor = cursor->next;
    }
    // if the cursor is NULL, none of the words match and it is spelled incorrectly.
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // Input word, output hash between 0 and N - 1, should give same result no matter capitilization
    return tolower(word[0]) + tolower(word[1]) + tolower(word[2]);
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // Open dictionary file
    FILE *file = fopen(dictionary, "r");
    if (file == NULL)
    {
        return false;
    }

    // Repeats for every word
    char word[LENGTH];
    while (fscanf(file, "%s", word) != EOF)
    {
        // Create new node with malloc
        node *n = malloc(sizeof(node));

        // Return false if NULL
        if (n == NULL)
        {
            return false;
        }

        // Copy word to node
        strcpy(n->word, word);
        n->next = NULL;

        // Hash word to obtain hash value
        int index = hash(word);

        // Insert node into hash table at that location

        // If there isn't anything at that table
        if (table[index] == NULL)
        {
            table[index] = n;
        }
        // Table is already started
        else
        {
            n->next = table[index];
            table[index] = n;
        }
        // add to total amount of words
        word_count++;
    }
    fclose(file);
    // Return true if successful
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    return word_count;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    for (int i = 0; i < N; i++)
    {
        node *head = table[i];
        node *cursor = head;
        node *tmp = head;

        while (cursor != NULL)
        {
            cursor = cursor->next;
            free(tmp);
            tmp = cursor;
        }
    }
    return true;
}