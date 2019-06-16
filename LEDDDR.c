#include <avr/io.h>
#include <avr/interrupt.h>

#define timerStart (1 << 16)-(1 << 7)
#define timer0Start (1 << 8)-(1 << 6)

#define LEFTDDR DDRC
#define LEFTP PORTC

#define RIGHTDDR DDRD
#define RIGHTP PORTD

#define CLKDDR DDRA
#define CLKP PINA
#define CLK 4

#define ENDDR DDRA
#define ENP PINA
#define EN 5

#define ADDRDDR DDRA
#define ADDRP PINA

char leftOrder[8] = {0,2,4,5,7,6,3,1};
char rightOrder[8] = {7,5,3,2,0,1,4,6};

long ledPwdCounter = 0;

unsigned int left[8] = {0,0,0,0,0,0,0,0};
unsigned int right[8] = {0,0,0,0,0,0,0,0};

ISR(TIMER0_OVF_vect)
{
	for(long i=0; i<8; i++){
		if(right[i]) right[i] = (right[i]+1)%8;
	}
	for(long i=0; i<8; i++){
		if(left[i]) left[i] = (left[i]+1)%8;
	}
    TCNT0 = timer0Start;
}

ISR (TIMER1_OVF_vect)
{
	for(long i=0; i<8; i++){
		if(right[i] > ledPwdCounter)
			RIGHTP |= (1 << rightOrder[i]);
		else RIGHTP &= ~(1 << rightOrder[i]);
	}
	for(long i=0; i<8; i++){
		if(left[i] > ledPwdCounter)
			LEFTP |= (1 << leftOrder[i]);
		else LEFTP &= ~(1 << leftOrder[i]);
	}
	
	ledPwdCounter = (ledPwdCounter + 1)%7;
	TCNT1 = timerStart;   // for 1 sec at 8 MHz
}

int main(void)
{
    /* Replace with your application code */
    CLKDDR &= ~(1 << CLK);
    CLKP &= ~(1 << CLK);
    ENDDR &= ~(1 << EN);
    ENP &= ~(1 << EN);
	
	LEFTDDR = 0xFF;
	LEFTP = 0x00;
	RIGHTDDR = 0xFF;
	RIGHTP = 0x00;
	
	ADDRDDR &= ~(0xFF);
	ADDRP &= ~(0xFF);
	
	unsigned long pos = 0;
	unsigned long group = 0;
	
	
	TIMSK= 1<<TOIE1;
	TCNT1 = timerStart;
	TCCR1B |= (1 << CS10);
	
	TIMSK |= (1 << TOIE0);
	TCCR0 |= (1 << CS02)|(1 << CS00);
	TCNT0 = timer0Start;
	
	sei();
	
	DDRB |= (1 << 1);
	DDRB |= (1 << 2);
	unsigned long a=0,b=0;
    while (1) 
    {
		
		if(a != ((PINA >> 6) & 1)){
			right[(PINA >> 3) & 0x0007] = 1;
			a = 1-a;
		}
		if(b != ((PINA >> 7) & 1)){
			left[PINA & 0x0007] = 1;
			b = 1-b;
		}
    }
}

