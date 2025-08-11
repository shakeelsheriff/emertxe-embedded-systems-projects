#include "inverted_search.h"

void file_validation_n_file_list(Flist **f_head, char *argv[]){
    int i=1, empty;
    while(argv[i] != NULL){
        empty = isFileEmpty(argv[i]);//used to check whether the file is available or not
        if(empty == FILE_NOT_AVAILABLE){
            printf("ERROR: File %s is not available\n",argv[i]);
            printf("Impossible to add the file into the Linked List\n");
            i++;
            continue;
    }
        else if(empty == FILE_EMPTY){
                printf("ERROR: File %s is Empty, has no contents\n",argv[i]);
                printf("Impossible to add the Empty file into the Linked List\n");
                i++;
                continue;
            }
        else{
            int ret_val = to_create_list_of_files(f_head, argv[i]);
            if(ret_val == SUCCESS){
                printf("Successfully Inserted the File %s into the File Linked List\n",argv[i]);
        }
            else if(ret_val == REPEATATION){
                printf("ERROR: This File %s is already present in the File Linked List.\n",argv[i]);
            }
            else{
                printf("ERROR: Unknown Failure");
            }
        }
	i++;
        continue;
    }
}
    
    
    //function to check for availability of file and the contents in the file
    int isFileEmpty(char *filename){
        FILE *fptr = fopen(filename, "r");
        if(fptr == NULL){
            if(errno == ENOENT){ //an error number that is generated as a result of non availability of file
                return FILE_NOT_AVAILABLE;
            }
        }
        fseek(fptr,0,SEEK_END);
        if(ftell(fptr)==0){
            return FILE_EMPTY;
        }
    }

int to_create_list_of_files(Flist **f_head, char *name)
{
    // Allocate memory for a new file node
    Flist *new_node = malloc(sizeof(Flist));
    if (new_node == NULL)
    {
        return FAILURE; // Memory allocation failed
    }

    // Set the file name and link of the new node
    strcpy(new_node->filename, name);
    new_node->link = NULL;

    // If the list is empty, add the new node as the head
    if (*f_head == NULL)
    {
        *f_head = new_node;
        return SUCCESS;
    }

    // Traverse the list to find the end and check for duplicates
    Flist *current = *f_head;
    while (current->link)
    {
        if (strcmp(current->filename, name) == 0)
        {
            free(new_node);
            return REPEATATION; // Duplicate file name found
        }
        current = current->link;
    }

    // Check the last node for duplicates
    if (strcmp(current->filename, name) == 0)
    {
        free(new_node);
        return REPEATATION; // Duplicate file name found
    }

    // Add the new node to the end of the list
    current->link = new_node;
    return SUCCESS;
}
