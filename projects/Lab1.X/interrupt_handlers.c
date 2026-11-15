/*****************************************************************************************

DISCLAIMER OF LIABILITY REGARDING CODE AND SERVICES

Upon your request and for your convenience Microchip Technology Incorporated
("Microchip") has modified your code.  Your acceptance and/or use of this
modified code constitutes agreement to the terms and conditions of this notice.

CODE ACCOMPANYING THIS MESSAGE AND RELATED SERVICES ARE SUPPLIED BY MICROCHIP "AS IS".
NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED
TO, IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY AND FITNESS FOR A
PARTICULAR PURPOSE APPLY TO THIS CODE, ITS INTERACTION WITH MICROCHIP’S PRODUCTS,
COMBINATION WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION.

YOU ACKNOWLEDGE AND AGREE THAT, IN NO EVENT, SHALL MICROCHIP BE
LIABLE, WHETHER IN CONTRACT, WARRANTY, TORT (INCLUDING NEGLIGENCE OR BREACH OF
STATUTORY DUTY), STRICT LIABILITY, INDEMNITY, CONTRIBUTION, OR OTHERWISE, FOR ANY
INDIRECT, SPECIAL, PUNITIVE, EXEMPLARY, INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE,
FOR COST OR EXPENSE OF ANY KIND WHATSOEVER RELATED TO THE CODE, HOWSOEVER CAUSED,
EVEN IF MICROCHIP HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.
TO THE FULLEST EXTENT ALLOWABLE BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS
IN ANY WAY RELATED TO THIS CODE, SHALL NOT EXCEED THE PRICE YOU PAID DIRECTLY TO
MICROCHIP SPECIFICALLY TO HAVE THIS CODE DEVELOPED.

You agree that you are solely responsible for testing the code and determining its
suitability.  Microchip has no obligation to modify, test, certify, or support the code.
******************************************************************************************/

#include <xc.h> 
#include "constants.h"
#include "interrupt_handlers.h"


/***********************************************************************************
void TC3_Handler(void)

FUNCTION:  Interrupt handler for the TC3 interrupt
				- Interrupt occurs every 1ms
***********************************************************************************/
void TC3_Handler()
{
//Check for a TC3 interrupt by looking at the OVF bit in the INTFLAG register.  If it
// is set, clear it by setting the bit.      
    if(TC3_REGS->COUNT16.TC_INTFLAG & TC_INTFLAG_OVF(1))
    {
        TC3_REGS->COUNT16.TC_INTFLAG = TC_INTFLAG_OVF(1);

 
//Check the switch level to see if it is pressed.  SW0 is a mask value that is declared
// in the "constants.h" file.  If it is low, the switch is pressed and the 'u16debounceCounter'
// variable is decremented once on each interrupt.  If the counter reaches 0, the switch is
// pressed and the 'b1switchStatus' flag is set.  If the switch is not pressed, the value in the
// counter reaches the 'DEBOUNCE_HIGH_LIMIT' value and the flag is cleared.
        if(!(PORT_REGS->GROUP[1].PORT_IN & SW0))
		{
			if(--u16debounceCounter == 0)
			{
				u16debounceCounter = DEBOUNCE_START;
				STRUCT_statusFlags.b1switchStatus = 1;
			}
		}
			
		else 
		{
			if(++u16debounceCounter >= DEBOUNCE_HIGH_LIMIT)
			{
				u16debounceCounter = DEBOUNCE_START;
				STRUCT_statusFlags.b1switchStatus = 0;
				STRUCT_statusFlags.b1ledState = 0;
			}
		}


//If the 'biswitchStatus' flag is set, change the 'b1ledState' flag to toggle the LED.
// This flag is read in the main loop and the LED is controlled there.		
		if(STRUCT_statusFlags.b1switchStatus)
		{
			if(++u16blinkCounter >= LED_BLINK_PERIOD)
			{
				u16blinkCounter = 0;
				STRUCT_statusFlags.b1ledState ^= 1;
			}			
		}
	}
}


/***********************************************************************************
void enable_interrupts(void)

FUNCTION:  Enable the TC3 interrupt
***********************************************************************************/

void enable_interrupts(void)
{
	NVIC_EnableIRQ(TC3_IRQn);
}
