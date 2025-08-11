#include "inverted_search.h"

void write_databasefile(Wlist *head, FILE *databasefile);

void save_database(Wlist *head[])
{
    // Prompt the user for the filename
    char filename[FNAME_SIZE];
    printf("Enter the filename: ");
    scanf("%s", filename);

    FILE *fptr = fopen(filename, "w");
    if (fptr == NULL) {
        perror("Error opening file");
        return;
    }

    for (int i = 0; i < 27; i++) {
        if (head[i] != NULL) {
            // Call write_databasefile to write the data
            write_databasefile(head[i], fptr);
        }
    }

    fclose(fptr);
    printf("Successful: Database has been saved to %s\n", filename);
}

void write_databasefile(Wlist *head, FILE *databasefile)
{
    while (head != NULL) {
        int index = tolower(*(head->word)) - 'a';
        if (index < 0 || index >= 26) {
            index = 26; // For special characters or non-alphabetic characters
        }
        fprintf(databasefile, "#[%d]\n", index); // Write the index value to the output file

        fprintf(databasefile, "Word: [%s], File count: %d, Files: ", head->word, head->file_count);
        Ltable *temp_table = head->Tlink;

        // Iterate through the linked list of Ltable
        while (temp_table != NULL) {
            fprintf(databasefile, "File: %s, Word count: %d\n", temp_table->filename, temp_table->word_count);
            temp_table = temp_table->table_link;
        }

        head = head->link;
    }
}
