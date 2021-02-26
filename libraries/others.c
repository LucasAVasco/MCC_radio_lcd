// Arquivo de "header"
#include "others.h"




//-------------------------------------------------------------------------------------------------------------
// Funções




// Retorna o tamanho de uma string (desconsiderando a vírgula)
int no_comma_strlen(char str[])
{
	int res = 0;

	for (int tmp0 = strlen(str) - 1; tmp0 >= 0; tmp0--)
	{
		if(str[tmp0] != '.')
			res+=1;
	}

	return res;
}
