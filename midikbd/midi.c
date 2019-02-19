#include "midi.h"
#include "irq.h"
#include "usb_lib.h"
#include "usb_pwr.h"
#include "usb_regs.h"


extern uint8_t rx_new_data;					//extern from usb_endp.c

extern uint32_t Receive_Buffer[64];			//from usb_midi.c
extern volatile uint16_t rx_buffer_size;	//from usb_midi.c
static volatile uint16_t rx_buffer_tail = 0;

extern volatile uint8_t tx_pack_send;

extern uint32_t Transmit_Buffer[64];		//from usb_midi.c
extern volatile uint16_t tx_buffer_size;	//from usb_midi.c
static volatile uint16_t tx_buffer_head = 0;

uint8_t MIDI_Read(midi_pack *dataPointer) {
	if(!rx_buffer_size) {					//check if receive buffer is not empty
		return 0;
	}

	uint32_t tempBuff = 0;
	IRQ_Disable();

	tempBuff = Receive_Buffer[rx_buffer_tail];	//copy data

	dataPointer->data0 = tempBuff & 0xFF;		  //cin & event
	dataPointer->data1 = (tempBuff >> 8) & 0xFF;  //channel = ((data1 & 0x0F) + 1), what is 0xF0?
	dataPointer->data2 = (tempBuff >> 16) & 0xFF; //note
	dataPointer->data3 = (tempBuff >> 24) & 0xFF; //velocity

	if(++rx_buffer_tail >= 64) {
		rx_buffer_tail = 0;
	}

	rx_buffer_size--;

	IRQ_Enable();

	return 1;
}

static void MIDI_SendRaw(uint8_t byte1, uint8_t byte2, uint8_t byte3, uint8_t byte4) {
	//TODO: Pack all 4 bytes to 1 word and record it to transmit buffer,
	//and notify host that there is some data to read from buffer
	if(tx_buffer_size < 64 && tx_pack_send == 1) {
		IRQ_Disable();
		uint32_t tempBuff = 0;

		tempBuff = byte1;
		tempBuff = (tempBuff << 8) | byte2;
		tempBuff = (tempBuff << 8) | byte3;
		tempBuff = (tempBuff << 8) | byte4;

		Transmit_Buffer[tx_buffer_head] = tempBuff;

		//TODO: Move three lower lines usb_midi.c
     	UserToPMABufferCopy(&(Transmit_Buffer[tx_buffer_head]), ENDP1_TXADDR, 4);
     	SetEPTxCount(ENDP1, 4);

		SetEPTxValid(ENDP1);

		if(++tx_buffer_head >= 64)
			tx_buffer_head = 0;

		//tx_buffer_size++;
		tx_pack_send = 0;
		IRQ_Enable();

		//TODO: Call usb transmit func
		//USB_MIDI_TransmitHandle();
	} else {
		//TODO: buffer is full
	}
}

void MIDI_Send(uint8_t type, uint8_t channel, uint8_t data1, uint8_t data2) {
	//MIDI_SendRaw(0x7F, 0x30, 0x90, 0x09); //need to send in reversed order
	//			velocity, note, channel, event
	switch(type){
		case(NoteOn):
			MIDI_SendRaw(data2 & 0x7F, data1 & 0x7F, type | (channel-1), 0x00 | (type>>4));
			break;
		case(NoteOff):
			MIDI_SendRaw(data2 & 0x7F, data1 & 0x7F, type | (channel-1), 0x00 | (type>>4));
			break;
		case(ControlChange):
			MIDI_SendRaw(data2 & 0x7F, data1 & 0x7F, type | (channel-1), 0x00 | (type>>4));
			break;
		case(ProgramChange):
			MIDI_SendRaw(data2 & 0x7F, data1 & 0x7F, type | (channel-1), 0x00 | (type>>4));
			break;
		case(PitchBend):
			MIDI_SendRaw(data2 & 0x7F, data1 & 0x7F, type | (channel-1), 0x00 | (type>>4));
			break;
		case Clock:
		case Start:
		case Continue:
		case Stop:
		case ActiveSensing:
		case SystemReset:
			MIDI_SendRaw(0, 0, type | 0, 0x00 | (type>>4));
			break;
		default:
			break;
	}
}

void MIDI_SendNoteOn(uint8_t channel, uint8_t note, uint8_t velocity) {
	MIDI_Send(NoteOn, channel, note, velocity);
}

void MIDI_SendNoteOff(uint8_t channel, uint8_t note) {
	MIDI_Send(NoteOff, channel, note, 0);
}

void MIDI_SendControlChange(uint8_t channel, uint8_t control, uint8_t value) {
	MIDI_Send(ControlChange, channel, control, value);
}

void MIDI_SendProgramChange(uint8_t programm, uint8_t channel) {
	MIDI_Send(ProgramChange, channel, programm, 0);
}

void MIDI_SendPitchBend(uint8_t channel, int16_t value) {
	if(value < -8192) {
		value = -8192;
	} else if(value > 8191) {
		value = 8191;
	}
	value += 8191;
	MIDI_Send(PitchBend, channel, value, value >> 7);
}

void MIDI_SendPolyPressure(uint8_t channel, uint8_t note, uint8_t pressure) {

}

void MIDI_SendAfterTouchPoly(uint8_t channel, uint8_t note, uint8_t pressure) {

}

void MIDI_SendAfterTouch(uint8_t channel, uint8_t pressure) {

}

void MIDI_SendSysEx() {
	//TODO: dis...
}

void MIDI_SendRealTime(uint8_t type) {
	switch(type) {
		case Clock:
		case Start:
		case Continue:
		case Stop:
		case ActiveSensing:
		case SystemReset:
			MIDI_Send(type, 0, 0, 0);
			break;
		default: //invalid marker
			break;
	}
}
