
#ifndef INVERTED_SEARCH_H
#define INVERTED_SEARCH_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <ctype.h>
#include <errno.h>
#include <stdio_ext.h>
#include <stdarg.h>  // Include this for va_list, va_start, va_end

// Color codes for terminal output
#define RESET_COLOR   "\033[0m"
#define RED_COLOR     "\033[31m"
#define GREEN_COLOR   "\033[32m"
#define YELLOW_COLOR  "\033[33m"
#define CYAN_COLOR    "\033[36m"
#define MAGENTA_COLOR "\033[35m"

/* defining macros */
#define FAILURE    -1
#define SUCCESS    0
#define FNAME_SIZE 15
#define WORD_SIZE  10
#define FILE_EMPTY -2
#define FILE_NOT_AVAILABLE  -3
#define REPEATATION    -4

/* Structure for file list */
typedef char data_t;
typedef struct file_node
{
    data_t filename[FNAME_SIZE];
    struct file_node *link;
}Flist;

/* Structure for link table */
typedef struct linkTable_node
{
    /* data */
    int word_count;
    data_t filename[FNAME_SIZE];
    struct linkTable_node *table_link;
}Ltable;


/* Structure to store word count */
typedef struct word_node
{
    /* data */
    int file_count;
    data_t word[WORD_SIZE];
    Ltable *Tlink;
    struct  word_node *link;
}Wlist;

int to_create_list_of_files(Flist **fhead, char *name);
void create_database(Flist *fhead, Wlist *head[]);
/* read contents of a file */
Wlist * read_datafile(Flist *file, Wlist *head[], char *filename);
/* create word_list */
int insert_at_last(Wlist **head, data_t *data);
/* update linktable */
int update_link_table(Wlist **head);
/* update */
int update_word_count(Wlist ** head, char *filename);
/* print_word_count */
int print_word_count(Wlist *head);
/* searching a word */
void search(Wlist *hhead, char *word);
/* display */
void display_database(Wlist *head[]);
/* save_database */
void save_database(Wlist *head[]);
/* write database file */
void write_databasefile(Wlist *head, FILE* databasefile);
/* update */
void update_database(Wlist *head[], Flist **fhead);
/* checking file status*/
int isFileEmpty(char *filename);
/* validating files */
void file_validation_n_file_list(Flist **fhead, char *argv[]);

#endif
