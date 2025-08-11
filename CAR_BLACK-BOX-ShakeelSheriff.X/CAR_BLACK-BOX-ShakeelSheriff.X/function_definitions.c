/*
 * File:   function_definitions.c
 * Author: Shakeel Sheriff M
 * P1 :- Car black box
 */
#include "main.h"

// Variables used in the Car Black Box project
char time[7]; // Stores time in "HH:MM:SS" format
unsigned char clock_reg[3]; // Array to hold hours, minutes, and seconds
char records[11]; // Stores log data in "HHMMSSEVSP" format
int pos = -1; // Tracks position for storing up to 10 records (0 to 9)
unsigned char return_time, sec; // return_time for redirecting to Dashboard, sec for screen block timing
unsigned char *menu[] = {"View Log", "Clear Log", "Download Log", "Set Time", "Change Passwd"}; // Menu options
int record_count = -1; // Counter for the number of records stored

// Function Definitions of Car Black Box Project

// Retrieves the current time from the DS1307 RTC and formats it into a string

static void get_time(void) {
    // Read hours, minutes, and seconds from the DS1307
    clock_reg[0] = read_ds1307(HOUR_ADDR); // Read hour (in BCD format)
    clock_reg[1] = read_ds1307(MIN_ADDR); // Read minute (in BCD format)
    clock_reg[2] = read_ds1307(SEC_ADDR); // Read second (in BCD format)

    // Convert BCD to ASCII characters for display
    time[0] = ((clock_reg[0] >> 4) & 0x03) + '0'; // Extract and convert hour tens place
    time[1] = (clock_reg[0] & 0x0F) + '0'; // Extract and convert hour ones place

    // Convert minutes from BCD to ASCII
    time[2] = ((clock_reg[1] >> 4) & 0x07) + '0'; // Extract and convert minute tens place
    time[3] = (clock_reg[1] & 0x0F) + '0'; // Extract and convert minute ones place

    // Convert seconds from BCD to ASCII
    time[4] = ((clock_reg[2] >> 4) & 0x07) + '0'; // Extract and convert second tens place
    time[5] = (clock_reg[2] & 0x0F) + '0'; // Extract and convert second ones place
    time[6] = '\0'; // Null-terminate the string
}

// Displays the current time on the CLCD

static void display_time(void) {
    get_time(); // Retrieve the current time
    clcd_putch(time[0], LINE2(0)); // Display hour tens digit
    clcd_putch(time[1], LINE2(1)); // Display hour ones digit
    clcd_putch(':', LINE2(2)); // Display colon
    clcd_putch(time[2], LINE2(3)); // Display minute tens digit
    clcd_putch(time[3], LINE2(4)); // Display minute ones digit
    clcd_putch(':', LINE2(5)); // Display colon
    clcd_putch(time[4], LINE2(6)); // Display second tens digit
    clcd_putch(time[5], LINE2(7)); // Display second ones digit
}

// Displays the dashboard information including time, event, and speed

void display_dashboard(unsigned char event[], unsigned char speed) {
    // Display titles for the dashboard
    clcd_print("TIME     E  SP", LINE1(2));

    // Display the current time
    display_time(); // Shows time in HH:MM:SS format

    // Display the event information
    clcd_print(event, LINE2(11)); // Print the current event on the second line

    // Display the speed on the dashboard
    clcd_putch((speed / 10) + '0', LINE2(14)); // Display the tens digit of speed
    clcd_putch((speed % 10) + '0', LINE2(15)); // Display the ones digit of speed
}

// Stores records into EEPROM memory

void store_records_EEPROM(void) {
    static unsigned char addr = 0x05; // Starting address for storing logs
    pos++; // Increment position for the new record
    if (pos == 10) { // If maximum records reached, reset position
        pos = 0; // Circular buffer implementation
    }
    addr = (pos * 10) + 0x05; // Calculate EEPROM address based on position
    eeprom_write_string(addr, records); // Write the log entry to EEPROM at the calculated address
    if (record_count < 9) { // Ensure record count does not exceed 10
        record_count++; // Increment the record count
    }
}

// Records an event along with the current time and speed

void record_event(unsigned char event[], unsigned char speed) {
    get_time(); // Retrieve the current time
    strncpy(records, time, 6); // Copy the formatted time (HHMMSS) into records
    strncpy(&records[6], event, 2); // Append the event type to the records
    records[8] = speed / 10 + '0'; // Store the tens digit of speed
    records[9] = speed % 10 + '0'; // Store the ones digit of speed
    records[10] = '\0'; // Null-terminate the records string
    store_records_EEPROM(); // Store the complete record in EEPROM
}

// Function to handle user login process

unsigned char login(unsigned char key, unsigned char reset_flag) {
    // 4 Key Press. SW4 - 1  SW5 - 0
    static char usr_pwd[4]; // Array to store user password input
    static unsigned char i; // To track the number of digits entered
    static unsigned char attempts_left; // To track remaining login attempts

    // Check if the password needs to be reset
    if (reset_flag == RESET_PASSWORD) {
        attempts_left = 3; // Reset attempts to 3 for new login session
        i = 0; // Reset digit counter
        // Initialize password array with null characters
        usr_pwd[0] = '\0';
        usr_pwd[1] = '\0';
        usr_pwd[2] = '\0';
        usr_pwd[3] = '\0';
        key = ALL_RELEASED; // Reset key value to avoid first input conflict
        return_time = 5; // Set return time for dashboard transition
    }

    // Check if the return timer has expired
    if (return_time == 0) {
        // Return back to the DASHBOARD_SCREEN if timer is up
        return RETURN_BACK;
    }

    // Check if the user pressed SW4 (to enter '1') and if the limit is not reached
    if (key == SW4 && i < 4) {
        usr_pwd[i] = '1'; // Store '1' in password array
        clcd_putch('*', LINE2(6 + i)); // Display a star for password input
        i++; // Increment the digit counter
        return_time = 5; // Reset return timer
    }        // Check if the user pressed SW5 (to enter '0') and if the limit is not reached
    else if (key == SW5 && i < 4) {
        usr_pwd[i] = '0'; // Store '0' in password array
        clcd_putch('*', LINE2(6 + i)); // Display a star for password input
        i++; // Increment the digit counter
        return_time = 5; // Reset return timer
    }

    // Check if 4 digits have been entered
    if (i == 4) {
        unsigned char saved_pwd[4]; // Array to hold the saved password from EEPROM
        for (int j = 0; j < 4; j++) {
            saved_pwd[j] = eeprom_read(j); // Read the saved password from EEPROM
        }
        // Compare the entered password with the saved password
        if (strncmp(usr_pwd, saved_pwd, 4) == 0) {
            clear_screen(); // Clear the display screen
            clcd_write(DISP_ON_AND_CURSOR_OFF, INST_MODE); // Set CLCD display mode
            __delay_us(100); // Short delay for stability
            clcd_print("Login Success", LINE1(2)); // Inform user of successful login
            __delay_ms(2000); // Delay to allow user to see message
            return LOGIN_SUCCESS; // Return success status
        } else {
            attempts_left--; // Decrement the attempts left
            // Check if there are no attempts remaining
            if (attempts_left == 0) {
                // Block the screen if maximum attempts reached
                clear_screen(); // Clear the display screen
                clcd_write(DISP_ON_AND_CURSOR_OFF, INST_MODE); // Set CLCD display mode
                clcd_print("You are Blocked", LINE1(0)); // Inform user they are blocked
                // __delay_ms(5000); 5 seconds for testing (commented out)
                clcd_print("Wait for...", LINE2(0)); // Indicate wait time
                clcd_print("sec", LINE2(13)); // Label for seconds
                sec = 60; // Initialize wait time
                while (sec) { // Countdown loop
                    clcd_putch(sec / 10 + '0', LINE2(11)); // Display tens digit of seconds
                    clcd_putch(sec % 10 + '0', LINE2(12)); // Display ones digit of seconds
                }
                attempts_left = 3; // Reset attempts for next session
            } else {
                clear_screen(); // Clear the display screen
                clcd_write(DISP_ON_AND_CURSOR_OFF, INST_MODE); // Set CLCD display mode
                clcd_print("Wrong Password", LINE1(0)); // Inform user of wrong password
                clcd_putch(attempts_left + '0', LINE2(0)); // Show remaining attempts
                clcd_print("attempts left", LINE2(2)); // Inform user of attempts left
                __delay_ms(2000); // Delay for user to see message
            }
            // Reset after no key press for 5 seconds
            clear_screen(); // Clear the display screen
            return_time = 5; // Reset return time
            clcd_print("Enter Password", LINE1(1)); // Prompt user to enter password
            clcd_write(LINE2(5), INST_MODE); // Set cursor position for input
            clcd_write(DISP_ON_AND_CURSOR_ON, INST_MODE); // Enable cursor on display
            __delay_us(100); // Short delay for stability
            i = 0; // Reset digit counter for next entry
        }
    }
}

// Function to display the menu and handle user input for menu navigation

unsigned char display_menu(unsigned char key, unsigned char reset_flag) {
    // static unsigned char *menu[] = {"View Log", "Clear Log", "Download Log", "Set Time", "Change Passwd"};
    static unsigned char menu_pos; // Track current position in the menu

    // Check if the return timer has expired
    if (return_time == 0) {
        // Return back to the dashboard if timer is up
        return RETURN_BACK;
    }

    // Reset menu position if the reset flag is set
    if (reset_flag == RESET_LOGIN_MENU) {
        return_time = 15; // Set return timer for menu navigation
        menu_pos = 0; // Reset menu position to the first item
    }

    // Navigate up in the menu if SW4 is pressed and not at the top
    if (key == SW4 && menu_pos > 0) {
        return_time = 15; // Reset return timer
        menu_pos--; // Move up in the menu
        clear_screen(); // Clear the display for fresh output
    }        // Navigate down in the menu if SW5 is pressed and not at the bottom
    else if (key == SW5 && menu_pos < 4) {
        return_time = 15; // Reset return timer
        menu_pos++; // Move down in the menu
        clear_screen(); // Clear the display for fresh output
    }
    // Display the selected menu item
    if (menu_pos == 4) {
        clcd_putch('*', LINE2(0)); // Indicate selection with a star
        clcd_print(menu[menu_pos - 1], LINE1(2)); // Display previous item
        clcd_print(menu[menu_pos], LINE2(2)); // Display current item
    } else {
        clcd_putch('*', LINE1(0)); // Indicate selection with a star
        clcd_print(menu[menu_pos], LINE1(2)); // Display current item
        clcd_print(menu[menu_pos + 1], LINE2(2)); // Display next item
    }
    return menu_pos; // Return current menu position
}

// Function to view records stored in EEPROM

unsigned char view_records(unsigned char key, unsigned char reset_flag) {
    static char record_number; // Track the currently viewed record number
    unsigned char records[11]; // Array to hold the record data
    int record_pos; // Position of the current record
    unsigned char addr; // Address for reading from EEPROM

    // Check if there are no records available
    if (record_count == -1) {
        clcd_print("    No logs     ", LINE2(0)); // Display message if no logs are found
    } else {
        // Reset the record number if reset flag is set
        if (reset_flag == RESET_VIEW_LOG_POS) {
            record_number = 0; // Reset to the first record
        }
        // Navigate to the previous record if SW4 is pressed
        if (key == SW4 && record_number > 0) {
            record_number--; // Move to the previous record
        }            // Rollover to the last record when decrementing from the first record
        else if (key == SW4 && record_number == 0) {
            record_number = record_count; // Roll over to last record
        }            // Navigate to the next record if SW5 is pressed
        else if (key == SW5 && record_number < record_count) {
            record_number++; // Move to the next record
        }            // Rollover to the first record when incrementing from the last record
        else if (key == SW5 && record_number == record_count) {
            record_number = 0; // Roll over to the first record
        }
        //To Logout to Dashboard Screen
        if(key == SW5_LP)
            return RETURN_BACK;
        
        record_pos = record_number; // Set the record position

        // Read 10 events from External EEPROM
        for (int i = 0; i < 10; i++) {
            addr = (record_pos * 10) + 5 + i; // Update the address for each record
            records[i] = eeprom_read(addr); // Read the data from EEPROM
        }
        records[10] = '\0'; // Null-terminate the records array
        clcd_putch(record_number + '0', LINE2(0)); // Display the current record number

        // Display hours value
        clcd_putch(records[0], LINE2(2)); // Display hours
        clcd_putch(records[1], LINE2(3)); // Display minutes
        clcd_putch(':', LINE2(4)); // Display colon separator

        // Display minutes value
        clcd_putch(records[2], LINE2(5)); // Display minutes
        clcd_putch(records[3], LINE2(6)); // Display seconds
        clcd_putch(':', LINE2(7)); // Display colon separator

        // Display seconds value
        clcd_putch(records[4], LINE2(8)); // Display seconds
        clcd_putch(records[5], LINE2(9)); // Display seconds

        // Display event character
        clcd_putch(records[6], LINE2(11)); // Display event type
        clcd_putch(records[7], LINE2(12)); // Display event type

        // Display speed value
        clcd_putch(records[8], LINE2(14)); // Display tens digit of speed
        clcd_putch(records[9], LINE2(15)); // Display ones digit of speed

        __delay_us(500); // Short delay for stability
    }
}

// Function to clear the logged records if the memory reset flag is set

char clear_records(unsigned char reset_flag) {
    // Check if the memory reset flag is set
    if (reset_flag == RESET_MEMORY) {
        pos = -1; // Reset position
        record_count = -1; // Reset record count

        // Display the message on the LCD
        clcd_print("Logs cleared", LINE1(2)); // Indicate that logs were cleared
        clcd_print("successfully", LINE2(2)); // Confirm successful clearing
        return TASK_SUCCESS; // Return success status
    }
    return TASK_FAIL; // Return failure status if reset flag is not set
}

// Function to download and display the logged records

void download_records(void) {
    int index = -1; // Initialize index for logs
    char records[11]; // Array to hold record data
    records[10] = '\0'; // Null-terminate the records array
    int position = 0; // Position for reading logs
    unsigned char addr; // Address for reading from EEPROM

    // If no logs are available, display the message
    if (record_count == -1) {
        puts("No logs available"); // Indicate that no logs are found
    }        // If logs are available, display logs on CLCD
    else {
        puts("Logs :"); // Display header for logs
        putchar('\n'); // New line for better formatting
        // Print the table for log display
        puts(" #     Time        Event       Speed");
        putchar('\n'); // New line for better formatting

        // Loop until the 10 logs (0 to 9)
        while (index < record_count) {
            // Increment the log position for reading
            position = index + 1; // Update position
            // Incrementing to next log
            index++; // Move to the next log

            // Read the log data from EEPROM
            for (int i = 0; i < 10; i++) {
                // Updating the address with that of event data
                addr = (unsigned char) (position * 10 + 5 + i); // Calculate address
                // Read the log data from EEPROM
                records[i] = eeprom_read(addr); // Fetch data from EEPROM
            }

            // Printing all logs in a formatted manner
            if (index < 10)
                putchar(' '); // Add space for formatting
            // Print the index
            putchar(index + '0'); // Display log index
            puts("   "); // Add spaces for alignment

            // Printing hours value HH
            putchar(records[0]); // Display hour
            putchar(records[1]); // Display hour
            putchar(':'); // Add colon separator

            // Printing minute value MM
            putchar(records[2]); // Display minute
            putchar(records[3]); // Display minute
            putchar(':'); // Add colon separator

            // Printing second value SS
            putchar(records[4]); // Display second
            putchar(records[5]); // Display second
            // 6 spaces for alignment
            puts("      "); // Add spaces for alignment

            // Printing event character
            putchar(records[6]); // Display event type
            putchar(records[7]); // Display event type
            // 10 spaces for alignment
            puts("          "); // Add spaces for alignment

            // Printing speed value 
            putchar(records[8]); // Display speed
            putchar(records[9]); // Display speed
            putchar('\n'); // New line for the next log entry
        }
    }
}

// Function to set the current time

unsigned char set_time(unsigned char key, unsigned char reset_flag) {
    static unsigned int new_time[3]; // Array to hold new time values: hours, minutes, seconds
    static unsigned int blink_pos; // Position of the current field being edited
    static unsigned char wait; // Counter for blink timing
    static unsigned char blink; // State for blinking control
    static char t_done = 0; // Flag to indicate if the time change is complete
    char buffer; // Temporary variable for time conversion

    // If reset_time is set, initialize the change time
    if (reset_flag == RESET_TIME) {
        // Get the current time
        get_time();
        // Storing values of new time hours
        new_time[0] = (time[0] & 0x0F) * 10 + (time[1] & 0x0F); // Convert from BCD to decimal
        // Storing values of new time minutes
        new_time[1] = (time[2] & 0x0F) * 10 + (time[3] & 0x0F); // Convert from BCD to decimal
        // Storing values of new time seconds
        new_time[2] = (time[4] & 0x0F) * 10 + (time[5] & 0x0F); // Convert from BCD to decimal
        // Print the prompt for changing the time
        clcd_print("Time (HH:MM:SS)", LINE1(0));

        // Initialize blink and control variables
        blink_pos = 2; // Start editing seconds
        wait = 0; // Reset wait timer
        blink = 0; // Reset blink state
        t_done = 0; // Reset done flag
        key = ALL_RELEASED; // Reset key state
    }

    // If the time change is done, return failure status
    if (t_done)
        return TASK_FAIL;

    // Handle key inputs for changing time
    switch (key) {
            // If SW4 is pressed, increment the value of the current field (HHMMSS)
        case SW4:
            new_time[blink_pos]++; // Increment the current field
            break;
            // If SW5 is pressed, move to the next field (HHMMSS)
        case SW5:
            blink_pos = (blink_pos + 1) % 3; // Move to next field, wrap around
            break;
            // Long press SW4 to save the new time
        case SW4_LP:
            // Get the current time again
            get_time();
            // Set time fetching from RTC for hours
            buffer = (char) (((new_time[0] / 10) << 4) | new_time[0] % 10); // Convert to BCD
            clock_reg[0] = (clock_reg[0] & 0xC0) | buffer; // Update clock register
            // Store time in DS1307
            write_ds1307(HOUR_ADDR, clock_reg[0]);

            // Set time fetching from RTC for minutes
            buffer = (char) (((new_time[1] / 10) << 4) | new_time[1] % 10); // Convert to BCD
            clock_reg[1] = (clock_reg[1] & 0x80) | buffer; // Update clock register
            write_ds1307(MIN_ADDR, clock_reg[1]);

            // Set time fetching from RTC for seconds
            buffer = (char) (((new_time[2] / 10) << 4) | new_time[2] % 10); // Convert to BCD
            clock_reg[2] = (clock_reg[2] & 0x80) | buffer; // Update clock register
            write_ds1307(SEC_ADDR, clock_reg[2]);

            // Clear the LCD screen
            clcd_write(CLEAR_DISP_SCREEN, INST_MODE);
            // Print success message
            clcd_print("Time changed", LINE1(2));
            clcd_print("Successfully", LINE2(2));
            // Mark the time change by changing the flag value  
            t_done = 1; // Set time change complete
            // Delay to allow message to be read
            __delay_ms(1000);
            // Return success macro
            return TASK_SUCCESS;
    }

    // Check that time values do not exceed valid ranges
    if (new_time[0] > 23) // Hours must not exceed 23
        new_time[0] = 0; // Reset to 0 if exceeded
    if (new_time[1] > 59) // Minutes must not exceed 59
        new_time[1] = 0; // Reset to 0 if exceeded
    if (new_time[2] > 59) // Seconds must not exceed 59
        new_time[2] = 0; // Reset to 0 if exceeded

    // Implement blinking logic for the current field (HH MM SS)
    if (wait++ == 1) {
        wait = 0; // Reset wait counter
        blink = !blink; // Toggle blink state

        // Blink the current field
        if (blink) {
            switch (blink_pos) {
                    // Blink the hours field
                case 0:
                    clcd_print("  ", LINE2(0)); // Blink indication for hours
                    __delay_ms(160); // Delay for blink effect
                    break;
                    // Blink the minutes field    
                case 1:
                    clcd_print("  ", LINE2(3)); // Blink indication for minutes
                    __delay_ms(160); // Delay for blink effect
                    break;
                    // Blink the seconds field
                case 2:
                    clcd_print("  ", LINE2(6)); // Blink indication for seconds
                    __delay_ms(160); // Delay for blink effect
                    break;
            }
        }
    }

    // Displaying hours field 
    clcd_putch((char) new_time[0] / 10 + '0', LINE2(0)); // Display tens of hours
    clcd_putch(new_time[0] % 10 + '0', LINE2(1)); // Display units of hours
    clcd_putch(':', LINE2(2)); // Display colon separator

    // Displaying minutes field
    clcd_putch((char) new_time[1] / 10 + '0', LINE2(3)); // Display tens of minutes
    clcd_putch(new_time[1] % 10 + '0', LINE2(4)); // Display units of minutes
    clcd_putch(':', LINE2(5)); // Display colon separator

    // Displaying seconds field 
    clcd_putch((char) new_time[2] / 10 + '0', LINE2(6)); // Display tens of seconds
    clcd_putch(new_time[2] % 10 + '0', LINE2(7)); // Display units of seconds
    // Return failure macro if time setting is still in progress
    return TASK_FAIL;
}

// Function to change the password

unsigned char password_change(unsigned char key, unsigned char reset_flag) {
    // 4 Key Press. SW4 - 1  SW5 - 0
    static char usr_pwd[4]; // Array to store the new password
    static char re_usr_pwd[4]; // Array to store re-entered password
    static char pwd_changed = 0; // Flag to indicate if the password has been changed
    static unsigned int pass_pos; // Position tracker for the new password
    static unsigned int re_pass_pos; // Position tracker for re-entering the password
    static unsigned char stage = 0; // Stage indicator: 0 for entering new pwd, 1 for re-entering

    // If reset_flag is set, initialize the password change process
    if (reset_flag == RESET_PASSWORD) {
        pass_pos = 0; // Reset position for new password input
        re_pass_pos = 0; // Reset position for re-entering password
        pwd_changed = 0; // Reset password changed flag
        stage = 0; // Start at the first stage

        // Storing password with null terminators
        usr_pwd[0] = '\0';
        usr_pwd[1] = '\0';
        usr_pwd[2] = '\0';
        usr_pwd[3] = '\0';

        re_usr_pwd[0] = '\0';
        re_usr_pwd[1] = '\0';
        re_usr_pwd[2] = '\0';
        re_usr_pwd[3] = '\0';

        key = ALL_RELEASED; // To avoid any key input at first
        return_time = 5; // Set return time to 5 seconds
    }

    // If return time has expired, return back to the dashboard screen
    if (return_time == 0) {
        return RETURN_BACK; // Return to dashboard
    }

    // If the password has already been changed, return failure
    if (pwd_changed) {
        return TASK_FAIL; // Password already changed
    }

    // First stage: entering new password
    if (stage == 0) {
        if (key == SW4 && pass_pos < 4) { // If SW4 is pressed and position is valid
            usr_pwd[pass_pos] = '1'; // Store '1' in the password array
            clcd_putch('*', LINE2(6 + pass_pos)); // Display star for entered password
            pass_pos++; // Move to the next position
            return_time = 5; // Reset return time
        } else if (key == SW5 && pass_pos < 4) { // If SW5 is pressed and position is valid
            usr_pwd[pass_pos] = '0'; // Store '0' in the password array
            clcd_putch('*', LINE2(6 + pass_pos)); // Display star for entered password
            pass_pos++; // Move to the next position
            return_time = 5; // Reset return time
        }

        // If 4 digits have been entered, proceed to re-entering password
        if (pass_pos == 4) {
            clear_screen(); // Clear the display
            clcd_print("Re-enter new pwd", LINE1(0)); // Prompt for re-entry
            clcd_write(LINE2(5), INST_MODE); // Set cursor position
            clcd_write(DISP_ON_AND_CURSOR_ON, INST_MODE); // Turn on display and cursor
            __delay_us(100); // Small delay for stability
            stage = 1; // Move to the second stage for re-entering password
            re_pass_pos = 0; // Reset for re-entering password
        }
    } else if (stage == 1) { // Second stage: re-entering new password
        switch (key) {
            case SW5: // If SW5 is pressed
                re_usr_pwd[re_pass_pos] = '0'; // Store '0' in the re-entered password
                clcd_putch('*', LINE2(6 + re_pass_pos)); // Display star for entered password
                re_pass_pos++; // Move to the next position
                return_time = 5; // Reset return time
                break;
            case SW4: // If SW4 is pressed
                re_usr_pwd[re_pass_pos] = '1'; // Store '1' in the re-entered password
                clcd_putch('*', LINE2(6 + re_pass_pos)); // Display star for entered password
                re_pass_pos++; // Move to the next position
                return_time = 5; // Reset return time
                break;
        }

        // If 4 digits have been re-entered
        if (re_pass_pos == 4) {
            // Check if the entered passwords match
            if (strncmp(usr_pwd, re_usr_pwd, 4) == 0) { // If both passwords match
                // Successfully changing the password
                eeprom_write_string(0x00, &usr_pwd[4]); // Storing new password in EEPROM
                pwd_changed = 1; // Set the password changed flag
                clear_screen(); // Clear the display
                clcd_write(DISP_ON_AND_CURSOR_OFF, INST_MODE); // Turn off cursor
                __delay_us(100); // Small delay for stability
                clcd_print("Password changed", LINE1(0)); // Display success message
                clcd_print("successfully", LINE2(2)); // Display success message
                __delay_ms(1000); // Wait before moving on
                return_time = 15; // Set return time to 15 seconds
                return TASK_SUCCESS; // Return success
            } else {
                // Password change failed
                clear_screen(); // Clear the display
                clcd_write(DISP_ON_AND_CURSOR_OFF, INST_MODE); // Turn off cursor
                __delay_us(100); // Small delay for stability
                clcd_print("Password  change", LINE1(0)); // Display failure message
                clcd_print("failed", LINE2(5)); // Display failure message
                pwd_changed = 1; // Set the password changed flag
                __delay_ms(1000); // Wait before moving on
                return_time = 15; // Set return time to 15 seconds
                return TASK_SUCCESS; // Return success
            }
        }
    }

    return TASK_FAIL; // Return failure if neither stage completed
}

// Function to clear the LCD screen

void clear_screen(void) {
    clcd_write(CLEAR_DISP_SCREEN, INST_MODE); // Clear the display
    __delay_us(500); // Small delay for stability
}
