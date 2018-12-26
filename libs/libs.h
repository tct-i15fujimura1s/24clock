#ifndef _LIBS_H
#define _LIBS_H

#include "3664.h"
#include "lcd.h"
#include "util.h"
#include "wait.h"
#include "lcd_util.h"
#include "matrix.h"
#include "key.h"
#include "timer.h"
#include "ir.h"
#include "speaker.h"
#include "serial.h"
#include "ADconv.h"
#include "eeprom.h"
#include "watchdog.h"
#include "led.h"

/* ここに書いていた宣言などは，           */
/* ソースリストの依存関係を軽減するために */
/* 以下のlibsCoreに追い出した             */

#include "libsCore.h"

#endif
