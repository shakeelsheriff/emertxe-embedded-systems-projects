/* 
 * File:   function_prototypes.h
 * Author: Shakeel Sheriff M
 * P1 :- Car black box
 * Created on October 3, 2024, 4:00 AM
 */
#include "main.h"

#ifndef FUNCTION_PROTOTYPES_H
#define	FUNCTION_PROTOTYPES_H

void display_dashboard(unsigned char event[], unsigned char speed);
void record_event(unsigned char event[], unsigned char speed);
void store_records_EEPROM(void);
void clear_screen(void);
unsigned char login(unsigned char key, unsigned char reset_flag);
unsigned char display_menu(unsigned char key, unsigned char reset_flag);
unsigned char view_records(unsigned char key, unsigned char reset_flag);
void download_records(void);
char clear_records(unsigned char reset_flag);
unsigned char set_time(unsigned char key, unsigned char reset_flag);
unsigned char password_change(unsigned char key, unsigned char reset_flag);

#endif	/* FUNCTION_PROTOTYPES_H */

