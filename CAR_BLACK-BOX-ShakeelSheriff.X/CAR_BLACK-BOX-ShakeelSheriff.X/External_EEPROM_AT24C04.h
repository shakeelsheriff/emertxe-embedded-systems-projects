/* 
 * File:   External_EEPROM_AT24C04.h
 */

#ifndef EXTERNAL_EEPROM_AT24C04_H
#define	EXTERNAL_EEPROM_AT24C04_H

#define SLAVE_WRITE_EEPROM             0xA0
#define SLAVE_READ_EEPROM              0xA1


unsigned char eeprom_read(unsigned char addr);
void eeprom_write(unsigned char addr, unsigned char data);
void eeprom_write_string(unsigned char addr, char *data);

#endif	/* EXTERNAL_EEPROM_AT24C04_H */

