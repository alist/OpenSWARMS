/*********************************************************
** All rights for this code have been reserved by their appropiate owners
** Creator and affiliated organizations have been removed to comply with
** competition regulations
** C 2008 - 2009
*///******************************************************

#include <p18f2620.h>
#include <delays.h>
#include <string.h>
#include <adc.h>
#include <pwm.h>
#include <EEP.h>
#include <usart.h>
#include "mainconfig.h"
#include "functionheaders.h"


//setup
void setup_ports(void);
void setup_device(void);
void setup_interrupts(void);
void setup_peripherals(void);
void activate_peripherals(void);
void initiate_services(void);
void InterruptHandlerHigh (void);

//test
void TestSendUSART(void);
void test_drive(void);

//procedures
int WaitForACK (void);
int set_servo_pos(char position); //sets servo location where 48 is centered >49 is right and <48 is left : returns 1 if number is out of bounds --return 0 if success
int set_drive_speed(char speed);  //sets motor speed where 46 is neutral and > 48 is forward and <44 is back returns 1 if number is out of bounds --return 0 if success	
void SendUSART(char byte);
void SendUSART_string (char bytes[], int size);
void input_router(char in_byte); //move the input to where it needs to be handled
void input_handler(char in_byte); 
int Safe_WriteEEP(char bite_address, char bite_data); //1=success 0=fail wraps the read & write EEPs to make sure the data gets written
void Setup_Controller_Initiation(void); //setup the device for generic use
void Verbose_Mode(char ext_command); //enter textual mode
void config_utility_mode(char ext_command); //enter config mode
void TMR0Interrupt (void);
void TMR2Interrupt(void); //system tiner interrupt
void setupNextInput (void);//sets the adcon reg to be ready for next loop
void setup_mode(char ext_command);
void send_usart_text(int textId);
void toggle_settings(char letter1ofname);
void retrieve_eep_settings(void);//setup run settings
void handle_int0 (void);
void self_diag_Test (void); //self system test


char ADC_Convert_channel_8(void);
char ADC_Convert_channel_10(void);
char ADC_Convert_channel_11(void);

char get_sensor_data(char sensor); // returns current sensor value
char set_output(char output, int boolon); //sets output and returns new value
void set_number_digital_outputs(int asDigital); //go by number pin # 0,1,2,3 -- 2 sets pin 21, 22 digital -- zero sets all analog


//Omnicient variable
int current_outputs[2] = {0};
int systemIncrement = 0;
int nextAdcon = 1; //1-5 selects the next adcon to start getting


//encoder
int encoderHitsSinceLastCheck = 0;
int wheelRoundsPerCheck = 0;
int systemIsAutoSpeedCorrectingAtLevel = 0; //One if yes
int encoderLastValue = 1; 

int lastSensorAN3[10] = {0};
int lastSensorAN8[10] = {0};
int lastSensorAN9[10] = {0};
int lastSensorAN10[10] = {0};
int lastSensorAN11[10] = {0};
int lastValueSetRow = 0;
void updateSensorValueSet(int anNumber, int value);
int getSensorAverage(int Sensor);

char recieve_single = 0;
char last_address = 0x00;// the spec works by addressing a peripheral 
int error_state = 0; //if 1 -- system displays red led and stops working
int stopDriveOnINT0InterruptIsWaiting = 1; // if it is set to 1 then it is waiting until the timout timer can set it to zero

//motor definitions
int motor_max_f = 59;
int motor_min_b = 44;
int servo_max_r = 62;
int servo_min_l = 38;
int motor_current = 50;
int servo_current = 50;

//for eep
char read_char = 0;
char write_true_char = 'T';
char write_false_char = 'F';
int write_location = 0;


//modes
int verbose_mode_run = 0; //controls where data is routed from the usart 1= enabled
int config_utility_run = 0; //1 for enabled // non textual
char get_or_set = 'G';

int timeouts_enabled = 1; //1 for enabled of auto stop
int stopDriveOnINT0Interrupt = 0; //1 is yes
int manage_speed_mode_enabled = 0; //1 for enabled
int debug_mode_enabled = 1; //1 for enabled
int say_all_mode_enabled = 0; //dumps data while debugging
int controller_setup_mode_run = 0; //controls where data is routed from the usart 1= enabled

const far rom char system_info_text[103]= "\n\rSystem Info:\n\r8 Mghz\n\r19.2 k baud rate Serial\n\rPIC18F2620 Controller\n\rWireless Availible\n\r";
const far rom char reset_eep_setting_text[45]= "\n\rReset a setting due to an invalid value!\n\r";
const far rom char quit_text[14] = "\nResetting!\n\r";   
const far rom char author_version[39] = "\n\rVr:2.03 OBSC -- Alexander List ECE\n"; //name/ version/date  
const far rom char unknown_string[21] = "\n\rUnknown Command!\n";
const far rom char settings_menu_text[69] = "\n\rToggle Settings:\n\r1:Debug\n\r2:Chatty\n\r3:Timeouts\r\nQ:Quit\n";
const far rom char verbose_menu_text[77] = "\n\rVerbose:\n\r1:Setup\n\r2:Self Diagnostics\n\r3:System Info\n\r4:Design\n\rQ:Quit\n";
const far rom char sayall_mode_set_text[20] = "\n\rChatty Mode Set: ";
const far rom char debug_mode_set_text[19] = "\n\rDebug Mode Set: ";
const far rom char timeouts_set_text[23] = "\n\rMotor Timeouts Set: ";
const far rom char Ckey_continue_text[31] = "\n\rPress 'C' key to continue:\n\r";
const far rom char testing_info_text[130] = "\n\rTesting material in this order to recieve confirmations:\n\rMotor Forward/Reverse:\n\rServo Left/Right:\n\rSerial Port:\n\rLeds:\n\rEEP\n\r";


void main (void)
{
Setup_int_osc_2mhz;
	
	setup_device();
	initiate_services();
	green_on; //run light on
	SendUSART('R'); //ready

	  while (1)
	{
		green_on; //run light on
		Delay10KTCYx(20);
		while (error_state == 1)
		{
			red_on;
		}
		green_off;
		yellow_off;
		red_off;
		Delay10KTCYx(80);
	}

}


void TestSendUSART()
{
	int testdata;
	for (testdata = 1; testdata <255; testdata++)
	{
		SendUSART(testdata);

	}
}
void test_drive()
{
		yellow_on;
		forward_slow_motor_pwm;
		Delay10KTCYx(20);
		set_servo_pos(58);
		yellow_off;
		neutral_motor_pwm;
		Delay10KTCYx(150);
		set_servo_pos(36);
		yellow_on;
		forward_medium_motor_pwm;
		Delay10KTCYx(20);
		yellow_off;
		neutral_motor_pwm;
		neutral_servo_pwm;
		Delay10KTCYx(150);
}

void input_router(char in_byte)
{
			
	if (verbose_mode_run == 1)
	{
		Verbose_Mode(in_byte);
	}
	else if (controller_setup_mode_run == 1)
	{
		setup_mode(in_byte);
	}
	else if (config_utility_run == 1)
	{
		config_utility_mode(in_byte);
	}
		
	else
		{
			input_handler(in_byte);
		}

}



void input_handler(char in_byte)
{
	reset_timeouts;
	if (last_address != 0x00)
		{
			if 	(last_address == '1') //servo addr.
				{
					if (set_servo_pos(in_byte) == 1)
					{
						USART_ACK_BD; //bad data!
					}
					else
					{
						USART_ACK;
					}	
				}
			else if (last_address == '2') //motor add
				{
					if (set_drive_speed(in_byte) == 1)
					{
						USART_ACK_BD;
					}
					else
					{
						USART_ACK;
					}	
				}
			else if (last_address == '3') //demo
				{
					if (in_byte == '1')
					{
						USART_ACK;
						test_drive();
						
					}
					else if (in_byte == '2')
					{
						TestSendUSART();
					}
					else
					{
						USART_ACK_BD;
					}	
				}
			else if (last_address == '4') //sensor polls
				{
					if (in_byte == '1') //sensor 1
					{
						SendUSART(get_sensor_data(1));
					}
					else if (in_byte == '2') //sensor 2
					{
						SendUSART(get_sensor_data(2));
					//	SendUSART(ADC_Convert_channel_10());
					}
				 	else if (in_byte == '3')//sensor 3
					{
						SendUSART(get_sensor_data(3));
					//		SendUSART(ADC_Convert_channel_8());
					}
					else if (in_byte == '4')//sensor 4
					{
						SendUSART(get_sensor_data(4));	
					}
					else if (in_byte == '5')//sensor 5
					{
						SendUSART(get_sensor_data(5));	
					}
					else if (in_byte == '6')//sensor 6
					{
						SendUSART(get_sensor_data(6));	
					}
					else if (in_byte == '7')//sensor bank 7 -- ENCODER
					{
						SendUSART(wheelRoundsPerCheck);
					}	
					else
					{
						USART_ACK_BD;
					}	
				}
			else if (last_address == '5')
				{
					if (in_byte == '1') //output1
					{
						if (current_outputs[0] == 0)
						{		
							current_outputs[0] = 1;
							set_output(1, 1); //#1 on
							SendUSART('P');
						}
						else
						{
							current_outputs[0] = 0;
							set_output(1, 0); //#1 off
							SendUSART('F');
						}
					}
					else if (in_byte == '2')  //output 2
					{
						if (current_outputs[1] == 0)
						{		
							current_outputs[1] = 1;
							set_output(2, 1); //#2 on
							SendUSART('P');
						}
						else
						{
							current_outputs[1] = 0;
							set_output(2, 0); //#2 off
							SendUSART('F');
						}
					}
					else
					{
						USART_ACK_BD;
					}	
				}
			else if (last_address == 'C')  //config
				{
					if (in_byte == 'N') //config verbose
					{
						Verbose_Mode(0x00);
						//verbose mode
					}
					else if (in_byte == 'U') //config utility
					{
						config_utility_mode(0x00); //enter config mode
					}
					else
					USART_ACK_BD;		
				}

			clear_last_addr;
		}
	else
	{
		if (in_byte == 'S') //emerg_stop
		{
			stop_now;
			error_state = 1;
			USART_ACK;	
		}
		else if (in_byte == 'A') //ack ack!
		{
			yellow_on;
			USART_ACK;	
		}
		else if (in_byte == 'R') //reset
		{
			if (debug_mode_enabled == 1)
			{
				send_usart_text(3);
			}
			USART_ACK;	
			stop_now;
			_asm reset	_endasm	
		}
		else if (in_byte == 'N') //neutral -- different than stop -- doesnt disable
		{
			neutral_motor_pwm;
			neutral_servo_pwm;
			USART_ACK;	
		}
		else if(in_byte == '1' || in_byte == '2' || in_byte == '3' || in_byte == '4' || in_byte == 'C' || (in_byte == '5'))
			{
				USART_ACK;
				last_address = in_byte;
			}
		else //not a function -- bad data
		{	
			if (debug_mode_enabled == 1)
			{
				SendUSART(in_byte);
			}
				else
				{
				USART_ACK_BD;
			}
			red_on;
			return;
		}
	}
}


int Safe_WriteEEP(char bite_address, char bite_data)
{
//1=success 0=fail wraps the read & write EEP.h functions to make sure the data gets written
	char testing_char;
	Busy_eep ();
	Write_b_eep(bite_address, bite_data);
	Busy_eep ();
	testing_char = Read_b_eep(bite_address);
	if (bite_data == testing_char)
	{
		return 1;//good data
	}
	else
	{
		Busy_eep ();
		Write_b_eep(bite_address, bite_data);
		Busy_eep ();
		testing_char = Read_b_eep(bite_address);
		if (bite_data == testing_char)
		{
			return 1;//good data
		}
	}
	
	return 0;
}
void Setup_Controller_Initiation(void)
{
	//setup the device for generic use
	for (write_location = 10; write_location <= 13; write_location++)
	{
		Safe_WriteEEP(write_location, write_false_char);
	}
	send_usart_text(6);
	return;
}

void toggle_settings(char letterOFname) //toggle the settings with the 1 letter abr. auto string outputs
{


	switch (letterOFname)
	{
		case 'T'://talky
		EEP_Say_all_setup;
		read_char = Read_b_eep(write_location);
		send_usart_text(10);
		if (read_char == write_true_char)
		{
			//set false	
			Safe_WriteEEP(write_location, write_false_char);
			SendUSART(write_false_char);
		}
		else if (read_char == write_false_char)
		{
			//set true
			Safe_WriteEEP(write_location, write_true_char);
			SendUSART(write_true_char);
		}
		else
		{
			Setup_Controller_Initiation();
		}
		break;
		
		case 'O': //timeouts
		EEP_Timeouts_setup;	
		read_char = Read_b_eep(write_location);
		send_usart_text(12);
		if (read_char == write_true_char)
		{
			//set false	
			Safe_WriteEEP(write_location, write_false_char);
			SendUSART(write_false_char);
		}
		else
		{
			//set true
			Safe_WriteEEP(write_location, write_true_char);
			SendUSART(write_true_char);
		}	
		break;
		case 'D': 	//debug mode
		EEP_Debug_mode_setup;
		read_char = Read_b_eep(write_location);
		send_usart_text(11);
		if (read_char == write_true_char)
		{
			//set false	
			Safe_WriteEEP(write_location, write_false_char);
			SendUSART(write_false_char);
		}
		else
		{
			//set true
			Safe_WriteEEP(write_location, write_true_char);
			SendUSART(write_true_char);
		}
		break;
		default:
	
		break;
	}

	return;

}

void setup_mode(char ext_command)
{
	if (last_address != 0)
	{

		clear_last_addr;
		send_usart_text(5); 
		return;
	}

	if (ext_command == 0)
	{
			send_usart_text(5);
			neutral_motor_pwm;
			neutral_servo_pwm;
			return;
	}	
	switch(ext_command)
	{
		case '1':
			toggle_settings('D');
			//eeprom slot 1
			//debug mode
			break;
		case '2':
			toggle_settings('T');
			//slot 2
			//say all -- chatty mode
			break;
		case '3':
			toggle_settings('O');
			//slot 3
			//drive timeouts
			break;
		case 'Q':
		case 'q':
		case 'R':
		case 'r':
		send_usart_text(3);
			stop_now;
			_asm reset	_endasm	
			return;
			break;
		default:	
			send_usart_text(4);
			break;
	}
		send_usart_text(5); 
	return;


}

void config_utility_mode(char ext_command) //enter config mode -- consol
{

	if (ext_command == 'G')
	{
		USART_ACK;
		get_or_set = 'G';
		return;
	}
	else if (ext_command == 'S')
	{
		USART_ACK;
		get_or_set = 'S';
		return;
	}
	else if (ext_command == 'R')
	{
		stop_now;
		_asm reset	_endasm	
		return;
	}

	if (last_address == 0 && ext_command != 0 && get_or_set == 'S')
	{
		USART_ACK;
 		last_address = ext_command;
		return;
	}

	if (ext_command == 0)
	{
			config_utility_run = 1;
			USART_ACK;
			clear_last_addr;
			neutral_motor_pwm;
			neutral_servo_pwm;
			return;
	}	
	
	if(get_or_set == 'G')
		switch(ext_command)
	{
		case '1':
			EEP_Debug_mode_setup;
			read_char = Read_b_eep(write_location);
			SendUSART(read_char);
			//debug mode
			break;
		case '2':
			EEP_Say_all_setup;
			read_char = Read_b_eep(write_location);
			SendUSART(read_char);
			//say all -- chatty mode
			break;
		case '3':
			EEP_Timeouts_setup;
			read_char = Read_b_eep(write_location);
			SendUSART(read_char);
			//drive timeouts
			break;
		case '4':
		//** Archaic -- does nothing!
			EEP_NumberPinsDigital_setup;
			read_char = Read_b_eep(write_location);
			SendUSART(read_char);
			break;
		case '5':
			EEP_Max_Motor_speed;
			read_char = Read_b_eep(write_location);
			SendUSART(read_char);
			break;
		case '6':
			EEP_Min_Motor_speed;
			read_char = Read_b_eep(write_location);
			SendUSART(read_char);
			break;
		case '7':
			EEP_Stop_inp1_setup;
			read_char = Read_b_eep(write_location);
			SendUSART(read_char);
			break;
		case '8':
			EEP_Manage_Speed_setup;
			read_char = Read_b_eep(write_location);
			SendUSART(read_char);
			break;
		default:
			USART_ACK_BD;	
			break;
	}
	if(get_or_set == 'S')
		switch(last_address)
	{
		case '1':
			EEP_Debug_mode_setup;
			Safe_WriteEEP(write_location, ext_command);
			USART_ACK;
			//debug mode
			break;
		case '2':
			EEP_Say_all_setup;
			Safe_WriteEEP(write_location, ext_command);
			USART_ACK;
			//say all -- chatty mode
			break;
		case '3':
			EEP_Timeouts_setup;
			Safe_WriteEEP(write_location, ext_command);
			USART_ACK;
			//drive timeouts
			break;
		case '4':
			EEP_NumberPinsDigital_setup;
			Safe_WriteEEP(write_location, ext_command);
			USART_ACK;
			//ADC number of digital pins
			break;
		case '5':
			EEP_Max_Motor_speed;
			Safe_WriteEEP(write_location, ext_command);
			USART_ACK;
			//max motor speed (unchanged)
			break;
		case '6':
			EEP_Max_Motor_speed;
			Safe_WriteEEP(write_location, ext_command);
			USART_ACK;
			//Maximum reverse speed -- min total
			break;
		case '7':
			EEP_Stop_inp1_setup;
			Safe_WriteEEP(write_location, ext_command);
			USART_ACK;
			break;
		case '8':
			EEP_Manage_Speed_setup;
			Safe_WriteEEP(write_location, ext_command);
			USART_ACK;
			break;
		default:
			USART_ACK_BD;	
			break;
	}
	
	if (last_address != ext_command)
	{
		clear_last_addr;
	}
	
	return;

}

void Verbose_Mode(char ext_command)
{
	if (last_address != 0 && ext_command != 0)
	{
		/*if (last_address == '2')
		{
			if (ext_command == 'C')
			{
				send_usart_text(20);//Begin
				Delay10KTCYx(254);
				self_diag_Test ();
			}
		}
		*/
		clear_last_addr;
		send_usart_text(2); 
		return;
	}
	if (ext_command == 0)
	{
			send_usart_text(2);
			verbose_mode_run = 1;
			Stop_Timer0;
			neutral_motor_pwm;
			neutral_servo_pwm;
			return;
	}	
		
	switch(ext_command)
	{
		case '1':
			controller_setup_mode_run = 1;
			verbose_mode_run = 0;
			setup_mode(0);
			break;
		case '2':	//self test
			self_diag_Test ();
			break;
		case '3': //system info
			send_usart_text(7);
			break;
		case '4'://design
			send_usart_text(1);
			break;
		case 'Q':
		case 'q':
		case 'R':
		case 'r':
			send_usart_text(3);
			stop_now;
			_asm reset	_endasm	
			return;
			break;
		default:
			send_usart_text(4);
			break;	
	}
	if (verbose_mode_run  == 1){ send_usart_text(2); } //give main menu, system doesnt live update
	return;
} //enter textual mode

void self_diag_Test (void)
{
		Gbl_Ints_DISABLED;
		Busy_eep ();

		forward_slow_motor_pwm;
		SendUSART('F');
		Delay10KTCYx(100);
		backward_motor_pwm;
		SendUSART('B');
		Delay10KTCYx(100);
		Busy_eep ();
		set_servo_pos(62);
		SendUSART('R');	
		Delay10KTCYx(100);
		Busy_eep ();
		set_servo_pos(36);
		SendUSART('L');
		Delay10KTCYx(100);
		Busy_eep ();
		TestSendUSART();
		SendUSART('S');
		Delay10KTCYx(100);
		Busy_eep ();

		green_on;
		yellow_on;
		red_on;
		SendUSART('L');
		Delay10KTCYx(254);

		if(Safe_WriteEEP(1, write_true_char) == 1)
		{
				SendUSART('E');
				Delay10KTCYx(254);
		}

		send_usart_text(3);
		stop_now;
			_asm reset	_endasm	
}

void SendUSART(char byte)
{
	yellow_on;

		while(PIR1bits.TXIF != 1)//while the previous send is not clear
		{};
		TXREG = byte;


	yellow_off;
}


void SendUSART_string (char bytes[], int size)
{
	int loop_int = 0;
	for (loop_int = 0; loop_int < size; loop_int++)
	{
		SendUSART(bytes[loop_int]);
	}
	return;
}
int set_servo_pos(char position)
{ 	
	servo_current = position;
	if (position > servo_max_r  || position < servo_min_l)
	{
		neutral_servo_pwm;//48
		red_on;
		return 1;
	}
	else
	{
		input_servo_fix;
		CCPR2L = position;
		return 0;
	}
}


int set_drive_speed(char speed)
{
	motor_current = speed;
	if (speed > motor_max_f  || speed < motor_min_b )
	{
		neutral_motor_pwm;//46
		red_on;
		return 1; //bad input
	}
	else
	{
		input_speed_fix;
		CCPR1L = speed;
		return 0; //good input
	}
}
void setup_device()
{
	setup_ports();
	setup_peripherals();
	retrieve_eep_settings();
	setup_interrupts();
}

char ADC_Convert_channel_8(void)
{
	int result;
	ADCON1 = 0b00000110; //setup analog/digital
	ADCON0 = 0b00100000; //1000 = Channel 8 (AN8)
	
	ADCON2 = 0b10101100; //1 = Right justified Unimplemented: Read as ‘0’ 101 = 12 TAD 100 = FOSC/4
	ADCON0bits.ADON = 1; //1 = A/D Converter module is enabled
	Delay10TCYx(30); // Delay for 50TCY
	ADCON0bits.GO_DONE = 1; //start conversion
	
	while(ADCON0bits.GO_DONE != 0) //wait for finnish
	{
		Delay1TCY(); //nop
	}

	result = ADRESL; //:ADRESL; // Read result
	ADCON0bits.ADON = 0; //1 = A/D Converter module is enabled
	return result;

}
char ADC_Convert_channel_10(void)
{
	int result;
	ADCON1 = 0b00000100; //setup analog/digital
	ADCON0 = 0b00101000; //1010 = Channel 10 (AN10)
	
	ADCON2 = 0b10101100; //1 = Right justified Unimplemented: Read as ‘0’ 101 = 12 TAD 100 = FOSC/4
	ADCON0bits.ADON = 1; //1 = A/D Converter module is enabled
	Delay10TCYx(30); // Delay for 50TCY
	ADCON0bits.GO_DONE = 1; //start conversion
	
	while(ADCON0bits.GO_DONE != 0) //wait for finnish
	{
		Delay1TCY(); //nop
	}

	result = ADRESL; //:ADRESL; // Read result
	ADCON0bits.ADON = 0; //1 = A/D Converter module is enabled
	return result;

}

char ADC_Convert_channel_11(void)
{
	int result;
	ADCON1 = 0b00000011; //setup analog/digital
	ADCON0 = 0b00101100; //1011 = Channel 11 (AN11)
	
	ADCON2 = 0b10101100; //1 = Right justified Unimplemented: Read as ‘0’ 101 = 12 TAD 100 = FOSC/4
	ADCON0bits.ADON = 1; //1 = A/D Converter module is enabled
	Delay10TCYx(15); // Delay for 50TCY
	ADCON0bits.GO_DONE = 1; //start conversion
	
	while(ADCON0bits.GO_DONE != 0) //wait for finnish
	{
		Delay1TCY(); //nop
	}

	result = ADRESL; //:ADRESL; // Read result
	ADCON0bits.ADON = 0; //1 = A/D Converter module is enabled
	return result;

}


char get_sensor_data(char sensor){
	char return_char = 255;

	switch(sensor)
	{
		case 1: //rb0
				return PORTBbits.RB0;		
		break;
		case 2: // rb1
				return	 getSensorAverage(10);//ADC_Convert_channel_10();
		break;
		case 3: //rb2
				return	getSensorAverage(8);//ADC_Convert_channel_8();
		break;
		case 4: //rb4
				return	getSensorAverage(11);//	ADC_Convert_channel_11();
		break;
		break;
		case 5: //RA3
				return	getSensorAverage(3);
		break;
				break;
		case 6: //rb3
				return	getSensorAverage(9);
		break;
		default:

		break;
	}
	return return_char;
}
char set_output(char output, int boolon){
	switch(output)
	{
		case 1:
			//output1 = boolon
			PORTAbits.RA4 = boolon;
		break;
		case 2:
			PORTAbits.RA5 = boolon;
		break;
		default:
			red_on;
			return 2;
		break;
	}
	return boolon;
}

void handle_int0 (void)
{		
		if (stopDriveOnINT0Interrupt == 1 && get_sensor_data(1) == 0)
		{
			if (timeouts_enabled == 0 || stopDriveOnINT0InterruptIsWaiting == 0) //if it is PRESSED and it is not waiting to refresh set
			{	
				
					yellow_on;
					neutral_motor_pwm;
				if (timeouts_enabled == 1){ //be sure it will refresh
					stopDriveOnINT0InterruptIsWaiting = 1; //wait until refreshed
				}
			
				if(debug_mode_enabled == 1 && say_all_mode_enabled == 1)
				{
					char timer_debug[37] = "\n\rThe input 1 motor stop occurred\n\r";
					SendUSART_string (timer_debug, 37);
				}
			}
		}
}

void retrieve_eep_settings()
{
	Busy_eep ();	
	EEP_Timeouts_setup;	
	read_char = Read_b_eep(write_location);
	if (read_char == write_true_char)
	{
		timeouts_enabled = 1;
	}
	else if (read_char == write_false_char)
	{
		timeouts_enabled = 0;	
		Stop_Timer0;
	}
	else //reinitialize values
	{
		Busy_eep ();
		Safe_WriteEEP(write_location, write_true_char);
		timeouts_enabled = 1;
		send_usart_text(6);
	}
	Busy_eep ();
	EEP_Say_all_setup;	
	read_char = Read_b_eep(write_location);
	if (read_char == write_true_char)
	{
		say_all_mode_enabled = 1;
	}
	else if (read_char == write_false_char)
	{
		say_all_mode_enabled = 0;
	}
	else//reinitialize values 
	{
		Busy_eep ();
		Safe_WriteEEP(write_location, write_false_char);
		say_all_mode_enabled = 0;
		send_usart_text(6);
	}
	Busy_eep ();
	EEP_Debug_mode_setup;	
	read_char = Read_b_eep(write_location);
	if (read_char == write_true_char)
	{
		debug_mode_enabled = 1;
	}	
	else if (read_char == write_false_char)
	{
			debug_mode_enabled = 0;
	}
	else //reinitialize values 
	{
		Busy_eep ();
		Safe_WriteEEP(write_location, write_false_char);
		debug_mode_enabled = 0;
		send_usart_text(6);
	}


	Busy_eep ();
	EEP_Stop_inp1_setup;
	read_char = Read_b_eep(write_location);
	if (read_char == write_true_char)
	{
		stopDriveOnINT0Interrupt = 1;
	}	
	else if (read_char == write_false_char)
	{
		stopDriveOnINT0Interrupt = 0;
	}
	else //reinitialize values 
	{
		Busy_eep ();
		Safe_WriteEEP(write_location, write_false_char);
		stopDriveOnINT0Interrupt = 0;
		send_usart_text(6);
	}



	Busy_eep ();
	EEP_Manage_Speed_setup;
	read_char = Read_b_eep(write_location);
	if (read_char == write_true_char)
	{
		manage_speed_mode_enabled = 1;
	}	
	else if (read_char == write_false_char)
	{
		manage_speed_mode_enabled = 0;
	}
	else //reinitialize values 
	{
		Busy_eep ();
		Safe_WriteEEP(write_location, write_false_char);
		manage_speed_mode_enabled = 0;
		send_usart_text(6);
	}


	Busy_eep ();
	EEP_Max_Motor_speed write_location;
	read_char = Read_b_eep(write_location);
	if (read_char > 20 && read_char < 80)
	{
		motor_max_f = read_char;
	}	
	else //reinitialize values 
	{
		Busy_eep ();
		Safe_WriteEEP(write_location, motor_max_f); //value already set so just get the defaults ...
		send_usart_text(6);
	}

	Busy_eep ();
	EEP_Min_Motor_speed write_location;
	read_char = Read_b_eep(write_location);
	if (read_char > 20 && read_char < 80)
	{
		motor_min_b = read_char;
	}	
	else //reinitialize values 
	{
		Busy_eep ();
		Safe_WriteEEP(write_location, motor_min_b);
		send_usart_text(6);
	}


	return;
}

void TMR0Interrupt ()
{
	if (timeouts_enabled == 0)
		return;


	TMR0L = 0x00;
	TMR0L = 0x00;
	if (timeouts_enabled == 1)
	{
		neutral_motor_pwm; 
		if(debug_mode_enabled == 1 && say_all_mode_enabled == 1)
		{
			char timer_debug[33] = "\n\rThe timeout has triggered\n\r";
			SendUSART_string (timer_debug, 33);
		}
	}

	if (stopDriveOnINT0Interrupt == 1)
	{
		stopDriveOnINT0InterruptIsWaiting = 0; //so that it doesn't overrun the stop and keep going nuts!
	}	

}

void TMR2Interrupt(void) //system interrupt
{
int result = 0;
	systemIncrement ++;

if (get_sensor_data(1) != encoderLastValue && systemIncrement % 1 == 0 && manage_speed_mode_enabled == 1)
{ 
		encoderLastValue = !encoderLastValue; 
		encoderHitsSinceLastCheck ++;
}

if (systemIncrement % 80 == 0 && manage_speed_mode_enabled == 1)
{
	wheelRoundsPerCheck =  encoderHitsSinceLastCheck / 4;
	encoderHitsSinceLastCheck = 0;	

	if (motor_current == 52 && wheelRoundsPerCheck == 0 && systemIsAutoSpeedCorrectingAtLevel == 0) // auto correct
	{
		systemIsAutoSpeedCorrectingAtLevel = 1;
		set_drive_speed(53);
		set_output(1, 1); //#1 on	
	}
	else if (motor_current == 53 && systemIsAutoSpeedCorrectingAtLevel == 1 && wheelRoundsPerCheck == 0)
	{
		systemIsAutoSpeedCorrectingAtLevel = 2;
		set_drive_speed(54);
	}	
	else if (systemIsAutoSpeedCorrectingAtLevel > 0 && (motor_current == 53 || motor_current == 54)) //if the system is currently auto corrcting and the speed is 54 or 53 -- to make sure the drive wasn't changed
	{ //corrected
		systemIsAutoSpeedCorrectingAtLevel = 0;
		set_drive_speed(52);
		set_output(1, 0); //#1 off
	}
	else //auto stop correcting due to system change
	{
		systemIsAutoSpeedCorrectingAtLevel = 0;
		set_output(1, 0); //#1 off
	}	
	
	
	if (debug_mode_enabled == 1 && say_all_mode_enabled == 1)
	{
		if (wheelRoundsPerCheck == 0 )
		{
			SendUSART('S');
		}
		else
		{
			SendUSART('G');
		}		
	}	
}
	

	if (systemIncrement % 1 == 0)
	{
		ADCON0bits.GO_DONE = 1; //start conversion
		
		while(ADCON0bits.GO_DONE != 0) //wait for finnish
		{
			TMR1L = 0x00;
		}

		result = ADRESH; //:ADRESH; // Read high register result
		
		switch(nextAdcon)
		{
			case 1: //AN8
			updateSensorValueSet(8, result);;		
			break;
			case 2://AN10
				updateSensorValueSet(10, result);	
			break;
			case 3://AN11
				updateSensorValueSet(11, result);		
			break;
			case 4://AN9
				updateSensorValueSet(9, result);	
			break;			
			case 5://AN3
				updateSensorValueSet(3, result);		
			break;
		}
		ADCON0bits.ADON = 0; //1 = A/D Converter module is enabled
		nextAdcon ++;
		setupNextInput();
	}
}

void updateSensorValueSet(int anNumber, int value)
{
	switch(anNumber)
	{
		case 3:
			lastSensorAN3[lastValueSetRow] = value;
		break;
		case 8:
			lastSensorAN8[lastValueSetRow] = value;
		break;
		case 9:
			lastSensorAN9[lastValueSetRow] = value;
		break;
		case 10:
			lastSensorAN10[lastValueSetRow] = value;
		break;
		case 11:
			lastSensorAN11[lastValueSetRow] = value;
		break;
	}
	if (anNumber == 3) //last analog in each loop --make sure each gets a value implemented
	{
		if (lastValueSetRow == 9)
		{
			lastValueSetRow = 0;
		}
		else
		{
			lastValueSetRow ++;
		}
	}	
}

int getSensorAverage(int Sensor)
{
	int result = 0;
	int sum = 0;
	
	switch(Sensor)
	{
		case 3:
			for (result =0; result <= 9; result++)
			{
				sum = sum + lastSensorAN3[result];
			}
		break;
		case 8:
			for (result =0; result <= 9; result++)
			{
				sum = sum + lastSensorAN8[result];
			}
		break;
		case 9:
			for (result =0; result <= 9; result++)
			{
				sum = sum + lastSensorAN9[result];
			}
		break;
		case 10:
			for (result =0; result <= 9; result++)
			{
				sum = sum + lastSensorAN10[result];
			}
		break;
		case 11:
			for (result =0; result <= 9; result++)
			{
				sum = sum + lastSensorAN11[result];
			}
		break;
	}
	result = sum / 10;

	return result;
}

void setupNextInput(void)
{
	if (nextAdcon < 1 || nextAdcon >5)
		nextAdcon = 1;

	ADCON1 = 0b00000011; //setup analog/digital -- All digital but 12
	switch(nextAdcon)
	{
	case 1: //AN8
		ADCON0 = 0b00100000; //1000 = Channel 8 (AN8)
	break;
	case 2://AN10
		ADCON0 = 0b00101000; //1010 = Channel 10 (AN10)
	break;
	case 3://AN11
		ADCON0 = 0b00101100; //1011 = Channel 11 (AN11)	
	break;
	case 4://AN9
		ADCON0 = 0b00100100; //1001 = Channel 9 (AN9)	
	break;
	case 5://AN3
		ADCON0 = 0b00001100; //0011 = Channel 3 (AN3)
	break;
	}
	ADCON2 = 0b00101100; //0 = left justified Unimplemented: Read as ‘0’ 101 = 12 TAD 100 = FOSC/4
	ADCON0bits.ADON = 1; //1 = A/D Converter module is enabled

}
void initiate_services()
{
	activate_peripherals();
	Gbl_Ints_ENABLED;

}

void setup_ports()
{
	ADCON1 = 0x0f;
	ADCON0bits.ADON = 0;
	PORTA = 0;
	TRISA = 0;
	PORTB = 0;
	TRISB = 0; 
	PORTC = 0;
	TRISC = 0;
	
	setup_Input_Pins; //ADC!
	setup_Output_Pins;
	enable_portb_weekpullups;

}

void setup_peripherals()
{

	//###############PWM
  	Setup_Motor_PWM;

	neutral_motor_pwm; 
	neutral_servo_pwm;
	//###############

	//##############EUSART

	OpenUSART( USART_TX_INT_OFF & USART_RX_INT_ON & USART_ASYNCH_MODE & USART_EIGHT_BIT & USART_CONT_RX & USART_BRGH_HIGH, 25 );
	BAUDCONbits.BRG16 = 1; // 16 bit mode
	//####################

	//#############Timeout Timer
	Setup_Timer0;
	Setup_Timer2;
		
	//################Analog Digital Converter

}

void setup_interrupts()
{
	Per_Ints_ENABLED;
	High_Pri_Ints_ENABLED;
	PIE1bits.RCIE = 1; //USART Receive Interrupt Enable bit
	IPR1bits.RCIP = 1; //HIGH - USART Receive Interrupt Priority bit
	PIE1bits.TMR2IE = 1;//1 = Enables the TMR2 & pr2 (ff for pwm) match interrupt
	IPR1bits.TMR2IP = 1; // TMR1 overflow interrupt -- high priority

if (stopDriveOnINT0Interrupt == 1)
{
	INTCON2bits.TMR0IP = 1; //high priority
	INTCONbits.INT0IE = 1; //enable interrupts on RB0 int0	-- Input 1!
}

if (timeouts_enabled == 1)
{
	INTCONbits.TMR0IE = 1; //enable timer 0 interupts
}	
	//INTCON2bits.INTEDG0= 0;// Interrupt on falling edge
	//allways high priority on rb0 int0

}

void activate_peripherals()
{
	setupNextInput ();//gets ready to use ADC
}
// High priority interrupt vector

#pragma code InterruptVectorHigh = 0x08
void InterruptVectorHigh (void)
{
		Gbl_Ints_DISABLED;
		High_Pri_Ints_DISABLED;
if (debug_mode_enabled == 1)
		yellow_on;

  _asm
    goto InterruptHandlerHigh //jump to interrupt routine
  _endasm
}

//----------------------------------------------------------------------------
// High priority interrupt routine

#pragma code
#pragma interrupt InterruptHandlerHigh
void InterruptHandlerHigh ()
{

	if(PIR1bits.TMR2IF == 1) //timer 1 overflow
	{
		TMR2Interrupt();
		PIR1bits.TMR2IF = 0;
	}


	if(INTCONbits.TMR0IF == 1) //timer 0 overflow
	{
		TMR0Interrupt();
		INTCONbits.TMR0IF = 0;
	}		

	 if(PIR1bits.RCIF == 1) //1 = The USART receive buffer, RCREG, is full (cleared when RCREG is read)
	{	
		yellow_on;
		//RCREG is full
		recieve_single = RCREG;
		input_router(recieve_single);
	}
	
	if (INTCONbits.INT0IF == 1) //if the ext int is triggered and set
	{
		handle_int0 ();
		INTCONbits.INT0IF = 0;
	}

		yellow_off;
		Gbl_Ints_ENABLED;  //re-enable interrupts
		High_Pri_Ints_ENABLED;
}

void send_usart_text(int textId)
{
	char string_to_output [105] = {0};

	switch (textId)
	{
	case 1:
		
		strcpypgm2ram (string_to_output, author_version);
		SendUSART_string (string_to_output, 38);
	break;
	case 2:
		strcpypgm2ram (string_to_output, verbose_menu_text);
		SendUSART_string (string_to_output, 72);
	break;
	case 3:
		strcpypgm2ram (string_to_output, quit_text);
		SendUSART_string (string_to_output, 14);
	break;
	case 4:
		strcpypgm2ram (string_to_output, unknown_string);
		SendUSART_string (string_to_output, 21);
	break;
	case 5:
		strcpypgm2ram (string_to_output, settings_menu_text);
		SendUSART_string (string_to_output, 58);
	break;
	case 6: //reset_eep_setting_text
		strcpypgm2ram (string_to_output, reset_eep_setting_text);
		SendUSART_string (string_to_output, 45);
	break;
	case 7: //system info 
		strcpypgm2ram (string_to_output, system_info_text);
		SendUSART_string (string_to_output, 103);
	break;
	case 10: //sayall
		strcpypgm2ram (string_to_output, sayall_mode_set_text);
		SendUSART_string (string_to_output, 19);
	break;
	case 11: //debug
		strcpypgm2ram (string_to_output, debug_mode_set_text);
		SendUSART_string (string_to_output, 18);
	break;
	case 12: //timeouts
		strcpypgm2ram (string_to_output, timeouts_set_text);
		SendUSART_string (string_to_output, 22);
	break;
	case 20: //testing info
		strcpypgm2ram (string_to_output, testing_info_text);
		SendUSART_string (string_to_output, 130);
	break;
	case 21: //press key to proceed
		strcpypgm2ram (string_to_output, Ckey_continue_text);
		SendUSART_string (string_to_output, 30);
	break;

	default:

	break;
	}


}