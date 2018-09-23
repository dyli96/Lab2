/*
 * GccApplication1.c
 *
 * Created: 9/20/2018 12:51:35 AM
 * Author : jiaang
 */ 

#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>
#include "uart.h"


int main(void)
{
	/* Replace with your application code */
	uart_init();

	DDRB |= 0x08;
	DDRD |= 0x28;
	TCCR2A |= 0x20;
	TCCR2A |= 0x03;
	TCCR2B |= 0x08;
	TCCR2B |= 0x05;

	
	OCR2A = 155;


	ADCSRA |= (1 << ADPS0) | (1 << ADPS1) | (1 << ADPS2); // 128 prescaling
	ADCSRA |= (1 << ADATE);//free mode
	ADCSRA |= (1 << ADEN);//power up ADC


	ADMUX |= (1 << REFS0); //AVcc
	ADCSRB |= (0 <<ADTS2) | (0 << ADTS1) | (0 <<ADTS0);//free running

	ADCSRA |= (1 << ADSC);//start converting
	while (1)
	{
		printf("%d\n",ADC);
	
		OCR2B = (int)(OCR2A * ADC / 1024); 
		

		//printf("%d OCR \n", OCR2B);

		_delay_ms(1000);

	}
}


