/*
 * GccApplication1.c
 *
 * Created: 9/20/2018 12:51:35 AM
 * Author : jiaang
 */ 


#include <avr/io.h>
#include <stdbool.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "uart.h"
#include <inttypes.h>
#include <math.h>

bool bIsTriggered = true;
int nTimeStart;
long nTimeEnd;
long nTimeInterval;
int nLoopCount;
int nMaxInterVal;
int nMinInterVal;
int fl;


void GenerateVol(int Val){
		switch (Val){

		case 0:
		
		PORTB &= 0xE3;
		PORTB |= (0 << PORTB2) | ( 0<< PORTB3) | (0 << PORTB4);
		printf("case 0\n");
		break;

		case 1:
		
		PORTB &= 0xE7;
		PORTB |= (1 << PORTB2) | ( 0<< PORTB3) | (0 << PORTB4);
		printf("case 1\n");
		break;

		case 2:
		
		PORTB &= 0xEB;
		PORTB |= (0 << PORTB2) | ( 1<< PORTB3) | (0 << PORTB4);
		printf("case 2\n");
		break;
		
		case 3:

		PORTB &= 0xEF;
		PORTB |= (1 << PORTB2) | ( 1<< PORTB3) | (0 << PORTB4);
		printf("case 3\n");
		break;

		case 4:

		PORTB &= 0xF3;
		PORTB |= (0 << PORTB2) | ( 0<< PORTB3) | (1 << PORTB4);
		printf("case 4\n");
		break;
		
		case 5:
		
		PORTB &= 0xF7;
		PORTB |= (1 << PORTB2) | ( 0 << PORTB3) | (1 << PORTB4);
		printf("case 5\n");
		break;

		case 6:
		
		PORTB &= 0xFB;
		PORTB |= (0 << PORTB2) | ( 1<< PORTB3) | (1 << PORTB4);
		printf("case 6\n");
		break;

		default:

		PORTB &= 0xFF;
		PORTB |= (1 << PORTB2) | ( 1 << PORTB3) | (1 << PORTB4);
		printf("case 7\n");

	}
}

void SetupADC(void){
	ADCSRA |= (1 << ADPS0) | (1 << ADPS1) | (1 << ADPS2); // 128 prescaling
	ADCSRA |= (1 << ADATE);//free mode
	ADCSRA |= (1 << ADEN);//power up ADC


	ADMUX |= (1 << REFS0); //AVcc
	ADCSRB |= (0 <<ADTS2) | (0 << ADTS1) | (0 <<ADTS0);//free running

	ADCSRA |= (1 << ADSC);//start converting
}

void GenerateDiscrete(int Val){

	switch (Val){

		case 0:
		
		OCR0A = 120;
		break;

		case 1:
		
		OCR0A = 106;
		break;

		case 2:
		
		OCR0A = 95;
		break;
		
		case 3:

		OCR0A = 90;
		break;

		case 4:
		
		OCR0A = 80;
		break;
		
		case 5:
		
		OCR0A = 71;
		break;

		case 6:
		
		OCR0A = 63;
		break;

		default:

		OCR0A = 60;

	}
}

void GenerateConti(long nTimeInterval){
	_delay_ms(1000);
	OCR0A =(int)(256 - (nTimeInterval -200) / 8);
	
	//printf("Continous %d",OCR0A);
}

int main(void)
{
	/* Replace with your application code */
	uart_init();
	DDRB = 0x3E;
	DDRB |= 0x22; //set PB1 as output
	PORTB |= 0x22; //set PB1 high

	TCNT1 = 0; //set timer1 to zero
	TIMSK1 |= (1 << OCIE1A); //output compare enable
	OCR1A = 80; //5us
	
	TCCR1A |= (1 << COM1A0); //toggle(compare mode), output to PB1

	TCCR1B |= (1 << CS10); // prescaling clk/1
	sei();
		
	DDRD |= 0x40;
	TCCR0A |= 0x40;
	TCCR0A |= 0x02;
	TCCR0B |= 0x03;
	SetupADC();
	while (1)
	{
		//printf("%d\n",ADC);
		
	}
}




ISR(TIMER1_COMPA_vect){ 
	
	fl = 1;
	DDRB |= 0x02; //set PB0 as input
	TIMSK1 |= (1 << ICIE1); // input capture enable, check PB0
	TCCR1B |= (1 << ICES1); //input rising edge select

	OCR1A = 3200; //200us
		
}

ISR(TIMER1_CAPT_vect){

	TCCR1B ^= (1 << ICES1); // input falling edge capture

	if (bIsTriggered){
		//printf("down edge triggered");
		nTimeStart = ICR1; //START
		nLoopCount = 0;
		//printf("Start time %d\n", nTimeStart);

		TIMSK1 |= (1 << TOIE1);  //overflow timer enabled
		
	}

	else{

		//printf("up edge triggered");
		TIMSK1 = (0 << TOIE1); //disable overflow interrupt
		TIMSK1 = (1 << OCIE1A); //enable output compare interupt

		nTimeEnd = (long)(ICR1 + nLoopCount * 256 * 256 / 2);
		//printf("start %d", nTimeStart);
		
		nTimeInterval = (long)nTimeEnd - (long)nTimeStart;
		//printf("the  time interval is %ld \n", nTimeInterval/16);
		
		int Val = (nTimeInterval/16 -260) / 155;
		//GenerateDiscrete(Val);
		
		if(PINB & (1 <<PINB5)){//test switch
			
			GenerateDiscrete(Val);
		}
		else{
			GenerateConti(nTimeInterval);
			
		}
		int Val_Light = (ADC - 40) / 123;
		GenerateVol(Val_Light);
	
	}

	bIsTriggered ^= true;

}

ISR(TIMER1_OVF_vect){
	nLoopCount++;

}




