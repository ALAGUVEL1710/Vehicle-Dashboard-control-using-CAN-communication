#include<LPC21xx.h>
#include "header.h"
#include "Delay.c"

#define LED0 1<<17
#define LED1 1<<18
#define LED2 1<<19


int left_flag = 0;
int right_flag = 0;
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
        
	if((m1.rtr==0)&&(m1.id == 0x02))
	{ 
	        left_flag = ! left_flag;
		if(left_flag)
		{
			right_flag=0;
		}
	}
	else if((m1.rtr==0)&&(m1.id == 0x01))
	{ 
	        right_flag = ! right_flag;
		if(right_flag)
		{
			left_flag=0;
		}
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

int main() {
    
	
    can_init();
    CAN_RX_Interrupt();
    
    IODIR0 |=LED0|LED1|LED2;
    IOSET0 |=LED0|LED1|LED2;
    

    while (1) {
   
	   
            

	 while (left_flag) {

   		IOCLR0 |=LED0;
		Delay_milliseconds(500); 
		IOCLR0 |=LED1;
		Delay_milliseconds(500); 		
 		IOCLR0 |=LED2;
		Delay_milliseconds(500);
		IOSET0 |=LED0;
		Delay_milliseconds(500); 
		IOSET0 |=LED1;
		Delay_milliseconds(500); 		
 		IOSET0 |=LED2;
		Delay_milliseconds(500);  				
		   
          }
	   IOSET0 |=LED0|LED1|LED2;
  	while(right_flag){

        	IOCLR0 |=LED2;
		Delay_milliseconds(500); 
		IOCLR0 |=LED1;
		Delay_milliseconds(500); 		
 		IOCLR0 |=LED0;
		Delay_milliseconds(500);
		IOSET0 |=LED2;
		Delay_milliseconds(500); 
		IOSET0 |=LED1;
		Delay_milliseconds(500); 		
 		IOSET0 |=LED0;
		Delay_milliseconds(500);  				
		   
       

			
          } 
       	   IOSET0 |=LED0|LED1|LED2;
    }

   
}
