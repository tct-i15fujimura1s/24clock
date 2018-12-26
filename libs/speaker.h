#ifndef _SPEAKER_H
#define _SPEAKER_H

#include "libsCore.h"
#include "3664.h"

/* 便利そうなspeaker関係の定義 */

DEFINE_PORT_ACTIVE_HIGH(SPEAKER_DIRECT_CONTROL,PDR8,2)


extern void snd_mng(void);
extern int snd_play(char *score);
extern void snd_stop(void);
extern int snd_check(void);

#endif
