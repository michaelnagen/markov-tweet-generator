#include "markov_chain.h"

#include <string.h>
#define START 0
#define SIZE 1

int get_random_number(int max_number)
{
    return rand() % max_number;
}

Node* get_node_from_database(MarkovChain *markov_chain, char *data_ptr)
{
    if (markov_chain==NULL||data_ptr==NULL)
    {
        return NULL;
    }
    Node *current= markov_chain->database->first;
    while (current!=NULL)
    {
        if (strcmp(current->data-> data, data_ptr)==START)
        {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

Node* add_to_database(MarkovChain *markov_chain, char *data_ptr)
{if (markov_chain==NULL||data_ptr==NULL)
{
    return NULL;
}
Node *current = get_node_from_database(markov_chain, data_ptr);
if (current!=NULL)
{   free(data_ptr);
    return current;
}
MarkovNode *new_markov_node= malloc(sizeof(MarkovNode));
    if ( new_markov_node== NULL)
        {free(data_ptr);
        return NULL;
    }
    new_markov_node->data = malloc(strlen(data_ptr) + SIZE);
    if (new_markov_node->data == NULL) {
        free(new_markov_node);
        return NULL;
    }
    strcpy(new_markov_node->data, data_ptr);

    new_markov_node->frequency_list = NULL;
    if (add(markov_chain->database, new_markov_node) != START) {
        free(new_markov_node->data);
        free(new_markov_node);
        return NULL;
    }
    return markov_chain->database->last;
}

int add_node_to_frequency_list(MarkovNode *first_node, MarkovNode *second_node)
{
    if (first_node->frequency_list == NULL)
    {
        first_node->frequency_list = malloc(sizeof(MarkovNodeFrequency));
        if (first_node->frequency_list == NULL) {
            return EXIT_FAILURE;
        }
        first_node->frequency_list_size = START;
    }
    for (int i = START; i < first_node->frequency_list_size; i++)
    {
        if (first_node->frequency_list[i].markov_node == second_node)
        {
            first_node->frequency_list[i].frequency++;
            return EXIT_SUCCESS;
        }
    }
    MarkovNodeFrequency *temp = realloc(first_node->frequency_list, sizeof(MarkovNodeFrequency) * (first_node->frequency_list_size + SIZE));
    if (temp == NULL) {
        return EXIT_FAILURE;
    }
    first_node->frequency_list = temp;

        first_node->frequency_list[first_node->frequency_list_size].markov_node = second_node;
        first_node->frequency_list[first_node->frequency_list_size].frequency = SIZE;
        first_node->frequency_list_size++;

        return EXIT_SUCCESS;
    }



void free_database(MarkovChain **ptr_chain) {
    if (ptr_chain == NULL || *ptr_chain == NULL) return;

    MarkovChain *chain = *ptr_chain;
    LinkedList *list = chain->database;

    if (list != NULL) {
        Node *current = list->first;
        while (current != NULL) {
            MarkovNode *markov_node = current->data;

            if (markov_node != NULL) {

                if (markov_node->frequency_list != NULL) {
                    free(markov_node->frequency_list);
                }

                if (markov_node->data != NULL) {
                    free(markov_node->data);
                }

                free(markov_node);
            }

            Node *temp = current;
            current = current->next;
            free(temp);
        }

        free(list);
    }
    free(chain);
    *ptr_chain = NULL;
}

bool is_end_of_sentence(const char *word)
{
    if (word == NULL || strlen(word) == START)
    {
        return false;
    }

    size_t len = strlen(word);
    return word[len - SIZE] == '.';
}

MarkovNode* get_first_random_node(MarkovChain *markov_chain)
{
    while (SIZE){
        int num= get_random_number(markov_chain->database->size);
        int count = START;
        Node *current = markov_chain->database->first;
        while (current != NULL && count < num)
        {
            current = current->next;
            count++;
        }
        if (is_end_of_sentence(current->data->data))
        {
            continue;
        }
        return current->data;

    }
}

MarkovNode* get_next_random_node(MarkovNode *cur_markov_node)
{int total_frequency = START;
    for (int i = START; i < cur_markov_node->frequency_list_size; i++) {
        total_frequency += cur_markov_node->frequency_list[i].frequency;
    }
    int score = get_random_number(total_frequency);
    int cumulative = START;
    for (int i = START; i < cur_markov_node->frequency_list_size; i++) {
        cumulative += cur_markov_node->frequency_list[i].frequency;
        if (score < cumulative) {
            return cur_markov_node->frequency_list[i].markov_node;
        }
    }

    return NULL;
}

void generate_tweet(MarkovNode *first_node, int max_length)
{
    if (first_node == NULL || max_length < (SIZE+SIZE))
    {
        return;
    }
    MarkovNode *current_node = first_node;

    for (int i = SIZE; i < max_length; i++)
    {
        if (is_end_of_sentence(current_node->data)|| current_node->frequency_list == NULL)
        {
            fprintf(stdout, "%s",current_node->data);
            break;
        }
        fprintf(stdout, "%s ",current_node->data);
        current_node = get_next_random_node(current_node);

    }
   fprintf(stdout,"\n");
}