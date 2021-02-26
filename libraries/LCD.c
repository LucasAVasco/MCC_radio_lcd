// Arquivo de "header"
#include "LCD.h"




// Enumeração dos caractéres customizados
enum LCD_DISPLAY_CUSTOM_CHARS {B_R, T_L, T_R, T_B, T_B_R, T_B_L, T, R};

// Variáveis do display LCD
unsigned int cursor_line, cursor_column;




//-------------------------------------------------------------------------------------------------------------
// Funções




// Ativa uma instrução do display LCD
void set_LCD_instruction()
{
	_delay_us(1);

	// Ativa o enable
	set_bit(LCD_PORT, LCD_ENABLE_BIT);

	_delay_us(1);

	// Desativa o enable
	clr_bit(LCD_PORT, LCD_ENABLE_BIT);

	_delay_us(45);
}


// Executa um comando no display LCD
void exec_LCD(unsigned char cmd)
{
	clr_bit(LCD_PORT, 4);

	// 4 bits da esquerda
	LCD_PORT = cmd>>4;
	update_led();
	set_LCD_instruction();

	// 4 bits da direita
	LCD_PORT = cmd & 0xF;
	update_led();
	set_LCD_instruction();

	_delay_ms(2);
}


// Escreve uma letra no display LCD
void write_LCD(unsigned char ch)
{
	// 4 bits da esquerda
	LCD_PORT = ch>>4;
	set_bit(LCD_PORT, 4);
	update_led();
	set_LCD_instruction();

	// 4 bits da direita
	LCD_PORT = ch & 0xF;
	set_bit(LCD_PORT, 4);
	update_led();
	set_LCD_instruction();

	// Coloca em modo de comando
	clr_bit(LCD_PORT, 4);
}


// Escreve uma String no LCD
void write_LCD_string(char str[])
{
	for (int tmp0 = 0; tmp0 < strlen(str); tmp0++)
	{
		write_LCD(str[tmp0]);
	}
}


// Move o cursor do LCD
void move_LCD_cursor(unsigned int column, unsigned int line)
{
	// Command variable
	unsigned char command;

	// Define the line
	if (line)
		command = 0xC0;
	else
		command = 0x80;

	// Define the column
	command += column;

	// Move to position
	exec_LCD(command);

	// Atualiza as variáveis de posição do cursor
	cursor_line = line;
	cursor_column = column;
}


// Inicia o display LCD 16x2
void init_LCD()
{
	// Configura as daídas do arduino
	DDRD = 0b11111;
	set_bit(DDRD, LCD_ENABLE_BIT);

	// Zera as saídas
	LCD_PORT = 0b0;
	_delay_ms(20);

	// Coloca o display em 4 bits
	LCD_PORT = 0b0011;
	set_LCD_instruction();

	_delay_ms(5);
	set_LCD_instruction();

	_delay_us(200);
	set_LCD_instruction();

	LCD_PORT = 0b0010;
	set_LCD_instruction();

	// 4 bits, 2 linhas, fonte 5x8
	exec_LCD(0b00101000);

	// Limpa o display e seta endereço zero
	exec_LCD(0b00000001);

	// Ativa o display, desativa o cursor e o blink
	exec_LCD(0b00001100);

	// Cursor avança da esquerda para direita
	exec_LCD(0b00000110);

	// Coloca na posição inicial
	move_LCD_cursor(0,0);
}


// Limpa o display LCD
void clear_LCD()
{
	// Limpa a primeira linha
	move_LCD_cursor(0,0);
	for(int tmp0 = 0; tmp0 < 16; tmp0++)
		write_LCD(' ');

	// Limpa a segunda linha
	move_LCD_cursor(0,1);
	for(int tmp0 = 0; tmp0 < 16; tmp0++)
		write_LCD(' ');

	// Coloca o cursor na posição inicial
	move_LCD_cursor(0, 0);
}


// Cria um caracter customizado do LCD
void create_LCD_char(const unsigned char matrix[], unsigned int address)
{
	// Coloca o cursor na posição de escrever na CGRAM
	exec_LCD(0x40 + address*8);

	// Escreve as partes do caractére
	for (unsigned int tmp0 = 0; tmp0 < 8; tmp0++)
	{
		write_LCD(matrix[tmp0]);
	}

	// Evita dados espúrios
	exec_LCD(0b0);

	// Coloca o cursor no início
	move_LCD_cursor(0,0);
}


// Desenha um Big Number na tela
void draw_LCD_big_number(unsigned char ch)
{
	// Vetor contendo as parte do big number
	unsigned int vector[4];

	// Define as partes do big number
	switch (ch)
	{
		#define SET_BIG_NUMBER(A,B,C,D) \
			vector[0] = A; \
			vector[1] = B; \
			vector[2] = C; \
			vector[3] = D; \
		break

		case '0':
			SET_BIG_NUMBER(T_L, T_R, 'L', B_R);

		case '1':
			SET_BIG_NUMBER(' ', R, ' ', R);

		case '2':
			SET_BIG_NUMBER(T_B, T_B_R, 'L', '_');

		case '3':
			SET_BIG_NUMBER(T, T_B_R, '_', B_R);

		case '4':
			SET_BIG_NUMBER('L', B_R, ' ', R);

		case '5':
			SET_BIG_NUMBER(T_B_L, T_B, '_', B_R);

		case '6':
			SET_BIG_NUMBER(T_B_L, T_B, 'L', B_R);

		case '7':
			SET_BIG_NUMBER(T, T_R, ' ', R);

		case '8':
			SET_BIG_NUMBER(T_B_L, T_B_R, 'L', B_R);

		case '9':
			SET_BIG_NUMBER(T_B_L, T_B_R, ' ', R);

		case 'A':
			SET_BIG_NUMBER(T_L, T_R, T_L, T_R);

		case 'B':
			SET_BIG_NUMBER(T_B_L, ')', 'L', ')');

		case 'C':
			SET_BIG_NUMBER(T_L, T, 'L', '_');

		case 'D':
			SET_BIG_NUMBER(' ', R, ' ', 'O');

		case 'E':
			SET_BIG_NUMBER(T_B_L, T_B, 'L', '_');

		case 'F':
			SET_BIG_NUMBER(T_L, T, T_L, T);

		case '.':
			SET_BIG_NUMBER(' ', ' ', ',', ' ');

		case ' ':
			SET_BIG_NUMBER(' ', ' ', ' ', ' ');

		#undef SET_BIG_NUMBER
	}


	// Apresenta as partes do big number na tela
	for (unsigned int tmp0 = 0; tmp0 < 4; tmp0++)
	{
		// Variáveis da posição do caractére
		unsigned int x = 0, y = 0;

		if (tmp0 == 2)
			{
				y = 1;
				x = -1;
			}

		else if (tmp0 == 1 || tmp0 == 3)
			x = 1;

		// Move para  aposição
		move_LCD_cursor(cursor_column + x, cursor_line + y);

		// Escreve o caractere
		write_LCD(vector[tmp0]);
	}

	// Ageita a posição do cursor verticalmente
	move_LCD_cursor(cursor_column, cursor_line - 1);

	// Se não for uma vírgula
	if (ch != '.')
		move_LCD_cursor(cursor_column + 1, cursor_line);
}


// Desenha uma String de Big Numbers
void draw_LCD_big_number_string(char str[])
{
	for (int tmp0 = 0; tmp0 < strlen(str); tmp0++)
	{
		draw_LCD_big_number(str[tmp0]);
	}
}
