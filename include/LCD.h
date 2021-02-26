#pragma once




// Arquivo de configurações
#include "defines.h"

// Bibliotecas
#include <avr/io.h>
#include <util/delay.h>

#include "others.h"
#include "keypad.h"
#include "LED.h"




// Macros do LCD
#define LCD_PORT PORTD
#define LCD_ENABLE_BIT 7


// Funções do LCD
void set_LCD_instruction();
void exec_LCD(unsigned char cmd);
void move_LCD_cursor(unsigned int column, unsigned int line);
void write_LCD(unsigned char ch);
void write_LCD_string(char str[]);
void init_LCD();
void clear_LCD();
void create_LCD_char(const unsigned char matrix[], unsigned int address);
void draw_LCD_big_number(unsigned char ch);
void draw_LCD_big_number_string(char str[]);
