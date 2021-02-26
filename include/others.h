#pragma once




// Arquivo de configurações
#include "defines.h"

// Bibliotecas
#include <stdbool.h>
#include <string.h>




// Macros de operação bit a bit
#define set_bit(y,bit) (y|=(1<<bit))
#define clr_bit(y,bit) (y&=~(1<<bit))


// Funções diversas
int no_comma_strlen(char str[]);
