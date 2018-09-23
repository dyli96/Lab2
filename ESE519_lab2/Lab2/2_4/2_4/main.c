/*
 * 2_4.c
 *
 * Created: 9/19/2018 9:32:46 PM
 * Author : jiaang
 */ 

/*
 * GccApplication2.c
 *
 * Created: 9/18/2018 11:15:02 PM
 * Author : danyangl
 */ 

#include <avr/io.h>
#include <stdbool.h>
#include <stdio.h>
#include <avr/interrupt.h>
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

void GenerateConti(int nTimeInterval){
	float Freq =(nTimeInterval/16 -260) / 155;
	OCR0A =(int)(ceil(-8.536 * Freq + 115.5));
}


int main(void)
{
	uart_init();
    /* Replace with your application code */

	DDRB |= 0x22; //set PB1 as output
	PORTB |= 0x22; //set PB1 high

	TCNT1 = 0; //set timer1 to zero
	TIMSK1 |= (1 << OCIE1A); //timer1 output compare enable
	OCR1A = 80; //5us
	
	TCCR1A |= (1 << COM1A0); //toggle(compare mode), output to PB1
	TCCR1B |= (1 << CS10); // prescaling clk/1
	sei();
		
	DDRD |= 0x40;//PD6 output
	TCCR0A |= 0x02;//CTC mode
	TCCR0A |= 0x40;//toggle OC0A(compare)
	TCCR0B |= 0x03;//clk/64
	
    while (1) 
    {

    }
}


ISR(TIMER1_COMPA_vect){ 
	
	fl = 1;
	DDRB |= 0x02; //set PB0 as input
	TIMSK1 |= (1 << ICIE1); // input capture enable, check PB0
	TCCR1B |= (1 << ICES1); //input rising edge select		
}

ISR(TIMER1_CAPT_vect){

	TCCR1B ^= (1 << ICES1); // input falling edge capture

	if (bIsTriggered){
		nTimeStart = ICR1; //START
		nLoopCount = 0;
		printf("Start time %d\n", nTimeStart);
		//printf("Start time %d\n", (int)nTimeStart);

		TIMSK1 |= (1 << TOIE1);  //overflow timer enabled
		
	}

	else{

		TIMSK1 = (0 << TOIE1); //disable overflow interrupt
		TIMSK1 = (1 << OCIE1A); //enable output compare interupt

		nTimeEnd = (long)(ICR1 + nLoopCount * 256 * 256 / 2);
			
		nTimeInterval = (long)nTimeEnd - (long)nTimeStart;
		printf("the  time interval is %ld \n", nTimeInterval/16);
		
		int Val = (nTimeInterval/16 -260) / 155;
		//GenerateDiscrete(Val);
		
		if(PINB & (1 <<PINB5)){
			GenerateConti(nTimeInterval);
			
		}
		else{
			GenerateDiscrete(Val);
		}
	
		
	}

	bIsTriggered ^= true;

}

ISR(TIMER1_OVF_vect){
	nLoopCount++;
}




