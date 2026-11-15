/*****************************************************************************************

DISCLAIMER OF LIABILITY REGARDING CODE AND SERVICES

Upon your request and for your convenience Microchip Technology Incorporated
("Microchip") has modified your code.  Your acceptance and/or use of this
modified code constitutes agreement to the terms and conditions of this notice.

CODE ACCOMPANYING THIS MESSAGE AND RELATED SERVICES ARE SUPPLIED BY MICROCHIP "AS IS".
NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED
TO, IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY AND FITNESS FOR A
PARTICULAR PURPOSE APPLY TO THIS CODE, ITS INTERACTION WITH MICROCHIP�S PRODUCTS,
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

// Config Source code for XC32 compiler.
// USER_WORD_0
#pragma config BOD33_DIS = SET
#pragma config BOD33USERLEVEL = 0x1C // Enter Hexadecimal value
#pragma config BOD33_ACTION = RESET
#pragma config BOD33_HYST = 0x2 // Enter Hexadecimal value
#pragma config NVMCTRL_BOOTPROT = 0xF

// USER_WORD_1
#pragma config NVMCTRL_SEESBLK = 0x0 // Enter Hexadecimal value
#pragma config NVMCTRL_SEEPSZ = 0x0 // Enter Hexadecimal value
#pragma config RAMECC_ECCDIS = SET
#pragma config WDT_ENABLE = CLEAR
#pragma config WDT_ALWAYSON = CLEAR
#pragma config WDT_PER = CYC16384
#pragma config WDT_WINDOW = CYC16384
#pragma config WDT_EWOFFSET = CYC16384
#pragma config WDT_WEN = CLEAR

// USER_WORD_2
#pragma config NVMCTRL_REGION_LOCKS = 0xFFFFFFFF // Enter Hexadecimal value


#include <xc.h> 
#include "main.h"
#include "constants.h"



int main(void)
{
//Initialize the SAM system
    systemInit();
	
//Enable interrupts
	enable_interrupts( );
	
//Initialize variables
	u16debounceCounter = DEBOUNCE_START;
    
    while (1) 
    {
		if(!STRUCT_statusFlags.b1ledState)
		{
            PORT_REGS->GROUP[1].PORT_OUTSET = XPLAINED_LED;
            PORT_REGS->GROUP[2].PORT_OUTCLR = E54_LED_OUT;
 		}
		
		else
		{
            PORT_REGS->GROUP[1].PORT_OUTCLR = XPLAINED_LED;
            PORT_REGS->GROUP[2].PORT_OUTSET = E54_LED_OUT;            
		}
    }
}
