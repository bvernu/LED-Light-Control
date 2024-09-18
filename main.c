#include <stdio.h>
//GPIO
#define SYSCTL_RCGCGPIO_R (*(( volatile unsigned long *)0x400FE608 ) )
#define GPIODATA_PA (*(( volatile unsigned long *) 0x40004038 ) )
#define GPIODIR_PA (*(( volatile unsigned long *) 0x40004400 ) )
#define GPIODEN_PA (*(( volatile unsigned long *) 0x4000451C ) )
#define GPIOAFSEL_PA (*(( volatile unsigned long *)0x40004420 ) )
#define GPIOPCTL_PA (*((volatile unsigned long *)(0x4000452C)))
#define GPIO_PORT_PDR (*((volatile unsigned long *)0x40004510))

//UART pg 902 on how to enable and initialize the UART
#define RCGC_UART (*(( volatile unsigned long *)0x400FE618 ) )
#define UART_CTL (*((volatile unsigned long *)(0x4000C030)))
#define UART0_IBRD (*(( volatile unsigned long *)0x4000C024 ) )
#define UART0_FBRD (*(( volatile unsigned long *)0x4000C028 ) )
#define UART_LCRH (*(( volatile unsigned long *)0x4000C02C ) )
#define UART_FR (*(( volatile unsigned long *)0x4000C018 ) ) //flag reg
#define UART_DR (*(( volatile unsigned long *)0x4000C000 ) ) //data reg
#define UART_CC (*(( volatile unsigned long *)0x4000CFC8 ) )
#define BAUDRATE 9600
#define CLOCK_FREQ 16000000


#define GPIO_PORT_PIN2_EN 0x04  // Assuming GPIO_PIN_2 is at bit position 2 in the register
#define GPIO_PORT_PIN3_EN 0x08  // Assuming GPIO_PIN_3 is at bit position 3 in the register

#define GPIO_PORT_CLK_EN 0x01   // Assuming GPIO is connected to bit 0 of RCGCGPIO register

char UART_Rx () {
    char data;
    while((UART_FR & 0x10));
    data = UART_DR;
    return ((unsigned char) data);
}

void UART_Tx(char data) {
    while ((UART_FR & 0x20));
    UART_DR = data;
}



void UART_Init() {
    RCGC_UART |= 0x01;
    SYSCTL_RCGCGPIO_R |= GPIO_PORT_CLK_EN;

        GPIO_PORT_PDR |= GPIO_PORT_PIN3_EN;
        GPIODEN_PA |= GPIO_PORT_PIN2_EN;
        GPIODEN_PA|= GPIO_PORT_PIN3_EN;
        GPIODIR_PA |= GPIO_PORT_PIN2_EN;
        GPIOAFSEL_PA &= ~GPIO_PORT_PIN2_EN;
        GPIOAFSEL_PA &= ~GPIO_PORT_PIN3_EN;

        UART_CTL = 0;
        UART0_IBRD = 104;
        UART0_FBRD = 11;
        UART_CC = 0x00;
        UART_LCRH = 0x60;
        UART_CTL = 0x301;

        GPIODEN_PA |= 0x03;
        GPIOAFSEL_PA |= 0x03;
        GPIOAFSEL_PA |= 0x00;

}



int main(void)
{
    char state = 'p';


    UART_Init();


    GPIODIR_PA |= 0x04;
    GPIODIR_PA = ~0x08;
    GPIODEN_PA |= 0x0E;
    GPIO_PORT_PDR = 0x08;

    while(1){
        //UART_Tx('n');
        if (UART_FR & (0x40)) {
            state = UART_Rx();
            if (state == 'p'){
                UART_Tx('p');

            }
            else if (state == 'n'){
                UART_Tx('n');
            }
           }

        if (state == 'p'){
            if ((GPIODATA_PA & 0x08)){
                GPIODATA_PA = 0x04;
                }
                else{
                    GPIODATA_PA = 0x00;
                }
            }
        else{
            if (!(GPIODATA_PA & 0x08)){
                GPIODATA_PA = 0x04;
            }
            else{
                GPIODATA_PA = 0x00;
            }
        }
    }


}




