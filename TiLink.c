#include <avr/io.h>
//#include "src/asf.h"
#include <stdlib.h>
#include "TiLink.h"
#include <util/delay.h>
//#include "ssd1306.h"
void resetLines() {
	//pinMode(TIring, INPUT);           // set pin to input
	TiRingInput();	
	//digitalWrite(TIring, HIGH);       // turn on pullup resistors
	TiRingHigh();
    //pinMode(TItip, INPUT);            // set pin to input
	TiTipInput();
	//digitalWrite(TItip, HIGH);        // turn on pullup resistors
	TiTipHigh();

}


uint16_t par_put(uint8_t *data, uint32_t len) {
	int bit;
	int j;
	long previousMillis = 0;
	uint8_t byte;
	
	for(j=0;j<len;j++) {
		LEDs_TurnOnLEDs(LEDS_LED2 | LEDS_LED4);
		byte = data[j];
		for (bit = 0; bit < 8; bit++) {
			previousMillis = 0;
			while ( ((TiR<<1) | TiT) != 0x03) {
				if (previousMillis++ > TIMEOUT)
				return (ERR_WRITE_TIMEOUT+j+100*bit);
				_delay_us(delayl);
			};
		if (byte & 1) {
			//pinMode(TIring,OUTPUT);
			TiRingOutput();
			//digitalWrite(TIring,LOW);
			TiRingLow();
			_delay_us(delay);	
			previousMillis = 0;
			while (TiT == 1) {
					if (previousMillis++ > TIMEOUT)
					return (ERR_WRITE_TIMEOUT+10+j+100*bit);
					_delay_us(delayl);
				};
			_delay_us(delay);	
			TiRingHigh();
			previousMillis = 0;
			while (TiT == 0) {
				if (previousMillis++ > TIMEOUT)
					return (ERR_WRITE_TIMEOUT+20+j+100*bit);
					_delay_us(delayl);
				};
			}
			else {
				//pinMode(TItip,OUTPUT);
				TiTipOutput();
				//digitalWrite(TItip,LOW);      //should already be set because of the pullup resistor register
				TiTipLow();
				previousMillis = 0;
				while (TiR == 1) {
					if (previousMillis++ > TIMEOUT)
					return (ERR_WRITE_TIMEOUT+30+j+100*bit);
					_delay_us(delayl);
				};
				_delay_us(delay);
				TiTipHigh();
				
				previousMillis = 0;
				while (TiR == 0) {
					if (previousMillis++ > TIMEOUT)
					return (ERR_WRITE_TIMEOUT+40+j+100*bit);
					_delay_us(delayl);
				};
			}
			_delay_us(delay);
			resetLines();
			byte >>= 1;
		}
	LEDs_TurnOffLEDs(LEDS_LED2 | LEDS_LED4);
	}
	resetLines();
	
	return 0;
}

uint16_t par_get(uint8_t *data, uint32_t len) {
	int bit;
	int j;
	long long previousMillis = 0;
	
	for(j = 0; j < len; j++) {
		LEDs_TurnOnLEDs(LEDS_LED2 | LEDS_LED4);
		uint8_t byteout = 0;
		for (bit = 0; bit < 8; bit++) {
			previousMillis = 0;
			while ((TiRed<<1 | TiW) == 0x03) {
				if (previousMillis++ > GET_ENTER_TIMEOUT)
				return ERR_READ_TIMEOUT+j+100*bit;
				_delay_us(delayl);
			}
			
			if (TiRed == 0x01) {
				byteout = (byteout >> 1) | 0x80;
				_delay_us(delay);
				TiRedOutput();
				TiRedLow();
				previousMillis = 0;
				while (TiW == 0) {            //wait for the other one to go low
					if (previousMillis++ > TIMEOUT)
					return ERR_READ_TIMEOUT+10+j+100*bit;
					_delay_us(delayl);
				}
				_delay_us(delay);
				TiRedHigh();
				} 
				else {
				byteout = (byteout >> 1) & 0x7F;
				_delay_us(delay);
				TiWhiteOutput();
				TiWhiteLow();
				previousMillis = 0;
				while (TiRed == 0) {
					if (previousMillis++ > TIMEOUT)
					return ERR_READ_TIMEOUT+20+j+100*bit;
					_delay_us(delayl);
				}
				//pinMode(TItip,OUTPUT);
				_delay_us(delay);
				TiWhiteHigh();
			}
			resetLines();
		}
		data[j] = byteout;
		//delayMicroseconds(6);
		LEDs_TurnOffLEDs(LEDS_LED2 | LEDS_LED4);
	}
	return 0;
}


/*
for grabbing from tios we can do this in 32byte bursts just to not have to fight with ram and because that
is our endpoint suize
length + 2 IF > 0 Says BW


Single byte Commands

CTS 09h
Request Ver 2Dh
ACK 56h
ERR 5Ah
RDY 68h
SCR 6Dh
KEY 87h
EOT 92h

*/