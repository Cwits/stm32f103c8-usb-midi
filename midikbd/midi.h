#ifndef __MIDI_H
#define __MIDI_H

#include "stm32f10x_conf.h"

typedef enum {
    InvalidType           = 0x00, // For notifying errors
    NoteOff               = 0x80, // Note Off
    NoteOn                = 0x90, // Note On
    AfterTouchPoly        = 0xA0, // Polyphonic AfterTouch
    ControlChange         = 0xB0, // Control Change / Channel Mode
    ProgramChange         = 0xC0, // Program Change
    AfterTouchChannel     = 0xD0, // Channel (monophonic) AfterTouch
    PitchBend             = 0xE0, // Pitch Bend
    SystemExclusive       = 0xF0, // System Exclusive
    TimeCodeQuarterFrame  = 0xF1, // System Common - MIDI Time Code Quarter Frame
    SongPosition          = 0xF2, // System Common - Song Position Pointer
    SongSelect            = 0xF3, // System Common - Song Select
    TuneRequest           = 0xF6, // System Common - Tune Request
    Clock                 = 0xF8, // System Real Time - Timing Clock
    Start                 = 0xFA, // System Real Time - Start
    Continue              = 0xFB, // System Real Time - Continue
    Stop                  = 0xFC, // System Real Time - Stop
    ActiveSensing         = 0xFE, // System Real Time - Active Sensing
    SystemReset           = 0xFF, // System Real Time - System Reset
} midi_event_t;

typedef struct {
	uint8_t data0;
	uint8_t data1;
	uint8_t data2;
	uint8_t data3;
} midi_pack;


uint8_t MIDI_Read(midi_pack *dataPointer);

static void MIDI_SendRaw(uint8_t byte1, uint8_t byte2, uint8_t byte3, uint8_t byte4);
void MIDI_Send(uint8_t type, uint8_t channel, uint8_t data1, uint8_t data2);
void MIDI_SendNoteOn(uint8_t channel, uint8_t note, uint8_t velocity);
void MIDI_SendNoteOff(uint8_t channel, uint8_t note);
void MIDI_SendCC(uint8_t channel, uint8_t control, uint8_t value);
void MIDI_SendProgramChange(uint8_t program, uint8_t channel);
void MIDI_SendPitchBend(uint8_t channel, int16_t value);
void MIDI_SendPolyPressure(uint8_t channel, uint8_t note, uint8_t pressure);
void MIDI_SendAfterTouchPoly(uint8_t channel, uint8_t note, uint8_t pressure);
void MIDI_SendAfterTouch(uint8_t channel, uint8_t pressure);
void MIDI_SendSysEx();
void MIDI_SendRealTime(uint8_t type);
#endif /* __MIDI_H */

