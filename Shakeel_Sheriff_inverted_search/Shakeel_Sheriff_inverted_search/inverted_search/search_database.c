#include "inverted_search.h"

 void search(Wlist *head, char *word){
    //Check if list is empty or not
     if(head == NULL){
         printf("Search not possible as the Database is Empty");
     }

     //If list is not empty traverse through the list to search for search_data

     while(head != NULL){
        //compare search_data with each node words available
         if(!(strcmp(head->word, word))) //
            {
                printf("Word %s is found in %d file(s) \n", word, head->file_count);
                Ltable *Thead = head->Tlink;
                while (Thead)
                {
                    printf("In file : %s %d time(s)\n", Thead->filename, Thead->word_count);
                    Thead = Thead->table_link;
                }
                printf("\n");
            }
         head = head->link;
     }
     printf("Search Data not found in the Database");
 }
