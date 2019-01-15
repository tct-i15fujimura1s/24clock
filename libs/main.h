#ifndef MAIN_H
#define MAIN_H
extern volatile int sec_flag;
extern volatile int sec;
extern volatile int sec_flag;

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

extern void mode_go(UI_DATA *ud, int mode);

#endif
