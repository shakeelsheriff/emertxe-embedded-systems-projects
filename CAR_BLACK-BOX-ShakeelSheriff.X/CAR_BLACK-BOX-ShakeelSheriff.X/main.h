/* 
 * File:   main.h
 * Author: Shakeel Sheriff M
 * P1 :- Car black box
 * Created on October 3, 2024, 3:39 AM
 */

#ifndef MAIN_H
#define MAIN_H

#include <xc.h>
#include <string.h>
#include "adc.h"
#include "clcd.h"
#include "digital_keypad.h"
#include "ds1307.h"
#include "i2c.h"
#include "function_prototypes.h"
#include "External_EEPROM_AT24C04.h"
#include "timers.h"
#include "uart.h"

// Status Codes
#define LOGIN_SUCCESS          0x01
#define RETURN_BACK            0x33
#define TASK_SUCCESS           0x11
#define TASK_FAIL              0x22

// Reset Codes
#define RESET_NOTHING          0x00
#define RESET_PASSWORD         0x02
#define RESET_LOGIN_MENU       0x05
#define RESET_MEMORY           0x44
#define RESET_VIEW_LOG_POS     0x08
#define RESET_TIME             0x30

// Display Settings
#define DISP_ON_AND_CURSOR_ON  0x0F

// Screen Identifiers
#define DASHBOARD_SCREEN          0
#define LOGIN_SCREEN              1
#define MENU_SCREEN               4
#define VIEW_RECORDS_SCREEN       8
#define CLEAR_RECORDS_SCREEN      10
#define DOWNLOAD_RECORDS_SCREEN   20
#define SET_CLOCK_TIME_SCREEN     40
#define CHANGE_PASSWORD_SCREEN    80

#endif /* MAIN_H */
