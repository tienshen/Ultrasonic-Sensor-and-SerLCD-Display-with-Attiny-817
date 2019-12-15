/*
 * Ultasonic Sensor & LCD
 * The Attiny chip can read signals from the ultrasonic sensor, 
 * calculate the distance with width of the signal, 
 * and display distance on the 16x2 serLCD display.
 *
 * Created: 12/11/2019 
 * Author : Tien Li Shen
 */ 

#include <avr/io.h>
#include <string.h>
#include <util/delay.h>
#define USART0_BAUD_RATE(BAUD_RATE) ((float)(3333333 * 64 / (16 *(float)BAUD_RATE)) + 0.5)

#define LCD_FRAME_PERIOD 100
#define F_CPU (10000000UL)
#define BUFFER_SIZE 32
volatile int textBuffer[BUFFER_SIZE];
volatile int buffer_data_size = 0;
volatile int head = 0;
volatile int counter = 0;
volatile int act = 0; // stores the distance

int main(void) {
	SPI_init();
	//USART0_init();
	
	
	PORTB.DIRSET = PIN4_bm; // trigger
	PORTB.DIRCLR = PIN3_bm; // echo
	
	TCA0.SINGLE.PER = 0xFFFF; // set top value
	TCA0.SINGLE.CTRLA = 0x1; // TCA enabled and
	
	TCB0.CTRLB = 0b00010100; // enable input capture pulse width mode and Compare/Capture Output has a valid value
	TCB0.INTCTRL = 0x01; // enable capture interrupt
	TCB0.EVCTRL = 0x01; // enables the event input capture
	TCB0.CTRLA = 0x01; // enable TCB 
	
	EVSYS.ASYNCCH1 = 0x0D; // channel is PB3
	EVSYS.ASYNCUSER0 = 0x4; // set user to ASYNCCH1
	
	
	while(1) {
		/* send 10 us trigger pulse */
		TCA0.SINGLE.CNT = 0; // start count at 0
		PORTB.OUTSET |= PIN4_bm; // pin 4 high
		while(TCA0.SINGLE.CNT < 50); // wait for 17.125 us
		PORTB.OUTCLR |= PIN4_bm; // pin 4 low

		// 3 ^ 6 ticks per second
		// 1/ 3^6 second per tick
		// time (micro s) = CNT * 1/3
		// distance (cm) = time (microseconds) / 58
		// distance (mm) = distance (cm) * 10
		// distance (mm) = CNT * 1/3 * 10 / 58
		
		/* capture echo and calculate distance */
		while(!TCB0.INTFLAGS); // wait for interrupt to clear
		act = TCB0.CCMP * 3.333 / 58 ; // retrieve capture/compare value 
		writeToLCD();
		_delay_ms(10);
	}
}

void writeToLCD() {
	counter++; // 
	if((counter>= LCD_FRAME_PERIOD) && (head >= buffer_data_size)) {
		counter = 0;
		int LCD_index = 0;
		clearLCD(&LCD_index);
		char text[32];
		sprintf(text, "Dist: %d mm", act);
		writeToTextBuffer(text, strlen(text), &LCD_index);
		buffer_data_size = LCD_index;
		head = 0;
	}
	
	else if (head < buffer_data_size) {
		//USART0.TXDATAL = textBuffer[head++];
		SPI0.DATA = textBuffer[head++];
	}
}

void writeToTextBuffer(char* text, int length, int* offset){
	for (int i = 0; i < length; i++) {
		textBuffer[(*offset)++] = text[i];
	}
}

void clearLCD(int* offset) {
	textBuffer[(*offset)++] = 124;
	textBuffer[(*offset)++] = 45;
}

void SPI_init(){
	// SPI Master mode selected, prescaler = 0x0 = DIV128 = CLK_PER/128, SPI enabled
	SPI0.CTRLA = SPI_MASTER_bm | SPI_ENABLE_bm | 0x6;
	SPI0.CTRLB = 0b10000100;
	SPI0.INTCTRL = 0b11110001;
	SPI0.CTRLB = SPI_SSD_bm; //
	// Output on pins PA3 & PA1, PA3 = SPI Clock, PA1 = SPI Data
	PORTA.DIRSET = PIN3_bm | PIN1_bm; //
}

void USART0_init(void)
{
	PORTB.DIRSET = PIN2_bm; // PB2 is the USART TX (transmitter)
	USART0.BAUD = (uint16_t)USART0_BAUD_RATE(9600); // 9600 is the default baud for serLCD
	USART0.CTRLB |= USART_TXEN_bm; // set 
}