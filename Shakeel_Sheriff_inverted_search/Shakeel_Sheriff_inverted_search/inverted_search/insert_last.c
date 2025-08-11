#include "inverted_search.h"

extern char *flname;
int insert_at_last(Wlist **head, data_t *data){
    //Create a node for Word List
    Wlist *new = malloc(sizeof(Wlist));

    //error check
    if(new == NULL){
        return FAILURE;
    }

    new -> file_count = 1;
    strcpy(new->word, data);
    new -> Tlink = NULL;
    new -> link = NULL;

    //Call function to update link table node
    update_link_table(&new);

    //Check if Wlist is empty or not.
    if(*head == NULL){
    (*head) = new;
    return SUCCESS;
    }

    //If not empty, traverse the list
    Wlist *temp = *head;
    while(temp -> link != NULL){
        temp = temp -> link;
    }
    temp -> link = new;
    return SUCCESS;
}

//Function to update the Link Table List
int update_link_table(Wlist **head){
    
    //create a node
    Ltable *new = malloc(sizeof(Ltable));

    //error check
    if(new == NULL){
        return FAILURE;
    }

    new -> word_count = 1;
    strcpy(new -> filename, flname);
    new -> table_link = NULL;

    (*head) -> Tlink = new;
    return SUCCESS;
}
