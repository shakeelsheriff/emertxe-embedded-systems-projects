#include "main.h"

void init_digital_keypad(void) {
    /* Set Keypad Port as input */
    KEYPAD_PORT_DDR = KEYPAD_PORT_DDR | INPUT_LINES;
}

unsigned char read_digital_keypad(unsigned char mode) {
    static unsigned char key_state = ALL_RELEASED; // Holds the current state of the key
    static int press_duration = 0; // Counter to track how long the key is pressed
    static unsigned char long_press_detected = 0; // Flag to track if long press has been detected
    unsigned char key = KEYPAD_PORT & INPUT_LINES; // Read the current key input

    // Debouncing logic (Optional)
    __delay_ms(50); // Small delay to ensure the key press is stable

    if (mode == LEVEL_DETECTION) {
        return key; // Return the current level of the key (whether it's pressed or not)
    } else if (mode == STATE_DETECTION) {
        if (key != ALL_RELEASED && key_state == ALL_RELEASED) // Detect key press
        {
            key_state = key; // Save the key that is pressed
            press_duration = 0; // Reset the duration counter
            long_press_detected = 0; // Reset the long press flag
        } else if (key == ALL_RELEASED && key_state != ALL_RELEASED) // Detect key release
        {
            if (press_duration < 10 && !long_press_detected) // If key was pressed for a short time
            {
                unsigned char short_key_press = key_state; // Store the short press key
                key_state = ALL_RELEASED; // Reset the state
                return short_key_press; // Return the short press key
            } else // Long press was detected
            {
                key_state = ALL_RELEASED; // Reset the state
                press_duration = 0;
                return ALL_RELEASED; // Return nothing for long press release
            }
        }

        if (key_state != ALL_RELEASED) // If a key is being held down
        {
            press_duration++; // Increment the counter to detect long press
            if (press_duration >= 10) // Long press threshold
            {
                if (!long_press_detected) // If a long press hasn't been handled yet
                {
                    long_press_detected = 1; // Set the long press flag
                    if (key_state == SW4) // Long press for SW4
                    {
                        return SW4_LP;
                    } else if (key_state == SW5) // Long press for SW5
                    {
                        return SW5_LP;
                    }
                }
            }
        }
    }

    return ALL_RELEASED; // Default state, no key pressed
}
