/*
 *  Filename   : main.c	
 *  Description: MP3 TAG READER AND EDITOR ID3
 *  Created    : 23 August 2024
 *  Author     : Shakeel Sheriff M
 *  Batch      : 24002
 */

#include <stdio.h>
#include "mp3_types.h"
#include "mp3_operations.h"
#include "view_data.h"
#include "edit_data.h"

int main(int argc, char* argv[]) {
    TagInfo mp3tagInfo;   // Structure to hold MP3 tag information for viewing
    TagData mp3tagData;   // Structure to hold MP3 tag data for editing

    // Check if the number of command line arguments is less than expected
    if (argc < 2) {
        // Print error message for insufficient arguments
        printf("===============================================================================\n");
        printf("                   ERROR: /a.out: INVALID ARGUMENTS                            \n");
        printf("===============================================================================\n");
        printf(" USAGE:\n");
        printf("  ./a.out -v <mp3_filename>          View MP3 tag information\n");
        printf("  ./a.out -e <-t/-a/-A/-m/-y/-c> <mp3_filename>  Edit MP3 tag information\n");
        printf("  ./a.out --help                     Display help menu\n");
        printf("===============================================================================\n");
        return 1;  // Exit with error code
    } else {
        // Determine the operation mode based on command line arguments
        OpMode retType = Operation_Check(argv);
        if (retType == OP_HELP) {
            // Print help menu for tag reader and editor
            printf("===============================================================================\n");
            printf("                      HELP MENU FOR MP3 TAG READER & Editor                    \n");
            printf("===============================================================================\n");
            printf("  1. ./a.out -v <mp3_filename>     View MP3 tag information\n");
            printf("  2. ./a.out -e <modifier> <mp3_filename>  Edit MP3 tags\n");
            printf("  Modifiers:\n");
            printf("    -t    Title  |  -A    Artist  |  -a    Album  |  -y    Year\n");
            printf("    -G    Genre  |  -c    Comments\n");
            printf("  3. ./a.out --help               Display help menu\n");
            printf("===============================================================================\n");
        } else if (retType == OP_VIEW) {
            // Process the file in view mode
            printf("===============================================================================\n");
            printf("                       MP3 TAG READER AND EDITOR FOR ID3 V2     ::    VIEW MODE\n");
            printf("===============================================================================\n");
            // Validate the MP3 file and read its tags
            RetStatus ret1 = read_validate_mp3_file(argv, &mp3tagInfo);
            if (ret1 == SUCCESS) {
                // View the tags of the MP3 file
                RetStatus ret2 = view_tag(argv, &mp3tagInfo);
                if (ret2 == SUCCESS) {
                    // Print success message if tags are displayed successfully
                    printf("===============================================================================\n");
                    printf("                     DETAILS DISPLAYED SUCCESSFULLY                            \n");
                    printf("===============================================================================\n");
                } else {
                    // Print error message if there was an issue displaying the tags
                    printf("===============================================================================\n");
                    printf("Some details not found or errors in retrieving information.\n");
                    return FAILURE;
                }
            } else {
                // Print error message if the file could not be validated
                printf("Error: Provide required arguments or a valid MP3 file name.\n");
                printf("===============================================================================\n");
                return FAILURE;
            }
        } else if (retType == OP_EDIT) {
            // Process the file in edit mode
            printf("===============================================================================\n");
            printf("                       MP3 TAG READER AND EDITOR FOR ID3 V2     ::    EDIT MODE\n");
            printf("===============================================================================\n");
            // Validate the MP3 file and arguments for editing
            RetStatus ret1 = read_and_validate_mp3_file_args(argv, &mp3tagData);
            if (ret1 == SUCCESS) {
                // Edit the tags of the MP3 file
                RetStatus ret2 = edit_tag(argv, &mp3tagData);
                if (ret2 == SUCCESS) {
                    // Print success message if tags are updated successfully
                    printf("                     DETAILS UPDATED SUCCESSFULLY                            \n");
                    printf("===============================================================================\n");
                    // Optionally call view_tag here to verify and display updated content
                } else {
                    // Print error message if there was an issue editing the tags
                    printf("Error: Unable to edit the MP3 file tags.\n");
                    printf("===============================================================================\n");
                    return FAILURE;
                }
            } else {
                // Print error message if the file or arguments for editing are invalid
                printf("Error: Provide required arguments, a new Value & a valid MP3 file name.\n");
                printf("===============================================================================\n");
                return FAILURE;
            }
        } else if (retType == OP_UNSUPPORTED) {
            // Print error message for unsupported operations
            printf("ERROR: Unsupported Operation.\n");
            printf("INFO: Use \"./a.out --help\" for Help menu.\n");
        }
    }

    return 0;  // Exit with success code
}

