#define F_CPU 16000000UL
#define BAUD 9600        //The baudrate that we want to use
#define BAUD_PRESCALLER (((F_CPU / (BAUD * 16UL))) - 1)    //The formula that does all the required maths


#include <avr/io.h>
#include <util/delay.h>
#include <string.h>
#include "hc06.h"






//Declaration of our functions probably to now havce to put them in reverse order?!
void USART_init(void);
unsigned char USART_receive(void);
void USART_send( unsigned char data);
void USART_putstring(char* StringPtr);


				


char String[] = "AT+NAMEEGBT-04";    //String[] is in fact an array but when we put the text between the " " symbols the compiler threats it as a String and automatically puts the null termination character in the end of the text


int main(void) {

//register magic move to own funvtion if too complex
DDRC |= _BV(DDC0);


	unsigned char received_data;
	/*Variable declaration*/
	

	usart_init(); /*USART initialization*/
 

 	_delay_ms(500);
	received_data = hc_06_bluetooth_device_name_change("ABLab");

 _delay_ms(100);

 USART_putstring(String);    //Pass the string to the USART_putstring function and sends it over the serial
 _delay_ms(50);        //Delay for 5 seconds so it will re-send the string every 5 seconds


 return 0;
}



void USART_init(void) {

	UBRR0H = (uint8_t)(BAUD_PRESCALLER>>8);
	UBRR0L = (uint8_t)(BAUD_PRESCALLER);
	UCSR0B = (1<<RXEN0)|(1<<TXEN0);
	UCSR0C = ((1<<UCSZ00)|(1<<UCSZ01));
}


void USART_putstring(char* StringPtr){

	while(*StringPtr != 0x00) { //If char == null we have send the whole string 
	USART_send(*StringPtr);    //send string
	StringPtr++;

	PORTC ^= _BV(PC0);
}

}



void USART_send( unsigned char data) {

		while(!(UCSR0A & (1<<UDRE0))); //Check register for space
		UDR0 = data; //SEND 

	}


	unsigned char USART_receive(void) {

		while(!(UCSR0A & (1<<RXC0)));
		return UDR0;

	}