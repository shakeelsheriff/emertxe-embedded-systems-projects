#include "inverted_search.h"

void update_database(Wlist *head[], Flist **fhead){
    // Prompt the user for file to update the database
    char filename[FNAME_SIZE];
    printf("To Update the Database, Enter the File name : ");
    scanf("%s", filename);
    // Validate the file and add to the file list
    int empty = isFileEmpty(filename); // used to check whether the file is available or not
    if (empty == FILE_NOT_AVAILABLE) {
        printf("ERROR: File %s is not available\n", filename);
        printf("Impossible to add the file into the Linked List\n");
    } else if (empty == FILE_EMPTY) {
        printf("ERROR: File %s is Empty, has no contents\n", filename);
        printf("Impossible to add the Empty file into the Linked List\n");
    } else {
        // Allocate memory for a new node in the File List
        Flist *new_node = malloc(sizeof(Flist));
        if (new_node == NULL) {
            printf("ERROR: Failed to allocate memory for the file %s you have updated\n", filename); // Memory allocation failed
        } else {
            // Set the file name and link of the new node
            strcpy(new_node->filename, filename);
            new_node->link = NULL;
            // If the list is empty, add the new node as the head
            if (*fhead == NULL) {
                *fhead = new_node;
                printf("Successfully Updated File %s in the File list Database, as the first file.\n", filename);
            } else {
                // Traverse the list to find the end and check for duplicates
                Flist *temp = *fhead;
                while (temp->link != NULL) {
                    if (strcmp(temp->filename, filename) == 0) {
                        free(new_node);
                        //printf("File %s is already present in the File list Database\n", filename); // Duplicate file name found
                    }
                    temp = temp->link;
                }
                // Check the last node for duplicates
                if (strcmp(temp->filename, filename) == 0) {
                    free(new_node);
                    //printf("ERROR: File %s is already present in the File list Database\nCannot Update it anymore!!\n", filename); // Duplicate file name found
                }
                // Add the new node to the end of the list
                temp->link = new_node;
            }

            int ret_val = to_create_list_of_files(fhead, filename); // Adding Filename to the File List
            // If Duplicate file name is passed
            if (ret_val == REPEATATION) {
                printf("ERROR: File %s is already present in the File list Database\nCannot Update it anymore!!\n", filename);
            } else if (ret_val == FAILURE) {
                printf("ERROR: Unknown Failure stopped update of File");
            } else {
                printf("Successfully Updated the File %s into the File Linked List\n", filename);

                Flist *temp = *fhead;
                while (temp != NULL) {
                    if (!(strcmp(temp->filename, filename))) {
                        create_database(temp, head); // Updating the database with the new file
                    }
                    temp = temp->link;
                }

                printf("Successful : Database has been updated\n");
            }
        }
    }
}

