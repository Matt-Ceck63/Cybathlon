#include <stm32f3xx_hal.h>


#define GREEN_LED 0x000A
#define OFF 0
#define ON 1


int state = ON;
volatile uint32_t myCounter = 0;
// volatile uint32_t activated = 0;

void LED_Init();
void delay(uint32_t nTime);

int main(){

    // Reset all peripherals, initializes SysTick
    HAL_Init();
    HAL_SYSTICK_Config(10);
    LED_Init();

    //GPIOB->BRR = GREEN_LED;

    //if(SysTick_Config(SystemCoreClock/10000)){while(1);}

    // NVIC_SetPriority(SysTick_IRQn, 4);

    while(1){

        HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_3);
        // delay(1000);
        HAL_Delay(2000);

        // switch (state)
        // {
        //     case ON:

        //         GPIOB->BSRR = 0x0000; // turn off
        //         state = OFF;
        //         break;

        //     case OFF:

        //         GPIOB->BSRR = GREEN_LED; // turn on
        //         state = ON;
        //         break;
        // }

        // delay(1000); // Not working, debug
        
    }
}

void LED_Init(){

    // Enable clock on port B
    
    __HAL_RCC_GPIOB_CLK_ENABLE();

    // Configure pins
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.Pin = GPIO_PIN_3;
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStructure.Pull = GPIO_PULLUP;
    GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);

}

void delay(uint32_t nTime){
    while(myCounter > 0);
    myCounter = nTime;
}


void HAL_SYSTICK_Callback(void){ // Systick Interrupt Handler
    //if (myCounter > 0) myCounter --;
    HAL_IncTick();
}
