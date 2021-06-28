/*
 * Descriptors.h
 *
 * Created: 5/4/2014 1:04:39 AM
 *  Author: geek
 */ 


#ifndef DESCRIPTORS_H_
#define DESCRIPTORS_H_

#include <LUFA-src/Drivers/USB/USB.h>
#include <avr/pgmspace.h>

/** Enum for the device string descriptor IDs within the device. Each string descriptor should
		 *  have a unique ID index associated with it, which can be used to refer to the string from
		 *  other descriptors.
		 */
enum StringDescriptors_t
		{
			STRING_ID_Language     = 0, /**< Supported Languages string descriptor ID (must be zero) */
			STRING_ID_Manufacturer = 1, /**< Manufacturer string ID */
			STRING_ID_Product      = 2, /**< Product string ID */
		};
		/** Endpoint address of the Bulk Vendor device-to-host data IN endpoint. */
		#define VENDOR_IN_EPADDR               (ENDPOINT_DIR_IN  | 1)

		/** Endpoint address of the Bulk Vendor host-to-device data OUT endpoint. */
		#define VENDOR_OUT_EPADDR              (ENDPOINT_DIR_OUT | 2)

		/** Size in bytes of the Bulk Vendor data endpoints. */
		#define VENDOR_IO_EPSIZE               32
		
typedef struct
		{
			USB_Descriptor_Configuration_Header_t Config;

			// Vendor Interface
			USB_Descriptor_Interface_t            Vendor_Interface;
			USB_Descriptor_Endpoint_t             Vendor_DataInEndpoint;
			USB_Descriptor_Endpoint_t             Vendor_DataOutEndpoint;
		} USB_Descriptor_Configuration_t;

		/** Enum for the device interface descriptor IDs within the device. Each interface descriptor
		 *  should have a unique ID index associated with it, which can be used to refer to the
		 *  interface from other descriptors.
		 */
enum InterfaceDescriptors_t
		{
			INTERFACE_ID_Vendor = 0, /**< Vendor interface descriptor ID */
		};

	/* Function Prototypes: */
uint16_t CALLBACK_USB_GetDescriptor(const uint16_t wValue,
									const uint8_t wIndex,
									const void** const DescriptorAddress)
									ATTR_WARN_UNUSED_RESULT ATTR_NON_NULL_PTR_ARG(3);

#endif /* DESCRIPTORS_H_ */