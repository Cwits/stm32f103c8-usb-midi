#include "stm32f10x_conf.h"
#include "hw_config.h"
#include "usb_lib.h"
#include "usb_pwr.h"
#include "midi.h"

void cfgPeriphnGPIO(void);
void toggleLED();
void delay(uint32_t ms)
{
        volatile uint32_t nCount;
        RCC_ClocksTypeDef RCC_Clocks;
        RCC_GetClocksFreq (&RCC_Clocks);

        nCount=(RCC_Clocks.HCLK_Frequency/10000)*ms;
        for (; nCount!=0; nCount--);
}


void delayMC(uint32_t mc)
{
        volatile uint32_t nCount;
        RCC_ClocksTypeDef RCC_Clocks;
        RCC_GetClocksFreq (&RCC_Clocks);

        nCount=(RCC_Clocks.HCLK_Frequency/10000000)*mc;
        for (; nCount!=0; nCount--);
}

typedef enum {
	FALSE = 0,
	TRUE
} bool_t;


volatile bool_t notifyMe = FALSE;
extern volatile uint8_t tx_pack_send;
bool_t midiTest = FALSE;

int main(void)
{
	SystemInit();

	cfgPeriphnGPIO();

	Set_System();
	Set_USBClock();
	USB_Interrupts_Config();
	USB_Init();

	while (bDeviceState != CONFIGURED);
	toggleLED();

	SetEPRxValid(ENDP1);


    while(1){
    	midi_pack stu;
    	uint8_t rcv = MIDI_Read(&stu);

    	if(rcv != 0) {
    		//uint8_t cin = stu.data0 & 0xF0;
    		midi_event_t event = stu.data1 & 0xF0; // or stu.data0 & 0x0F, but then have to edit midi_event_t and del zero after numbers

    		if(event == NoteOn) {
    			uint8_t channel = (stu.data1 & 0x0F) + 1;
    			uint8_t note = stu.data2;
    			uint8_t velocity = stu.data3;

    			toggleLED();
    		}

    		if(event == NoteOff) {
    			uint8_t channel = (stu.data1 & 0x0F) + 1;
    			uint8_t note = stu.data2;
    			uint8_t velocity = stu.data3;

        		toggleLED();
    		}

    		if(event == AfterTouchPoly) {
    			uint8_t channel = (stu.data1 & 0x0F) + 1;
    			uint8_t note = stu.data2;
    			uint8_t pressure = stu.data3;

    			toggleLED();
    		}

    		if(event == PitchBend) {
    			uint8_t channel = (stu.data1 & 0x0F) + 1;
    			uint8_t control = stu.data2;
    			uint8_t value = stu.data3;

    			toggleLED();
    		}

    		if(event == ControlChange) {
    			uint8_t channel = (stu.data1 & 0x0F) + 1;
    			uint8_t control = stu.data2;
    			uint8_t value = stu.data3;

    			toggleLED();
    		}
    	}//if(rcv ! 0)
    /*if(tx_pack_send) {
    	if(midiTest == FALSE) {
    		MIDI_SendNoteOn(1, 41, 90);
    		midiTest = TRUE;
    	} else if(midiTest == TRUE) {
    		MIDI_SendNoteOff(1, 41);
    		midiTest = FALSE;
    	}
    	tx_pack_send = 0;
    }*/
    }//while(1)
}//main

void toggleLED() {
	GPIOC->ODR ^= GPIO_Pin_13;
}


void cfgPeriphnGPIO(void) {
	RCC_APB2PeriphClockCmd(RCC_APB2ENR_IOPAEN | RCC_APB2ENR_IOPBEN | RCC_APB2ENR_IOPCEN | RCC_APB2ENR_AFIOEN, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SPI1, DISABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2ENR_SPI1EN, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1ENR_USART2EN, ENABLE);

	GPIO_InitTypeDef gpioCFG;
	GPIO_StructInit(&gpioCFG);

	//Init usart gpio & periph
	gpioCFG.GPIO_Pin = GPIO_Pin_2; //tx
	gpioCFG.GPIO_Speed = GPIO_Speed_50MHz;
	gpioCFG.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &gpioCFG);

	gpioCFG.GPIO_Pin = GPIO_Pin_3; //rx
	gpioCFG.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &gpioCFG);

	USART_InitTypeDef usartCFG;
	usartCFG.USART_BaudRate = 250000;
	usartCFG.USART_WordLength = USART_WordLength_8b;
	usartCFG.USART_StopBits = USART_StopBits_2;
	usartCFG.USART_Parity = USART_Parity_No;
	usartCFG.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	usartCFG.USART_Mode =/* USART_Mode_Rx | */USART_Mode_Tx;

	USART_Init(USART2, &usartCFG);
	USART_Cmd(USART2, ENABLE);

	gpioCFG.GPIO_Pin = GPIO_Pin_13; //built in led
	gpioCFG.GPIO_Mode = GPIO_Mode_Out_PP;
	gpioCFG.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &gpioCFG);
}

