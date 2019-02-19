/******************** (C) COPYRIGHT 2010 STMicroelectronics ********************
* File Name          : usb_desc.c
* Author             : MCD Application Team
* Version            : V3.2.1
* Date               : 07/05/2010
* Description        : Descriptors for Mass Storage Device
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "usb_desc.h"

const uint8_t MASS_DeviceDescriptor[MASS_SIZ_DEVICE_DESC] =
  {
			0x12,					// bLength
			0x01,					// bDescriptorType
			0x10, 0x01,				// bcdUSB LSB MSB
			0x00,					// bDeviceClass
			0x00,					// bDeviceSubClass
			0x00,					// bDeviceProtocol
			0x40,				// bMaxPacketSize0
			0xc0, 0x16,		// idVendor LSB MSB
			0x85, 0x04,	// idProduct LSB MSB
			0x00, 0x01,				// bcdDevice LSB MSB
			0x01,					// iManufacturer
			0x01,					// iProduct
			0x00,					// iSerialNumber
			0x01,					// bNumConfigurations
  };
const uint8_t MASS_ConfigDescriptor[MASS_SIZ_CONFIG_DESC] =
  {

			// configuration descriptor, USB spec 9.6.3, page 264-266, Table 9-10
			0x09, 					// bLength;
			0x02,					// bDescriptorType;
			0x65,	0x00, 	// wTotalLength
			0x02,				// bNumInterfaces
			0x01,					// bConfigurationValue
			0x00,					// iConfiguration
			0x80,					// bmAttributes
			0x32,					// bMaxPower

			// This MIDI stuff is a copy of the example from the Audio Class
			// MIDI spec 1.0 (Nov 1, 1999), Appendix B, pages 37 to 43.

		//#if 0
/*
			http://www.usb.org/developers/devclass_docs/midi10.pdf

			Section B.3 seems to say these extra descriptors are required,
			but when I add them, MIDI breaks on Linux (haven't tried Mac and
			Windows yet).  TODO: investigate these....
			reported by "John K." on May 7, 2012, subject "USB MIDI descriptors"
*/
			// Standard AC Interface Descriptor
			0x09,					// bLength
			0x04,					// bDescriptorType = INTERFACE
			0x00,					// bInterfaceNumber
			0x00,					// bAlternateSetting
			0x00,					// bNumEndpoints
			0x01,					// bInterfaceClass = AUDIO
			0x01,					// bInterfaceSubclass = AUDIO_CONTROL
			0x00,					// bInterfaceProtocol
			0x00,					// iInterface

			// Class-specific AC Interface Descriptor
			0x09,					// bLength
			0x24,					// bDescriptorType = CS_INTERFACE
			0x01,					// bDescriptorSubtype = HEADER
			0x00, 0x01,				// bcdADC
			0x09, 0x00,				// wTotalLength
			0x01,					// bInCollection
			0x01,					// baInterfaceNr(1)
		//#endif

		        // Standard MS Interface Descriptor,
		        0x09,                                      // bLength
		        0x04,                                      // bDescriptorType
		        0x01,			                           // bInterfaceNumber
		        0x00,                                      // bAlternateSetting
		        0x02,                                      // bNumEndpoints
		        0x01,                                   // bInterfaceClass (0x01 = Audio)
		        0x03,                                   // bInterfaceSubClass (0x03 = MIDI)
		        0x00,                                   // bInterfaceProtocol (unused for MIDI)
		        0x00,                                      // iInterface

			// MIDI MS Interface Header, USB MIDI 6.1.2.1, page 21, Table 6-2
			0x07,					// bLength
			0x24,					// bDescriptorType = CS_INTERFACE
			0x01,					// bDescriptorSubtype = MS_HEADER
			0x00, 0x01,				// bcdMSC = revision 01.00
			0x41, 0x00,				// wTotalLength

			// MIDI IN Jack Descriptor, B.4.3, Table B-7 (embedded), page 40
			0x06,					// bLength
			0x24,					// bDescriptorType = CS_INTERFACE
			0x02,					// bDescriptorSubtype = MIDI_IN_JACK
			0x01,					// bJackType = EMBEDDED
			0x01,					// bJackID, ID = 1
			0x00,					// iJack

			// MIDI IN Jack Descriptor, B.4.3, Table B-8 (external), page 40
			0x06,					// bLength
			0x24,					// bDescriptorType = CS_INTERFACE
			0x02,					// bDescriptorSubtype = MIDI_IN_JACK
			0x02,					// bJackType = EXTERNAL
			0x02,					// bJackID, ID = 2
			0x00,					// iJack

			// MIDI OUT Jack Descriptor, B.4.4, Table B-9, page 41
			0x09,
			0x24,					// bDescriptorType = CS_INTERFACE
			0x03,					// bDescriptorSubtype = MIDI_OUT_JACK
			0x01,					// bJackType = EMBEDDED
			0x03,					// bJackID, ID = 3
			0x01,					// bNrInputPins = 1 pin
			0x02,					// BaSourceID(1) = 2
			0x01,					// BaSourcePin(1) = first pin
			0x00,					// iJack

			// MIDI OUT Jack Descriptor, B.4.4, Table B-10, page 41
			0x09,
			0x24,					// bDescriptorType = CS_INTERFACE
			0x03,					// bDescriptorSubtype = MIDI_OUT_JACK
			0x02,					// bJackType = EXTERNAL
			0x04,					// bJackID, ID = 4
			0x01,					// bNrInputPins = 1 pin
			0x01,					// BaSourceID(1) = 1
			0x01,					// BaSourcePin(1) = first pin
			0x00,					// iJack

		        // Standard Bulk OUT Endpoint Descriptor, B.5.1, Table B-11, pae 42
		        0x09,                                      // bLength
		        0x05,                                      // bDescriptorType = ENDPOINT
		        0x01,                       // bEndpointAddress
		        0x02,                                   // bmAttributes (0x02=bulk)
		        0x40, 0x00,                        // wMaxPacketSize
		        0x00,                                      // bInterval
		        0x00,					// bRefresh
		        0x00,					// bSynchAddress

			// Class-specific MS Bulk OUT Endpoint Descriptor, B.5.2, Table B-12, page 42
			0x05,					// bLength
			0x25,					// bDescriptorSubtype = CS_ENDPOINT
			0x01,					// bJackType = MS_GENERAL
			0x01,					// bNumEmbMIDIJack = 1 jack
			0x01,					// BaAssocJackID(1) = jack ID #1

		        // Standard Bulk IN Endpoint Descriptor, B.5.1, Table B-11, pae 42
		        0x09,                                      // bLength
		        0x05,                                      // bDescriptorType = ENDPOINT
		        0x81,                // bEndpointAddress
		        0x02,                                   // bmAttributes (0x02=bulk)
		        0x40, 0x00,                        // wMaxPacketSize
		        0x00,                                      // bInterval
			0x00,					// bRefresh
			0x00,					// bSynchAddress

			// Class-specific MS Bulk IN Endpoint Descriptor, B.5.2, Table B-12, page 42
			0x05,					// bLength
			0x25,					// bDescriptorSubtype = CS_ENDPOINT
			0x01,					// bJackType = MS_GENERAL
			0x01,					// bNumEmbMIDIJack = 1 jack
			0x03,					// BaAssocJackID(1) = jack ID #3
  };
const uint8_t MASS_StringLangID[MASS_SIZ_STRING_LANGID] =
  {
    MASS_SIZ_STRING_LANGID,
    0x03,
    0x09,
    0x04
  }
  ;      /* LangID = 0x0409: U.S. English */
const uint8_t MASS_StringVendor[MASS_SIZ_STRING_VENDOR] =
  {
    MASS_SIZ_STRING_VENDOR, /* Size of manufaturer string */
    0x03,           /* bDescriptorType = String descriptor */
    /* Manufacturer: "WWW.powermcu.com" */   	
    'T', 0, 'E', 0, 'T', 0, 'S', 0, ' ', 0, ' ', 0, ' ', 0, ' ', 0,
    ' ', 0, ' ', 0, ' ', 0, ' ', 0, ' ', 0, ' ', 0, ' ', 0, ' ', 0,
  };
const uint8_t MASS_StringProduct[MASS_SIZ_STRING_PRODUCT] =
  {
    MASS_SIZ_STRING_PRODUCT,
    0x03,
  	/* Multi Media Development Board V1.0 */
    'M', 0, 'u', 0, 'l', 0, 't', 0, 'i', 0, ' ', 0, 'M', 0,
    'e', 0, 'd', 0, 'i', 0, 'a', 0, ' ', 0, 'D', 0, 'e', 0,	
    'v', 0, 'e', 0, 'l', 0, 'o', 0, 'p', 0, 'm', 0, 'e', 0,
	'n', 0, 't', 0, ' ', 0, 'B', 0, 'o', 0, 'a', 0, 'r', 0,
	'd', 0, ' ', 0, 'V', 0, '1', 0, '.', 0, '0'
  };

uint8_t MASS_StringSerial[MASS_SIZ_STRING_SERIAL] =
  {
    MASS_SIZ_STRING_SERIAL,
    0x03,
    /* Serial number*/
    'S', 0, 'T', 0, 'M', 0, '3', 0, '2', 0, '1', 0, '0', 0
  };
const uint8_t MASS_StringInterface[MASS_SIZ_STRING_INTERFACE] =
  {
    MASS_SIZ_STRING_INTERFACE,
    0x03,
    /* Interface 0: "ST Mass" */
    'S', 0, 'T', 0, ' ', 0, 'M', 0, 'I', 0, 'D', 0, 'I', 0
  };

/******************* (C) COPYRIGHT 2010 STMicroelectronics *****END OF FILE****/
