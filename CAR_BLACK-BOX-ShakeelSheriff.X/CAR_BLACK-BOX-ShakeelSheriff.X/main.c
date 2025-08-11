/*
 * File:   main.c
 * Author: Shakeel Sheriff M
 * P1 :- Car black box
 * Created on October 3, 2024, 1:07 AM
 */

#include "main.h"
#pragma config WDTE = OFF      // Watchdog Timer Enable bit (WDT disabled)

// global variable Char Array of Pointers
char *gear[] = {"GN", "GR", "G1", "G2", "G3", "G4"};

void init_config(void) {
    //Initializations here
    // To Initialize I2C
    init_i2c(100000);

    // To Initialize RTC
    init_ds1307();

    // To Initialize CLCD
    init_clcd();

    // To Initialize DKP
    init_digital_keypad();

    // To Initialize ADC
    init_adc();

    // To Initialize Timer2
    init_timer2();
    GIE = 1;
    PEIE = 1;

    //To Initialize UART Polling
    init_uart(9600);

    puts("UART Test Code\n\r");
}

void main(void) {
    init_config();
    char event[3] = "ON";
    unsigned char speed = 0, key, gr = 0;
    unsigned char screen_flag = DASHBOARD_SCREEN, reset_flag, menu_pos;
    record_event(event, speed);
    eeprom_write_string(0x00, "1010"); // Default Password Stored
    while (1) {
        //Application code here

        //speed control by potentiometer with ADC
        speed = read_adc() / 10;
        if (speed > 99) /* check max speed */ {
            speed = 99;
        }
        //Detect DKP
        key = read_digital_keypad(STATE);
        /* to remove bouncing effect */
        for (int j = 500; j--;);
        if (key == SW1) {
            strcpy(event, "C "); // Set event to the new gear
            record_event(event, speed);
        } else if (key == SW2 && gr < 6) {
            strcpy(event, gear[gr]); // Set event to the new gear
            record_event(event, speed);
            gr++;
        } else if (key == SW3 && gr > 0) {
            gr--;
            strcpy(event, gear[gr]); // Set event to the new gear
            record_event(event, speed);
        } else if ((key == SW4 || key == SW5) && screen_flag == DASHBOARD_SCREEN) {
            screen_flag = LOGIN_SCREEN;
            clear_screen();
            clcd_print("Enter Password", LINE1(1));
            clcd_write(LINE2(5), INST_MODE);
            clcd_write(DISP_ON_AND_CURSOR_ON, INST_MODE);
            __delay_us(100);
            reset_flag = RESET_PASSWORD;
            TMR2ON = 1;
        } else if ((key == SW4_LP) && screen_flag == MENU_SCREEN) {
            switch (menu_pos) {
                case 0: // To View Records

                    screen_flag = VIEW_RECORDS_SCREEN;
                    clear_screen();
                    record_event("VL", speed);
                    clcd_print("# TIME     E  SP", LINE1(0));
                    reset_flag = RESET_VIEW_LOG_POS;
                    break;
                case 1: // To Clear Records

                    screen_flag = CLEAR_RECORDS_SCREEN;
                    clear_screen();
                    reset_flag = RESET_MEMORY;
                    break;
                case 2:// To Download Records

                    screen_flag = DOWNLOAD_RECORDS_SCREEN;
                    clear_screen();
                    record_event("DL", speed);
                    clcd_print(" OPEN TERATERM ", LINE1(0));
                    __delay_ms(2000);
                    reset_flag = RESET_LOGIN_MENU;
                    break;

                case 3:// To Change and Set Time Clock

                    screen_flag = SET_CLOCK_TIME_SCREEN;
                    clear_screen();
                    record_event("ST", speed);
                    reset_flag = RESET_TIME;
                    break;

                case 4:// To Change and Set New Password

                    screen_flag = CHANGE_PASSWORD_SCREEN;
                    clear_screen();
                    clcd_print("Enter New Pwd:", LINE1(0));
                    clcd_write(LINE2(5), INST_MODE);
                    clcd_write(DISP_ON_AND_CURSOR_ON, INST_MODE);
                    __delay_us(100);
                    reset_flag = RESET_PASSWORD;
                    TMR2ON = 1;
            }
        }// if long press of SW4 in view log screen
        else if (key == SW4_LP && screen_flag == VIEW_RECORDS_SCREEN) {
            //Return to login menu
            screen_flag = MENU_SCREEN;
            //clear display
            clear_screen();
        }// if long press of SW4 in change password screen
        else if (key == SW4_LP && screen_flag == CHANGE_PASSWORD_SCREEN) {
            //Return to login menu
            screen_flag = MENU_SCREEN;
            clcd_write(DISP_ON_AND_CURSOR_OFF, INST_MODE);
            __delay_us(100);
            //clear display
            clear_screen();
        }//if long press of SW5 in login menu
        else if (key == SW5_LP && screen_flag == MENU_SCREEN) {
            //return to dashboard
            screen_flag = DASHBOARD_SCREEN;
            //Clear display
            clear_screen();
        }


        //To check which screen we are in
        switch (screen_flag) {
            case DASHBOARD_SCREEN: // Default Screen/ Dashboard
                //Display TIME EVENT SPEED
                display_dashboard(event, speed);
                break;
            case LOGIN_SCREEN:
                //Login Screen to access Records
                switch (login(key, reset_flag)) {
                    case RETURN_BACK:
                        screen_flag = DASHBOARD_SCREEN;
                        TMR2ON = 0;
                        clcd_write(DISP_ON_AND_CURSOR_OFF, INST_MODE);
                        __delay_us(100);
                        clear_screen();
                        break;
                    case LOGIN_SUCCESS:
                        clear_screen();
                        screen_flag = MENU_SCREEN;
                        TMR2ON = 1;
                        clcd_write(DISP_ON_AND_CURSOR_OFF, INST_MODE);
                        __delay_us(100);
                        reset_flag = RESET_LOGIN_MENU;
                        continue;
                }
                break;
            case MENU_SCREEN:
                //Menu Screen to Display Options
                switch (display_menu(key, reset_flag)) {
                        //if return back from login menu
                    case RETURN_BACK:
                        //clear display
                        clear_screen();
                        //switch to dashboard
                        screen_flag = DASHBOARD_SCREEN;
                        //turn off timer2
                        TMR2ON = 0;
                        break;
                        //menu option 0
                    case 0:
                        menu_pos = 0;
                        break;
                        //menu option 1
                    case 1:
                        menu_pos = 1;
                        break;
                        //menu option 2
                    case 2:
                        menu_pos = 2;
                        break;
                        //menu option 3
                    case 3:
                        menu_pos = 3;
                        break;
                        //menu option 4
                    case 4:
                        menu_pos = 4;
                        break;
                }
                break;
            case VIEW_RECORDS_SCREEN:
                //To View LOG Records
                if(view_records(key, reset_flag) == RETURN_BACK){// To Logout
                    clear_screen();
                    screen_flag = DASHBOARD_SCREEN;
                    reset_flag = RESET_LOGIN_MENU;
                }
                break;
            case CLEAR_RECORDS_SCREEN:
                //To clear LOG Records
                if (clear_records(reset_flag) == TASK_SUCCESS)
                    __delay_ms(1000);

                if (reset_flag == RESET_MEMORY)
                    record_event("CL", speed);

                screen_flag = MENU_SCREEN;
                reset_flag = RESET_LOGIN_MENU;
                clear_screen();
                break;
            case DOWNLOAD_RECORDS_SCREEN:
                //To Download Log Records
                download_records();
                screen_flag = MENU_SCREEN;
                reset_flag = RESET_LOGIN_MENU;
                clear_screen();
                break;
            case SET_CLOCK_TIME_SCREEN:
                //To Set Clock Time
                if (set_time(key, reset_flag) == TASK_SUCCESS) {
                    screen_flag = MENU_SCREEN;
                    reset_flag = RESET_LOGIN_MENU;
                    clear_screen();
                    continue;
                }
                break;
            case CHANGE_PASSWORD_SCREEN:
                //To Change Login Password
                switch (password_change(key, reset_flag)) {
                    case TASK_SUCCESS:
                        __delay_ms(1000);
                        record_event("CP", speed);
                        screen_flag = MENU_SCREEN;
                        reset_flag = RESET_LOGIN_MENU;
                        TMR2ON = 1;
                        clcd_write(DISP_ON_AND_CURSOR_OFF, INST_MODE);
                        __delay_us(100);
                        clear_screen();
                        break;
                    case RETURN_BACK:
                        //return to dashboard
                        clear_screen();
                        screen_flag = DASHBOARD_SCREEN;
                        TMR2ON = 0;
                        clcd_write(DISP_ON_AND_CURSOR_OFF, INST_MODE);
                        __delay_us(100);
                        reset_flag = RESET_LOGIN_MENU;
                        break;
                }
                break;
        }
        reset_flag = RESET_NOTHING;
    }
}
