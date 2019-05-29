/******************************************************************************/
/*Files to Include                                                            */
/******************************************************************************/

#include <xc.h>             /* HiTech General Includes */
#include <stdint.h>         /* For uint8_t definition */
#include <stdbool.h>        /* For true/false definition */
#include "user.h"
#include "OLIMEX.h"


/******************************************************************************/
/* Interrupt Routines                                                         */

/******************************************************************************/
void interrupt
isr(void)
{

    unsigned char count;

    if (TMR2IF)
    {
        PIR1bits.TMR2IF = 0;
        if (t2presc < 16)
        {
            t2presc++;
        }
          else
        {
            guard = 0;
            t2presc = 0; //One second entry
            if (updown>0)
            {
                if (updown==1)
                {
                   CommandSetRelay(0); //switch off relays                    
                }
                updown--;
            }
            
            
        }
        
        
    }
    
   #ifdef RF433 
    
     if (TMR1IF)
        {
            //SYNC  
            PIR1bits.TMR1IF = 0;
            T1CONbits.TMR1ON = 0;
            if (!JMP_PORT) 
            {
            // Write new code to free cell  
                fifo &= 0x3FFFF0;
                if ((fifo == lastfifo)&&(fifo!=0))
                {    
                  if (crc > 0)
                   crc--;
                  else
                  {    
                for (count=0;count++;count<4)
                {
                if (ReadRFCode(count)==0xFFFFFF)
                           WriteCell(count,fifo);
                if (ReadRFCode(count)==fifo)
                    count = 4;
                }
                }
                }
                else
                {
                    lastfifo = fifo;
                    fifo = 0;
                    crc = 3; //must receive 4 equal commands   
                }    
            }
            else
            {
            // do command    
                unsigned char rfcmd = 0;
                rfcmd = fifo & 0x0f;
                
                    fifo &= 0x3FFFF0;
                if (fifo == lastfifo)
                {    
                  if (crc > 0)
                   crc--;
                  else
                  {    
                 
                 if (guard!=1) {     
                      
                for (count=0;count++;count<4)
                {
               
                if (ReadRFCode(count)==fifo)
                {
                    if(updown>0)
                {
                        
                 CommandSetRelay(0);
                 guard = 1;
                 t2presc = 8;
                 if (updown>3)   
                     updown = 3;
                    
                }    
                else
                { 
                  // do up or down  
                 if (rfcmd == 0x03)
                     CommandSetRelay(1);
                 if (rfcmd == 0x0c)
                     CommandSetRelay(2);
                 guard = 1;
                 t2presc = 8;           
                updown = 40; // 40 seconds
                }
                    
                    count = 4;
                }    
                }
                 }
                
            }    
                }
                else
                {
                    lastfifo = fifo;
                    crc = 1;
                }
            }
        }
     if (TMR0IF)
     {
         TMR0IF = 0;
         rfbit = GPIO2_PORT;
   
     }
    if (INTF)
    {
        INTCONbits.INTF = 0;
        // 
        TMR1 = 0xEC00; //10mS
        PIR1bits.TMR1IF = 0;
        T1CONbits.TMR1ON = 1;
        TMR0 = 0x2d;
        INTCONbits.T0IF = 0;
        
        fifo <<=1;
        fifo |= rfbit;
        
        
    }


#endif  
	if (SSP1IF) {
		unsigned char status;
		unsigned char foo;
		unsigned int adc;
		int i;
        unsigned char command;
  

		if (SSPSTAT & STOP_C) {
        command = DATA_BUFFER[0];
        DATA_BUFFER[0]= 0xff;  
            
			switch (command) {
			case SET_TRIS:
				CommandSetTris(DATA_BUFFER[1]);
				break;
			case SET_LAT:
				CommandSetLat(DATA_BUFFER[1]);
				break;
			case SET_PU:
				CommandSetPullUps(DATA_BUFFER[1]);
				break;
			case SET_ADDRESS:
				CommandSetAddress(DATA_BUFFER[1]);
				break;
            case SET_REL_P:
                T2CONbits.TMR2ON = 1;
                if (guard!=1){
                   guard = 1;
                 t2presc = 8; 
                    
                 if(updown>0)
                {
                 CommandSetRelay(0);
                 if (updown>2)   
                     updown = 2;
                  
                }    
                else
                { 
                  // do up or down  
                 updown = DATA_BUFFER[1];    
                 CommandSetRelay(DATA_BUFFER[1]);
                }
                }
                 break;
			case SET_REL:
				CommandSetRelay(DATA_BUFFER[1]);
				break;
            case SET_RELON:
                CommandSetOnX(DATA_BUFFER[1]);
                break;    
            case SET_RELOFF:
                CommandSetOffX(DATA_BUFFER[1]);
                break;    
        	case GET_PORT:
				DATA_OUT[0] = CommandGetPort();
				break;
            case GET_REL:    
                DATA_OUT[0] = REL2*2 + REL1;
                break;
			case GET_LAT:
				DATA_OUT[0] = CommandGetLat();
				break;
			case GET_TRIS:
				DATA_OUT[0] = CommandGetTris();
				break;
			case GET_AN_GPIO0:
				adc = CommandGetAnalog(0);
				DATA_OUT[0] = adc & 0xFF;
				DATA_OUT[1] = (adc >> 8) & 0xFF;
				break;
			case GET_AN_GPIO1:
				adc = CommandGetAnalog(1);
				DATA_OUT[0] = adc & 0xFF;
				DATA_OUT[1] = (adc >> 8) & 0xFF;
				break;
			case GET_AN_GPIO2:
				adc = CommandGetAnalog(2);
				DATA_OUT[0] = adc & 0xFF;
				DATA_OUT[1] = (adc >> 8) & 0xFF;
				break;
			case GET_AN_GPIO3:
				adc = CommandGetAnalog(3);
				DATA_OUT[0] = adc & 0xFF;
				DATA_OUT[1] = (adc >> 8) & 0xFF;
				break;
			case GET_AN_GPIO5:
				adc = CommandGetAnalog(7);
				DATA_OUT[0] = adc & 0xFF;
				DATA_OUT[1] = (adc >> 8) & 0xFF;
				break;
			case GET_ID:
				DATA_OUT[0] = IO2_ID;
				break;
            case GET_FV:
                DATA_OUT[0] = VERSION;
                break;
            case SET_PWM1:
                 SetPWM1(DATA_BUFFER[1]);
                 break;
            case SET_PWM2:
                 SetPWM2(DATA_BUFFER[1]);
                 break;        
            case PWM_OFF: 
                ClosePWM(DATA_BUFFER[1]);
                break;
            case SET_DAC:
                SetDAC(DATA_BUFFER[1]);
                break;    
  			default:
				break;
			}
		} else {
			status = SSPSTAT & 0x2D; //DA | P | S | R/W | x | BF
			switch (status) {
				//STATE 1
			case STATE1:
			{
				foo = SSPBUF;
				DATA_INDEX = 0;
				for (i = 0; i < MAX_BUFFER_SIZE; i++)
					DATA_BUFFER[i] = 0;
				if (SEN)
					CKP = 1;
			}
				break;
				//STATE 2
			case STATE2:
			{
				DATA_BUFFER[DATA_INDEX++] = SSPBUF;
				if (DATA_INDEX >= MAX_BUFFER_SIZE)
					DATA_INDEX = 0;
				if (SEN)
					CKP = 1;
			}
				break;
				//STATE 3
			case STATE3:
			{
				foo = SSPBUF;
				while (BF);
				SSPBUF = DATA_OUT[0];
				if (SEN)
					CKP = 1;
			}
				break;
				//STATE 4
			case STATE4:
			{
				foo = SSPBUF;
				while (BF);
				SSPBUF = DATA_OUT[1];
				if (SEN)
					CKP = 1;
			}
				break;
				//STATE 5
			case STATE5:
				break;
			default:
				break;
			}
		}
	}
	SSP1IF = 0;

} 
