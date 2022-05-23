/******************************************************************************
 *
 * Module: DIO
 *
 * File Name: Det.h
 *
 * Description: Error development tracer
 *
 * Authors: Hady Emad, Mohanad, Ahmed Adel, Omar Adel
 *
 * AUTOSAR Standard Release: 4.3.1
 *
 *******************************************************************************/
#ifndef DET_H
#define DET_H

#include "Std_Types.h"

//*****************************************************************************
//
//  Functions' Prototypes
//
//*****************************************************************************

Std_ReturnType Det_ReportError(uint16 ModuleId, uint8 InstanceId, uint8 ApiId, uint8 ErrorId);


#endif /* DET_H */
