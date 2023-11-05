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


// Number of buckets in hash table

const unsigned int N = 3 * 'z';

// Hash table

node *table[N];
int word_count = 0;


// Returns true if word is in dictionary, else false

bool check(const char *word)

{

    node *p = table[hash(word)];


    while(p != NULL)

    {

        // 0 means no dif betw 2 strings

        if (strcasecmp(word, p->word) == 0)

        {

            return true;

        }

        p = p->next;

    }

    return false;

}


// Hashes word to a number

unsigned int hash(const char *word)

{

    //return ((int) tolower(word[0]) - 97) % N;

    return tolower(word[0]) + tolower(word[1]) + tolower(word[2]);

}


// Loads dictionary into memory, returning true if successful, else false

bool load(const char *dictionary)

{

    // TODO

    FILE *dictionaryFile = fopen(dictionary, "r");

    if (dictionaryFile != NULL)

    {

        char word[LENGTH];

        while (fscanf(dictionaryFile, "%s", word) != EOF)

        {

            int bucket = hash(word);

            //n = malloc(sizeof(node));

            node *n = malloc(sizeof(node));

            if (n == NULL) return false;

            strcpy(n->word, word);

            n->next = NULL;


            if (table[bucket] == NULL)

            {

                //table[bucket] = malloc(sizeof(node));

                table[bucket] = n;

            }

            else

            {

                n->next = table[bucket];

                table[bucket] = n;

            }

            word_count++;

        }

        fclose(dictionaryFile);

        return true;

    }

    return false;

}


// Returns number of words in dictionary if loaded, else 0 if not yet loaded

unsigned int size(void)

{
    // iterate over linked lists in hash table
    /*int num = 0;

    for (int i = 0; i < N; i++) {

        node *n = table[i];
        while(n != NULL)

        {
            num++;
            n = n->next;
        }
    }

    return num; */

    return word_count;

    //if (node->next != NULL)

    //{

        //node = node->next;

        //return numberOfnodes = bucket++;

    //}

    //else return 0;

}


// Unloads dictionary from memory, returning true if successful, else false

bool unload(void)

{

    for (int i = 0; i < N; i++)

    {

        node *head = table[i];


        node *cursor = head;


        node *tmp = head;


        // if p exists

        while(cursor != NULL)

        {

            cursor = cursor->next;

            free(tmp);

            tmp = cursor;

        }

    }

    return true;

}


