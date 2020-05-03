/*
 * _Global.h
 *
 *  Created on: 2020Äê5ÔÂ1ÈÕ
 *      Author: HJKF
 */

#ifndef GLOBAL_H_
#define GLOBAL_H_

#include "IQmathLib.h"

extern _iq IQsinTable[];
extern _iq IQcosTable[];

#define PA11out PAout(11)
#define PA10out PAout(10)
#define PB12out PBout(12)

#define PA11OUT_HIGH PA11out=1
#define PA10OUT_HIGH PA10out=1
#define PB12OUT_HIGH PB12out=1

#define PA10OUT_LOW PA10out=0
#define PA11OUT_LOW PA11out=0
#define PB12OUT_LOW PB12out=0

#define PC15OUT PCout(15)
#define PC15OUT_HIGH PC15OUT=1
#define PC15OUT_LOW PC15OUT=0

void led_Init();

#endif /* GLOBAL_H_ */
