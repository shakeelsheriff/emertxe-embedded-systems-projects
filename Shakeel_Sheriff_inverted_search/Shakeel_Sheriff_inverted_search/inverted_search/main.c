#include "inverted_search.h"
/*-----------------------------------------------
Name : Shakeel Sheriff.M
Date : 24/07/2024
Project Title : Inverted Search
  -----------------------------------------------*/
// Function prototypes
void print_colored(const char* color, const char* format, ...);

int main(int argc, char *argv[]){
    system("clear");
    Wlist *head[27] = {NULL};
    char ch;
    char word[WORD_SIZE];

    // Validation of CLA
    if(argc <= 1){
        print_colored(RED_COLOR, "Enter the valid number of arguments\n");
        print_colored(RED_COLOR, "USAGE: ./Slist.exe file1.txt file2.txt........\n");
        return 0;
    }

    // Declare a file head pointer to add the validated files to the file Linked List
    Flist *fhead = NULL;

    // Validation of the filename
    file_validation_n_file_list(&fhead, argv);

    // Check if File Linked List is created or not
    if(fhead == NULL){
        print_colored(YELLOW_COLOR, "Warning: No Files were added to the Linked list\n");
        print_colored(YELLOW_COLOR, "*************PROCESS TERMINATED**************\n");
        return 0;
    }

    char yes_no;
    do{
        int choice;

        // Display menu options
        printf(CYAN_COLOR "----------------------------------------------------------\n" RESET_COLOR);
        printf(MAGENTA_COLOR "        Choose an Operation:\n" RESET_COLOR);
        printf(MAGENTA_COLOR "   %-5s %-25s\n" RESET_COLOR, "[1]", "Create database");
        printf(MAGENTA_COLOR "   %-5s %-25s\n" RESET_COLOR, "[2]", "Display database");
        printf(MAGENTA_COLOR "   %-5s %-25s\n" RESET_COLOR, "[3]", "Update database");
        printf(MAGENTA_COLOR "   %-5s %-25s\n" RESET_COLOR, "[4]", "Search");
        printf(MAGENTA_COLOR "   %-5s %-25s\n" RESET_COLOR, "[5]", "Save database");
        printf(CYAN_COLOR "----------------------------------------------------------\n" RESET_COLOR);
        printf("Enter your choice: ");
        scanf("%d", &choice);
        // Execute the selected option
        switch (choice)
        {
            case 1:
                create_database(fhead, head);
                break;

            case 2:
                display_database(head);
                break;

            case 3:
                update_database(head, &fhead);
                break;

            case 4:
                printf("Enter the word to search: ");
                scanf("%s", word);
                search(head[tolower(word[0]) % 97], word);
                break;

            case 5:
                save_database(head);
                break;

            default:
                print_colored(RED_COLOR, "Please enter a valid option.\n");
                break;
        }

        // Prompt to continue or exit
        print_colored(CYAN_COLOR, "Do you want to continue?\nEnter y/Y to continue or n/N to exit: ");
        __fpurge(stdin); // Clear input buffer
        scanf(" %c", &yes_no); // Handle character input

    } while (yes_no == 'y' || yes_no == 'Y');

    print_colored(GREEN_COLOR, "Thank you for using the program. Goodbye!\n");

    return SUCCESS;
}

// Function to print colored text
void print_colored(const char* color, const char* format, ...) {
    va_list args;
    va_start(args, format);
    printf("%s", color);
    vprintf(format, args);
    printf(RESET_COLOR);
    va_end(args);
}
