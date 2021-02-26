// Arquivo de "header"
#include "LED.h"




// Várial de se o LED está aceso
bool is_led_on = false;




//-------------------------------------------------------------------------------------------------------------
// Funções




// Muda o valor do LED
void set_led(bool value)
{
	is_led_on = value;
}


// Atualiza o LED
void update_led()
{
	if (is_led_on)
		set_bit(PORTD, LED_ENABLE_BIT);

	else
		clr_bit(PORTD, LED_ENABLE_BIT);
}
