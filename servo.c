#define F_CPU 10000000UL

#include <avr/io.h>
#include <util/delay.h>

int
main (void)
{
    DDRC |= _BV(DDC0); 
    
    while(1) 
    {
        PORTC ^= _BV(PC0);
        

        _delay_ms(20);
    }
}
