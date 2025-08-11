/*
 *  Filename   : edit_data.c	
 *  Description: MP3 TAG READER AND EDITOR ID3
 *  Created    : 23 August 2024
 *  Author     : Shakeel Sheriff M
 *  Batch      : 24002
 */
#include <stdio.h>
#include <string.h>
#include "mp3_types.h"
#include "edit_data.h"

// Function to read and validate command-line arguments and MP3 file
RetStatus read_and_validate_mp3_file_args(char* argv[], TagData* mp3tagData)
{
    // Check if the modifier is provided
    if (argv[2] == NULL)
    {
        printf("INFO: For Editing the Tags -> ./a.out -e <modifier> \"New_Value\" <file_name.mp3>\n");
        printf("INFO: Modifier Functions:\n");
        printf("-t\tModify Title Tag\n-A\tModify Artist Tag\n-a\tModify Album Tag\n-y\tModify Year Tag\n-G\tModify Content Type Tag\n-c\tModify Comments Tag\n");
        return FAILURE;
    }
    else
    {
        // Set frame ID and tag name based on the modifier
        if (strncmp(argv[2], "-t", 2) == 0)
        {
            strncpy(mp3tagData->frame_Id, "TIT2", 5);
            strncpy(mp3tagData->Tag_name, "Title", 6);
        }
        else if (strncmp(argv[2], "-A", 2) == 0)
        {
            strncpy(mp3tagData->frame_Id, "TPE1", 5);
            strncpy(mp3tagData->Tag_name, "Artist", 7);
        }
        else if (strncmp(argv[2], "-a", 2) == 0)
        {
            strncpy(mp3tagData->frame_Id, "TALB", 5);
            strncpy(mp3tagData->Tag_name, "Album", 6);
        }
        else if (strncmp(argv[2], "-y", 2) == 0)
        {
            strncpy(mp3tagData->frame_Id, "TYER", 5);
            strncpy(mp3tagData->Tag_name, "Year", 5);
        }
        else if (strncmp(argv[2], "-G", 2) == 0)
        {
            strncpy(mp3tagData->frame_Id, "TCON", 5);
            strncpy(mp3tagData->Tag_name, "Genre", 6);
        }
        else if (strncmp(argv[2], "-c", 2) == 0)
        {
            strncpy(mp3tagData->frame_Id, "COMM", 5);
            strncpy(mp3tagData->Tag_name, "Comment", 8);
        }
        else
        {
            // Print error for unsupported modifier
            printf("ERROR: Unsupported Modifier.\n");
            printf("INFO: Modifier Functions:\n");
            printf("-t\tModify Title Tag\n-A\tModify Artist Tag\n-a\tModify Album Tag\n-y\tModify Year Tag\n-G\tModify Content Type Tag\n-c\tModify Comments Tag\n");
            return FAILURE;
        }
        printf("Tag being edited: %s.\n", mp3tagData->Tag_name);
        printf("Frame ID to be changed: %s.\n", mp3tagData->frame_Id);
    }

    // Check if the new value is provided
    if (argv[3] == NULL)
    {
        printf("ERROR: New_Value to be updated on the Frame ID %s is Empty.\n", mp3tagData->frame_Id);
        printf("INFO: For Editing the Tags -> ./a.out -e <modifier> \"New_Value\" <file_name.mp3>\n");
        return FAILURE;
    }
    else
    {
        // Copy new value into frame_Id_value
        mp3tagData->frame_Id_size = strlen(argv[3]) + 1;
        if (mp3tagData->frame_Id_size < 100)
        {
            strncpy(mp3tagData->frame_Id_value, argv[3], mp3tagData->frame_Id_size);
        }
        else
        {
            printf("ERROR: Length of the New Data is too Long!.\n");
            return FAILURE;
        }
    }

    // Check if the file name is provided
    if (argv[4] == NULL)
    {
        printf("INFO: For Editing the Tags -> ./a.out -e <modifier> \"New_Value\" <file_name.mp3>\n");
        return FAILURE;
    }
    else
    {
        // Open the MP3 file for reading
        mp3tagData->fptr_mp3 = fopen(argv[4], "r");
        if (mp3tagData->fptr_mp3 != NULL)
        {
            char str[3];
            // Read the first 3 bytes to check the file Tag ID3
            fread(str, 1, 3, mp3tagData->fptr_mp3);
            if (strncmp(str, "ID3", 3) == 0)
            {
                printf("Version ID: v2.3\n");
                
            }
            else
            {
                // Print error if file Tag ID3 is incorrect
                printf("ERROR: The file Signature is not matching with that of a '.mp3' file.\n");
                printf("INFO: For Editing the Tags -> ./a.out -e <modifier> \"New_Value\" <file_name.mp3>\n");
                return FAILURE;
            }
        }
        else
        {
            // Print error if unable to open the file
            printf("ERROR: Unable to Open the %s file.\n", argv[4]);
            printf("INFO: For Editing the Tags -> ./a.out -e <modifier> \"New_Value\" <file_name.mp3>\n");
            return FAILURE;
        }

        fclose(mp3tagData->fptr_mp3);
    }

    return SUCCESS;
}

// Function to edit the MP3 tag
RetStatus edit_tag(char* argv[], TagData* mp3tagData)
{
    // Open the MP3 file for reading and create a temporary file for writing
    mp3tagData->fptr_mp3 = fopen(argv[4], "r");
    mp3tagData->fptr_temp = fopen("temp.mp3", "w");

    // Reset file pointers to the beginning
    fseek(mp3tagData->fptr_mp3, 0L, SEEK_SET);
    fseek(mp3tagData->fptr_temp, 0L, SEEK_SET);

    // Read and write the first 10 bytes
    char str[10];
    int count = fread(str, 1, 10, mp3tagData->fptr_mp3);
    if (count < 10)
    {
        printf("ERROR: Unable to read from the MP3 file.\n");
        return FAILURE;
    }
    else
    {
        count = fwrite(str, 1, 10, mp3tagData->fptr_temp);
        if (count < 10)
        {
            printf("ERROR: Unable to write on the MP3 file.\n");
            return FAILURE;
        }
    }

    // Array of frame tags
    char* frame_tag[6] = {"TIT2", "TPE1", "TALB", "TYER", "TCON", "COMM"};
    for (int i = 0; i < 6; i++)
    {
        // Read and write the first 4 bytes (frame ID)
        count = fread(str, 1, 4, mp3tagData->fptr_mp3);
        if (count < 4)
        {
            printf("ERROR: Unable to read from the MP3 file.\n");
            return FAILURE;
        }
        else
        {
            count = fwrite(str, 1, 4, mp3tagData->fptr_temp);
            if (count < 4)
            {
                printf("ERROR: Unable to write on the MP3 file.\n");
                return FAILURE;
            }
            else
            {
                // Check if the frame ID matches and copy data accordingly
                if (strncmp(mp3tagData->frame_Id, frame_tag[i], 4) == 0)
                {
                    RetStatus ret = copy_data(mp3tagData, 1);
                    if (ret == FAILURE)
                    {
                        printf("ERROR: Unable to copy the updated Data from Source to Destination '.mp3' file.\n");
                        return FAILURE;
                    }
                }
                else
                {
                    RetStatus ret = copy_data(mp3tagData, 0);
                    if (ret == FAILURE)
                    {
                        printf("ERROR: Unable to copy the remaining Data from Source to Destination '.mp3' file.\n");
                        return FAILURE;
                    }
                }
            }
        }
    }

    // Copy any remaining data from the original file
    RetStatus ret = copy_remaining_data(mp3tagData);
    if (ret == FAILURE)
    {
        printf("ERROR: Unable to copy the remaining Data from Source to Destination '.mp3' file.\n");
        return FAILURE;
    }
    
    // Close the file pointers
    fclose(mp3tagData->fptr_mp3);
    fclose(mp3tagData->fptr_temp);

    // Reopen the original file for writing and the temporary file for reading
    mp3tagData->fptr_mp3 = fopen(argv[4], "w");
    mp3tagData->fptr_temp = fopen("temp.mp3", "r");
    
    // Copy data back from temporary file to original file
    ret = copy_back_to_src(mp3tagData);
    if (ret == FAILURE)
    {
        printf("ERROR: Unable to copy the remaining Data from Destination to Source '.mp3' file.\n");
        return FAILURE;
    }

    // Print the updated value
    printf("Updated Value of Frame ID is %s.\n", mp3tagData->frame_Id_value);
    printf("===============================================================================\n");

    return SUCCESS;
}

// Function to copy data (with or without modification) from source to destination
RetStatus copy_data(TagData* mp3tagData, char flag)
{
    if (flag == 1)
    {
        u_int old_size, new_size;
        int count = fread(&old_size, 4, 1, mp3tagData->fptr_mp3);
        if (count < 1)
        {
            printf("ERROR: Unable to read from the MP3 file.\n");
            return FAILURE;
        }
        else
        {
            // Reverse byte order of old_size
            char* pos = (char*)&old_size;
            for (int i = 0; i < ((sizeof(u_int)) / 2); i++)
            {
                pos[i] = pos[i] ^ pos[(sizeof(u_int)) - i - 1];
                pos[(sizeof(u_int)) - i - 1] = pos[i] ^ pos[(sizeof(u_int)) - i - 1];
                pos[i] = pos[i] ^ pos[(sizeof(u_int)) - i - 1];
            }
        }

        // Set new_size to the size of the new frame value
        new_size = mp3tagData->frame_Id_size;
        char* pos = (char*)&mp3tagData->frame_Id_size;
        for (int i = 0; i < ((sizeof(u_int)) / 2); i++)
        {
            pos[i] = pos[i] ^ pos[(sizeof(u_int)) - i - 1];
            pos[(sizeof(u_int)) - i - 1] = pos[i] ^ pos[(sizeof(u_int)) - i - 1];
            pos[i] = pos[i] ^ pos[(sizeof(u_int)) - i - 1];
        }

        // Write the new size to the temp file
        count = fwrite(&mp3tagData->frame_Id_size, 4, 1, mp3tagData->fptr_temp);
        if (count < 1)
        {
            printf("ERROR: Unable to write on the MP3 file.\n");
            return FAILURE;
        }

        // Read and write the frame ID
        char f[3];
        count = fread(f, 1, 3, mp3tagData->fptr_mp3);
        if (count < 3)
        {
            printf("ERROR: Unable to read from the MP3 file.\n");
            return FAILURE;
        }
        else
        {
            count = fwrite(f, 1, 3, mp3tagData->fptr_temp);
            if (count < 3)
            {
                printf("ERROR: Unable to write on the MP3 file.\n");
                return FAILURE;
            }
        }

        // Write the new frame value to the temp file
        count = fwrite(mp3tagData->frame_Id_value, 1, (new_size - 1), mp3tagData->fptr_temp);
        if (count < (new_size - 1))
        {
            printf("ERROR: Unable to write on the MP3 file.\n");
            return FAILURE;
        }
        // Skip over the old frame data
        fseek(mp3tagData->fptr_mp3, (long)(old_size - 1), SEEK_CUR);
    }
    else
    {
        u_int size;
        int count = fread(&size, 4, 1, mp3tagData->fptr_mp3);
        if (count < 1)
        {
            printf("ERROR: Unable to read from the MP3 file.\n");
            return FAILURE;
        }
        else
        {
            // Write the size to the temp file
            count = fwrite(&size, 4, 1, mp3tagData->fptr_temp);
            if (count < 1)
            {
                printf("ERROR: Unable to write on the MP3 file.\n");
                return FAILURE;
            }
        }

        // Reverse byte order of size
        char* pos = (char*)&size;
        for (int i = 0; i < ((sizeof(u_int)) / 2); i++)
        {
            pos[i] = pos[i] ^ pos[(sizeof(u_int)) - i - 1];
            pos[(sizeof(u_int)) - i - 1] = pos[i] ^ pos[(sizeof(u_int)) - i - 1];
            pos[i] = pos[i] ^ pos[(sizeof(u_int)) - i - 1];
        }

        // Read and write the frame ID
        char f[3];
        count = fread(f, 1, 3, mp3tagData->fptr_mp3);
        if (count < 3)
        {
            printf("ERROR: Unable to read from the MP3 file.\n");
            return FAILURE;
        }
        else
        {
            count = fwrite(f, 1, 3, mp3tagData->fptr_temp);
            if (count < 3)
            {
                printf("ERROR: Unable to write on the MP3 file.\n");
                return FAILURE;
            }
        }

        // Read and write the rest of the frame data
        char str[size - 1];
        count = fread(str, 1, (size - 1), mp3tagData->fptr_mp3);
        if (count < (size - 1))
        {
            printf("ERROR: Unable to read from the MP3 file.\n");
            return FAILURE;
        }
        else
        {
            str[size - 1] = '\0';
            count = fwrite(str, 1, (size - 1), mp3tagData->fptr_temp);
            if (count < (size - 1))
            {
                printf("ERROR: Unable to write on the MP3 file.\n");
                return FAILURE;
            }
        }
    }

    return SUCCESS;
}

// Function to copy remaining data from the MP3 file to the temporary file
RetStatus copy_remaining_data(TagData* mp3tagData)
{
    char ch[1024];
    int count;
    long start = ftell(mp3tagData->fptr_mp3);

    // Seek to the end of the MP3 file
    fseek(mp3tagData->fptr_mp3, 0L, SEEK_END);
    long end = ftell(mp3tagData->fptr_mp3);

    // Seek back to the start position
    fseek(mp3tagData->fptr_mp3, start, SEEK_SET);

    // Copy the remaining data
    while (ftell(mp3tagData->fptr_mp3) != end)
    {
        count = fread(ch, 1, 1024, mp3tagData->fptr_mp3);
        if (count < 0)
        {
            printf("ERROR: Unable to read from the MP3 file.\n");
            return FAILURE;
        }
        else
        {
            fwrite(ch, 1, count, mp3tagData->fptr_temp);
        }
    }

    return SUCCESS;
}

// Function to copy data from the temporary file back to the original MP3 file
RetStatus copy_back_to_src(TagData* mp3tagData)
{
    fseek(mp3tagData->fptr_mp3, 0L, SEEK_SET);
    fseek(mp3tagData->fptr_temp, 0L, SEEK_SET);
    int count;
    char ch[1024];
    long start = ftell(mp3tagData->fptr_temp);

    // Seek to the end of the temporary file
    fseek(mp3tagData->fptr_temp, 0L, SEEK_END);
    long end = ftell(mp3tagData->fptr_temp);

    // Seek back to the start position
    fseek(mp3tagData->fptr_temp, 0L, SEEK_SET);

    // Copy the data back to the original file
    while (ftell(mp3tagData->fptr_temp) != end)
    {
        count = fread(ch, 1, 1024, mp3tagData->fptr_temp);
        if (count < 0)
        {
            printf("ERROR: Unable to read from the MP3 file.\n");
            return FAILURE;
        }
        else
        {
            fwrite(ch, 1, count, mp3tagData->fptr_mp3);
        }
    }

    return SUCCESS;
}
