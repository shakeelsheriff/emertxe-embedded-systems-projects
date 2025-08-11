/*
 *  Filename   : view_data.c	
 *  Description: MP3 TAG READER AND EDITOR ID3
 *  Created    : 23 August 2024
 *  Author     : Shakeel Sheriff M
 *  Batch      : 24002
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "mp3_types.h"
#include "view_data.h"

// Function to read and validate the MP3 file
RetStatus read_validate_mp3_file(char* argv[], TagInfo* mp3tagInfo) {
    // Check if file name argument is provided
    if (argv[2] == NULL) {
        printf("INFO: For Viewing the Tags -> ./a.out -v <file_name.mp3>\n");
        return FAILURE;
    } else {
        // Attempt to open the MP3 file for reading
        mp3tagInfo->fptr_mp3 = fopen(argv[2], "r");
        if (mp3tagInfo->fptr_mp3 != NULL) {
            char str[3];
            // Read the first 3 bytes to check the file Tag ID3
            fread(str, 1, 3, mp3tagInfo->fptr_mp3);
            if (strncmp(str, "ID3", 3) == 0) {
                printf("                              Version ID: v2.3\n");
                printf("===============================================================================\n");
            } else {
                // Print error if file Tag ID3 is not matching
                printf("ERROR: The file Signature is not matching with that of a '.mp3' file.\n");
                printf("INFO: For Viewing the Tags -> ./a.out -v <file_name.mp3>\n");
                return FAILURE;
            }
        } else {
            // Print error if unable to open the file
            printf("ERROR: Unable to Open the %s file.\n", argv[2]);
            printf("INFO: For Viewing the Tags -> ./a.out -v <file_name.mp3>\n");
            return FAILURE;
        }

        // Close the file pointer
        fclose(mp3tagInfo->fptr_mp3);
    }

    return SUCCESS;
}

// Function to view all the tags in the MP3 file
RetStatus view_tag(char* argv[], TagInfo* mp3tagInfo) {
    // Open the MP3 file for reading
    mp3tagInfo->fptr_mp3 = fopen(argv[2], "r");
    // Move the file pointer past the ID3 header (10 bytes)
    fseek(mp3tagInfo->fptr_mp3, 10L, SEEK_SET);

    RetStatus ret;
    // Retrieve and display each tag using helper function
    ret = get_and_display_data("Title: ", "TIT2", mp3tagInfo->frame_Id, &mp3tagInfo->title_tag_size, mp3tagInfo->title_tag, mp3tagInfo->fptr_mp3);
    if (ret == FAILURE) {
        printf("ERROR: Title Frame ID Contents cannot be displayed.\n");
        return FAILURE;
    }

    ret = get_and_display_data("Artist: ", "TPE1", mp3tagInfo->frame_Id, &mp3tagInfo->artist_tag_size, mp3tagInfo->artist_tag, mp3tagInfo->fptr_mp3);
    if (ret == FAILURE) {
        printf("ERROR: Artist Frame ID Contents cannot be displayed.\n");
        return FAILURE;
    }

    ret = get_and_display_data("Album: ", "TALB", mp3tagInfo->frame_Id, &mp3tagInfo->album_tag_size, mp3tagInfo->album_tag, mp3tagInfo->fptr_mp3);
    if (ret == FAILURE) {
        printf("ERROR: Album Frame ID Contents cannot be displayed.\n");
        return FAILURE;
    }

    ret = get_and_display_data("Year: ", "TYER", mp3tagInfo->frame_Id, &mp3tagInfo->year_size, mp3tagInfo->year, mp3tagInfo->fptr_mp3);
    if (ret == FAILURE) {
        printf("ERROR: Year Frame ID Contents cannot be displayed.\n");
        return FAILURE;
    }

    ret = get_and_display_data("Genre: ", "TCON", mp3tagInfo->frame_Id, &mp3tagInfo->content_type_size, mp3tagInfo->content_type, mp3tagInfo->fptr_mp3);
    if (ret == FAILURE) {
        printf("ERROR: Content Type Frame ID Contents cannot be displayed.\n");
        return FAILURE;
    }

    ret = get_and_display_data("Comments: ", "COMM", mp3tagInfo->frame_Id, &mp3tagInfo->comment_size, mp3tagInfo->comments, mp3tagInfo->fptr_mp3);
    if (ret == FAILURE) {
        printf("ERROR: Comments Frame ID Contents cannot be displayed.\n");
        return FAILURE;
    }

    // Close the file pointer
    fclose(mp3tagInfo->fptr_mp3);

    return SUCCESS;
}

// Helper function to get and display data from a specific frame ID
RetStatus get_and_display_data(const char* str_frame, const char* str_Id, char* frame_Id, u_int* tag_size, char* tag, FILE* fptr) {
    int count;
    // Read the frame ID from the file
    count = fread(frame_Id, 1, 4, fptr);
    if (count < 4) {
        printf("ERROR: Unable to read from the MP3 file.\n");
        return FAILURE;
    } else {
        // Check if the frame ID matches the expected ID
        if (strncmp(frame_Id, str_Id, 4) == 0) {
            // Read the size of the frame
            count = fread(tag_size, 4, 1, fptr);
            if (count < 1) {
                printf("ERROR: Unable to read from the MP3 file.\n");
                return FAILURE;
            } else {
                // Reverse byte order of the size
                char* pos = (char*) tag_size;
                for (int i = 0; i < ((sizeof(u_int)) / 2); i++) {
                    pos[i] = pos[i] ^ pos[(sizeof(u_int)) - i - 1];
                    pos[(sizeof(u_int)) - i - 1] = pos[i] ^ pos[(sizeof(u_int)) - i - 1];
                    pos[i] = pos[i] ^ pos[(sizeof(u_int)) - i - 1];
                }

                // Allocate memory for the tag data
                tag = (char*) malloc((*tag_size) * sizeof(char));
                // Skip over the frame ID header bytes
                fseek(fptr, 3L, SEEK_CUR);
                // Read the frame data
                count = fread(tag, 1, (*tag_size - 1), fptr);
                if (count < (*tag_size - 1)) {
                    printf("ERROR: Unable to read from the MP3 file.\n");
                    return FAILURE;
                } else {
                    tag[*tag_size - 1] = '\0'; // Null-terminate the tag string
                    // Print the frame data
                    printf("%-10s%s.\n", str_frame, tag);
                }
            }
        } else {
            // Print error if the frame ID is not found
            printf("ERROR: %s Frame ID not found.\n", str_Id);
            return FAILURE;
        }
    }

    // Free allocated memory for the tag data
    free(tag);
    tag = NULL;

    return SUCCESS;
}
