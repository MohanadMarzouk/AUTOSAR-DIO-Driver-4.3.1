/******************************************************************************
 *
 * Module: Dio
 *
 * File Name: Dio.c
 *
 * Description: Functions declaration of Dio module
 *
 * Authors: Mohanad Mohamed
 *
 * AUTOSAR Standard Release: 4.3.1
 *
 *******************************************************************************/

/*******************************************************************************
 *                             Included header files                           *
 *******************************************************************************/
#include "Dio.h"
#include "Det.h"
#include "Dio_Types.h"


/*******************************************************************************
 *                        Set and Clear Functions                              *
 *******************************************************************************/
static void set_bit (uint32  Reg, uint32 bits)
{
    uint32 bit_band_alias =((Reg - Start_of_bit_band_region) *32 + Start_of_bit_band_alias + (bits * 4)) ;
    *((volatile unsigned long *) bit_band_alias) = 0x1;            //Setting a bit. Use the bitwise OR operator ( | ) to set a bit. number |= 1 << x; That will set a bit x .
}

static void clear_bit (uint32  Reg,uint32 bits)
{
    uint32 bit_band_alias =((Reg - Start_of_bit_band_region) *32 + Start_of_bit_band_alias + (bits * 4)) ;
    *((volatile unsigned long *) bit_band_alias) = 0x0;        //Clearing a bit. Use the bitwise AND operator ( & ) to clear a bit. number &= ~(1 << x); That will clear bit x . ...
}

/*******************************************************************************
 *                        AUTOSAR DIO API's Declaration                        *
 *******************************************************************************/

void Port_Init(struct PortConfig_s *ConfigPtr)
{
    if (ConfigPtr == NULL_PTR)
    {
        /*error*/
    }
    else
    {
        /*start of service*/

        uint8 counter = 0;
        uint8 portIndex = ConfigPtr[counter].DioPinId / 8;
        uint8 pinIndex = ConfigPtr[counter].DioPinId % 8;

        for(counter = 0; counter < PORT_CONFIG_SIZE; counter++)
        {

            /*Configuring Direction*/
            (*((volatile uint32_t *)(GPIO_BASE_ADDRESS[portIndex] + GPIO_DIR_Offset))) |=
                    (ConfigPtr[counter].PinDirection << pinIndex);

            /*Configuring Resistor Type*/
            if(ConfigPtr[counter].PullUp_PullDown)
            {
//                (*((volatile uint32_t *)(GPIO_BASE_ADDRESS[portIndex] + GPIO_PUR_Offset))) |= (1<<pinIndex);

                set_bit((GPIO_BASE_ADDRESS[portIndex] + GPIO_PUR_Offset) , pinIndex);
            }
            else
            {
                set_bit((GPIO_BASE_ADDRESS[portIndex] + GPIO_PDR_Offset) , pinIndex);
            }

            /*Configuring Level Type*/
            if (ConfigPtr[counter].DioLevelType)
            {
                set_bit((GPIO_BASE_ADDRESS[portIndex] + GPIO_DATA_Offset), pinIndex);
            }
            else
            {
                clear_bit((GPIO_BASE_ADDRESS[portIndex] + GPIO_DATA_Offset), pinIndex);

            }

            /*Configuring Slew Rate*/
            (*((volatile uint32_t *)(GPIO_BASE_ADDRESS[portIndex] + GPIO_SLR_Offset)))
                    |= (ConfigPtr[counter].SlewRateControl << pinIndex);
        }
    }

}



/************************************************************************************
 * Service Name: Dio_ReadChannel
 * Service ID[hex]: 0x00
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant
 * Parameters (in): ChannelId
 * Parameters (inout): None
 * Parameters (out): None
 * Return value: Dio_LevelType
 * Description: Returns the value of the specified DIO channel.
 ************************************************************************************/
Dio_LevelType Dio_ReadChannel( Dio_ChannelType ChannelId )
{
    Dio_LevelType levelType;
    if(ChannelId > MAX_CHANNEL_ID)
    {
#if DioDevErrorDetect == STD_ON
        Det_ReportError(DIO_MODULE_ID, DIO_INSTANCE_ID, Dio_ReadChannel_Service_ID, DIO_E_PARAM_INVALID_CHANNEL_ID);
#endif
    }
    else
    {
        /*start of service*/
        uint8 portIndex = ChannelId / 8;
        uint8 pinIndex = ChannelId % 8;
        uint32 reg = (*((volatile uint32_t *)(GPIO_BASE_ADDRESS[portIndex]+GPIO_DATA_Offset)));

        if(reg & (1<<pinIndex))
        {
            levelType = STD_HIGH;
        }
        else
        {
            levelType = STD_LOW;
        }

        /*[SWS_Dio_00027] The Dio_ReadChannel function shall
         * return the value of the specified DIO channel.*/
    }
    return levelType;
}/*End of Function*/

/************************************************************************************
 * Service Name: Dio_WriteChannel
 * Service ID[hex]: 0x01
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant
 * Parameters (in): ChannelId, Level
 * Parameters (inout): None
 * Parameters (out): None
 * Return value: Dio_LevelType
 * Description: Service to set a level of a channel.
 ************************************************************************************/
void Dio_WriteChannel( Dio_ChannelType ChannelId, Dio_LevelType Level )
{
    if(ChannelId > MAX_CHANNEL_ID)
    {
#if DioDevErrorDetect == STD_ON
        Det_ReportError(DIO_MODULE_ID, DIO_INSTANCE_ID, Dio_WriteChannel_Service_ID, DIO_E_PARAM_INVALID_CHANNEL_ID);
#endif
    }
    else if(Level > 1)
    {
        /*return error*/
    }
    else
    {
        uint8 portIndex = ChannelId / 8;
        uint8 pinIndex = ChannelId % 8;
        uint32 dataReg = (*((volatile uint32_t *)(GPIO_BASE_ADDRESS[portIndex]+GPIO_DATA_Offset)));
        uint32 dirReg = (*((volatile uint32_t *)(GPIO_BASE_ADDRESS[portIndex]+GPIO_DIR_Offset)));

        if(dirReg & (1<<pinIndex))
        {
            switch(Level)
            {
            case STD_HIGH:
                set_bit(dataReg, pinIndex);
                break;
            case STD_LOW:
                clear_bit(dataReg, pinIndex);
                break;
            }
        }

    }
}/*End of Function*/

/************************************************************************************
 * Service Name: Dio_ReadPort
 * Service ID[hex]: 0x02
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant
 * Parameters (in): PortId
 * Parameters (inout): None
 * Parameters (out): None
 * Return value: Dio_PortLevelType
 * Description: Returns the level of all channels of that port.
 ************************************************************************************/
Dio_PortLevelType Dio_ReadPort( Dio_PortType PortId )
{
    Dio_PortLevelType portLevel = 0;

    if(PortId > PORT_ID_MAX)
    {
#if DioDevErrorDetect == STD_ON
        Det_ReportError(DIO_MODULE_ID, DIO_INSTANCE_ID, Dio_ReadPort_Service_ID, DIO_E_PARAM_INVALID_PORT_ID);
#endif
    }
    else
    {
        /*start of service*/
        uint32 dataReg = (*((volatile uint32_t *)(GPIO_BASE_ADDRESS[PortId]+GPIO_DATA_Offset)));
        portLevel = dataReg;
    }

    /*[SWS_Dio_00031] The Dio_ReadPort function
     *  shall return the level of all channels of that port.*/
    return portLevel;
}

/************************************************************************************
 * Service Name: Dio_WritePort
 * Service ID[hex]: 0x03
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant
 * Parameters (in): PortId, Level
 * Parameters (inout): None
 * Parameters (out): None
 * Return value: None
 * Description: Service to set a value of the port.
 ************************************************************************************/
void Dio_WritePort(Dio_PortType PortId, Dio_PortLevelType Level)
{
    if(PortId > PORT_ID_MAX)
    {
#if DioDevErrorDetect == STD_ON
        Det_ReportError(DIO_MODULE_ID, DIO_INSTANCE_ID, Dio_WritePort_Service_ID, DIO_E_PARAM_INVALID_PORT_ID);
#endif
    }
    else if(Level > 1)
    {
        /*return error*/
    }
    else
    {
        /*start of service*/
        uint32 dataReg = (*((volatile uint32_t *)(GPIO_BASE_ADDRESS[PortId]+GPIO_DATA_Offset)));
        uint32 dirReg = (*((volatile uint32_t *)(GPIO_BASE_ADDRESS[PortId]+GPIO_DIR_Offset)));
        uint8 pinIndex = 0;
        uint8 pinNums = 7;

        for(pinIndex = 0; pinIndex < pinNums; pinIndex++)
        {
            if(dirReg & (1<<pinIndex))
            {
                switch(Level)
                {
                case STD_HIGH:
                    set_bit(dataReg, pinIndex);
                    break;
                case STD_LOW:
                    clear_bit(dataReg, pinIndex);
                }
            }
        }
    }
}


/************************************************************************************
 * Service Name: Dio_FlipChannel
 * Service ID[hex]: 0x11
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant
 * Parameters (in): ChannelId
 * Parameters (inout): None
 * Parameters (out): None
 * Return value: None
 * Description: Service to flip (change from 1 to 0 or from 0 to 1) the level of a channel and
 *              return the level of the channel after flip.
 ************************************************************************************/
Dio_LevelType Dio_FlipChannel( Dio_ChannelType ChannelId)
{
    Dio_LevelType levelType = 0;
    if(ChannelId > MAX_CHANNEL_ID)
    {
#if DioDevErrorDetect == STD_ON
        Det_ReportError(DIO_MODULE_ID, DIO_INSTANCE_ID, Dio_FlipChannel_Service_ID, DIO_E_PARAM_INVALID_CHANNEL_ID);
#endif
    }
    else
    {
        /*start of service*/

        uint8 portIndex = ChannelId / 8;
        uint8 pinIndex = ChannelId % 8;
        uint32 dirReg = (*((volatile uint32_t *)(GPIO_BASE_ADDRESS[portIndex]+GPIO_DIR_Offset)));
        uint32 dataReg = (*((volatile uint32_t *)(GPIO_BASE_ADDRESS[portIndex]+GPIO_DATA_Offset)));

        /*Pin is configured as output pin*/
        if(dirReg & (1<< pinIndex))
        {
            /*Pin is HIGH*/
            if(dataReg & (1 << pinIndex))
            {
                /*[SWS_Dio_00191] If the specified channel is configured as an output channel,
                 * the Dio_FlipChannel function shall read level of the channel
                 * and invert it, then write the inverted level to the channel.*/

                clear_bit((GPIO_BASE_ADDRESS[portIndex]+GPIO_DATA_Offset), pinIndex);
                levelType = STD_LOW;
            }
            /*Pin is LOW*/
            else
            {
                /*[SWS_Dio_00192] If the specified channel is configured as an input channel,
                 * the Dio_FlipChannel function shall have no influence on the physical output.
                 * The return value shall be the level of the specified channel.*/

                set_bit((GPIO_BASE_ADDRESS[portIndex]+GPIO_DATA_Offset), pinIndex);
                levelType = STD_HIGH;
            }
        }
        /*pin is configured as input pin*/
        else
        {
            if(dataReg & (1<<pinIndex))
            {
                levelType = STD_HIGH;
            }
            else
            {
                levelType = STD_LOW;
            }
        }
    }
    return levelType;
}

/************************************************************************************
 * Service Name: Dio_GetVersionInfo
 * Service ID[hex]: 0x12
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant
 * Parameters (in): None
 * Parameters (inout): None
 * Parameters (out): VersionInfo
 * Return value: None
 * Description: Service to get the version information of this module.
 ************************************************************************************/
#if DioVersionInfoApi == STD_ON
void Dio_GetVersionInfo(struct Std_VersionInfoType* versioninfo)
{
    if(versioninfo == NULL_PTR)
    {
#if DioDevErrorDetect == STD_ON
        Det_ReportError(DIO_MODULE_ID, DIO_INSTANCE_ID, Dio_DetVersionInfo_Service_ID, DIO_E_PARAM_POINTER);
#endif
    }
    else
    {
        versioninfo->moduleID = VENDOR_ID;
        versioninfo->moduleID = DIO_MODULE_ID;
        versioninfo->sw_major_version = DIO_SW_MAJOR_VERSION;
        versioninfo->sw_minor_version = DIO_SW_MINOR_VERSION;
        versioninfo->sw_patch_version = DIO_SW_PATCH_VERSION;
    }
}
#endif
