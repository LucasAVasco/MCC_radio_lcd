#pragma once




// Arquivo de configurações
#include "defines.h"

// Bibliotecas
#include <avr/io.h>
#include <stdbool.h>

#include "others.h"




// Macros do LED
#define LED_ENABLE_BIT 6


// Funções do LED
void set_led(bool value);
void update_led();
