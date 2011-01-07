/********************************************************************
 FileName:		main.c
 Dependencies:	See INCLUDES section
 Processor:		PIC18 or PIC24 USB Microcontrollers
 Hardware:		The code is natively intended to be used on the following
 				hardware platforms: PICDEM™ FS USB Demo Board, 
 				PIC18F87J50 FS USB Plug-In Module, or
 				Explorer 16 + PIC24 USB PIM.  The firmware may be
 				modified for use on other USB platforms by editing the
 				HardwareProfile.h file.
 Complier:  	Microchip C18 (for PIC18) or C30 (for PIC24)
 

********************************************************************
 File Description:

 Change History:
  Rev   Date         Description
  1.0   11/19/2004   Initial release
  2.1   02/26/2007   Updated for simplicity and to use common
                     coding style
********************************************************************/
#define SWARMS_RELEASE_VERSION 2.00
#define OSCILATOR_SPEEDks 24 //oscilator speed
#define USBCommandByte OUTPacket[0] /*bascially the same..*/
#define SWARMS_USB_TIMEOUT_COMMAND_LOOPS 5000
/** INCLUDES *******************************************************/
#include "USB/usb.h"
#include "HardwareProfile.h"
#include "USB/usb_function_generic.h"
#include "fifoTxRx.h" 
#include "usart.h"
#include "SWARMSDefines.h"
#include "SWARMSControllerDefines.h"

#include "delays.h"
/** CONFIGURATION **************************************************/


#if defined(UBW)
//#error "dihdihi"

	#if OSCILATOR_SPEEDks == 24
        #pragma config PLLDIV   = 6         // (24 MHz crystal on UBW) *** 24 mhz!
	#elif OSCILATOR_SPEEDks == 20
        #pragma config PLLDIV   = 5         // (20 MHz crystal on UBW)
	#else
		#error "No appropiate processor speed defined for __OSCILATOR_SPEEDks__"
	#endif	
	    #pragma config CPUDIV   = OSC1_PLL2   
        #pragma config USBDIV   = 2         // Clock source from 96MHz PLL/2
        #pragma config FOSC     = HSPLL_HS
        #pragma config FCMEN    = OFF
        #pragma config IESO     = OFF
        #pragma config PWRT     = OFF
        #pragma config BOR      = ON
        #pragma config BORV     = 3
        #pragma config VREGEN   = ON      //USB Voltage Regulator
        #pragma config WDT      = OFF
        #pragma config WDTPS    = 32768
        #pragma config MCLRE    = ON
        #pragma config LPT1OSC  = OFF
        #pragma config PBADEN   = OFF
        #pragma config CCP2MX   = ON
        #pragma config STVREN   = ON
        #pragma config LVP      = OFF
//      #pragma config ICPRT    = OFF       // Dedicated In-Circuit Debug/Programming
        #pragma config XINST    = OFF       // Extended Instruction Set
        #pragma config CP0      = OFF
        #pragma config CP1      = OFF
        #pragma config CP2      = OFF
        #pragma config CP3      = OFF
        #pragma config CPB      = OFF
        #pragma config CPD      = OFF
        #pragma config WRT0     = OFF
        #pragma config WRT1     = OFF
        #pragma config WRT2     = OFF
        #pragma config WRT3     = OFF
        #pragma config WRTB     = ON       // Boot Block Write Protection
        #pragma config WRTC     = OFF
        #pragma config WRTD     = OFF
        #pragma config EBTR0    = OFF
        #pragma config EBTR1    = OFF
        #pragma config EBTR2    = OFF
        #pragma config EBTR3    = OFF
        #pragma config EBTRB    = OFF



#elif defined(PIC18F46J50_PIM) || defined(PIC18F_STARTER_KIT_1)
     #pragma config WDTEN = OFF          //WDT disabled (enabled by SWDTEN bit)
     #pragma config PLLDIV = 3           //Divide by 3 (12 MHz oscillator input)
     #pragma config STVREN = ON            //stack overflow/underflow reset enabled
     #pragma config XINST = OFF          //Extended instruction set disabled
     #pragma config CPUDIV = OSC1        //No CPU system clock divide
     #pragma config CP0 = OFF            //Program memory is not code-protected
     #pragma config OSC = HSPLL          //HS oscillator, PLL enabled, HSPLL used by USB
     #pragma config T1DIG = ON           //Sec Osc clock source may be selected
     #pragma config LPT1OSC = OFF        //high power Timer1 mode
     #pragma config FCMEN = OFF          //Fail-Safe Clock Monitor disabled
     #pragma config IESO = OFF           //Two-Speed Start-up disabled
     #pragma config WDTPS = 32768        //1:32768
     #pragma config DSWDTOSC = INTOSCREF //DSWDT uses INTOSC/INTRC as clock
     #pragma config RTCOSC = T1OSCREF    //RTCC uses T1OSC/T1CKI as clock
     #pragma config DSBOREN = OFF        //Zero-Power BOR disabled in Deep Sleep
     #pragma config DSWDTEN = OFF        //Disabled
     #pragma config DSWDTPS = 8192       //1:8,192 (8.5 seconds)
     #pragma config IOL1WAY = OFF        //IOLOCK bit can be set and cleared
     #pragma config MSSP7B_EN = MSK7     //7 Bit address masking
     #pragma config WPFP = PAGE_1        //Write Protect Program Flash Page 0
     #pragma config WPEND = PAGE_0       //Start protection at page 0
     #pragma config WPCFG = OFF          //Write/Erase last page protect Disabled
     #pragma config WPDIS = OFF          //WPFP[5:0], WPEND, and WPCFG bits ignored 
#else
    #error No hardware board defined, see "HardwareProfile.h"
#endif



/** VARIABLES ******************************************************/
#if defined(__18F14K50) || defined(__18F13K50) || defined(__18LF14K50) || defined(__18LF13K50) 
    #pragma udata usbram2
#elif defined(__18F2455) || defined(__18F2550) ||defined(__18F2553) || defined(__18F4455) || defined(__18F4550)\
    || defined(__18F2458) || defined(__18F2453) || defined(__18F4558) || defined(__18F4553)
    #pragma udata USB_VARIABLES=0x500
#elif defined(__18F4450) || defined(__18F2450)
    #pragma udata USB_VARIABLES=0x480
#else
    #pragma udata
#endif

BYTE VehicleType = VCL_TYPE_OBSC; //default setup

unsigned char OUTPacket[64];	//User application buffer for receiving and holding OUT packets sent from the host
unsigned char INPacket[64];		//User application buffer for sending IN packets to the host
unsigned char tempDataChars[64];		//User application buffer for sending IN packets to the host

unsigned int USBSWARMSCommandTimeout = 0; //where 1 is trigger and zero is idle

char *currentSWARMSCommandHandling = NULL;
#pragma udata
BOOL blinkStatusValid;
USB_HANDLE USBGenericOutHandle;
USB_HANDLE USBGenericInHandle;
#pragma udata


BOOL buttonPressHandled;
/* RS232/SPI ****************/
BYTE currentlyConnectedNode = USB_IDENT_UNKNOWN;
BYTE commandRequested = EMPTY;
char serialNode_In_Buffer[10] = {NIL};
char Node_Play_Buffer[10] = {NIL};
char serialNode_Out_Buffer[10] = {NIL}; 
char serialNode_Out_Iterator = 0;
BOOL isWaitingForReturnData = FALSE;
unsigned int USARTCommandTimeout = 0; //where 1 is trigger and zero is idle
#define RestartSerialDataCountdownTimeouts USARTCommandTimeout=5000;
#define StopSerialTimeouts USARTCommandTimeout=0;
#define SERIAL_NEEDS_SEND_DATA (serialNode_Out_Buffer[serialNode_Out_Iterator] != 0)


/**** OBSC ****/
char OBSC_Command_Addr = NIL;
char OBSC_Command_Funct = NIL;

/** PRIVATE PROTOTYPES *********************************************/
static void InitializeSystem(void);
void USBDeviceTasks(void);
void YourHighPriorityISRCode(void);
void YourLowPriorityISRCode(void);
void UserInit(void);
void ManageAnteriorLighting(void);
void SWARMSProcessing(void);
void ProcessSWARMSInput(void);
void ProcessSWARMSOutput(void);
void ProcessBaseHardwareIO(void);

void BlinkUSBStatus(void);
void HandleSWARMSCommand(char *command);
void SWARMSCommandCleanUp(void);
void SWARMSMakeCommandSuccess(char commandID, BOOL success, char *flags);

/******************		SWARMS Command Handlers	**********/
void handleSystemCommand(void);
void handleMotorCommand(void);
void handleIOCommand(void);
/********* OBSC *********/
void setupOBSCMotorStuff(char motor, signed int pm100Motor);
void setupOBSCSensorStuff(char * sensor);
/********* Serial *******/
void ProcessUSARTDataIO(void);
void USARTxService(void);
void ServiceUSARTCommands(void);
void routeCommandDataToNode(char * frame, BYTE length);
void serialNodeDeviceTasks(void);
void handleReceivedData(void);
void triggerNodeControlLine(BOOL isOn, BYTE Command);


/** VECTOR REMAPPING ***********************************************/

#if defined(PROGRAMMABLE_WITH_USB_MCHPUSB_BOOTLOADER)	

	#define REMAPPED_RESET_VECTOR_ADDRESS			0x800
	#define REMAPPED_HIGH_INTERRUPT_VECTOR_ADDRESS	0x808
	#define REMAPPED_LOW_INTERRUPT_VECTOR_ADDRESS	0x818
		
	extern void _startup (void);        // See c018i.c in your C18 compiler dir
	#pragma code REMAPPED_RESET_VECTOR = REMAPPED_RESET_VECTOR_ADDRESS
	void _reset (void)
	{
	    _asm goto _startup _endasm
	}

	#pragma code REMAPPED_HIGH_INTERRUPT_VECTOR = REMAPPED_HIGH_INTERRUPT_VECTOR_ADDRESS
	void Remapped_High_ISR (void)
	{
	     _asm goto YourHighPriorityISRCode _endasm
	}
	#pragma code REMAPPED_LOW_INTERRUPT_VECTOR = REMAPPED_LOW_INTERRUPT_VECTOR_ADDRESS
	void Remapped_Low_ISR (void)
	{
	     _asm goto YourLowPriorityISRCode _endasm
	}
	
	
	#pragma code HIGH_INTERRUPT_VECTOR = 0x08
	void High_ISR (void)
	{
	     _asm goto REMAPPED_HIGH_INTERRUPT_VECTOR_ADDRESS _endasm
	}
	#pragma code LOW_INTERRUPT_VECTOR = 0x18
	void Low_ISR (void)
	{
	     _asm goto REMAPPED_LOW_INTERRUPT_VECTOR_ADDRESS _endasm
	}

	#pragma code
	
	//These are your actual interrupt handling routines.
	#pragma interrupt YourHighPriorityISRCode
	void YourHighPriorityISRCode()
	{
		//Check which interrupt flag caused the interrupt.
		//Service the interrupt
		//Clear the interrupt flag
		//Etc.
		
        #if defined(USB_INTERRUPT)
	        USBDeviceTasks();
        #endif
	
	}	//This return will be a "retfie fast", since this is in a #pragma interrupt section 
	#pragma interruptlow YourLowPriorityISRCode
	void YourLowPriorityISRCode()
	{
		//Check which interrupt flag caused the interrupt.
		//Service the interrupt
		//Clear the interrupt flag
		//Etc.
	
	}	
#endif




/** DECLARATIONS ***************************************************/
#pragma code

/******************************************************************************
 * Function:        void main(void)
 *
 * PreCondition:    None
 *
 * Input:           None
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        Main program entry point.
 *
 * Note:            None
 *******************************************************************/

#if defined(__18CXX)
void main(void)
#else
int main(void)
#endif
{   
    InitializeSystem();

    #if defined(USB_INTERRUPT)
        USBDeviceAttach();
    #endif

    while(1)
    {
        #if defined(USB_POLLING) //interrupt mode currently
		// Check bus status and service USB interrupts.
     //   USBDeviceTasks(); // Interrupt or polling method.  If using polling, must call
        				  // this function periodically.  This function will take care
        				  // of processing and responding to SETUP transactions 
        				  // (such as during the enumeration process when you first
        				  // plug in).  USB hosts require that USB devices should accept
        				  // and process SETUP packets in a timely fashion.  Therefore,
        				  // when using polling, this function should be called 
        				  // frequently (such as once about every 100 microseconds) at any
        				  // time that a SETUP packet might reasonably be expected to
        				  // be sent by the host to your device.  In most cases, the
        				  // USBDeviceTasks() function does not take very long to
        				  // execute (~50 instruction cycles) before it returns.
        #endif
    				  

		// Application-specific tasks.
		// Application related code may be added here, or in the ProcessIO() function.
        ManageAnteriorLighting(); ///lights and mood!
		ProcessBaseHardwareIO();       	
		ProcessSWARMSInput();	
		SWARMSProcessing();
		ProcessSWARMSOutput();
		
		ProcessUSARTDataIO();
    }//end while
}//end main


/********************************************************************
 * Function:        static void InitializeSystem(void)
 * Overview:        InitializeSystem is a centralize initialization
 *                  routine. All required USB initialization routines
 *                  are called from here.
 *
 *                  User application initialization routine should
 *                  also be called from here.                  
 *
 * Note:            None
 *******************************************************************/
static void InitializeSystem(void)
{
        ADCON1 |= 0x0F;                 // Default all pins to digital
 

//	The USB specifications require that USB peripheral devices must never source
//	current onto the Vbus pin.  Additionally, USB peripherals should not source
//	current on D+ or D- when the host/hub is not actively powering the Vbus line.
//	When designing a self powered (as opposed to bus powered) USB peripheral
//	device, the firmware should make sure not to turn on the USB module and D+
//	or D- pull up resistor unless Vbus is actively powered.  Therefore, the
//	firmware needs some means to detect when Vbus is being powered by the host.
//	A 5V tolerant I/O pin can be connected to Vbus (through a resistor), and
// 	can be used to detect when Vbus is high (host actively powering), or low
//	(host is shut down or otherwise not supplying power).  The USB firmware
// 	can then periodically poll this I/O pin to know when it is okay to turn on
//	the USB module/D+/D- pull up resistor.  When designing a purely bus powered
//	peripheral device, it is not possible to source current on D+ or D- when the
//	host is not actively providing power on Vbus. Therefore, implementing this
//	bus sense feature is optional.  This firmware can be made to use this bus
//	sense feature by making sure "USE_USB_BUS_SENSE_IO" has been defined in the
//	HardwareProfile.h file.    
    #if defined(USE_USB_BUS_SENSE_IO)
    tris_usb_bus_sense = INPUT_PIN; // See HardwareProfile.h
    #endif
    
//	If the host PC sends a GetStatus (device) request, the firmware must respond
//	and let the host know if the USB peripheral device is currently bus powered
//	or self powered.  See chapter 9 in the official USB specifications for details
//	regarding this request.  If the peripheral device is capable of being both
//	self and bus powered, it should not return a hard coded value for this request.
//	Instead, firmware should check if it is currently self or bus powered, and
//	respond accordingly.  If the hardware has been configured like demonstrated
//	on the PICDEM FS USB Demo Board, an I/O pin can be polled to determine the
//	currently selected power source.  On the PICDEM FS USB Demo Board, "RA2" 
//	is used for	this purpose.  If using this feature, make sure "USE_SELF_POWER_SENSE_IO"
//	has been defined in HardwareProfile.h, and that an appropriate I/O pin has been mapped
//	to it in HardwareProfile.h.
    #if defined(USE_SELF_POWER_SENSE_IO)
    tris_self_power = INPUT_PIN;	// See HardwareProfile.h
    #endif
    
	USBGenericOutHandle = 0;	
	USBGenericInHandle = 0;		

    UserInit();			//Application related initialization.  See user.c

    USBDeviceInit();	//usb_device.c.  Initializes USB module SFRs and firmware
    					//variables to known states.
}//end InitializeSystem



void UserInit(void)
{
	
	//usart
		unsigned char c;
  	    //ANSELHbits.ANS11 = 0;	// Make RB5 digital so USART can use pin for Rx

        UART_TRISRx=1;				// RX
        UART_TRISTx=1;				// TX
        TXSTA = 0b00100000;       	// TX enable BRGH=1
        RCSTA = 0b10110000;// 0x90;       	// Single Character RX
        SPBRG = 38;
        SPBRGH = 0;      	// 259 for 48MHz -> 19200 baud
        BAUDCON = 0;     	// BRG16 = 0
        c = RCREG;				// read 
	
    mInitAllLEDs();
    mInitSwitch();

	blinkStatusValid = TRUE;	//Blink the normal USB state on the LEDs.

}//end UserInit


/**************************************************
*				SWARMS Input PRECESSING
* 
*
*
****************************************************/
void ProcessSWARMSInput(void){
	char commandSuccess;
	char SWARMSCommandID;
 
	if((USBDeviceState < CONFIGURED_STATE)||(USBSuspendControl==1)) return;
	    // this is where I should insert the code for specific control 
    if(!USBHandleBusy(USBGenericOutHandle))		//Check if the endpoint has received any data from the host.
    {   
	    if(! canfitCommandInOutputCommandBuffer(5)){
		    //can't ensure reply -- just drop frame
		    USBGenericOutHandle = USBGenRead(USBGEN_EP_NUM,(BYTE*)&OUTPacket,USBGEN_EP_SIZE);
		    	mLED_2_On();
		    	mLED_1_Off()
			return;
		   } 
		   
		SWARMSCommandID = OUTPacket[0];	    
	    if(IS_VALID_COMMAND){
			commandSuccess = writeInputCommand(OUTPacket);
		switch(commandSuccess){
			case fifoBufferStatusGood:
				//report success
				break;
			case fifoBufferStatusFull:
			//abort buffer full
//			mLED_2_Toggle();
				SWARMSMakeCommandSuccess(SWARMSCommandID, FAILURE, NULL); //TODO: ADD FLAGS
			break;
			case fifoBufferStatusBadInputOversize:
			//abort buffer oversize input
				SWARMSMakeCommandSuccess(SWARMSCommandID, FAILURE, NULL); //TODO: ADD FLAGS
			break;
			case fifoBufferStatusBadInput:
			default:
			//notify bad command
				SWARMSMakeCommandSuccess(SWARMSCommandID, FAILURE, NULL); //TODO: ADD FLAGS
			break;	
		}	  
		   
		}else{
			SWARMSMakeCommandSuccess(SWARMSCommandID, FAILURE, NULL); //TODO: ADD FLAGS
		}	
  			 		
	    	USBGenericOutHandle = USBGenRead(USBGEN_EP_NUM,(BYTE*)&OUTPacket,USBGEN_EP_SIZE);
	    
	  } 
	    
}	


/**************************************************
*				SWARMS Output facilitation  
* 
*
*
****************************************************/
void ProcessSWARMSOutput(void){
	char *SWARMSCommandToSend = NULL;
	int iterator;

	if((USBDeviceState < CONFIGURED_STATE)||(USBSuspendControl==1)) return;
	
	if (readOutputCommand() != NULL){
		if(!USBHandleBusy(USBGenericInHandle)){
			SWARMSCommandToSend = readOutputCommand();
			for (iterator = 0; iterator < ( 2 + *(SWARMSCommandToSend + 1)); iterator++){
				INPacket[iterator] = *(SWARMSCommandToSend + iterator);
			}
			INPacket[2 + *(SWARMSCommandToSend + 1)] = NULL; //null terminate
		    USBGenericInHandle = USBGenWrite(USBGEN_EP_NUM,(BYTE*)&INPacket,USBGEN_EP_SIZE);
		    
		    popHeadOutputCommand();
	 	} 		
   	}
}

/**************************************************
*				Handle Hardware IO
* 
*
*
****************************************************/
void ProcessBaseHardwareIO(void){
    if (!UserSW){
	  if (!buttonPressHandled){
			  buttonPressHandled = TRUE;
			 setupOBSCMotorStuff('1', 80);
			 mLED_1_Off();
			//TXREG = 0b01011000;
			//  mLED_2_Toggle(); //moved to the txcontrol function
		 }   
	 }
	 else{
		 buttonPressHandled = FALSE;
	 }
}

/**************************************************
*				SWARMS PRECESSING
* 
*
*
****************************************************/
void SWARMSProcessing(void){
	char commandSuccess;
	char SWARMSCommandID;
	
	if(currentSWARMSCommandHandling != NULL){
		//handle specifics or route 
		//read usart.sensors.etc.
		if (USBSWARMSCommandTimeout > 0){
			USBSWARMSCommandTimeout++;
			if (USBSWARMSCommandTimeout > SWARMS_USB_TIMEOUT_COMMAND_LOOPS){ //timeout occured
				//TODO: send command abort
				SWARMSCommandCleanUp();	
			}	
		}	
	}
	
	if (canfitCommandInOutputCommandBuffer(5)  && currentSWARMSCommandHandling == NULL){
		currentSWARMSCommandHandling = readInputCommand();
		SWARMSCommandID = currentSWARMSCommandHandling[0];
		
		if (currentSWARMSCommandHandling != NULL){
			USBSWARMSCommandTimeout = 1; //activate timeouts
			if (USB_COMMAND_SYSTEM){
				handleSystemCommand();
			}
			else if(USB_COMMAND_DRIVE){
			handleMotorCommand();
			USBSWARMSCommandTimeout = 1; // activate timeouts
			}else if (USB_COMMAND_SENS_IO){
				handleIOCommand();
			}else{
				tempDataChars[0] = 3;
				tempDataChars[1] = 1;
				tempDataChars[2] = currentSWARMSCommandHandling[0];
				writeOutputCommand(tempDataChars);	
				SWARMSCommandCleanUp();				
			}
		}
	}		
}	


void SWARMSCommandCleanUp(void){
	currentSWARMSCommandHandling = NULL;
	popHeadInputCommand();	
	USBSWARMSCommandTimeout = 0;
}

void SWARMSMakeCommandSuccess(char commandID, BOOL success, char *flags){
	char iterator = NULL;
	char nextFlag = NULL;
	if (commandID == NULL)
		return;
	
	if (success){
		tempDataChars[0] = 1;
		tempDataChars[1] = 1;
		tempDataChars[2] = commandID;
	}else{
		tempDataChars[0] = 2;
		tempDataChars[1] = 1;
		tempDataChars[2] = commandID;	
	}
	if (flags != NULL){
		for(iterator = 0; flags[iterator] != NULL && iterator < 64; iterator++){
			tempDataChars[1]++;
			tempDataChars[3 + iterator] = flags[iterator];
		}	
	}
	writeOutputCommand(tempDataChars);		
}	

/******************************************************************************
 * Function:        void ManageAnteriorLighting(void)
 *
 * Overview:       makes lights look nice -- does cool ambient lighting stuff
 *                 makes system visually appealing
 *
 * Note:            None
 *****************************************************************************/
void ManageAnteriorLighting(void)
{   
   //Blink the LEDs according to the USB device status, but only do so if the PC application isn't connected and controlling the LEDs.
    if(blinkStatusValid)
    {
        BlinkUSBStatus();
    }
    

    
//	    LIB USB Microchip
//	Basically, this system uses 'handles' that represent 'endpoints' that represent data ports for receiving and transmitting data
//	The initial computer data in port (labeled as handle "out") is used for receiving data
//		This port is used to send data to the device…
//			At this point it can be picked up by the device 
//	The device can write to the (IN) buffer to send data to the computer.
//	


	    /*
        switch(OUTPacket[0])					//Data arrived, check what kind of command might be in the packet of data.
        {   
            case 0x80:  //Toggle LED(s) command from PC application.	
				if(blinkStatusValid){
		        	blinkStatusValid = FALSE;		//Disable the regular LED blink pattern indicating USB state, PC application is controlling the LEDs.
		        	mLED_1_On();
                	mLED_2_On();
         		}     
               	mLED_2_Toggle();
                break;
            case 0x81:  //Get push button state command from PC application.
                INPacket[0] = 0x81;				//Echo back to the host PC the command we are fulfilling in the first byte.  In this case, the Get Pushbutton State command.
//				if(sw2 == 1)					//pushbutton not pressed, pull up resistor on circuit board is pulling the PORT pin high
				if (UserSW == 1)
				{
					INPacket[1] = 0x01;			
				}
				else							//sw2 must be == 0, pushbutton is pressed and overpowering the pull up resistor
				{
					INPacket[1] = 0x00;
				}				
				//Now check to make sure no previous attempts to send data to the host are still pending.  If any attemps are still
	             if(!USBHandleBusy(USBGenericInHandle))		
	            {	
		            //The endpoint was not "busy", therefore it is safe to write to the buffer and arm the endpoint.					
	                //The USBGenWrite() function call "arms" the endpoint (and makes the handle indicate the endpoint is busy).
	                //Once armed, the data will be automatically sent to the host (in hardware by the SIE) the next time the 
	                //host polls the endpoint.  Once the data is successfully sent, the handle (in this case USBGenericInHandle) 
	                //will indicate the the endpoint is no longer busy.
					USBGenericInHandle = USBGenWrite(USBGEN_EP_NUM,(BYTE*)&INPacket,USBGEN_EP_SIZE);	
                }
                break;
        }
        */
        
        //Re-arm the OUT endpoint for the next packet:
	    //The USBGenRead() function call "arms" the endpoint (and makes it "busy").  If the endpoint is armed, the SIE will 
	    //automatically accept data from the host, if the host tries to send a packet of data to the endpoint.  Once a data 
	    //packet addressed to this endpoint is received from the host, the endpoint will no longer be busy, and the application
	    //can read the data which will be sitting in the buffer.
    
 

}//end ProcessIO



/********************************************
**
**            SWARMS Command Handlers
**
**
*********************************************/

void handleSystemCommand(void)
{	
	switch(currentSWARMSCommandHandling[0]){
		case USB_ACK:
			SWARMSMakeCommandSuccess(1, SUCCESS, NULL);
			SWARMSCommandCleanUp();
		break;	
		case USB_ABORT:
			SWARMSMakeCommandSuccess(2, SUCCESS, NULL);
			
			SWARMSCommandCleanUp();
		break;
		case USB_STATUS:
//		if (USBCommandBuffer[0] == 0){
//			USB_MAKE_COMMAND_ABORT; 
//		}
			tempDataChars[0] = 3;
			tempDataChars[1] = 1;
			tempDataChars[2] = currentSWARMSCommandHandling[0];
			writeOutputCommand(tempDataChars);	
			SWARMSCommandCleanUp();
		break;	
		case USB_RESET:
//			handleResetRequest(USBCommandBuffer[0]);
			SWARMSMakeCommandSuccess(4, SUCCESS, NULL);
			
			SWARMSCommandCleanUp();
		break;	
		case USB_VALIDATE_COMMAND:
//			switch (VehicleType)
//			{
//				case VCL_TYPE_4WHL_FRNTTURN:
//					if (COMMAND_IS_PROHIB_BY_VCL_TYPE_4WHL_FRNTTURN){USB_Out_Buffer[0] = 0x03;USB_Out_Buffer[1] = 0x00; sendFrame(USB_VALIDATE_COMMAND,USB_Out_Buffer);}
//					else {USB_Out_Buffer[0] = 0x01;USB_Out_Buffer[1] = 0x00; sendFrame(USB_VALIDATE_COMMAND,USB_Out_Buffer);}
//				break;
//				case VCL_TYPE_4WHL_SQRDIF:
//					if (COMMAND_IS_PROHIB_BY_VCL_TYPE_4WHL_SQRDIF){USB_Out_Buffer[0] = 0x03;USB_Out_Buffer[1] = 0x00; sendFrame(USB_VALIDATE_COMMAND,USB_Out_Buffer);}
//					else {USB_Out_Buffer[0] = 0x01;USB_Out_Buffer[1] = 0x00; sendFrame(USB_VALIDATE_COMMAND,USB_Out_Buffer);}
//				break;
//				case VCL_TYPE_CRCDIF:
//					if (COMMAND_IS_PROHIB_BY_VCL_TYPE_4WHL_CRCDIF){USB_Out_Buffer[0] = 0x03;USB_Out_Buffer[1] = 0x00; sendFrame(USB_VALIDATE_COMMAND,USB_Out_Buffer);}
//					else {USB_Out_Buffer[0] = 0x01;USB_Out_Buffer[1] = 0x00; sendFrame(USB_VALIDATE_COMMAND,USB_Out_Buffer);}
//				break;
//				case VCL_TYPE_3WHL_OMNIDRV:
//					if (COMMAND_IS_PROHIB_BY_VCL_TYPE_3WHL_OMNIDRV){USB_Out_Buffer[0] = 0x03;USB_Out_Buffer[1] = 0x00; sendFrame(USB_VALIDATE_COMMAND,USB_Out_Buffer);}
//					else {USB_Out_Buffer[0] = 0x01;USB_Out_Buffer[1] = 0x00; sendFrame(USB_VALIDATE_COMMAND,USB_Out_Buffer);}
//				break;					
//			}	
		break;	
		case USB_SYS_VERSION:
			tempDataChars[0] = 7;
			btoa(SWARMS_RELEASE_VERSION, (tempDataChars + 2));//* changed from 1.01 convert\
			tempDataChars[1] = strlen((tempDataChars+2));
			writeOutputCommand(tempDataChars);
//			sendFrame(USB_SYS_VERSION,USB_Out_Buffer);
		break;	
		case USB_ACT_LED:
			if(blinkStatusValid){
				blinkStatusValid = FALSE;		//Disable the regular LED blink pattern indicating USB state, PC application is controlling the LEDs.
			    mLED_1_On();
	            mLED_2_On();
	   		}   
			switch(currentSWARMSCommandHandling[2])
			{
				case LED_1_IDENT:
					mLED_1_Toggle();
					SWARMSMakeCommandSuccess(5, SUCCESS, NULL);
					SWARMSCommandCleanUp();
//   	   				  mLED_1_On();
//   	   				  USB_MAKE_ACK;				
				break;
				case LED_2_IDENT:
					mLED_2_Toggle();
					SWARMSMakeCommandSuccess(5, SUCCESS, NULL);
					SWARMSCommandCleanUp();
				break;	
				default:
		 			tempDataChars[0] = 3;
					tempDataChars[1] = 2;
					tempDataChars[2] = currentSWARMSCommandHandling[0];
					tempDataChars[3] = currentSWARMSCommandHandling[2];
					writeOutputCommand(tempDataChars);	
					SWARMSCommandCleanUp();
				break;
			}	
		break;
		default:
			tempDataChars[0] = 3;
			tempDataChars[1] = 1;
			tempDataChars[2] = currentSWARMSCommandHandling[0];
			writeOutputCommand(tempDataChars);	
			SWARMSCommandCleanUp();
		break;	
		}	
}


void handleMotorCommand(void)
{
	char iterator = 0;
	switch(currentSWARMSCommandHandling[0]){
		case USB_M_MOVE_ABSOLUTE_SPEED:
			//launch obsc sys setup
			for (iterator = 0; iterator < *(currentSWARMSCommandHandling + 1); iterator++){
				tempDataChars[iterator] = *(currentSWARMSCommandHandling + 2 + iterator);		
			}	
			tempDataChars[*(currentSWARMSCommandHandling + 1)] = NULL;
			setupOBSCMotorStuff('1',atoi(tempDataChars));
		break;	
		case USB_M_DRIVE_DIRECTION:
			//launch obsc sys setup
			for (iterator = 0; iterator < *(currentSWARMSCommandHandling + 1); iterator++){
				tempDataChars[iterator] = *(currentSWARMSCommandHandling + 2 + iterator);		
			}	
			tempDataChars[*(currentSWARMSCommandHandling + 1)] = NULL;
			
			setupOBSCMotorStuff('2',atoi(tempDataChars));
		break;	
		/*
		case USB_M_VEHICLE_TYPE:
			if(USBCommandBuffer[0])
			{
				//send out vehicle type
			}
			else
			{	
				VCL_TYPE_HOLDER = USBCommandBuffer[0];
				if(!IS_VALID_VCL_TYPE)
				{
					VCL_TYPE_HOLDER = VCL_TYPE_4WHL_FRNTTURN;
					USB_MAKE_COMMAND_ABORT;					
				}
				else {  USB_MAKE_ACK;}
			}	
		break;
		*/
		default:
			tempDataChars[0] = 3;
			tempDataChars[1] = 1;
			tempDataChars[2] = currentSWARMSCommandHandling[0];
			writeOutputCommand(tempDataChars);	
			SWARMSCommandCleanUp();
		break;	
		}	
}	

void handleIOCommand(void)
{
	signed int tempInt =0;
	switch(currentSWARMSCommandHandling[0]){
		case USB_IO_ANALOG_INPUT:
			tempInt = currentSWARMSCommandHandling[2];
			setupOBSCSensorStuff(ultoa(tempInt,Node_Play_Buffer));			
		break;			
		default:
			tempDataChars[0] = 3;
			tempDataChars[1] = 1;
			tempDataChars[2] = currentSWARMSCommandHandling[0];
			writeOutputCommand(tempDataChars);	
			SWARMSCommandCleanUp();
		break;	
	}	
}	


/*******************	OBSC Commands	*****************/
/************** 		FIX THIS UP		 ****************/

/**************************************************
*				Process USART IO  
* 			Facilitate SWARMS Control
*
*
****************************************************/
void setupOBSCMotorStuff(char motor, signed int pm100Motor)
{
	char motorPowerToSend = 50 + (pm100Motor/10);
	serialNode_Out_Buffer[0] = motor;
	serialNode_Out_Buffer[1] = motorPowerToSend;
	serialNode_Out_Buffer[2] = NIL;	
	
	commandRequested = serialNode_Out_Buffer[0];
}
void setupOBSCSensorStuff(char * sensor)
{
	serialNode_Out_Buffer[0] = '4';
	serialNode_Out_Buffer[1] = sensor[0];
	serialNode_Out_Buffer[2] = NIL;		
	
	commandRequested = serialNode_Out_Buffer[0];
}

void ProcessUSARTDataIO(void){
	char timeoutSWARMSFlag[] = "T";
	if(!BusyUSART( ))
	{					
		if(DataRdyUSART())
		{
			serialNode_In_Buffer[0] = ReadUSART();
			//service usart commands
			ServiceUSARTCommands();
		}	
		
			if (USARTCommandTimeout == 1) //timeout
			{
				
				SWARMSMakeCommandSuccess(currentSWARMSCommandHandling[0], FAILURE, timeoutSWARMSFlag);
				SWARMSCommandCleanUp();
				
				isWaitingForReturnData = FALSE;
				serialNode_In_Buffer[0] = NIL;		
				serialNode_Out_Buffer[0] = NIL;		
				commandRequested = NIL;
				USARTCommandTimeout = 0;
				serialNode_Out_Iterator = 0;
			}	
			else if (isWaitingForReturnData && USARTCommandTimeout > 1)
			{
				USARTCommandTimeout--;
			}
			//handle xmition
			USARTxService();		
	}	
}	

void ServiceUSARTCommands(void)
{   
	if (VehicleType == VCL_TYPE_OBSC)
	{
		if(serialNode_In_Buffer[0] == 'A')
		{		
			isWaitingForReturnData = FALSE;
			StopSerialTimeouts;
		}else
		if(serialNode_In_Buffer[0] == 'B')
		{		
			isWaitingForReturnData = FALSE;
			StopSerialTimeouts;
		//	mLED_3_On();
		}
		if(isWaitingForReturnData == FALSE && !SERIAL_NEEDS_SEND_DATA)
		{
			serialNode_In_Buffer[0] = NIL;		
			commandRequested = NIL;
			StopSerialTimeouts;
			serialNode_Out_Iterator = 0;
			SWARMSMakeCommandSuccess(currentSWARMSCommandHandling[0], SUCCESS, NULL);
			SWARMSCommandCleanUp();
		}
		
						
		if(commandRequested == '4' && !SERIAL_NEEDS_SEND_DATA)
		{
			tempDataChars[0] = 40;
			tempDataChars[1] = 1;
			tempDataChars[2] = serialNode_In_Buffer[0]; 
			writeOutputCommand(tempDataChars);	
			SWARMSCommandCleanUp();
			
			isWaitingForReturnData = FALSE;	
			StopSerialTimeouts;
		}
	}
}	

void USARTxService(void)
{
	if (SERIAL_NEEDS_SEND_DATA && isWaitingForReturnData == FALSE)
		{	
			
			mLED_1_Toggle();
			
			WriteUSART(serialNode_Out_Buffer[serialNode_Out_Iterator]);
			serialNode_Out_Iterator ++;
			
			if (VehicleType == VCL_TYPE_OBSC)
				{
					isWaitingForReturnData = TRUE;
					RestartSerialDataCountdownTimeouts;
				}	
			if(!SERIAL_NEEDS_SEND_DATA)
			{
				serialNode_Out_Iterator = 0;
				serialNode_Out_Buffer[0] = NIL;
			}	
		}	
}	

/********************************************************************
 * Function:        void BlinkUSBStatus(void)
 *
 * Overview:        BlinkUSBStatus turns on and off LEDs 
 *                  corresponding to the USB device state.
 *
 * Note:            mLED macros can be found in HardwareProfile.h
 *                  USBDeviceState is declared and updated in
 *                  usb_device.c.
 *******************************************************************/
void BlinkUSBStatus(void)
{
    static WORD led_count=0;
    
    if(led_count == 0)led_count = 10000U;
    led_count--;

    if(USBSuspendControl == 1)
    {
        if(led_count==0)
        {  
              mLED_2_Toggle();
              mLED_1_Off();
        }//end if
    }
    else
    {
        if(USBDeviceState == DETACHED_STATE)
        {
            mLED_Both_Off();
        }
        else if(USBDeviceState == ATTACHED_STATE)
        {
            mLED_Both_On();
        }
        else if(USBDeviceState == POWERED_STATE)
        {
            mLED_Only_1_On();
        }
        else if(USBDeviceState == DEFAULT_STATE)
        {
            mLED_Only_2_On();
        }
        else if(USBDeviceState == ADDRESS_STATE)
        {
            if(led_count == 0)
            {
                mLED_1_Toggle();
                mLED_2_Off();
            }//end if
        }
        else if(USBDeviceState == CONFIGURED_STATE)
        {
            if(led_count==0)
            {
                mLED_1_Toggle();
                if(mGetLED_1())
                {
                    mLED_2_Off();
                }
                else
                {
                    mLED_2_On();
                }
            }//end if
        }//end if(...)
    }//end if(UCONbits.SUSPND...)

}//end BlinkUSBStatus




// ******************************************************************************************************
// ************** USB Callback Functions ****************************************************************
// ******************************************************************************************************
// The USB firmware stack will call the callback functions USBCBxxx() in response to certain USB related
// events.  For example, if the host PC is powering down, it will stop sending out Start of Frame (SOF)
// packets to your device.  In response to this, all USB devices are supposed to decrease their power
// consumption from the USB Vbus to <2.5mA each.  The USB module detects this condition (which according
// to the USB specifications is 3+ms of no bus activity/SOF packets) and then calls the USBCBSuspend()
// function.  You should modify these callback functions to take appropriate actions for each of these
// conditions.  For example, in the USBCBSuspend(), you may wish to add code that will decrease power
// consumption from Vbus to <2.5mA (such as by clock switching, turning off LEDs, putting the
// microcontroller to sleep, etc.).  Then, in the USBCBWakeFromSuspend() function, you may then wish to
// add code that undoes the power saving things done in the USBCBSuspend() function.

// The USBCBSendResume() function is special, in that the USB stack will not automatically call this
// function.  This function is meant to be called from the application firmware instead.  See the
// additional comments near the function.


void USBCBSuspend(void)
{	
	//ConfigureIOPinsForLowPower();
	//SaveStateOfAllInterruptEnableBits();
	//DisableAllInterruptEnableBits();
	//EnableOnlyTheInterruptsWhichWillBeUsedToWakeTheMicro();	//should enable at least USBActivityIF as a wake source
	//Sleep();
	//RestoreStateOfAllPreviouslySavedInterruptEnableBits();	//Preferrably, this should be done in the USBCBWakeFromSuspend() function instead.
	//RestoreIOPinsToNormal();									//Preferrably, this should be done in the USBCBWakeFromSuspend() function instead.

	//IMPORTANT NOTE: Do not clear the USBActivityIF (ACTVIF) bit here.  This bit is 
	//cleared inside the usb_device.c file.  Clearing USBActivityIF here will cause 
	//things to not work as intended.	
	

    #if defined(__C30__)
    #if 0
        U1EIR = 0xFFFF;
        U1IR = 0xFFFF;
        U1OTGIR = 0xFFFF;
        IFS5bits.USB1IF = 0;
        IEC5bits.USB1IE = 1;
        U1OTGIEbits.ACTVIE = 1;
        U1OTGIRbits.ACTVIF = 1;
        Sleep();
    #endif
    #endif
}


/******************************************************************************
 * Function:        void _USB1Interrupt(void)
 *
 * Overview:        This function is called when the USB interrupt bit is set
 *					In this example the interrupt is only used when the device
 *					goes to sleep when it receives a USB suspend command
 *
 * Note:            None
 *****************************************************************************/
#if 0
void __attribute__ ((interrupt)) _USB1Interrupt(void)
{
    #if !defined(self_powered)
        if(U1OTGIRbits.ACTVIF)
        {
            IEC5bits.USB1IE = 0;
            U1OTGIEbits.ACTVIE = 0;
            IFS5bits.USB1IF = 0;
        
            //USBClearInterruptFlag(USBActivityIFReg,USBActivityIFBitNum);
            USBClearInterruptFlag(USBIdleIFReg,USBIdleIFBitNum);
            //USBSuspendControl = 0;
        }
    #endif
}
#endif

/******************************************************************************
 * Function:        void USBCBWakeFromSuspend(void)
 * Overview:        The host may put USB peripheral devices in low power
 *					suspend mode (by "sending" 3+ms of idle).  Once in suspend
 *					mode, the host may wake the device back up by sending non-
 *					idle state signalling.
 *					
 *					This call back is invoked when a wakeup from USB suspend 
 *					is detected.
 *
 * Note:            None
 *****************************************************************************/
void USBCBWakeFromSuspend(void)
{
	// If clock switching or other power savings measures were taken when
	// executing the USBCBSuspend() function, now would be a good time to
	// switch back to normal full power run mode conditions.  The host allows
	// a few milliseconds of wakeup time, after which the device must be 
	// fully back to normal, and capable of receiving and processing USB
	// packets.  In order to do this, the USB module must receive proper
	// clocking (IE: 48MHz clock must be available to SIE for full speed USB
	// operation).
}

/********************************************************************
 * Function:        void USBCB_SOF_Handler(void)
 *
 * Overview:        The USB host sends out a SOF packet to full-speed
 *                  devices every 1 ms. This interrupt may be useful
 *                  for isochronous pipes. End designers should
 *                  implement callback routine as necessary.
 *
 * Note:            None
 *******************************************************************/
void USBCB_SOF_Handler(void)
{
    // No need to clear UIRbits.SOFIF to 0 here.
    // Callback caller is already doing that.
}

/*******************************************************************
 * Function:        void USBCBErrorHandler(void)
 * Overview:        The purpose of this callback is mainly for
 *                  debugging during development. Check UEIR to see
 *                  which error causes the interrupt.
 *
 * Note:            None
 *******************************************************************/
void USBCBErrorHandler(void)
{
    // No need to clear UEIR to 0 here.
    // Callback caller is already doing that.

	// Typically, user firmware does not need to do anything special
	// if a USB error occurs.  For example, if the host sends an OUT
	// packet to your device, but the packet gets corrupted (ex:
	// because of a bad connection, or the user unplugs the
	// USB cable during the transmission) this will typically set
	// one or more USB error interrupt flags.  Nothing specific
	// needs to be done however, since the SIE will automatically
	// send a "NAK" packet to the host.  In response to this, the
	// host will normally retry to send the packet again, and no
	// data loss occurs.  The system will typically recover
	// automatically, without the need for application firmware
	// intervention.
	
	// Nevertheless, this callback function is provided, such as
	// for debugging purposes.
}


/*******************************************************************
 * Function:        void USBCBCheckOtherReq(void)
 *
 * Overview:        When SETUP packets arrive from the host, some
 * 					firmware must process the request and respond
 *					appropriately to fulfill the request.  Some of
 *					the SETUP packets will be for standard
 *					USB "chapter 9" (as in, fulfilling chapter 9 of
 *					the official USB specifications) requests, while
 *					others may be specific to the USB device class
 *					that is being implemented.  For example, a HID
 *					class device needs to be able to respond to
 *					"GET REPORT" type of requests.  This
 *					is not a standard USB chapter 9 request, and 
 *					therefore not handled by usb_device.c.  Instead
 *					this request should be handled by class specific 
 *					firmware, such as that contained in usb_function_hid.c.
 *
 * Note:            None
 *****************************************************************************/
void USBCBCheckOtherReq(void)
{
}//end


/*******************************************************************
 * Function:        void USBCBStdSetDscHandler(void)
 *
 * Overview:        The USBCBStdSetDscHandler() callback function is
 *					called when a SETUP, bRequest: SET_DESCRIPTOR request
 *					arrives.  Typically SET_DESCRIPTOR requests are
 *					not used in most applications, and it is
 *					optional to support this type of request.
 *
 * Note:            None
 *****************************************************************************/
void USBCBStdSetDscHandler(void)
{
    // Must claim session ownership if supporting this request
}//end


/******************************************************************************
 * Function:        void USBCBInitEP(void)
 * Overview:        This function is called when the device becomes
 *                  initialized, which occurs after the host sends a
 * 					SET_CONFIGURATION (wValue not = 0) request.  This 
 *					callback function should initialize the endpoints 
 *					for the device's usage according to the current 
 *					configuration.
 *
 * Note:            None
 *****************************************************************************/
void USBCBInitEP(void)
{
    USBEnableEndpoint(USBGEN_EP_NUM,USB_OUT_ENABLED|USB_IN_ENABLED|USB_HANDSHAKE_ENABLED|USB_DISALLOW_SETUP);
    USBGenericOutHandle = USBGenRead(USBGEN_EP_NUM,(BYTE*)&OUTPacket,USBGEN_EP_SIZE);
}

/********************************************************************
 * Function:        void USBCBSendResume(void)
 *
 * Overview:        The USB specifications allow some types of USB
 * 					peripheral devices to wake up a host PC (such
 *					as if it is in a low power suspend to RAM state).
 *					This can be a very useful feature in some
 *					USB applications, such as an Infrared remote
 *					control	receiver.  If a user presses the "power"
 *					button on a remote control, it is nice that the
 *					IR receiver can detect this signalling, and then
 *					send a USB "command" to the PC to wake up.
 *					
 *					The USBCBSendResume() "callback" function is used
 *					to send this special USB signalling which wakes 
 *					up the PC.  This function may be called by
 *					application firmware to wake up the PC.  This
 *					function should only be called when:
 *					
 *					1.  The USB driver used on the host PC supports
 *						the remote wakeup capability.
 *					2.  The USB configuration descriptor indicates
 *						the device is remote wakeup capable in the
 *						bmAttributes field.
 *					3.  The USB host PC is currently sleeping,
 *						and has previously sent your device a SET 
 *						FEATURE setup packet which "armed" the
 *						remote wakeup capability.   
 *
 *					This callback should send a RESUME signal that
 *                  has the period of 1-15ms.
 *
 * Note:            Interrupt vs. Polling
 *                  -Primary clock
 *                  -Secondary clock ***** MAKE NOTES ABOUT THIS *******
 *                   > Can switch to primary first by calling USBCBWakeFromSuspend()
 
 *                  The modifiable section in this routine should be changed
 *                  to meet the application needs. Current implementation
 *                  temporary blocks other functions from executing for a
 *                  period of 1-13 ms depending on the core frequency.
 *
 *                  According to USB 2.0 specification section 7.1.7.7,
 *                  "The remote wakeup device must hold the resume signaling
 *                  for at lest 1 ms but for no more than 15 ms."
 *                  The idea here is to use a delay counter loop, using a
 *                  common value that would work over a wide range of core
 *                  frequencies.
 *                  That value selected is 1800. See table below:
 *                  ==========================================================
 *                  Core Freq(MHz)      MIP         RESUME Signal Period (ms)
 *                  ==========================================================
 *                      48              12          1.05
 *                       4              1           12.6
 *                  ==========================================================
 *                  * These timing could be incorrect when using code
 *                    optimization or extended instruction mode,
 *                    or when having other interrupts enabled.
 *                    Make sure to verify using the MPLAB SIM's Stopwatch
 *                    and verify the actual signal on an oscilloscope.
 *******************************************************************/
void USBCBSendResume(void)
{
    static WORD delay_count;
    
    USBResumeControl = 1;                // Start RESUME signaling
    
    delay_count = 1800U;                // Set RESUME line for 1-13 ms
    do
    {
        delay_count--;
    }while(delay_count);
    USBResumeControl = 0;
}


/*******************************************************************
 * Function:        BOOL USER_USB_CALLBACK_EVENT_HANDLER(
 *                        USB_EVENT event, void *pdata, WORD size)
 *
 * PreCondition:    None
 *
 * Input:           USB_EVENT event - the type of event
 *                  void *pdata - pointer to the event data
 *                  WORD size - size of the event data
 *
 * Output:          None
 *
 * Side Effects:    None
 *
 * Overview:        This function is called from the USB stack to
 *                  notify a user application that a USB event
 *                  occured.  This callback is in interrupt context
 *                  when the USB_INTERRUPT option is selected.
 *
 * Note:            None
 *******************************************************************/
BOOL USER_USB_CALLBACK_EVENT_HANDLER(USB_EVENT event, void *pdata, WORD size)
{
    switch(event)
    {
        case EVENT_CONFIGURED: 
            USBCBInitEP();
            break;
        case EVENT_SET_DESCRIPTOR:
            USBCBStdSetDscHandler();
            break;
        case EVENT_EP0_REQUEST:
            USBCBCheckOtherReq();
            break;
        case EVENT_SOF:
            USBCB_SOF_Handler();
            break;
        case EVENT_SUSPEND:
            USBCBSuspend();
            break;
        case EVENT_RESUME:
            USBCBWakeFromSuspend();
            break;
        case EVENT_BUS_ERROR:
            USBCBErrorHandler();
            break;
        case EVENT_TRANSFER:
            Nop();
            break;
        default:
            break;
    }      
    return TRUE; 
}
/** EOF main.c ***************************************************************/
