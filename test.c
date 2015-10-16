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





char String[] = "AT+NAMEVoltageDivider";    //String[] is in fact an array but when we put the text between the " " symbols the compiler threats it as a String and automatically puts the null termination character in the end of the text


int main(void) {


//register magic move to own funvtion if too complex
	DDRC |= _BV(DDC0);

USART_init();        //Call the USART initialization code

_delay_ms(100);

 USART_putstring(String);    //Pass the string to the USART_putstring function and sends it over the serial


 return 0;

}



void USART_init(void) {


	//UBRRH = (BAUD_PRESCALLER>>8);                      // shift the register right by 8 bits
    //UBRRL = BAUD_PRESCALLER;                           // set baud rate
    //UCSRB|= (1<<TXEN)|(1<<RXEN);      			// enable receiver and transmitter
    //UCSRC|= (1<<URSEL)|(1<<UCSZ0)|(1<<UCSZ1);   // 8bit data format


/* Set baud rate */
	UBRRH = (unsigned char)(BAUD_PRESCALLER>>8);
	UBRRL = (unsigned char)BAUD_PRESCALLER;
/* Enable receiver and transmitter */
	UCSRB = (1<<RXEN)|(1<<TXEN);
/* Set frame format: 8data, 2stop bit */
	UCSRC = (1<<URSEL)|(1<<USBS)|(3<<UCSZ0);



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






