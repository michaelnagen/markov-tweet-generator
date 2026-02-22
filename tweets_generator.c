//Don't change the macros!
#define FILE_PATH_ERROR "Error: incorrect file path"
#define NUM_ARGS_ERROR "Usage: invalid number of arguments"
#define START 0
#define FIRST 1
#define SECOND 2
#define THIRD 3
#define FOURTH 4
#define FIFTH 5
#define MAX_NUM 10
#define MAX_LENGTH 20
#define MAX_DIGET 1000
#define DELIMITERS " \n\t\r"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "markov_chain.h"

int string_to_int(const char *str) {
    char *endptr;
    long num = strtol(str, &endptr, MAX_NUM);
    return (int)num;}

void fill_the_fill(MarkovChain* markov_chain, char* first, char* last)
{if (last == NULL)
    {
        return;
    }
    Node * first_node=get_node_from_database(markov_chain,first);
    if (get_node_from_database(markov_chain,last)==NULL)
    {
        add_to_database(markov_chain,last);
    }
    Node * last_node=get_node_from_database(markov_chain,last);
    if (first == NULL|| is_end_of_sentence(first))
    {
        return;
    }
    add_node_to_frequency_list(first_node->data,last_node->data);
}

int fill_database(FILE *fp, int words_to_read, MarkovChain *markov_chain)
{
    char line[MAX_DIGET];
    int word_count = START;
    char *prev_word = NULL;
    char *curr_word = NULL;

    while (fgets(line, sizeof(line), fp) != NULL &&
           (words_to_read== START|| word_count < words_to_read))
    {
        char *token = strtok(line, DELIMITERS);
        if (!token)
        {
            continue;
        }

        curr_word = token;

        while (curr_word != NULL &&
               (words_to_read == START || word_count < words_to_read))
        {
            char *next_word = strtok(NULL, DELIMITERS);

            fill_the_fill(markov_chain, prev_word, curr_word);
            word_count++;

            prev_word = curr_word;
            curr_word = next_word;
        }

        prev_word = NULL;
        curr_word = NULL;
    }

    return EXIT_SUCCESS;
}

int main(int argc, char *argv[])
{
    if (argc !=FIFTH  && argc != FOURTH)
    {
        fprintf(stdout, NUM_ARGS_ERROR);
        return EXIT_FAILURE;
    }
    FILE *file = fopen(argv[THIRD], "r");
    if (file == NULL)
    {
        fprintf(stdout, FILE_PATH_ERROR);
        return EXIT_FAILURE;
    }
    int seed = string_to_int(argv[FIRST]);
    srand(seed);
    int tweet = string_to_int(argv[SECOND]);
    MarkovChain *new_markov= malloc(sizeof(MarkovChain));
    if ( new_markov== NULL)
    {
        return EXIT_FAILURE;
    }
    LinkedList *new_list = malloc(sizeof(LinkedList));
    if (new_list == NULL)
    {
        free(new_markov);
        return EXIT_FAILURE;
    }
    new_markov->database = new_list;
    new_markov->database->first = NULL;
    new_markov->database->last = NULL;
    new_markov->database->size = START;
    if (argc == FIFTH)
    {
        int num_letter = string_to_int(argv[4]);
        fill_database(file,num_letter,new_markov);
    }
    else
    {
        fill_database(file,START,new_markov);
    }
    for (int i = START; i < tweet; i++)
    {
        fprintf(stdout,"%s %d%s ", "Tweet", i+FIRST, ":" );
        MarkovNode *first_node = get_first_random_node(new_markov);
        generate_tweet(first_node,MAX_LENGTH);
    }
    free_database(&new_markov);
    fclose(file);
return EXIT_SUCCESS;}
