/*
 * GccApplication1.c
 *
 * Created: 9/13/2018 5:45:10 PM
 * Author : danyangl
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#define FCPU 16000000

int main(void)
{
    /* Replace with your application code */
    DDRD |= 0x40;//PD6 output
	
	TCCR0A |= (1 << WGM01) | (1 << COM0A0); // enable CTC, toggle OC0A
	OCR0A = 70; 
	
	TCCR0B |= (1 << CS02); //PRESCALING 256

	while (1) {
 

    }
}

