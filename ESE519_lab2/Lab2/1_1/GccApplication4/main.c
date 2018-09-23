/*
 * GccApplication4.c
 *
 * Created: 9/11/2018 8:59:41 PM
 * Author : danyangl
 */ 

 #define FCPU 16000000000

 #include <avr/io.h>
 #include <avr/interrupt.h>

 int main(void)
 {
	 DDRB |= 0x20;//set PB5 output
	 PORTB |= 0x21//PB5 PB0 1
	 
	 TIMSK1 |= (1 << ICIE1);//overflow interrupt enable

	 TIFR1 |= (1 << ICF1);//input capture flag

	 sei();

	 TCCR1B |= (1 << CS12) | (1 << CS10) | (0 << ICES1);//prescaling 1024, input capture falling edge
	 
	 
	 /* Replace with your application code */
	 while (1)
	 {

	 }
 }

 ISR(TIMER1_CAPT_vect){
	 
	 PORTB ^= (1 << PORTB5);
	 TCCR1B ^= (1 << ICES1);//reverse input capture edge
	 
 }