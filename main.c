/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/

#include <xc.h>           /* Global Header File */

#include "system.h"        /* System funct/params, like osc/peripheral config */
#include "user.h"          /* User funct/params, such as InitApp */


/******************************************************************************/
/* User Global Variable Declaration                                           */
/******************************************************************************/


unsigned char DATA_BUFFER[6];
unsigned char DATA_OUT[8];
unsigned int DATA_INDEX = 0;
unsigned char ADDRESS;
#ifdef RF433
unsigned short long fifo;
unsigned short long lastfifo;
unsigned char crc;
unsigned char rfbit;
#endif
unsigned char t2presc;
unsigned char updown;
unsigned char guard;
/******************************************************************************/
/* Main Program                                                               */

/******************************************************************************/


void main(void)
{
	/* Configure the oscillator for the device */
	ConfigureOscillator();

	/* Initialize I/O and Peripherals for application */
	InitApp();

#ifdef RF433  
     if (!JMP_PORT) 
            {
         
         unsigned char count = 0;
                for (count=0;count++;count<4)
                {
                    EraseCell(count);
                }
            }
#endif    
	while (1) {
	}


}



