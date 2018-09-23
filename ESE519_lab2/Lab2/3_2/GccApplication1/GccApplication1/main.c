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
#include <util/delay.h>


bool bIsTriggered = true;
int nTimeStart;
int nTimeEnd;
int nTimeInterval;
int nLoopCount;
int nMaxInterVal;
int nMinInterVal;

void GenerateDiscrete(int Val){
		switch (Val){

		case 0:
		
			PORTB &= 0xE3;
			PORTB |= (0 << PORTB2) | ( 0<< PORTB3) | (0 << PORTB4);//0
			printf("case 0\n");
			break;

		case 1:
		
			PORTB &= 0xE7;
			PORTB |= (1 << PORTB2) | ( 0<< PORTB3) | (0 << PORTB4);//1
			printf("case 1\n");
			break;

		case 2:
		
			PORTB &= 0xEB;
			PORTB |= (0 << PORTB2) | ( 1<< PORTB3) | (0 << PORTB4);//2
			printf("case 2\n");
			break;
		
		case 3:

			PORTB &= 0xEF;
			PORTB |= (1 << PORTB2) | ( 1<< PORTB3) | (0 << PORTB4);//3
			printf("case 3\n");
			break;

		case 4:

			PORTB &= 0xF3;
			PORTB |= (0 << PORTB2) | ( 0<< PORTB3) | (1 << PORTB4);//4
			printf("case 4\n");
			break;
	
		case 5:
			
			PORTB &= 0xF7;
			PORTB |= (1 << PORTB2) | ( 0 << PORTB3) | (1 << PORTB4);//5
			printf("case 5\n");
			break;

		case 6:
		
			PORTB &= 0xFB;
			PORTB |= (0 << PORTB2) | ( 1<< PORTB3) | (1 << PORTB4);//6
			printf("case 6\n");
			break;

		default:

			PORTB &= 0xFF;
			PORTB |= (1 << PORTB2) | ( 1 << PORTB3) | (1 << PORTB4);//7
			printf("case 7\n");

		}
}


int main(void)
{
    /* Replace with your application code */
	uart_init();

	DDRB  = 0x1E; //set PB1 as output
	PORTB |= 0x22; //set PB1 high

	TCNT1 = 0; //set timer1 to zero
	TIMSK1 |= (1 << OCIE1A); //output compare enable
	OCR1A = 80; //5us
	TCCR1A |= (1 << COM1A0); //toggle(compare mode), output to PB1

	TCCR1B |= (1 << CS10); // prescaling clk/1
	sei();
		
	DDRD |= 0x40;//PD6 output
	TCCR0A |= 0x40;//toggle
	TCCR0A |= 0x02;//CTC
	TCCR0B |= 0x03;//clk/64
	
    while (1) 
    {
		

    }
}


ISR(TIMER1_COMPA_vect){ 
	
	TIMSK1 |= (1 << ICIE1); // input capture enable, check PB0
	TCCR1B |= (1 << ICES1); //input rising edge select		
}

ISR(TIMER1_CAPT_vect){

	TCCR1B ^= (1 << ICES1); // input falling edge capture

	if (bIsTriggered){
		//printf("down edge triggered");
		nTimeStart = ICR1; //START
		nLoopCount = 0;
		printf("Start time %d\n", nTimeStart);
		//printf("Start time %d\n", (int)nTimeStart);

		TIMSK1 |= (1 << TOIE1);  //overflow timer enabled
		
	}

	else{

		//printf("up edge triggered");
		TIMSK1 = (0 << TOIE1); //disable overflow interrupt
		TIMSK1 = (1 << OCIE1A); //enable output compare interupt

		nTimeEnd = ICR1 + nLoopCount * 256;
		
		nTimeInterval = nTimeEnd - nTimeStart;
		printf("the  time interval is %d \n", nTimeInterval/16);
		
		int Val = (nTimeInterval/16 -260) / 155;
		GenerateDiscrete(Val);		
		printf("%d\n", OCR0A);

		//printf("end icr1 %d \n", (int)ICR1);
		nTimeInterval = 0;
		nTimeStart = 0;
		nTimeEnd = 0;	

	}

	bIsTriggered ^= true;

}

ISR(TIMER1_OVF_vect){
	nLoopCount++;

}





