#ifndef MAIN_H
#define MAIN_H

extern volatile int sec_flag;
extern volatile int sec;
extern volatile int tma_flag;
extern volatile int tmv_flag;

extern unsigned int matrix_led_pattern[8];

extern volatile int tempo_flag;
extern int tempo_compare;

/* 適切なモード名を入れることが望ましいが，MODE_0〜とする。 */
/* 美しい書き方をするならば，適切なテーブルを持つ必要がある */
enum MENU_MODE{
  MODE_OUT_OF_MIN=-1,
  MODE_0 = 0,
  MODE_1,
  MODE_10 = 10,
  MODE_11,
  MODE_20 = 20,
  MODE_30 = 30,
  MODE_40 = 40,
  MODE_50 = 50,
  MODE_60 = 60,
  MODE_70 = 70,
  MODE_71,
  MODE_72,
  MODE_73,
  MODE_OUT_OF_MAX
};

//うぅ。下記のKとkの見分け(大文字小文字の見分け)が付かずに，
//１時間半の痛恨のロス(2011/12/19 00:37 by T.NITTA)
enum SW_CODE{
  KEY_NONE=0,         //キー入力無し 
  KEY_SHORT_U=(1<<4), //上短押し
  KEY_SHORT_D=(1<<3), //下短押し
  KEY_SHORT_L=(1<<2), //左短押し
  KEY_SHORT_R=(1<<1), //右短押し
  KEY_SHORT_C=(1<<0), //中央短押し
  KEY_LONG_U=(0x80 | (1<<4)), //上長押し
  KEY_LONG_D=(0x80 | (1<<3)), //下長押し
  KEY_LONG_L=(0x80 | (1<<2)), //左長押し
  KEY_LONG_R=(0x80 | (1<<1)), //右長押し
  KEY_LONG_C=(0x80 | (1<<0))  //中央長押し
};

typedef struct _UI_DATA{
  int mode;
  int prev_mode;
  unsigned char sw;
}UI_DATA;

#include "mode0.h"
#include "mode10.h"
#include "mode20.h"
#include "mode30.h"
#include "mode40.h"
#include "mode50.h"
#include "mode60.h"
#include "mode70.h"

#  ifdef MODE0_C
char *tithe_mode_titles[] = {
  mode0_title,
  mode10_title,
  mode20_title,
  mode30_title,
  mode40_title,
  mode50_title,
  mode60_title,
  mode70_title
};
#  endif

#  ifdef MAIN_C
UI_DATA* ui(char sw){ /* ミーリ型？ムーア型？どっちで実装？良く考えて */
  static UI_DATA ui_data={MODE_0,MODE_0,};
  int prev_mode;

  ui_data.sw=(sw & 0x9f); /*念のために，b6,b5を0にしておく*/
  prev_mode=ui_data.mode;
#    define DEF_MODE(n) case MODE_##n: do_mode##n(&ui_data); break;
  switch(ui_data.mode){
  DEF_MODE(0)
  DEF_MODE(10)
  DEF_MODE(11)
  DEF_MODE(20)
  DEF_MODE(30)
  DEF_MODE(40)
  DEF_MODE(50)
  DEF_MODE(60)
  DEF_MODE(70)
  DEF_MODE(71)
  DEF_MODE(72)
  DEF_MODE(73)
  default:
    break;
 }
  
  ui_data.prev_mode=prev_mode;

  return &ui_data;
}
#  endif

#endif
