#include <lpc21xx.h>
#include "header.h"
#include "Delay.c"

int adc_value = 0;
int temp = 0;
int percent = 0;

void can_init(void) {
    PINSEL1 |= 0x00014000;   
    VPBDIV = 0x01;           
    C2MOD = 0x1;             
    C2BTR = 0x001C001D;      
    AFMR = 0x02;             
    C2MOD = 0x0;             
}

void adc_init(void) {
    ADCR = 0x00200604;       
}

int ADC_Conversion() {
    int ab;
    ADCR |= 0x01000000;                   
    while ((ADDR & 0x80000000) == 0);     
    ab = (ADDR & 0x0000FFC0) >> 6;        
    return ab;
}

void can2_tx(CAN2_MSG m1) {
    C2TID1 = m1.id;
    C2TFI1 = (m1.dlc << 16);

    if (m1.rtr == 0) {
        C2TFI1 &= ~(1 << 30);         
        C2TDA1 = m1.byteA;
        C2TDB1 = m1.byteB;
    } else {
        C2TFI1 |= (1 << 30);         
    }

    C2CMR = (1 << 0) | (1 << 5);     
    while ((C2GSR & (1 << 3)) == 0);  
	        
    
}

int main() {
    CAN2_MSG m1;
  
	 
    can_init();
    adc_init();

    m1.rtr = 0;
    m1.dlc = 1;
    m1.byteB = 0;
    m1.id = 0x03;

    while (1) {
        adc_value = ADC_Conversion();
        percent = (adc_value * 100) / 1023;

        if (percent != temp) {
                 
            m1.byteA = percent;
            can2_tx(m1);
            temp = percent;
            Delay_milliseconds(100); 
            
        }
		Delay_milliseconds(1000);
		
    }
}
