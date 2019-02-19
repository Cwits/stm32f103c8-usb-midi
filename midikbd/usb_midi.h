#ifndef __USB_MIDI_H
#define __USB_MIDI_H

#include "stm32f10x.h"



void USB_MIDI_ReceiveHande();	//process input data to ENDPOINT
void USB_MIDI_TransmitHandle();	//process output data from ENDPOINT

#endif
