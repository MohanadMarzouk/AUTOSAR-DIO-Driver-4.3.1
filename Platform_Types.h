/*
 * Platform_Types.h
 *
 *  Created on: May 14, 2022
 *      Author: Lenovo
 */

#ifndef PLATFORM_TYPES_H_
#define PLATFORM_TYPES_H_
#include <stdbool.h>


#define NULL_PTR    ((void*)0)
#define STRING_SIZE 30
#define TRUE    1
#define FALSE   0



typedef unsigned char         uint8;          /*           0 .. 255             */
typedef signed char           sint8;          /*        -128 .. +127            */
typedef unsigned short        uint16;         /*           0 .. 65535           */
typedef signed short          sint16;         /*      -32768 .. +32767          */
typedef unsigned long         uint32;         /*           0 .. 4294967295      */
typedef signed long           sint32;         /* -2147483648 .. +2147483647     */
typedef unsigned long long    uint64;         /*       0..18446744073709551615  */
typedef signed long long      sint64;
typedef float                 float32;
typedef double                float64;

typedef unsigned long       EcucIntegerParamDef;
typedef char EcucStringParamDef[STRING_SIZE];
typedef bool EcucBooleanParamDef;


#endif /* PLATFORM_TYPES_H_ */
