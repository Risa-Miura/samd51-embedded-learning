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
REVISIONS

rev1		Denny Hopp
			12/19/18	
			- Started with rev18 or LAB4 and reduced the functionality
rev2        Denny Hopp
            1/2/19
            - comment changes
rev3        Denny Hopp
            1/8/19
            - code cleanup and comment changes
            - DFLL48M_open_loop_init( ) modified
            - DFLL48M_closed_loop_init( ) modified
rev4        Denny Hopp
            1/9/19
            - write synchronization checks moved to after the register or bits are set
rev6        Denny Hopp
            2/15/19
            - project cleanup
rev7        Denny Hopp
            2/15/19
            - converted to XC32
rev8        Denny Hopp
            2/18/19
            - comment cleanup
rev9        Denny Hopp
            2/18/19
            - DFLL closed loop reference is external crystal
rev10       Denny Hopp
            2/19/19
            - project converted from legacy Atmel header files to new Harmony include files
rev11       Denny Hopp
            2/20/19
            - code and comment cleanup
rev12       Denny Hopp
            2/20/19
            - code cleanup during entry into lab manual
 
MPLABX 5.15
XC32 v2.15
rev13       Denny Hopp
            2/25/19
            - MPLABX 5.15
            - new SAME54 board revision 
rev14       Denny Hopp
            2/27/19
            - config bits added
rev15       Denny Hopp
            3/13/19
            - write synchronization checks added for DFLL DFLLMUL and
                DFLLCTRLB registers
rev16       Denny Hopp
            4/10/19
            - int changed to short for two variables

MPLABX 5.20RC5
XC32 v2.15
rev17       Denny Hopp
            5/2/19
            - Builds with new MPLABX revision
rev18       Denny Hopp
            5/2/19
            - Device Pack DFP updated
 
MPLABX 5.20
XC32 v2.15
rev19       Denny Hopp
            5/20/19
            - Builds with new MPLABX revision 
rev20       Denny Hopp
            6/7/19
            - Component and PIO folders removed from project

MPLABX 5.30
XC32 v2.30
DFP 3.1.43
rev21           Denny Hopp
                1/9/20
                - Fixed SYNCBUSY bits for the GENCTRL registers

MPLABX 5.35
XC32 v2.30
DFP 3.2.56
rev22           Denny Hopp
                4/30/20
                - added revision_history.h
                - added component and pio folders with header files

MPLABX 5.40
XC32 v2.41
DFP 3.2.56
rev23           Denny Hopp
                5/12/20
                - XC32 and MPLABX upgrades
                - component and pio header files removed
 
MPLABX 5.45
XC32 v2.50
DFP 3.3.64 
rev24           Denny Hopp
                2/9/21
                - XC32 and MPLABX upgrades
 
MPLABX 5.45
XC32 v2.50
DFP 3.5.87 
rev25           Denny Hopp
                6/7/21
                - DFP Update 
			
*****************************************************************************************/