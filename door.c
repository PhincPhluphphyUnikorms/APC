#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h> 
#include <stdbool.h> 

#define F_CPU 4000000UL
#define BAUD 9600       //The baudrate that we want to use
#define BAUD_PRESCALLER ((F_CPU)/(BAUD*16UL)-1)     //The formula that does all the required maths

                                  // set the CPU clock



//Declaration of our functions 
void USART_init(void);
unsigned char USART_receive(void);
void USART_send( unsigned char data);
void USART_putstring(char * streng);
void lock(void);
void unlock(void);
void changestate(int state);
void sleep(void);
void wake(void);
void PIN_init(void);
int checkDoor(int old);
void checkPIR(void);




int main(void) {

PIN_init();

USART_init();        //Call the USART initialization code

_delay_ms(100);

int old;
while(1) {

	old = checkDoor(old);

	checkPIR();

}


 return 0;

}

void checkPIR(void){


	if(bit_is_set(PINC, 2)){


		USART_putstring("HEEEY MENNESKER");
		PORTC ^= _BV(PC0);
        _delay_ms(100);
        PORTC ^= _BV(PC0);
        _delay_ms(100);
	}


}

int checkDoor(int old){

	int new = bit_is_clear(PINC, 1);

	//USART_putstring( bit_is_clear(PINC, 1));

	if(new != old){

		changestate(new);
	}

	return old;

	_delay_ms(4000);
}

void changestate(int state){

	if (state) { // runs if reed switch is on the run

		lock();

	} else {

		unlock();
	}



}


void lock(void) {

	USART_putstring("LOCK\n");    //Pass the string to the USART_putstring function and sends it over the serial

}

void unlock(void){

	USART_putstring("UNLOCK\n");	
}

void sleep(void){

	USART_putstring("$$$"); // put in command mode

	_delay_ms(100);
	
	USART_putstring("Q\n"); // Q for quite mode = ingen forbindelse til andre moduler

	_delay_ms(100);

	USART_putstring("Z\n"); //Z for low-power mode

	_delay_ms(100);


}

void wake(void){

	USART_putstring("R,1\n"); // Reboot to wake up
	_delay_ms(100);

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

void PIN_init(void){

//register magic move to own funvtion if too complex
	DDRC |= _BV(DDC0);
	PORTC = _BV(PC1);
	DDRC |= _BV(PC2); // PC2 as input

}


void USART_putstring(char * streng){

	for (int i = 0; i < strlen(streng); i++){ 

		USART_send(streng[i]);    //send string
		
		PORTC ^= _BV(PC0); //Makes status LED blink

		_delay_ms(1);
	}

}



void USART_send(unsigned char data) {

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






