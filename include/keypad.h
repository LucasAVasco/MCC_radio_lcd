#pragma once




// Arquivo de configurações
#include "defines.h"

// Bibliotecas
#include <avr/io.h>
#include <util/delay.h>




// Funções do keypad
void init_keypad();
char return_keypad_button();
