/*
 * GccApplication4.c
 *
 * Created: 9/11/2018 8:28:30 PM
 * Author : danyangl
 */ 

 
 #define FCPU 16000000
 #include <avr/io.h>
 #include <avr/interrupt.h>


 int main(void)
 {
	 /* Replace with your application code */

	 DDRD |= 0x40;//set PD6 output
	 PORTD |= 0x40;//PD3 1
	
	 // initialize counter
	 TIMSK0 |= (1 << TOIE0);// overflow interrupt enable
	 sei();
	 TCCR0B |= (1 << CS02); //prescaling 1024

	 while (1)
	 {


	 }
 }

 ISR(TIMER0_OVF_vect){
	 PORTD ^= (1 << PORTD6);
 }


