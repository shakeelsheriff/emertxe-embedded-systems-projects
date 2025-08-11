#ifndef VIEW_DATA_H
#define VIEW_DATA_H

#include "mp3_types.h" // Includes user-defined data types

/*
 * Structure to store information for MP3 tag details and output data
 */
typedef struct _TagInfo
{
    /* Pointer to the source MP3 file */
    FILE* fptr_mp3;

    /* Frame identifier (4 bytes) */
    char frame_Id[4];

    /* Title of the MP3 */
    char* title_tag;
    u_int title_tag_size;

    /* Artist of the MP3 */
    char* artist_tag;
    u_int artist_tag_size;

    /* Album of the MP3 */
    char* album_tag;
    u_int album_tag_size;

    /* Release year of the MP3 */
    char* year;
    u_int year_size;

    /* Content type of the MP3 */
    char* content_type;
    u_int content_type_size;

    /* Comments associated with the MP3 */
    char* comments;
    u_int comment_size;

} TagInfo;

/* 
 * Function to read and validate the MP3 file 
 * Arguments:
 *   - argv[]: Array of command-line arguments
 *   - mp3tagInfo: Pointer to TagInfo structure to be populated
 * Returns:
 *   - SUCCESS if the file is successfully read and validated
 *   - FAILURE otherwise
 */
RetStatus read_validate_mp3_file(char* argv[], TagInfo* mp3tagInfo);

/* 
 * Function to view the MP3 tag information 
 * Arguments:
 *   - argv[]: Array of command-line arguments
 *   - mp3tagInfo: Pointer to TagInfo structure containing MP3 tag info
 * Returns:
 *   - SUCCESS if the tag information is successfully displayed
 *   - FAILURE otherwise
 */
RetStatus view_tag(char* argv[], TagInfo* mp3tagInfo);

/* 
 * Function to get and display tag data from the MP3 file 
 * Arguments:
 *   - str_frame: Frame string to be read
 *   - str_Id: Identifier for the frame
 *   - frame_Id: Buffer to store the frame identifier
 *   - tag_size: Pointer to store the size of the tag
 *   - tag: Buffer to store the tag data
 *   - fptr: File pointer to the MP3 file
 * Returns:
 *   - SUCCESS if the data is successfully read and displayed
 *   - FAILURE otherwise
 */
RetStatus get_and_display_data(const char* str_frame, const char* str_Id, char* frame_Id, u_int* tag_size, char* tag, FILE* fptr);

#endif /* VIEW_DATA_H */
