#ifndef MP3_TYPES_H
#define MP3_TYPES_H

// Define an alias for the unsigned int type
typedef unsigned int u_int;

// Enumeration for representing function return statuses
typedef enum
{
    SUCCESS,  // Operation completed successfully
    FAILURE   // Operation encountered an error
} RetStatus;

// Enumeration for specifying different operation modes
typedef enum
{
    OP_VIEW,         // View mode: for viewing content
    OP_EDIT,         // Edit mode: for modifying content
    OP_HELP,         // Help mode: for displaying help information
    OP_UNSUPPORTED   // Unsupported mode: for unsupported operations
} OpMode;

#endif /* MP3_TYPES_H */
