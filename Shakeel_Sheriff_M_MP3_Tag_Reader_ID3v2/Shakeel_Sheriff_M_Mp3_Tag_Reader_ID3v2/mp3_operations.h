#ifndef MP3_OPERATIONS_H
#define MP3_OPERATIONS_H

#include "mp3_types.h"  // Includes user-defined data types

/*
 * Function to determine the operation mode based on command-line arguments
 * Arguments:
 *   - argv[]: Array of command-line arguments
 * Returns:
 *   - OP_VIEW if the operation is to view MP3 tags
 *   - OP_EDIT if the operation is to edit MP3 tags
 *   - OP_HELP if the operation is to display help information
 *   - OP_UNSUPPORTED if the operation is not recognized or unsupported
 */
OpMode Operation_Check(char* argv[]);

#endif /* MP3_OPERATIONS_H */
