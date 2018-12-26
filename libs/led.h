#ifndef _LED_H
#define _LED_H

#include "libsCore.h"
#include "3664.h"

/* 便利そうなLED関係の定義 */

DEFINE_PORT_ACTIVE_LOW(LED_RED,PDR8,7)
DEFINE_PORT_ACTIVE_LOW(LED_GREEN,PDR8,6)
DEFINE_PORT_ACTIVE_LOW(LED_BLUE,PDR8,5)


#endif
