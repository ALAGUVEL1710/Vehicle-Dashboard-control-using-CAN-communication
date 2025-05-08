#include<LPC21xx.h>
#include "header.h"
#include "Delay.c"

int flag = 0;
CAN2_MSG m1;
void can_init(void) {
   
	

    PINSEL1 |= 0x00014000;  
    VPBDIV = 1;             

    C2MOD = 0x1;            
    C2BTR = 0x001C001D;     

  AFMR = 0x02;
	 C2IER |= (1 << 0);
    C2MOD=0x0; 
        
} 




void can_isr(void) __irq {
    
	if (C2GSR & (1 << 0)) { 
        m1.id = C2RID;       
        m1.dlc = (C2RFS >> 16) & 0xF;  
        m1.rtr = (C2RFS >> 30) & 0x1;  
        
		if((m1.rtr==0)&&(m1.id == 0x00))
		{ 
	    flag = ! flag;
		 }
        
    }
	C2CMR = (1 << 2);
    VICVectAddr = 0; 
}
void CAN_RX_Interrupt(void) {
    VICIntSelect = 0; 
    VICVectCntl1 = (1 << 5) | 27; 
    VICVectAddr1 = (int)can_isr;  
    VICIntEnable |= (1 << 27);  
}



void wiper_init(void) {
    
    PINSEL0 |= 0x00000008; 


    PWMTCR = 0x02; 
    PWMPR = 0x1D; 
    PWMMR0 = 20000; 
    
    PWMMCR = 0x00000202; 
    PWMLER = 0x09; 
    PWMPCR = 0x0800; 
    PWMTCR = 0x09; 
}


   



int main() {
    
	
    can_init();
    CAN_RX_Interrupt();
    wiper_init();
    
    

    while (1) {
   
	   
            

	 while (flag) {
	 int duty; 
          
              
	            
				
	for (duty = 650; duty <= 2450; duty += 50) {
            PWMMR3 = duty;  
            PWMLER = 0x08;  
            Delay_milliseconds(20);   
        }

        Delay_milliseconds(500); 

        
        for (duty = 2450; duty >= 650; duty -= 50) {
            PWMMR3 = duty;
            PWMLER = 0x08;
            Delay_milliseconds(20);
        }

        Delay_milliseconds(500); 

			
          } 
       
    }

   
}
