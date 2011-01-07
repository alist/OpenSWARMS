//fifoTxRx.h
//header file for controlling buffering SWARMS commands
//created by Alexander List for Drexel DFL on 3.5.10

#ifndef FIFOTXRX_H
#define FIFOTXRX_H

#define fifoMaxInBufferSize 128
#define fifoMaxOutBufferSize 128

#define fifoMaxCommandPayloadLength 64

typedef enum fifoBufferStatus{
	fifoBufferStatusGood,
	fifoBufferStatusFull,
	fifoBufferStatusEmpty,
	fifoBufferStatusBadInput,
	fifoBufferStatusBadInputOversize
}fifoBufferStatus;

//*********************  input **************//
//reads top command in fifo
char * readInputCommand(void);

//pops command in fifo
char popHeadInputCommand(void);

//writes command to end of fifo buffer
//devices should validate commandID first
char writeInputCommand(char *command);

//removes all data from fifo
char flushInputCommandBuffer(void);

//left (clear) shift FIFO
char emptyShiftInputCommandBuffer(void);

//********************* output **************//
//reads top command in fifo
char * readOutputCommand(void);

//pops command in fifo
char popHeadOutputCommand(void);

//writes command to end of fifo buffer
char writeOutputCommand(char *command);

//removes all data from fifo
char flushOutputCommandBuffer(void);

//left (clear) shift FIFO
char emptyShiftOutputCommandBuffer(void);

char canfitCommandInOutputCommandBuffer(char length);

#endif