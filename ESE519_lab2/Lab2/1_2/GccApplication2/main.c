/*
 * GccApplication2.c
 *
 * Created: 9/8/2018 8:51:11 PM
 * Author : jiaang
 */ 

#define FCPU 16000000
#include <avr/io.h>
#include <avr/interrupt.h>


int main(void)
{
    /* Replace with your application code */

	DDRD |= 0x40;//PD6 output
	TCCR0A = (1 << WGM01);//CTC mode

	OCR0A = 70;
	// initialize counter	
	TIMSK0 |= (1 << OCIE1A);//output compare  match interrupt enale

	sei();

	TCCR0B |= (1 << CS02); //prescaling 1024


    while (1) 
    {


    }
}

ISR(TIMER0_COMPA_vect){
	PORTD ^= (1 << PORTD6);	//reverse PD6
}

