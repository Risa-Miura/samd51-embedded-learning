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

/*****************************************************************************************
FUNCTIONALITY:

This project uses the SAME54 Xplained board.  The project has the following functionality:
 - Shows the DFLL setup in both open and closed loop modes
 - Shows timer interrupt and LED output control
 - Shows how to read an input switch
 
When SW0 is pressed, the LED's on both the SAME54 Xplained board and IO Xplained board
 toggle every 500ms.  When one LED is on, the other is off.  When the switch is released,
 the LED on the IO Xplained board is off and the LED on the SAME54 board is on. 

*****************************************************************************************/

#ifndef CONSTANTS_H_
#define CONSTANTS_H_

//Use EXT1 on the Xplained Pro board to interface to the I/O Xplained board

//Pin names
#define XPLAINED_LED            PORT_PB08
#define SW0                     PORT_PB31
#define GCLK0_OUT               PORT_PB14
#define E54_LED_OUT             PORT_PC18

/*
Frequency of the system clock is 48MHZ.  The divider at TC3 is 16, so the frequency at TC3  
is 3MHZ (0.333us).  The timer is set up as an 16-bit counter.  To get a 1ms interrupt, 1ms/0.333us = 
3000.  
*/
#define TC3_CC0					3000

//LED blinks every 500ms.  Interrupt interval is 1ms.
#define LED_BLINK_PERIOD		500	

//Switch is debounced each TC3 interrupt interval (1ms).  It starts from the DEBOUNCE_START value and
// counts up or down, depending on the switch level.  If it reaches 0, the switch is pressed.  If it
// reaches the DEBOUNCE_HIGH_LIMIT, the switch is not pressed
#define DEBOUNCE_HIGH_LIMIT		100
#define DEBOUNCE_START			50 


#endif /* CONSTANTS_H_ */