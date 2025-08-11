/*
 *  Filename   : mp3_operations.c	
 *  Description: MP3 TAG READER AND EDITOR ID3
 *  Created    : 23 August 2024
 *  Author     : Shakeel Sheriff M
 *  Batch      : 24002
 */
#include <stdio.h>
#include <string.h>
#include "mp3_types.h"
#include "mp3_operations.h"

/**
 * @brief Determine the operation mode based on command line arguments.
 * 
 * @param argv Command line arguments array.
 * @return OpMode Enum representing the operation mode (view, edit, help, or unsupported).
 */
OpMode Operation_Check(char* argv[]) {
    // Check if the argument is "-v" for view mode
    if (strncmp(argv[1], "-v", 2) == 0) {
        return OP_VIEW;
    }
    // Check if the argument is "-e" for edit mode
    else if (strncmp(argv[1], "-e", 2) == 0) {
        return OP_EDIT;
    }
    // Check if the argument is "--help" for displaying help menu
    else if (strncmp(argv[1], "--help", 6) == 0) {
        return OP_HELP;
    }
    // Return unsupported if the argument does not match any known options
    else {
        return OP_UNSUPPORTED;
    }
}
