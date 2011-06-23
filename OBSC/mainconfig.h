/*********************************************************
** All rights for this code have been reserved by their appropiate owners
** Creator and affiliated organizations have been removed to comply with
** competition regulations
** C 2008 - 2009
*///******************************************************

//configs
#pragma config OSC = INTIO67// Internal oscelator, RC, OSC1 as RA7, OSC2 as FOSC/4
#pragma config WDT = OFF //watchdog timer off -- *TURN ON WHEN RELEASING FOR STABILITY ASSURANCE*
#pragma config WDTPS = 32768 //watchdog prescale 1:32768
#pragma config BOREN = OFF //brown out Disabled
#pragma config MCLRE = ON //MCLEAR Enabled
#pragma config PBADEN = OFF //Digital on reset
#pragma config FCMEN = OFF //Fail-Safe Clock Monitor un-ENABLED --Backup oscilator
#pragma config IESO = OFF //Internal External Switch Over mode disabled
#pragma config PWRT = OFF //power up timer dis-Enabled

#pragma config CCP2MX = PORTC // ccp2 std on RC1  ********* -- check if awry **********

#pragma config STVREN = OFF //stack overflow reset un-Enabled
#pragma config LVP = OFF//low voltage programing Disabled
#pragma config DEBUG = OFF // Disabled
#pragma config CP0 = OFF //code protect Disabled
#pragma config CP1 = OFF //code protect Disabled
#pragma config CPB = OFF //code protect Disabled
#pragma config CPD = OFF //code protect Disabled
#pragma config WRT0 = OFF //write protection Disabled
#pragma config WRT1 = OFF //write protection Disabled
#pragma config WRTB = OFF //write protection Disabled
#pragma config WRTC = OFF //write protection Disabled
#pragma config WRTD = OFF //write protection Disabled
#pragma config EBTR0 = OFF //write protection Disabled
#pragma config EBTR1 = OFF //write protection Disabled
#pragma config EBTRB = OFF //write protection Disabled

#define TRUE 1
#define FALSE 0

//wireless board

#define green_on		PORTAbits.RA2 = 1;
#define green_off		PORTAbits.RA2 = 0;
#define green_toggle	PORTAbits.RA2 = !PORTAbits.RA2;
#define yellow_on			PORTAbits.RA1 = 1;
#define yellow_off			PORTAbits.RA1 = 0;
#define yellow_toggle		PORTAbits.RA1 = !PORTAbits.RA1;
#define red_on			PORTAbits.RA0 = 1;
#define red_off			PORTAbits.RA0 = 0;
#define red_toggle		PORTAbits.RA0 = !PORTAbits.RA0;

//interupt constants
#define Per_Ints_ENABLED		INTCONbits.PEIE = 1;
#define Per_Ints_DISABLED		INTCONbits.PEIE = 0;
#define High_Pri_Ints_ENABLED		RCONbits.IPEN = 1;            //enable priority levels          //enable interrupts -- on high priority
#define High_Pri_Ints_DISABLED		RCONbits.IPEN = 0;            //enable priority levels          //disable interrupts -- on high priority
#define Gbl_Ints_ENABLED		INTCONbits.GIEH = 1;
#define Gbl_Ints_DISABLED		INTCONbits.GIEH = 0;

//Pin setups
#define Setup_I2C_Pins {TRISCbits.TRISC4 = 1; TRISCbits.TRISC3 = 1;} 

//Peripheral setups
#define Setup_int_osc_8mhz OSCCON = 0b01111110;
#define Setup_int_osc_4mhz OSCCON = 0b01101110;
#define Setup_int_osc_2mhz OSCCON = 0b01011110;

#define Setup_Motor_PWM PR2 = 0xff; CCP1CON = 0b00111100; CCP2CON = 0b00111100;	T2CON = 0b00000111;			//set PWM period (approx. 122 htz) 1/16 prescale

#define Setup_Timer0 T0CON = 0b10000010; // on,8bit(0 =16),clock cycle,on rising edge, uses prescale, 1:8 prescale
#define Stop_Timer0 T0CON = 0x00;
#define reset_timeouts 	TMR0H = 0x00; TMR0L = 0x00; //reset the timouts
#define Setup_Timer1 T1CON  = 0b00001001;//0 = 8-bit operations  0 = clock is derived from another source 01 = 1:1 Prescale 0 = Timer1 osc is off 1 = no external clock input 0 = (FOSC/4) 1 = Enables Timer1
#define Stop_Timer1 T1CON = 0x00;
#define Setup_Timer2 T2CON  = 0b00001110;//0, 0000 = 1:2 postscale, 1 = 0n, 00 = 1:1 prescale
#define Stop_Timer2 T2CON = 0x00;

#define EEP_Say_all_setup write_location = 10;  write_false_char = 'F';  write_true_char = 'T';
#define EEP_Debug_mode_setup write_location = 11;  write_false_char = 'F';  write_true_char = 'T';
#define EEP_Timeouts_setup write_location = 12;  write_false_char = 'F';  write_true_char = 'T';
#define EEP_Stop_inp1_setup write_location = 13;  write_false_char = 'F';  write_true_char = 'T';
#define EEP_Manage_Speed_setup write_location = 14;  write_false_char = 'F';  write_true_char = 'T';
#define EEP_NumberPinsDigital_setup write_location = 20;
#define EEP_Max_Motor_speed write_location = 21;
#define EEP_Min_Motor_speed write_location = 22;


#define Setup_USART TRISCbits.TRISC7 = 1; TXSTA = 0b00100000; RCSTA = 0b10010000; //8 bit mode -- enables tx & rx
#define Setup_USART_Clock_19_2 SPBRG = 25;// Where fosc = 8mhz or 2mhz -- baud is 19.2 k 
		
#define neutral_motor_pwm motor_current = 50; CCPR1L = 46;//46 CCP1CONbits.DC1B1 =1;//--not needed already set CCP1CONbits.DC1B2 =1; //0b0010111011 = 187
#define forward_slow_motor_pwm CCPR1L = 50;//0b00101111 = 47 LSB still =11 total - 0b0010111111 = 191 
#define forward_medium_motor_pwm CCPR1L = 52;
#define backward_motor_pwm CCPR1L = 43; //** should work
#define neutral_servo_pwm servo_current = 50; CCPR2L = 48;//0b00101110;// CCP1CONbits.DC1B1 =1;//--not needed already set CCP1CONbits.DC1B2 =1; //0b0010111011 = 187
#define stop_now neutral_motor_pwm; neutral_servo_pwm; red_on;
#define input_speed_fix speed = speed - 4;// to make input standardized @ input 50 subtract 4 from speed
#define input_servo_fix position = position - 2;// to make input standardized @ input 50 subtract 2 from position

#define clear_last_addr last_address = 0x00;
#define USART_ACK SendUSART('A'); //send ack w/ bad data flag
#define USART_ACK_BD SendUSART('B'); //send ack w/ bad data flag

//more defines ***********
#define set_ADC_config 	ADCON1 = 0b00000011; ADCON0 = 0b00101100; ADCON2 = 0b10101100; ADCON0bits.ADON = 1; //1 = Right justified Unimplemented: Read as ‘0’ 101 = 12 TAD 100 = FOSC/4; //setup analog/digital for an11 and below
#define set_ADC_on ADCON0bits.ADON = 1;
#define set_ADC_off ADCON0bits.ADON = 0;
#define setup_Input_Pins TRISAbits.TRISA3 = 1; TRISBbits.TRISB0 = 1; TRISBbits.TRISB1 = 1; TRISBbits.TRISB2 = 1; TRISBbits.TRISB3 = 1; TRISBbits.TRISB4 = 1;
#define setup_Output_Pins TRISAbits.TRISA4 = 0; TRISAbits.TRISA5 = 0;
#define enable_portb_weekpullups INTCON2bits.RBPU = 0; // on all inputs
#define enable_portb_intonchange INTCONbits.RBIE = 1; INTCON2bits.RBIP = 1; //enable int. high priority or ports rb7-rb4