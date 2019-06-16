#include <avr/io.h>
#include "lcd.h"
#include "avr.h"

#define b2 405
#define c3 382
#define cs3 361
#define d3 341
#define ds3 321
#define e3 303
#define f3 286
#define fs3 270
#define g3 255
#define gs3 241
#define a3 227
#define as3 215
#define b3 202
#define c4 191
#define cs4 180
#define d4 170
#define ds4 161
#define e4 152
#define f4 143
#define fs4 135
#define g4 128
#define gs4 120
#define a4 114
#define as4 107
#define b4 101
#define c5 96
#define cs5 90
#define d5 85
#define ds5 80
#define e5 76
#define f5 72
#define fs5 68
#define g5 64
#define gs5 60
#define a5 57

#define full 16
#define half 8
#define quarter 4
#define quaver 2
#define semiquaver 1
#define timer2Start 1

int t = 70, play=1, last=0, pitch=10, curr=0, currInst=0, elapsed=0, h2=0;
long left = 0x000000, right = 0x000000;
char randm[17] = {2,5,1,2,3,2,1,5,6,7,3,6,4,2,8,2,6};
char randomN = 0;
char lInstr=-1, rInstr=-1;

int x_pos[9] = {508,1000,530,480,0,50,370,590,1023};
int y_pos[9] = {510,700,510,150,0,300,650,830,1023};
char pos_hash[9] = {2,3,4,1,0,5,8,7,6};
char to_click[2] = {0,0,0,0};
char points[2] = {0,0};

signed char classify(int x, int y){
	char x_ori = x/342;
	char y_ori = y/342;
	
	return pos_hash[y_ori*3+(2-x_ori)];
}

void display2(signed char num){
	char s[7];
	snprintf(s, 5, "%1d", num);
	lcd_puts2(s);
}

ISR(TIMER2_OVF_vect){

	char s[5];
	unsigned long x = 0;
	unsigned long y = 0;
	for(int i=0; i<4; i++){
		ADMUX &= ~(0x7);
		ADMUX |= (i*2);
		
		ADCSRA |= (1 << 6);
		while(ADCSRA & (1 << 6));
		
		x = ADC;
		lcd_pos(i%2,(i/2)*4);
		
		ADMUX &= ~(0x7);
		ADMUX |= (i*2)+1;
		
		ADCSRA |= (1 << 6);
		while(ADCSRA & (1 << 6));
		
		y = ADC;
		
		signed char ans = classify(x,y);
		if(ans==to_click[i] && ans){
			points[i/2]++;
			to_click[i] = 0;
		}
		
		lcd_pos(0,(i/2)*8);
		display2(points[i/2]);
	}
	
	TCNT2 = timer2Start;
}

typedef struct Note{
	int pitch;
	char dur;
}Note;

typedef struct Song{
	int length;
	char name[17];
	Note song[];
}Song;

Song faded={
	355,"Faded - A.Walker",{
	{g4,4},{g4,4},{g4,4},{b4,4},{e5,4},{e5,4},
	{e5,4},{d5,4},{b4,4},{b4,4},{b4,4},{b4,4},{fs4,4},
	{fs4,4},{fs4,4},{e4,4},{0,2},{g4,2},{g4,2},{e4,2},
	{g4,2},{e4,2},{g4,2},{a4,2},{b4,4},{g4,2},
		{g4,2},
		{d4,2},
		{b4,8},
		{0,8},
		{g4,2},
		{g4,2},
		{g4,2},
		{fs4,6},
		{fs4,2},
		{fs4,2},
		{e4,2},
		{g4,4},
		{0,2},
		{g4,2},
		{g4,2},
		{e4,2},
		{g4,2},
		{e4,2},
		{g4,2},
		{a4,2},
		{b4,4},
		{g4,2},
		{g4,2},
		{d5,2},
		{b4,6},
		{0,10},
		{g4,2},
		{g4,2},
		{a4,10},
		{0,2},
		{b4,2},
		{b4,2},
		{b4,2},
		{g4,4},
		{g4,4},
		{g4,4},
		{b4,4},
		{e5,4},
		{e5,4},
		{e5,2},
		{b4,2},
		{b4,2},
		{b4,2},
		{b4,4},
		{b4,4},
		{b4,4},
		{b4,4},
		{fs4,4},
		{fs4,4},
		{fs4,2},
		{b4,2},
		{fs4,2},
		{b4,2},
		{g4,8},
		{0,2},
		{g4,1},
		{a4,1},
		{a4,2},
		{a4,1},
		{b4,1},
		{a4,2},
		{g4,2},
		{g4,6},
		{b4,2},
		{b4,2},
		{b4,2},
		{b4,8},
		{0,2},
		{g4,1},
		{g4,1},
		{g4,2},
		{g4,1},
		{a4,1},
		{fs4,2},
		{fs4,2},
		{fs4,2},
		{g4,8},
		{0,2},
		{b4,2},
		{b4,2},
		{b4,2},
		{b4,8},
		{0,6},
		{g4,2},
		{e4,2},
		{d4,8},
		{g4,2},
		{d5,2},
		{d5,1},
		{b4,9},
		{0,2},
		{g4,2},
		{d5,2},
		{d5,1},
		{b4,7},
		{a4,4},
		{b4,2},
		{b4,2},
		{b4,2},
		{b4,8},
		{g4,3},
		{g4,1},
		{e4,2},
		{c5,14},
		{0,2},
		{d5,2},
		{b4,2},
		{d5,2},
		{b4,2},
		{d5,2},
		{b4,2},
		{d5,2},
		{b4,2},
		{a4,1},
		{c5,7},
		{e4,2},
		{c5,4},
		{b4,4},
		{0,23},
		{e4,2},
		{c5,4},
		{b4,4},
		{0,15},
		{g4,2},
		{fs4,4},
		{0,2},
		{e4,2},
		{c5,4},
		{b4,4},
		{0,23},
		{e4,2},
		{c5,4},
		{d5,4},
		{0,15},
		{g4,2},
		{fs4,4},
		{0,2},
		{e4,2},
		{c5,4},
		{b4,4},
		{0,2},
		{g4,2},
		{g4,2},
		{g4,2},
		{g4,2},
		{e4,2},
		{g4,2},
		{a4,2},
		{b4,4},
		{g4,2},
		{g4,2},
		{d4,2},
		{b4,6},
		{0,10},
		{g4,2},
		{g4,2},
		{g4,2},
		{fs4,6},
		{fs4,2},
		{fs4,2},
		{e4,2},
		{g4,4},
		{0,2},
		{g4,2},
		{g4,2},
		{a4,2},
		{g4,2},
		{e4,2},
		{g4,2},
		{a4,2},
		{b4,6},
		{g4,2},
		{d5,3},
		{b4,5},
		{0,10},
		{g4,2},
		{g4,2},
		{a4,10},
		{0,2},
		{b4,2},
		{b4,2},
		{b4,2},
		{b4,16},
		{0,11},
		{b4,2},
		{b4,2},
		{b4,2},
		{b4,16},
		{0,11},
		{g4,2},
		{d5,2},
		{d5,1},
		{b4,3},
		{g4,2},
		{d5,3},
		{b4,3},
		{g4,2},
		{d5,2},
		{d5,1},
		{a4,3},
		{g4,2},
		{g4,2},
		{b4,4},
		{g4,2},
		{d5,2},
		{d5,1},
		{a4,9},
		{0,2},
		{g4,2},
		{d5,2},
		{d5,1},
		{a4,9},
		{0,8},
		{g4,1},
		{g4,2},
		{a4,1},
		{g4,2},
		{g4,1},
		{d5,3},
		{g4,1},
		{a4,1},
		{g4,2},
		{g4,2},
		{g4,1},
		{g4,2},
		{a4,1},
		{g4,2},
		{g4,1},
		{d5,3},
		{g4,1},
		{a4,1},
		{g4,2},
		{g4,2},
		{g4,1},
		{g4,2},
		{a4,1},
		{g4,2},
		{g4,1},
		{d5,3},
		{g4,1},
		{a4,1},
		{g4,2},
		{g4,2},
		{g4,1},
		{g4,2},
		{a4,1},
		{g4,2},
		{g4,1},
		{d5,3},
		{g4,1},
		{a4,1},
		{g4,2},
		{g4,2},
		{0,2},
		{b4,2},
		{b4,2},
		{b4,2},
		{b4,8},
		{0,6},
		{g4,2},
		{e4,2},
		{d4,8},
		{g4,2},
		{d5,2},
		{d5,1},
		{b4,9},
		{0,2},
		{g4,2},
		{d5,2},
		{d5,1},
		{b4,7},
		{a4,4},
		{b4,2},
		{b4,2},
		{b4,2},
		{b4,8},
		{g4,3},
		{g4,1},
		{e4,2},
		{c5,14},
		{0,2},
		{d5,2},
		{b4,2},
		{d5,2},
		{b4,2},
		{d5,2},
		{b4,2},
		{d5,2},
		{b4,2},
		{a4,1},
		{c5,7},
		{e4,2},
		{c5,4},
		{b4,4},
		{0,23},
		{e4,2},
		{c5,4},
		{b4,4},
		{0,15},
		{g4,2},
		{fs4,4},
		{0,2},
		{e4,2},
		{c5,4},
		{b4,4},
		{0,23},
		{e4,2},
		{c5,4},
		{d5,4},
		{0,15},
		{g4,2},
		{fs4,4},
		{0,2},
		{e4,2},
		{c5,4},
		{b4,4},
		{g4,4},
		{g4,4},
		{g4,4},
		{b4,4},
		{e5,4},
		{e5,4},
		{e5,4},
		{d5,4},
		{b4,4},
		{b4,4},
		{b4,4},
		{b4,4},
		{fs4,16},
		{0,16}
	}};


Song* songs[] = {&faded,&faded};

void playNote(int note, unsigned long dur){
	unsigned long n = 1;
	if(note==0){
		note = 1000000/t;
	}else{
		note *= 10;
		note /= pitch;
		n = 1000000 / note / t;
	}
	for(unsigned long duration=0; duration<dur; duration++){
		randomN = (randomN+4)%18;
		if(elapsed%4==0){
			PORTC = ((PORTC & ~(0x07)) | randm[randomN]);
			PORTC ^= (1 << 5);
			to_click[1] = randm[randomN];
			to_click[3] = randm[randomN];
		}
		for(unsigned long i=0; i<n; i++){
			avr_wait(note);
			if(n>1) PORTC ^= (1 << 7);
		}
		elapsed++;
		/*
		if(elapsed==songs[curr+1]->song[h2].dur){
			to_click[1] = 0;
			to_click[3] = 0;
			h2++;
			elapsed=0;
		}*/
		
	}
}

void playSong(){
	int i=0;
	elapsed=0;
	h2=0;
	while(i<songs[curr]->length){
		if(play){
			PORTB = (PORTB & ~(3 << 3)) | (((songs[curr]->song[i].pitch >> 1) & 0x0003) << 3);
			PORTC = ((PORTC & ~(1 << 3)) | ((songs[curr]->song[i].pitch & 0x0001) << 3));
			if(songs[curr]->song[i].pitch != 0) 
				PORTC ^= (1 << 4);
			to_click[0] = 0;
			to_click[2] = 0;
			if(songs[curr]->song[i].pitch){
				to_click[0]=songs[curr]->song[i].pitch%8+1;
				to_click[2]=songs[curr]->song[i].pitch%8+1;
			}
			playNote(	songs[curr]->song[i].pitch,
						songs[curr]->song[i].dur);
			avr_wait(30000/t);
			to_click[0] = 0;
			to_click[2] = 0;
			i++;
		}
	}
}

int main(void)
{
	DDRC  = 0xFF;
	PORTC = 0x00;
	
	DDRB |= (3 << 3);
	PORTB &= ~(3 << 3);
	
	DDRA = 0x00;
	
	ADMUX = 0;
	ADMUX |= (1 << 6);
	ADCSRA |= (1 << 7);
	
	TIMSK |= (1 << TOIE2);
	TCCR2 |= (1 << CS20)|(1 << CS21)|(1 << CS22);
	TCNT2 = timer2Start;
	
	sei();
	
	lcd_init();
	lcd_clr();
    while (1) 
    {
		playSong();
    }
}

