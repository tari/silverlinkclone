/*
 * Usb_Test.h
 *
 * Created: 5/4/2014 1:05:12 AM
 *  Author: geek
 */ 


#ifndef USB_TEST_H_
#define USB_TEST_H_
		#include <avr/io.h>
		#include <avr/wdt.h>
		#include <avr/power.h>
		#include <avr/interrupt.h>
		#include <stdbool.h>
		#include <string.h>

		#include "Descriptors.h"
       // #include <LUFA/Common/Common.h>
      //  #include <LUFA/Version.h>
        #include <LUFA-src/Drivers/Board/LEDs.h>
		#include <LUFA-src/Drivers/Peripheral/Serial.h>
        #include <LUFA-src/Drivers/Peripheral/SPI.h>
		#include <LUFA-src/Drivers/USB/USB.h>
		#include <LUFA-src/Platform/Platform.h>
        
        #include "TiLink.h"
//		#include "src/asf.h"
		//#include <LUFA/Drivers/USB/USB.h>
		//#include <LUFA/Drivers/Board/LEDs.h>
		//#include <LUFA/Platform/Platform.h>
		
	/* Macros: */
	/** LED mask for the library LED driver, to indicate that the USB interface is not ready. */
	#define LEDMASK_USB_NOTREADY      LEDS_LED3

	/** LED mask for the library LED driver, to indicate that the USB interface is enumerating. */
	#define LEDMASK_USB_ENUMERATING  (LEDS_LED2 | LEDS_LED3)

	/** LED mask for the library LED driver, to indicate that the USB interface is ready. */
	#define LEDMASK_USB_READY        (LEDS_LED1)

	/** LED mask for the library LED driver, to indicate that an error has occurred in the USB interface. */
	#define LEDMASK_USB_ERROR        (LEDS_LED1 | LEDS_LED3 | LEDS_LED3)
	
void SetupHardware(void);
void EndPointOUTproccess(void);
void EndPointInProcess(void);
#endif /* USB TEST_H_ */