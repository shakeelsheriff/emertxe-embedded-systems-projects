#include "inverted_search.h"
char *flname;
void create_database(Flist *fhead, Wlist *head[]){
    //Traverse through the file list
    while(fhead != NULL){
        read_datafile(fhead, head, fhead -> filename);
        fhead = fhead -> link;
    }
}
    //Function to read contents of a File in the File Linked List
Wlist * read_datafile(Flist *file, Wlist *head[], char *filename){
    flname = filename;
    int flag = 1;
    //Open the File in read mode
    FILE *fptr = fopen(filename, "r");
    
    //declare an array to store the word
    char word[WORD_SIZE];

    while(fscanf(fptr, "%s", word) != EOF){
    // Index the words alphabet order using hashing formula
        int index = tolower(word[0]) % 97;
        //Index Range other than the Alphabets
        if(!(index >= 0 && index <= 25))
            index = 26;
            
        if(head[index] != NULL){
            Wlist *temp = head[index];
            //compare each node word with a new word
            while(temp != NULL){
                if(!strcmp(temp -> word, word)){
                    update_word_count(&temp, filename);
                    flag = 0;
                    break;
                }
                temp = temp -> link;
            }
        }
    //When Words are not repeated
        if((flag == 1) || (head[index] == NULL))
        {
            // function is called when words are unique
            insert_at_last(&head[index], word);
        }  
    }
    return SUCCESS;
}

int update_word_count(Wlist **head, char *filename)
{
    Ltable *temp = (*head)->Tlink;

    // Traverse the list to find and increment the word count if the file name matches
    while (temp -> table_link)
    {
        if (strcmp(temp->filename, filename) == 0)
        {
            temp -> word_count++;
            return SUCCESS;
        }
        temp = temp -> table_link;
    }
    // Check and increment the word count for the last node in the list
    if (strcmp(temp->filename, filename) == 0)
    {
        temp->word_count++;
        return SUCCESS;
    }
    // Allocate memory for a new Ltable node
    Ltable *new_node = malloc(sizeof(Ltable));
    // Check for memory allocation failure
    if (new_node == NULL)
    {
        return FAILURE;
    }
    // Initialize the new node's values
    new_node -> word_count = 1;
    strcpy(new_node->filename, filename);
    new_node -> table_link = NULL;
    // Update the links to include the new node
    temp -> table_link = new_node;
    (*head) -> file_count++;
    return SUCCESS;
}
