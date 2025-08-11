#include "inverted_search.h"

void display_database(Wlist *head[]) {
    // Header with alignment
    printf("------------------------------------------------------------------------------------------\n");
    printf("[Index]  [Word]               [File Count]  [Files]                    [Word Count]\n");
    printf("------------------------------------------------------------------------------------------\n");
    for (int i = 0; i < 27; i++) {
        if (head[i] != NULL) {
            print_word_count(head[i]);
        }
    }
}

int print_word_count(Wlist *head) {
    // Print each entry with proper alignment
    while (head != NULL) {
        // Print the word list with consistent column width
        printf(" %-6d  %-18s  %-12d", tolower(head->word[0]) % 97, head->word, head->file_count);

        Ltable *Thead = head->Tlink;

        // Print each file with consistent column width
        while (Thead != NULL) {
            printf(" File: %-20s %-10d", Thead->filename, Thead->word_count);
            Thead = Thead->table_link;
        }
        printf("\n");
	printf("------------------------------------------------------------------------------------------\n");
        head = head->link;
    }
    return SUCCESS;
}

