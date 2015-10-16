#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>  
#include <string.h>  
#include <avr/sfr_defs.h> //Loop unril bit it set

#define F_CPU 4000000UL
#define BAUD 9600       //The baudrate that we want to use
#define BAUD_PRESCALLER ((F_CPU)/(BAUD*16UL)-1)     //The formula that does all the required maths

                                  // set the CPU clock



//Declaration of our functions probably to now havce to put them in reverse order?!
void USART_init(void);
unsigned char USART_receive(void);
void USART_send(char data);
void USART_putstring(char * StringPtr);
unsigned char * receiveString();
void blink(void);
void USART_Flush(void);
void updateState(void);



char String[] = "BREAK!";    //String[] is in fact an array but when we put the text between the " " symbols the compiler threats it as a String and automatically puts the null termination character in the end of the text


int main(void) {



//register magic move to own funvtion if too complex
	DDRC |= _BV(DDC0);

USART_init();        //Call the USART initialization code

_delay_ms(100);


//USART_putstring(String);

while(1) {


	updateState();

}

return 0;

}

void updateState(void) {

	unsigned char * command = receiveString(); //Recieve command from serial

	command = command[strlen(command)-1] = 0; // STrips last char fro string (line break (\n))

	if(strcmp(command, "abcdefg")) {

		USART_putstring("HEEEYYYY UDEN BREAK!");


	} else if (strcmp(command, "abcdefg\n")) {


		USART_putstring("HEEEYYYY MED BREAK!");



	} else {


		USART_putstring("HEEEYYYY NOGET ANDET!!!!!!?!");

	}




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


	//not 2x speed !!!NEW!!!!
	UCSRA &= ~_BV(U2X);

/* Set frame format: 8data, 2stop bit */
	UCSRC = (1<<URSEL)|(1<<USBS)|(3<<UCSZ0);
	//USRC | = (1<<URSEL) | (1<<UCS21) | (1<<UCS20);	


}


void USART_putstring(char * streng){

	for (int i = 0; i < strlen(streng); i++){ 

		USART_send(streng[i]);    //send string
		
		PORTC ^= _BV(PC0); //Makes status LED blink

		_delay_ms(1);
	}

}



void USART_send(char data) {

/* Wait for empty transmit buffer */
	while ( !( UCSRA & (1<<UDRE)) );
/* Copy ninth bit to TXB8 */
	UCSRB &= ~(1<<TXB8);
	if ( data & 0x0100 )
		UCSRB |= (1<<TXB8);
/* Put data into buffer, sends the data */
	UDR = data;

}

unsigned char * receiveString(){

unsigned char * result;


int i = 0;
while(1) {


	unsigned char dataread = USART_receive();

	result[i] =  dataread;


	if(dataread == 10) {

		USART_putstring(String);
		break;

	}


	i ++;



}

result[i] = '\0'; 

return result;

}





void blink(void) {


	_delay_ms(500);

	PORTC ^= _BV(PC0); //Makes status LED blink

	_delay_ms(500);
		
	PORTC ^= _BV(PC0); //Makes status LED blink

	_delay_ms(500);

	PORTC ^= _BV(PC0); //Makes status LED blink

}


unsigned char USART_receive() {
/* Wait for data to be received */

loop_until_bit_is_set(UCSRA, RXC);

/* Get and return received data from buffer */
return UDR;
}

void USART_Flush( void )
{
unsigned char dummy;
while ( UCSRA & (1<<RXC) ) dummy = UDR;
}






