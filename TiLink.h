/*
 * TiLink.h
 *
 * Created: 5/4/2014 14:43:41 PM
 *  Author: geek
 */ 

#ifndef TILINK_H_
#define TILINK_H_

 #include <LUFA-src/Drivers/Board/LEDs.h>


#define nop() asm volatile("nop");
#define TIwhite TIring  //PF0
#define TIred TItip     //PF1


#define TiR (PINF &  1)
#define TiT ((PINF >> 1) &  1)
#define TiW (PINF &  1)
#define TiRed ((PINF >> 1) &  1)



#define ERR_READ_TIMEOUT 1000
#define ERR_WRITE_TIMEOUT 2000
#define TIMEOUT 12000
#define GET_ENTER_TIMEOUT 300000
#define delay 1
#define delayl 1
inline void TiRingOutput(void){
	DDRF |= (1 << PORTF0);
}

inline void TiTipOutput(void){
	DDRF |= (1 << PORTF1);
}
inline void TiRingInput(void){
	DDRF &= ~(1 << PORTF0);	
}

inline void TiTipInput(void){
	DDRF &= ~(1 << PORTF1);
}

inline void TiRingHigh(void){
	PORTF |= (1 << PORTF0);
}

inline void TiRingLow(void){
	PORTF &= ~(1 << PORTF0);
}


inline void TiTipHigh(void){
	PORTF |= (1 << PORTF1);
}

inline void TiTipLow(void){
	PORTF &= ~(1 << PORTF1);
}



inline void TiWhiteOutput(void){
	DDRF |= (1 << PORTF0);
}

inline void TiRedOutput(void){
	DDRF |= (1 << PORTF1);
}
inline void TiWhiteInput(void){
	DDRF &= ~(1 << PORTF0);
}

inline void TiRedInput(void){
	DDRF &= ~(1 << PORTF1);
}

inline void TiWhiteHigh(void){
	PORTF |= (1 << PORTF0);
}

inline void TiWhiteLow(void){
	PORTF &= ~(1 << PORTF0);
}


inline void TiRedHigh(void){
	PORTF |= (1 << PORTF1);
}

inline void TiRedLow(void){
	PORTF &= ~(1 << PORTF1);
}

void resetLines(void);
uint16_t par_put(uint8_t *data, uint32_t len);
uint16_t par_get(uint8_t *data, uint32_t len);
int TiPut(uint8_t *data, uint32_t len);
int TiGet(uint8_t *data, uint32_t len);
#endif /* TILINK_H_ */