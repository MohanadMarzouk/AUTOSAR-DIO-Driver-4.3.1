/******************************************************************************
 *
 * Module: Dio
 *
 * File Name: Dio.h
 *
 * Description: Functions definitions of Dio module
 *
 * Authors: Mohanad Mohamed
 *
 * AUTOSAR Standard Release: 4.3.1
 *
 *******************************************************************************/
#ifndef DIO_H_
#define DIO_H_

#include "Std_Types.h"
#include "Dio_Cfg.h"
#include "Dio_Types.h"
#include "tm4c123gh6pm.h"
#include "Dio_Lcfg.h"



#define MAX_CHANNEL_ID          47
#define PORT_ID_MAX             6

#define Start_of_bit_band_region    0x40000000
#define Start_of_bit_band_alias     0x42000000

static volatile uint32 GPIO_BASE_ADDRESS [] = {0x40004000,
                                               0x40005000,
                                               0x40006000,
                                               0x40007000,
                                               0x40024000,
                                               0x40025000,
};



/*******************************************************************************
 *                         Module version info                                *
*******************************************************************************/
#define DIO_SW_MAJOR_VERSION     1U
#define DIO_SW_MINOR_VERSION     0U
#define DIO_SW_PATCH_VERSION     0U


#define DIO_MODULE_ID          (1)                 /* Module ID for DET Function */
#define DIO_INSTANCE_ID        (0)                 /* Instance ID for DET Function */

/*******************************************************************************
 *                         AUTOSAR version info                                *
*******************************************************************************/

#define SW_MAJOR_VERSION     4U
#define SW_MINOR_VERSION     3U
#define SW_PATCH_VERSION     1U

#define VENDOR_ID            1U


/*******************************************************************************
 *                      Development Errors                                     *
*******************************************************************************/
#define DIO_E_PARAM_INVALID_CHANNEL_ID          (0x0A)
#define DIO_E_PARAM_INVALID_PORT_ID             (0x14)
#define DIO_E_PARAM_INVALID_GROUP               (0x1F)
#define DIO_E_PARAM_POINTER                     (0x20)


/*******************************************************************************
 *                           Services ID's                                     *
*******************************************************************************/
#define Dio_ReadChannel_Service_ID          (0x00)
#define Dio_DetVersionInfo_Service_ID       (0x12)
#define Port_Init_Service_ID                (0x01)
#define Dio_WriteChannel_Service_ID         (0x01)
#define Dio_WritePort_Service_ID            (0x03)
#define Dio_ReadPort_Service_ID             (0x02)
#define Dio_FlipChannel_Service_ID          (0x11)



/****************************************************************************
*                        AUTOSAR Functions' Definition                      *
*****************************************************************************/
Dio_LevelType Dio_ReadChannel( Dio_ChannelType ChannelId );
Dio_PortLevelType Dio_ReadPort( Dio_PortType PortId );
void Dio_WriteChannel( Dio_ChannelType ChannelId, Dio_LevelType Level );
void Dio_WritePort(Dio_PortType PortId, Dio_PortLevelType Level);
void Dio_GetVersionInfo(struct Std_VersionInfoType* versioninfo);
Dio_LevelType Dio_FlipChannel( Dio_ChannelType ChannelId);
void Port_Init(struct PortConfig_s *ConfigPtr);


/****************************************************************************
*                             Static Functions                              *
*****************************************************************************/
static void set_bit (uint32  Reg, uint32 bits);
static void clear_bit (uint32  Reg,uint32 bits);
#endif /* DIO_H_ */
