// Arquivo de "header"
#include "keypad.h"




// Caracteres de cada botão do display
unsigned char display[4][4] = {
	{' ', '9', '8', '7'},
	{' ', '6', '5', '4'},
	{'-', '3', '2', '1'},
	{'+', ' ', '0', '.'}
};




//-------------------------------------------------------------------------------------------------------------
// Funções




// Inicia o keypad
void init_keypad()
{
	// Configura as saídas para o keypad (colunas)
	DDRB = 0b1111;
	PORTB = 0b1111;

	// Configura as entradas do keypad (linhas)
	DDRC = 0b0000;
	PORTC = 0xFF;
}


// Retorna o botão pressionado
char return_keypad_button()
{
	// Botão pressionado
	char res = ' ';

	// Leitura do keypad
	for (int testI = 0; testI < 4; testI++)          // Define o valor de J
	{
		PORTB = 0b1111^(0b1 << testI);
		_delay_ms(20);

		char new_PINC = PINC & 0x0F;

		if (new_PINC != 0b1111)
		{
			for (int testJ = 0; testJ < 4; testJ++)  // Define o valor de I
			{
				if ((new_PINC | 0b1000 >> testJ) == 0b1111)
				{
					res = display[testI][testJ];
					break;
				}
			}
			break;
		}
	}

	PORTB = 0b1111;

	// Retorna o botão
	return res;
}
