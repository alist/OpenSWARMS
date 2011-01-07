//SWARMS Main Specification File
//Contains all embedded defines as listed
//in datasheet V: 1.01
#ifndef SWARMSDefines
#define SWARMSDefines

//*****USB Commands*****
//subboard allways present identifier
#define USB_IDENT_ALL 0
#define USB_IDENT_SYSTEM 1
#define USB_IDENT_DRIVE 2
#define USB_IDENT_SENS_IO 3
#define USB_IDENT_POWER_CONT 4
#define USB_IDENT_UNKNOWN 99
//sys
#define USB_ACK 1
#define USB_ABORT 2
#define USB_RESET 4
#define USB_ACT_LED 5
#define USB_VALIDATE_COMMAND 6
#define USB_SYS_VERSION 7 
#define USB_STATUS 8

//drive 
#define USB_M_BASE_RADIUS 10
#define USB_M_WHEEL_RADIUS 11
#define USB_M_MOVE_CM_DIST 12
#define USB_M_ZEROP_TURN 13
#define USB_M_RATIO_DRIVE_DIST 14
#define USB_M_RATIO_LEFT_SEG 16
#define USB_M_RATIO_RIGHT_SEG 15
#define USB_M_RATIO_CMS_SPEED 17
#define USB_M_RATIO_MODE_SET 18
#define USB_M_DRIVE_DIRECTION 19
#define USB_M_MOVE_ABSOLUTE_SPEED 20
#define USB_M_VEHICLE_TYPE 21
//io
#define USB_IO_ANALOG_INPUT 40
#define USB_IO_DIGITAL_INPUT 41 
#define USB_IO_DIGITAL_OUTPUT 42
#define USB_IO_DIGITAL_TO_ANALOG 43 


//***Internal Bus Commands***

#define VCL_TYPE char
#define VCL_TYPE_HOLDER VehicleType
#define IS_VALID_VCL_TYPE (VCL_TYPE_HOLDER >= 1 && VCL_TYPE_HOLDER <= 3)
#define VCL_TYPE_4WHL_FRNTTURN 1
#define VCL_TYPE_OBSC 1
#define VCL_TYPE_4WHL_SQRDIF 2
#define VCL_TYPE_CRCDIF 3
#define VCL_TYPE_3WHL_OMNIDRV 4

//drive commands
//#define SWARMSCommandID //your choice of variable
#define IS_VALID_COMMAND (SWARMSCommandID = C_SET_BASE_RADIUS)||(SWARMSCommandID ==  C_ACTIVATE_LED)||(SWARMSCommandID ==  C_WHEEL_RADIUS_SET)||(SWARMSCommandID == C_LTECHOMETER_CHANGES_PER_REV)||(SWARMSCommandID == C_RTECHOMETER_CHANGES_PER_REV)||(SWARMSCommandID == C_MOTOR_SPEED_AVG)||(SWARMSCommandID == C_MOTOR_MOVEMENT_CM_RMN)||(SWARMSCommandID == C_STOP)||(SWARMSCommandID == C_RESET)||(SWARMSCommandID == C_ACK)||(SWARMSCommandID == C_ZPOINT_TURN_DEG )||(SWARMSCommandID == C_DRIVE_SPEED_ABS)||(SWARMSCommandID == C_DRIVE_SPEED_CMpS)||(SWARMSCommandID == C_DRIVE_SPEED_CMS)
#define C_ACK 1
#define C_STOP 2
#define C_RESET 3
#define C_ZPOINT_TURN_DEG 4
#define C_DRIVE_SPEED_ABS 5 /*absolute speed centered at 0 +/- 100 */
#define C_DRIVE_SPEED_CMpS 6 /*defined as CM/S*/
#define C_DRIVE_SPEED_CMS 7
#define C_MOTOR_SPEED_AVG 8 /*return CM/s value avg*/
#define C_MOTOR_MOVEMENT_CM_RMN 9 /*return CMs avg remaining*/
#define C_WHEEL_RADIUS_SET 10
#define C_LTECHOMETER_CHANGES_PER_REV 11
#define C_RTECHOMETER_CHANGES_PER_REV 12
#define C_ACTIVATE_LED 13 /*payload with 1 byte ledCode*/
#define C_SET_BASE_RADIUS 14


//typedefs
#define TRUE 1
#define FALSE 0
#define SUCCESS 1
#define FAILURE 0
#define YES 1
#define NO 0
#define EMPTY 0x00
#define NIL 0x00
#define GOOD 1
#define BAD 0

#define C_MOVEMENT_DIR signed char
#define BOOL char
#define C_TECHOMETER_VALUE signed char
#define DTick signed int
#define DDist float
#define DSpeed signed int
#define DRawSpeed signed int // +/- 100 abs speed
#define DRatio signed int

#endif
