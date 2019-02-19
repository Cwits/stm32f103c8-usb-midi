#include "usb_midi.h"
#include "usb_lib.h"
#include "usb_pwr.h"
#include "irq.h"

extern volatile uint8_t rx_new_data;		//from usb_endp.c

uint32_t Receive_Buffer[64] = {0};				//Receive buffer, can contain 64 packages
static volatile uint16_t rx_buffer_head = 0;
volatile uint16_t rx_buffer_size = 0;

uint32_t Transmit_Buffer[64] = {0};
static volatile uint16_t tx_buffer_tail = 0;
volatile uint16_t tx_buffer_size = 0;
extern volatile uint8_t tx_pack_send;

extern void toggleLED();

void USB_MIDI_ReceiveHande() {
	//check if device is ready to use
	if(bDeviceState != CONFIGURED) {
		return;
	}

	IRQ_Disable();
	if(!(rx_buffer_size >= 64)) {
		uint32_t DataLength = 0;
		DataLength = GetEPRxCount(EP1_OUT & 0x7F);

		//TODO:  note: expected 'uint8_t * {aka unsigned char *}' but argument is of type 'uint32_t * {aka long unsigned int *}'
		//		 first arg of PAMToUserBufferCopy, but all works!
		PMAToUserBufferCopy(&(Receive_Buffer[rx_buffer_head]), /*GetEPRxAddr(EP1_OUT & 0x7F)*/0, DataLength);

		if(++rx_buffer_head >= 64)
			rx_buffer_head = 0;

		rx_buffer_size++;
		//SetEPRxValid(ENDP1);
	} else {
		//TODO: Buffer is full, user have to read from it something
		//we just lost package
		//SetEPRxStatus(ENDP1, EP_TX_NAK);
	}
	SetEPRxValid(ENDP1);
	IRQ_Enable();
}


void USB_MIDI_TransmitHandle() {
	//when callback have to send data, or send stall/nak

	if(bDeviceState != CONFIGURED) {
		return;
	}

	IRQ_Disable();
	if(tx_pack_send == 1) {
		if(tx_buffer_size) {	//check if buffer has data
			UserToPMABufferCopy(&(Transmit_Buffer[tx_buffer_tail]), GetEPTxAddr(EP2_IN & 0x7F), 4);

			if(++tx_buffer_tail >= 64)
				tx_buffer_tail = 0;

			tx_buffer_size--;

			SetEPTxCount(ENDP2, 4);
			SetEPTxValid(ENDP2);
		} else {
			//send NAK
			SetEPTxStatus(ENDP2, EP_TX_NAK);
		}
	}

	IRQ_Enable();
}

