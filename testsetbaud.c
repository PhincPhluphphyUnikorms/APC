#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>  

#define F_CPU 4000000UL
#define BAUD 9600       //The baudrate that we want to use
#define BAUD_PRESCALLER ((F_CPU)/(BAUD*16UL)-1)     //The formula that does all the required maths

                                  // set the CPU clock



//Declaration of our functions probably to now havce to put them in reverse order?!
void USART_init(void);
unsigned char USART_receive(void);
void USART_send( unsigned char data);
void USART_putstring(char* StringPtr);




static void
uart_9600(void)
{
   #define BAUD 9600
   #include <util/setbaud.h>
	UBRRH = UBRRH_VALUE;
	UBRRL = UBRRL_VALUE;
	UCSRA &= ~(1 << U2X);
}




char String[] = "HFHFHFH";    //String[] is in fact an array but when we put the text between the " " symbols the compiler threats it as a String and automatically puts the null termination character in the end of the text


int main(void) {


//register magic move to own funvtion if too complex
	DDRC |= _BV(DDC0);

uart_9600();        //Call the USART initialization code

_delay_ms(100);

 USART_putstring(String);    //Pass the string to the USART_putstring function and sends it over the serial


 return 0;

}




void USART_putstring(char* StringPtr){

	for (int i = 0; i < strlen(String); i++){ 

		USART_send(String[i]);    //send string
		
		PORTC ^= _BV(PC0); //Makes status LED blink
		_delay_ms(1);
	}

}



void USART_send( unsigned char data) {

/* Wait for empty transmit buffer */
	while ( !( UCSRA & (1<<UDRE)) );
/* Copy ninth bit to TXB8 */
	UCSRB &= ~(1<<TXB8);
	if ( data & 0x0100 )
		UCSRB |= (1<<TXB8);
/* Put data into buffer, sends the data */
	UDR = data;

}


unsigned char USART_receive(void) {

	while(!(UCSRA) & (1<<RXC));             // wait while data is being received
	return UDR; 

}






