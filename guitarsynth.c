/* 	GuitarSynth v.01

	ATtiny2313 picking up a guitar tone and sending out a square wave of the same frequency.

	Lowest frequency from guitar ~80Hz or 12.5ms or 12500us. No real need for overflow counter. Highest frequency ~1.2kHz or .83ms or 830us. Some prescaler magic needed on 8 bit timer 0.


	schizoakustik */

#include <avr/io.h>
#include <avr/interrupt.h>

unsigned int pulse_length; //  How many clocks between rising and falling edge? 
//unsigned int overflow_counter; // If timer overflows - how many times? (ovf_counter*1000+pulse_length).


ISR(TIMER1_CAPT_vect) // On capture interrupt
{
	if(TCCR1B & (1 << ICES1)) // If we found a rising edge...
	{

		TCNT1 = 0; // Clear counter
		TCCR1B &= ~(1 << ICES1); // Capture on falling edge
	}

	else // ...we found a falling edge
	{
		TCCR1B &= ~(1 << CS11); //Stop timer 1
		pulse_length = ICR1; // Save timer 1 count to value
		ovf_counter = 0;
		TCCR1B |= (1 << ICES1); // Capture on rising edge
	}
}

ISR(TIMER1_OVF_vect)
{
	overflow_counter++;
}

int main (void)
{	
		DDRB |= 0xFF; // Set Port B as output

		DDRD &= ~(1 << PD6); // Set Pin D6 (ICP) as input

		TCCR1B |= ((1 << WGM12) | (1 << ICNC1) | (1 << ICES1); // CTC Mode | Noise canceler | Capture on rising edge
		TIMSK |= ((1 << TOIE1) | (1 <<  ICIE1)); // Overflow interrupt enable | Input capture interrupt enable
		OCR1A = 0x03E7; // TOP = 1000-1 (1us)

		sei(); // Global interrupts enable

		TCCR1B |= (1 << CS11)); // Start timer @1MHz
}

while(1)
{

}
