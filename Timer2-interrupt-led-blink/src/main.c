#include "ch32v20x.h"
#include "debug.h"

volatile uint32_t tick10ms = 0;
volatile uint8_t toggleLED = 0;

// https://github.com/openwch/ch32v20x/blob/main/EVT/EXAM/SRC/Peripheral/inc/ch32v20x.h
// TIM2 IRQ handler declaration required by the WCH RISC-V interrupt ABI.
void TIM2_IRQHandler(void) __attribute__((interrupt("WCH-Interrupt-fast")));

// TIM2 interrupt service routine.
// Keep ISR work short: update state/flags rather than doing lengthy operations.
void TIM2_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
    {
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);

        tick10ms++;
        if(tick10ms > 19){
            tick10ms= 0;
            toggleLED = 1;   
        }
    }
}
void timerInit()
{
    TIM_TimeBaseInitTypeDef timer = {0};

    // https://github.com/openwch/arduino_core_ch32/blob/main/system/CH32V20x/SRC/Peripheral/src/ch32v20x_tim.c
    // Enable the APB1 clock supplying TIM2.
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

    timer.TIM_Prescaler = 1799;
    timer.TIM_Period = 99;
    timer.TIM_ClockDivision = TIM_CKD_DIV1;
    timer.TIM_CounterMode = TIM_CounterMode_Up;

    TIM_TimeBaseInit(TIM2, &timer);

    // Enable TIM2 update interrupt.
    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);

    // Configure the RISC-V interrupt controller.
    NVIC_InitTypeDef nvic = {0};

    nvic.NVIC_IRQChannel = TIM2_IRQn;
    nvic.NVIC_IRQChannelPreemptionPriority = 1;
    nvic.NVIC_IRQChannelSubPriority = 0;
    nvic.NVIC_IRQChannelCmd = ENABLE;

    NVIC_Init(&nvic);

    // Start TIM2.
    TIM_Cmd(TIM2, ENABLE);
}

int main(void) {
    RCC->CFGR0 |= RCC_HPRE_DIV8; // from 144MHz to 18MHz ( /8 )
    timerInit();
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    GPIO_InitTypeDef GPIO_InitStructure = {0};
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    uint8_t ledState = 0;
    while (1) {
        if(toggleLED){
            ledState ^= 1;
            GPIO_WriteBit(GPIOA, GPIO_Pin_3, ledState);
            toggleLED= 0;
        }
        
        
        
    }
    return 0;
}

void NMI_Handler(void) {}
void HardFault_Handler(void)
{
    while (1)
    {
    }
}