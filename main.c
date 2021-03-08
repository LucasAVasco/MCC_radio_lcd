/*
 * INTERFACE DE RÁDIO NO DISPLAY LCD 16x2
 *
 * Aluno: Lucas de Amorim Vasco       Turma: 822
 * U.C.: Microcontroladores
 *
 *
 * OBS.: Baseado no código fornecido pelo professor
 */




// Configura o Microcontrolador
#include "defines.h"
#include <avr/io.h>
#include <util/delay.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "keypad.h"
#include "LCD.h"
#include "LED.h"
#include "others.h"




// Variáveis da radio
char freq[6] = "";


// funções locais
void sum_freq(float value);
void clear_freq();




//-------------------------------------------------------------------------------------------------------------
// Código Principal




// Definição dos caracteres customizados
const unsigned char B_R_char[] =
{
	0b00001, //      █
	0b00001, //      █
	0b00001, //      █
	0b00001, //      █
	0b00001, //      █
	0b00001, //      █
	0b11111, //  █████
	0b00000  //
};

const unsigned char T_L_char[] =
{
	0b11111, //  █████
	0b10000, //  █
	0b10000, //  █
	0b10000, //  █
	0b10000, //  █
	0b10000, //  █
	0b10000, //  █
	0b10000  //  █
};

const unsigned char T_R_char[] =
{
	0b11111,//  █████
	0b00001,//      █
	0b00001,//      █
	0b00001,//      █
	0b00001,//      █
	0b00001,//      █
	0b00001,//      █
	0b00001 //      █
};

const unsigned char T_B_char[] =
{
	0b11111, //  █████
	0b00000, //
	0b00000, //
	0b00000, //
	0b00000, //
	0b00000, //
	0b11111, //  █████
	0b11111  //  █████
};

const unsigned char T_B_R_char[] =
{
	0b11111, //  █████
	0b00001, //      █
	0b00001, //      █
	0b00001, //      █
	0b00001, //      █
	0b00001, //      █
	0b11111, //  █████
	0b11111  //  █████
};

const unsigned char T_B_L_char[] =
{
	0b11111, //  █████
	0b10000, //  █
	0b10000, //  █
	0b10000, //  █
	0b10000, //  █
	0b10000, //  █
	0b11111, //  █████
	0b11111  //  █████
};

const unsigned char T_char[] =
{
	0b11111, //  █████
	0b00000, //
	0b00000, //
	0b00000, //
	0b00000, //
	0b00000, //
	0b00000, //
	0b00000  //
};

const unsigned char R_char[] =
{
	0b00001, //      █
	0b00001, //      █
	0b00001, //      █
	0b00001, //      █
	0b00001, //      █
	0b00001, //      █
	0b00001, //      █
	0b00001  //      █
};




int main()
{
	// Inicia o keypad
	init_keypad();

	// Inicia o LCD
	init_LCD();

	// Cria os caracteres
	create_LCD_char(B_R_char, B_R);
	create_LCD_char(T_L_char, T_L);
	create_LCD_char(T_R_char, T_R);
	create_LCD_char(T_B_char, T_B);
	create_LCD_char(T_B_R_char, T_B_R);
	create_LCD_char(T_B_L_char, T_B_L);
	create_LCD_char(T_char, T);
	create_LCD_char(R_char, R);

	// Configura o LED
	DDRD  |= 0b1 << LED_ENABLE_BIT;

	// Variáveis do botão
	char button, last_button = ' ';

	// Limpa e inicia o display
	clear_freq();

	while(1)
	{
		// Recebe o valor do botão pressionado
		button = return_keypad_button();

		if (button != last_button && button != ' ')
		{
			// Remove vírgulas no começo e repetidas
			if (button == '.' && (strlen(freq) == 0 || strstr(freq, ".")))
				_delay_ms(10);

			// Números de 0 a 9 ou vírgula
			else if ((button >= '0' && button <= '9') || button == '.')
			{
				int freq_len = strlen(freq);
				freq[freq_len] = button;
				freq[freq_len + 1] = '\0';
			}

			// Soma "1.0" à frequência
			else if (button == '+')
				sum_freq(1.00001);

			// Subtrai "1.0" da frequência
			else if (button == '-')
				sum_freq(-0.99999);


			// Determina o tamanho máximo da frequência
			int max_len = 0;

			if (freq[0] == '8' || freq[0] == '9')
				max_len = 3;

			else if (freq[0] == '1')
				max_len = 4;


			// Verifica se deve limpar a frequência ou apresentá-la no display
			if (no_comma_strlen(freq) > max_len)
				clear_freq();

			else
			{
				// Apaga o número anterior
				if (freq[0] != '\0')
				{
					move_LCD_cursor(4, 0);
					draw_LCD_big_number_string("     ");
				}

				// Obtem a coluna onde se deve começar a escrever
				int x = strlen(freq)*2;
				x -= strlen(freq) - no_comma_strlen(freq);
				x = 13 - x;

				// Escreve no Display o Big Number da frequência
				move_LCD_cursor(x,0);
				draw_LCD_big_number_string(freq);
			}


			// Apresenta a rádio na tela
			#define CMP_FREQ(VALUE) if (strcmp(freq, VALUE) == 0)

			CMP_FREQ("96.1")
			{
				move_LCD_cursor(0, 0);
				write_LCD_string("BAND ");
				set_led(true);
			}
			else CMP_FREQ("105.1")
			{
				move_LCD_cursor(0, 0);
				write_LCD_string("X FM ");
				set_led(true);
			}
			else CMP_FREQ("100.1")
			{
				move_LCD_cursor(0, 0);
				write_LCD_string("UDESC");
				set_led(true);
			}

			#undef CMP_FREQ

			// Atualiza o LED
			update_led();
		}

		// Atualiza a variável "last_button"
		last_button = button;

		// Delay para carregar o número
		_delay_ms(10);
	}

	return 0;
}




//-------------------------------------------------------------------------------------------------------------
// Funções




// Soma um valor à frequência
void sum_freq(float value)
{
	// Caso a string "freq" esteja vazia
	if (freq[0] == '\0')
	{
		freq[0] = '0';
		freq[1] = '\0';
	}

	// Novo valor da frequência
	float new_freq = atof(freq) + value;

	// Caso o número seja negativo
	if (new_freq < 0)
		new_freq = 0.0001;


	// Calcula o número de algorismos depois da vírgula (incluindo ela)
	int float_len = strlen(strchr(freq, '.'));

	// Escreve o novo valor na variável "freq"
	snprintf(freq, 6, "%f", new_freq);

	// Remove os algorítimos depois da vírgula desnecessários
	for (int tmp0 = 0; tmp0 < 5; tmp0++)
		if (freq[tmp0] == '.')
			freq[tmp0 + float_len] = '\0';
}


// Limpa a frequência
void clear_freq()
{
	// Limpa o displat
	clear_LCD();

	// Apaga o LED
	set_led(false);
	update_led();

	// Limpa a variável "freq"
	freq[0] = '\0';

	// Escreve o "0"
	move_LCD_cursor(11,0);
	draw_LCD_big_number('0');

	// Escreve o "MHz"
	move_LCD_cursor(13,1);
	write_LCD_string("MHz");

	// Escreve o NONE
	move_LCD_cursor(0, 0);
	write_LCD_string("NONE");

	// Move o cursor para a posição inicial
	move_LCD_cursor(0,0);
}
