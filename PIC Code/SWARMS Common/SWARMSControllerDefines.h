//SWARMSControllerDefines.h
//Swarms Specific Control Defines

//drive commands
//#define SWARMSCommandID //your choice of variable
#define USB_COMMAND_SYSTEM (SWARMSCommandID >= 1 && SWARMSCommandID <= 7)
#define USB_COMMAND_DRIVE (SWARMSCommandID >= 10 && SWARMSCommandID <= 21)
#define USB_COMMAND_SENS_IO (SWARMSCommandID >= 40 && SWARMSCommandID <= 50)

#define IS_VALID_USB_COMMAND USB_COMMAND_SYSTEM || USB_COMMAND_DRIVE || USB_COMMAND_SENS_IO

#define COMMAND_IS_PROHIB_BY_VCL_TYPE_4WHL_FRNTTURN 0 /*(SWARMSCommandID == 1)||(SWARMSCommandID == 1)*/
#define COMMAND_IS_PROHIB_BY_VCL_TYPE_4WHL_SQRDIF (SWARMSCommandID == 20)
#define COMMAND_IS_PROHIB_BY_VCL_TYPE_4WHL_CRCDIF (SWARMSCommandID == 20)
#define COMMAND_IS_PROHIB_BY_VCL_TYPE_3WHL_OMNIDRV 1

#define COMMAND_IS_PROHIB_BY_RATIO_MODE (SWARMSCommandID == 12)||(SWARMSCommandID == 19)||(SWARMSCommandID == 20)

#define USB_MAKE_ACK USB_Out_Buffer[0] = 0x01; USB_Out_Buffer[1] = 0; putUSBUSART(USB_Out_Buffer,2);
#define USB_MAKE_COMMAND_ABORT USB_Out_Buffer[0] = 0x02; USB_Out_Buffer[1] = 1;USB_Out_Buffer[2] = SWARMSCommandID; putUSBUSART(USB_Out_Buffer,3);

#define LED_1_IDENT 1
#define LED_2_IDENT 2
#define LED_3_IDENT 3
#define LED_4_IDENT 4