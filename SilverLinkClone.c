/*/*
 * Usb_Test.c
 *
 * Created: 5/3/2014 20:27:52 PM
 *  Author: geek
 */ 


#include <avr/io.h>
#include "SilverLinkClone.h"



int main(void)
{
	SetupHardware();
	
//	uint8_t SendingDataPacket = 0;   //Are we processing a Send packet?
//	uint8_t ReceivingDataPacket = 0; //Are we receiving a packet from the pc?
//	uint8_t ReceivedData[32];   // Received Data Buffer


	while(1)
	{
		if (USB_DeviceState == DEVICE_STATE_Configured)
		{//If we have a configured Usb Device
		
			EndPointInProcess();  //Process In first so there is no chance of collision
			//USB_USBTask();
			EndPointOUTproccess();
			
		}//End Configured USB Device
		//USB_USBTask();
	} //End main While while
}

void EndPointInProcess(){
	long loop = 0;
	if (!(TiW && TiRed))   //IF there is a line pulled try for incomming packet Else do NOTHIGN
	{
		Endpoint_SelectEndpoint(VENDOR_IN_EPADDR);
		if (Endpoint_IsINReady())
		{
			#ifdef DEBUG
			Serial_SendString("Calc -> PC\r\n");
			#endif
			uint8_t SendData[32];   //data Buffer
		
			uint16_t a;
			uint8_t len;
			uint16_t packetlength;
			if ( (a = par_get(SendData,4)) != 0)
			{
				resetLines();
				return;
			}//End Fubar?
			switch(SendData[1])
			{
				case 0x09:
				case 0x2D:
				case 0x56:
				case 0x5A:
				case 0x68:
				case 0x87:
				case 0x92: // If 4 byte packet
				case 0x6D:
				#if DEBUG
					Serial_SendString("Endpoint In Data[4] ");
					Serial_SendData(SendData,4);
					Serial_SendString("\r\n");
				#endif
					Endpoint_Write_Stream_LE(&SendData,4,NULL);
					Endpoint_ClearIN();    //4 byte packet sent as response quit now
					
					break;
			
				case 0x36:
					//Error codes are 5 bytes guarenteed we have 4 already 
					if ( (a = par_get(SendData+4,3) ) != 0 )
					{
						resetLines();
						break;
					} //end Fubar
					
					Endpoint_Write_Stream_LE(&SendData,7,NULL);
					Endpoint_ClearIN();
					
					break;
				default: //Large packet
					packetlength = ((uint16_t)SendData[3] << 8) | SendData[2];
					packetlength += 4+2;
					#ifdef DEBUG
					Serial_SendString("Initial Packet Length ");
					Serial_SendData(&packetlength,sizeof(uint16_t));
					Serial_SendString("\r\n");
					#endif					
					len = (packetlength>=32)?32:packetlength;  // Get len as 32 | less
					#ifdef DEBUG
					Serial_SendString("Initial Len ");
					Serial_SendByte(len);
					Serial_SendString("\r\n");
					#endif
					if ( (a = par_get(SendData+4,len-4)) != 0 )
					{
						resetLines();
						break;
					} //end Fubar
					
					Endpoint_Write_Stream_LE(&SendData,len,NULL);
					Endpoint_ClearIN();
					
					packetlength = (packetlength > len)?(packetlength - len):0;
					
					while(packetlength)
					{
						if (Endpoint_IsINReady())
						{
							len = (packetlength>32)?32:packetlength;  // Get len as 32 | less
						
							if ( (a = par_get(SendData,len) ) != 0 )
							{
								resetLines();
								break;
							} //end Fubar
						
							Endpoint_Write_Stream_LE(&SendData,len,NULL);
							Endpoint_ClearIN();
						
							packetlength = (packetlength >= len)?(packetlength - len):0;
						}
						
						
					} //End While packet Left
				
				break;
		
			}//End Switch large Small send
		}//End Is In ready?
	} //End if ! TiW && TiRed

}//End EndpointInProcess
void EndPointOUTproccess(){
	Endpoint_SelectEndpoint(VENDOR_OUT_EPADDR);
	if (Endpoint_IsOUTReceived() && Endpoint_IsReadWriteAllowed())
	{
		uint16_t ByteCount=0;
		uint8_t Count,ErrorCode;
		uint8_t ReceivedData[32];
		uint16_t packetlength;
		#ifdef DEBUG
		Serial_SendString("PC -> Calc\r\n");
		#endif
		Count = Endpoint_BytesInEndpoint();
		while((ErrorCode = Endpoint_Read_Stream_LE(ReceivedData, Count, &ByteCount)) == ENDPOINT_RWSTREAM_IncompleteTransfer);
		#ifdef DEBUG
		Serial_SendString("Endpoint Out data[4] ");
		Serial_SendData(ReceivedData,Count);
		Serial_SendString("\r\n");
		#endif
		
		if (ErrorCode != ENDPOINT_RWSTREAM_NoError)
		{
		}//End incoming Error
		
		
		Endpoint_ClearOUT();
		uint16_t a;
		switch(ReceivedData[1])
		{
			case 0x09:
			case 0x2D:
			case 0x56:
			case 0x5A:
			case 0x68:
			case 0x87:
			case 0x92: // If 4 byte packet
			case 0x6D:
				#if DEBUG
				Serial_SendString("Start 4 Byte Transaction\r\n");
				#endif
				if ( (a = par_put(ReceivedData,4) ) != 0 )
				{
					resetLines();
					break;
				}//End Foobar
				break;
			
			default://large packet
			#ifdef DEBUG
				Serial_SendString("Start MultiByte PC -> Calc Transaction\r\n");
			#endif
				packetlength = ((uint16_t)ReceivedData[3] << 8) | ReceivedData[2]; //Length of packet in packet length
				packetlength += 4+2; //packet length + header and checksum
					
				if( ( a = par_put(ReceivedData,Count)) != 0)
				{
					resetLines();
					break;
				} //End if Fubar
					
				packetlength = (packetlength >= Count)?(packetlength-Count):0;
				
				while (packetlength) //While we have data left in our packet
				{
					
					if(Endpoint_IsOUTReceived() && Endpoint_IsReadWriteAllowed())
					{
						Count = Endpoint_BytesInEndpoint();
						while((ErrorCode = Endpoint_Read_Stream_LE(ReceivedData, Count, &ByteCount)) == ENDPOINT_RWSTREAM_IncompleteTransfer);
						Endpoint_ClearOUT();
						
						if( ( a = par_put(ReceivedData,Count)) != 0)
						{
							resetLines(); 
							break;
						} //End if Fubar
						
						packetlength = (packetlength > Count)?(packetlength-Count):0;
						
					}//Endpoint is readable/has data
											
				} //End packetLength
			
			break;
		}	//End Switch Received Data	
		
	}//End Endpoint Out ready
	
	
}//End EndpointInProcess
void SetupHardware(void){
	
	    ADCSRA = 0;
	    MCUCR &= ~PUD;

		
		
		MCUSR &= ~(1 << WDRF);
		wdt_disable();
		
		/* Disable clock division */
		clock_prescale_set(clock_div_1);
		LEDs_Init();		
		resetLines();
		#ifdef DEBUG
		Serial_Init(115200,false);
		Serial_SendString("Init\r\n");
		#endif
		USB_Init();
		GlobalInterruptEnable();
		
		//Timer 1 for usb Task Handler interupt
			
		TCCR1B |= (1 <<	WGM12);
		OCR1A = 0x09C3;
		TCCR1B = ((1 << CS11) | (1 << CS11));
		
		TIMSK1 |= (1 << OCIE1A);
		
}
ISR(TIMER1_COMPA_vect){
	//LEDs_ToggleLEDs(LEDS_LED2);
	USB_USBTask();
}
void EVENT_USB_Device_Connect(void)
{
	/* Indicate USB enumerating */
	LEDs_SetAllLEDs(LEDMASK_USB_ENUMERATING);
	}

void EVENT_USB_Device_Disconnect(void)
{
	/* Indicate USB not ready */
	LEDs_SetAllLEDs(LEDMASK_USB_NOTREADY);
}

void EVENT_USB_Device_ConfigurationChanged(void)
{
	bool ConfigSuccess = true;

	/* Setup HID Report Endpoints */
    	ConfigSuccess &= Endpoint_ConfigureEndpoint(VENDOR_IN_EPADDR, EP_TYPE_BULK, 32, 1);
	    ConfigSuccess &= Endpoint_ConfigureEndpoint(VENDOR_OUT_EPADDR, EP_TYPE_BULK, 32, 1);
		

	/* Indicate endpoint configuration success or failure */
	LEDs_SetAllLEDs(ConfigSuccess ? LEDMASK_USB_READY : LEDMASK_USB_ERROR);
}
