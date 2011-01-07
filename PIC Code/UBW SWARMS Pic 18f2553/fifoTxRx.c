//fifoTxRx.c
//C file for controlling buffering SWARMS commands
//created by Alexander List for Drexel DFL on 3.5.10

#include "fifoTxRx.h"
#include "p18f2553.h"
#include "HardwareProfile.h"
#include "delays.h"

#pragma udata gpr2
char commandInBuffer[fifoMaxInBufferSize];
char *headInputPointer = commandInBuffer; 
char *tailInputPointer  = commandInBuffer;

#pragma udata gpr3
char commandOutBuffer[fifoMaxOutBufferSize];
char *headOutputPointer = commandOutBuffer;
char *tailOutputPointer = commandOutBuffer;


//shift distance = head pointer - command buffer
//head pointer should always ends commands on a command byte
//*********************  input **************//
//reads top command in fifo
char * readInputCommand(void){
	if (headInputPointer == tailInputPointer){
		return 0; //no data
	}
	return headInputPointer;
}	

//pops head command in fifo
char popHeadInputCommand(void){
	if (headInputPointer == tailInputPointer){
		flushInputCommandBuffer();	
		return fifoBufferStatusEmpty;
	}
	
	headInputPointer = headInputPointer + 2 + *(headInputPointer +1); //cID.PLenByte.Paylnth	
	
	return fifoBufferStatusGood;
}	

//writes command to end of fifo buffer
char writeInputCommand(char *command){
	char iterator = 0;
	
	
	if( (*command) == 0 ) return fifoBufferStatusBadInput;
	
	if (((tailInputPointer - commandInBuffer)+ (2 + *(command + 1))) > fifoMaxInBufferSize){
		emptyShiftInputCommandBuffer();	
		if (((tailInputPointer - commandInBuffer)/*filled ammount*/ + (2 + *(command + 1))) > fifoMaxInBufferSize){
			return fifoBufferStatusFull;
		}						
	}

	if(*(command + 1) > fifoMaxCommandPayloadLength){ 
		return fifoBufferStatusBadInputOversize;
	}	
	
	//everything checks out-- go ahead and drop in the data
	*tailInputPointer = *command;
	tailInputPointer++;	
	
	*tailInputPointer = *(command + 1);
	tailInputPointer++;
	
	for(iterator = 0; iterator < *(command + 1); iterator++)//if there're payload bytes
	{
		*tailInputPointer = *(command + 2 + iterator);
		tailInputPointer++;	
	}
	    
	return fifoBufferStatusGood;
}	

//removes all data from fifo
char flushInputCommandBuffer(void){
	headInputPointer = commandInBuffer;
	tailInputPointer = commandInBuffer;
	
	return fifoBufferStatusEmpty;
}

//left (clear) shift FIFO
char emptyShiftInputCommandBuffer(void){
	char iterator = 0;
	char *dataMoveInputPointer  = headInputPointer;
	char shiftAmount = (char)(headInputPointer - commandInBuffer);
	char bytesToShift = (char)(tailInputPointer - headInputPointer);
	
	if (headInputPointer == tailInputPointer){
		flushInputCommandBuffer();	
		return fifoBufferStatusEmpty;
	}	 
	
	if (headInputPointer == commandInBuffer){	
		return fifoBufferStatusGood;
	}	 

	if (shiftAmount == 0) return fifoBufferStatusGood;
	
	//iffy statement
	headInputPointer = headInputPointer - bytesToShift;
	tailInputPointer = tailInputPointer - bytesToShift;
	
	for (iterator = 0; iterator < bytesToShift; iterator++){
		*(dataMoveInputPointer - shiftAmount) = *(dataMoveInputPointer);
		dataMoveInputPointer++;	
		
				if(iterator > 5){
						while (1){
						mLED_1_On();
						mLED_2_Off();
						Delay10KTCYx(1);
						mLED_2_On();
						mLED_1_Off();
						Delay10KTCYx(1);
						mLED_2_On();
						mLED_2_On();
						Delay10KTCYx(1);
						}
					}	
	}	
	
	return fifoBufferStatusGood;
}

//********************* output **************//
//reads top command in fifo
char * readOutputCommand(void){
	if (headOutputPointer == tailOutputPointer){
		return 0; //no data
	}
	return headOutputPointer;
}

//pops head command in fifo
char popHeadOutputCommand(void){
		if (headOutputPointer == tailOutputPointer){
		flushOutputCommandBuffer();	
		return fifoBufferStatusEmpty;
	}
	
	headOutputPointer = headOutputPointer + 2 + *(headOutputPointer +1); //cID.PLenByte.Paylnth	
	
	return fifoBufferStatusGood;
}	

//writes command to end of fifo buffer
char writeOutputCommand(char *command){
	char iterator = 0;
	
	if(!1 == 1/*isValidCommand*/) return fifoBufferStatusBadInput;
	
	if ((tailOutputPointer - commandOutBuffer)/*filled ammount*/ + (2 + *(command + 1)) > fifoMaxOutBufferSize){
		emptyShiftOutputCommandBuffer();	
		if ((tailOutputPointer - commandOutBuffer)/*filled ammount*/ + (2 + *(command + 1)) > fifoMaxOutBufferSize){
			return fifoBufferStatusFull;
		}						
	}
	
	if(*(command + 1) > fifoMaxCommandPayloadLength){ 
		return fifoBufferStatusBadInputOversize;
	}	
	
	//everything checks out-- go ahead and drop in the data
	*tailOutputPointer = *command;
	tailOutputPointer++;	
	
	*tailOutputPointer = *(command + 1);
	tailOutputPointer++;
	
	for(iterator = 0; iterator < *(command + 1); iterator++)//if there're payload bytes
	{	
		*tailOutputPointer = *(command + 2 + iterator);
		tailOutputPointer++;	
	}	
	return fifoBufferStatusGood;
}

//removes all data from fifo
char flushOutputCommandBuffer(void){
	headOutputPointer = commandOutBuffer;
	tailOutputPointer = commandOutBuffer;
	
	return fifoBufferStatusEmpty;	
}

//left (clear) shift FIFO
char emptyShiftOutputCommandBuffer(void){
	char *dataMoveOutputPointer = commandOutBuffer;
	char iterator = 0;
	char shiftAmount = (char)(headOutputPointer - commandOutBuffer);
	char bytesToShift = (char)(tailOutputPointer - headOutputPointer);
	
	if (headOutputPointer == tailOutputPointer){
		flushOutputCommandBuffer();	
		return fifoBufferStatusEmpty;
	}	 
	
	if (headOutputPointer == commandOutBuffer){	
		return fifoBufferStatusGood;
	}	 

	if (shiftAmount == 0) return fifoBufferStatusGood;
	
	//iffy statement
	headOutputPointer = headOutputPointer - bytesToShift;
	tailOutputPointer = tailOutputPointer - bytesToShift;
	
	for (iterator = 0; iterator < bytesToShift; iterator++){
		*(dataMoveOutputPointer - shiftAmount) = *(dataMoveOutputPointer);
		dataMoveOutputPointer++;	
	}	
	
	return fifoBufferStatusGood;
}


char canfitCommandInOutputCommandBuffer(char length){
	if ((tailOutputPointer - commandOutBuffer)/*filled ammount*/ + (length) > fifoMaxOutBufferSize){
		emptyShiftOutputCommandBuffer();	
		if ((tailOutputPointer - commandOutBuffer)/*filled ammount*/ + (length) > fifoMaxOutBufferSize){
			return 0;
		}						
	}
	return 1;
}	