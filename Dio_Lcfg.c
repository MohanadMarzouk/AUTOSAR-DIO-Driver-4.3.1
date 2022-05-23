/*
 * Dio_Lcfg.c
 *
 *  Created on: May 19, 2022
 *      Author: Lenovo
 */

#include "Dio_Lcfg.h"



struct PortConfig_s PortConfig [PORT_CONFIG_SIZE] =
{
     {
      .DioPinId = 41,
      .PinDirection = OUTPUT,
      .PullUp_PullDown = 1,
      .DioLevelType = STD_HIGH,
      .SlewRateControl = DISABLED,
     },
     {
      .DioPinId = 42,
      .PinDirection = INPUT,
      .PullUp_PullDown = 0,
      .DioLevelType = STD_LOW,
      .SlewRateControl = ENABLED,
     }

};
