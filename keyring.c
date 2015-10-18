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
void lock(void);
void unlock(void);
void servoInit(void);
void init(void);


char String[] = "BREAK!";    //String[] is in fact an array but when we put the text between the " " symbols the compiler threats it as a String and automatically puts the null termination character in the end of the text


int main(void) {



//register magic move to own funvtion if too complex
	DDRC |= _BV(DDC0); //SET PC0 to output
	DDRC |= _BV(DDC1); //SET PC1 to output 

init();        //Call the initialization code

_delay_ms(100);


while(1) {


	updateState();

}

return 0;

}




/**
* Checks and changes state of keyring
*
*/
void updateState(void) {

	unsigned char * command = receiveString(); //Recieve command from serial

	
	//command = command[strlen(command)-1] = 0; // STrips last char fro string (line break (\n))

	if(!strcmp(command, "LOCK")) {

		lock();
		USART_putstring("LOCK!!");

	} else if (!strcmp(command, "UNLOCK") == 1) {


		unlock();
		USART_putstring("UNLOCK!!!!");



	} else {

		USART_putstring("SOMETHING!!!!");



	}



}



void lock(void) {


	PORTC ^= _BV(PC1);//Turn on power to servo

	OCR1A = 190;   //position furthest away from gear

	_delay_ms(1000);



	PORTC ^= _BV(PC1);//Turn on power to servo

	

}


void unlock(void) {


 
    PORTC ^= _BV(PC1); //Turn on power to servo

    OCR1A = 750;  //position closets to gear
    
    _delay_ms(2000);
    
    PORTC ^= _BV(PC1); //Turn off power to servo


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

		//USART_putstring(String);
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





void init(void) {

	servoInit();
	USART_init();



}


void USART_init(void) {

/* Set baud rate */
	UBRRH = (unsigned char)(BAUD_PRESCALLER>>8); 
	UBRRL = (unsigned char)BAUD_PRESCALLER;		 
/* Enable receiver and transmitter */
	UCSRB = (1<<RXEN)|(1<<TXEN);


/* not 2x speed !!!NEW!!!! */
	UCSRA &= ~_BV(U2X);

/* Set frame format: 8data, 2stop bit */
	UCSRC = (1<<URSEL)|(1<<USBS)|(3<<UCSZ0);


}

void servoInit(void) {


	/*FOR TIMER1*/


   DDRB |= (1 << DDB1); // PB1 as output

   TCCR1A |= (1 << COM1A1); // set non-inverting mode
   TCCR1A |= (1 << WGM11) | (1 << WGM10); // set 10bit phase corrected PWM Mode
   TCCR1B |= (1 << CS11); // set prescaler to 8 and starts PWM

   ICR1=500;  //fPWM=50Hz





}





