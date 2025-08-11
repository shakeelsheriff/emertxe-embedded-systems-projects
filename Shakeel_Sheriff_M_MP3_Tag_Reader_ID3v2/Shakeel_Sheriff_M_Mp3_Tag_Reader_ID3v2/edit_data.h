#ifndef EDIT_DATA_H
#define EDIT_DATA_H

#include "mp3_types.h"      // Includes user-defined data types

/*
 * Structure to store information for updating MP3 tag content
 * Includes details about the source and temporary MP3 files
 */
typedef struct _TagData
{
    /* Pointer to the source MP3 file */
    FILE* fptr_mp3;

    /* Pointer to the temporary MP3 file for processing updates */
    FILE* fptr_temp;

    /* Information provided via command line for updating the tag */
    char frame_Id[5];          // Frame identifier (up to 4 characters + null terminator)
    char Tag_name[10];         // Name of the tag being updated
    char frame_Id_value[100];  // New value for the frame identifier
    u_int frame_Id_size;       // Size of the frame identifier value

} TagData;

/* 
 * Function to validate command-line arguments and initialize TagData structure
 * Arguments:
 *   - argv[]: Array of command-line arguments
 *   - mp3tagData: Pointer to TagData structure to be populated
 * Returns:
 *   - SUCCESS if arguments are valid and stored correctly
 *   - FAILURE if there are errors in the arguments or initialization
 */
RetStatus read_and_validate_mp3_file_args(char* argv[], TagData* mp3tagData);

/* 
 * Function to perform the editing of MP3 tag details
 * Arguments:
 *   - argv[]: Array of command-line arguments
 *   - mp3tagData: Pointer to TagData structure containing update details
 * Returns:
 *   - SUCCESS if the tag is successfully edited
 *   - FAILURE if there are errors during the editing process
 */
RetStatus edit_tag(char* argv[], TagData* mp3tagData);

/* 
 * Function to copy frame contents from the source MP3 file to a temporary MP3 file
 * Arguments:
 *   - mp3tagData: Pointer to TagData structure with file pointers and tag details
 *   - flag: Indicates specific conditions or modes for copying
 * Returns:
 *   - SUCCESS if the data is copied successfully
 *   - FAILURE if there are errors during the copying process
 */
RetStatus copy_data(TagData* mp3tagData, char flag);

/* 
 * Function to copy the remaining data after frames have been copied to the temporary MP3 file
 * Arguments:
 *   - mp3tagData: Pointer to TagData structure with file pointers
 * Returns:
 *   - SUCCESS if the remaining data is copied successfully
 *   - FAILURE if there are errors during the copying process
 */
RetStatus copy_remaining_data(TagData* mp3tagData);

/* 
 * Function to copy all data from the temporary MP3 file back to the source MP3 file
 * Arguments:
 *   - mp3tagData: Pointer to TagData structure with file pointers
 * Returns:
 *   - SUCCESS if the data is copied back to the source file successfully
 *   - FAILURE if there are errors during the copying process
 */
RetStatus copy_back_to_src(TagData* mp3tagData);

#endif /* EDIT_DATA_H */
