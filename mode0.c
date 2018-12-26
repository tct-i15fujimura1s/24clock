#include "libs.h"
#include "main.h"
#include "mode0.h"

volatile int sec1=0;
volatile int sec10=5;
volatile int min1=9;
volatile int min10=5;
volatile int hour1=3;
volatile int hour10=2;


void do_mode0(UI_DATA* ud){
  static int matrix_scroll=FALSE;
  static int next_mode_data=MODE_0;
  int prev_next_mode_data;
  static char str[2];

  /*モード0で必ず実行するコードを記述*/
  prev_next_mode_data=next_mode_data;

  if(ud->prev_mode!=ud->mode){  /* 他のモードからモード0に遷移した時に実行 */
    /*必要なら，何らかのモードの初期化処理*/
    lcd_putstr(0,0,"CLOCK24"); /*モード0の初期表示*/
    next_mode_data=MODE_0;
    prev_next_mode_data=MODE_0;
    matrix_scroll=FALSE;
  }

  switch(ud->sw){  /*モード内でのキー入力別操作*/

  case KEY_SHORT_U: /* 上短押し */
    if(next_mode_data  < (MODE_OUT_OF_MAX-1)){
      next_mode_data++;
    }
    break;

  case KEY_SHORT_D: /* 下短押し */
    if(next_mode_data  > (MODE_OUT_OF_MIN+1)){
      next_mode_data--;
    }
    break;

  case KEY_SHORT_L: /* 左短押し */
    FLIP_LED_RED(); //赤色LEDのフリップを実行
    break;

  case KEY_SHORT_R: /* 右短押し */
    FLIP_LED_GREEN();
    break;

  case KEY_LONG_L: /* 左長押し */
    FLIP_LED_BLUE();
    break;

  case KEY_LONG_R: /* 右長押し */
    if(matrix_scroll==FALSE){//マトリクスLEDのスクロールフラグのフリップ
      matrix_scroll=TRUE;
    }else{
      matrix_scroll=FALSE;
    }

    break;

  case KEY_LONG_C: /* 中央キーの長押し */
    ud->mode=next_mode_data;/*次は，モード変更*/
    break;

  default: /*上記以外*/
    break;
  }

  /* モードの終了時に処理するコード */
  if((prev_next_mode_data!=next_mode_data) || sec_flag==TRUE){ 
    /* 次の希望するモードの値が変わった時の処理 */
    str[0]='0'+next_mode_data;
    str[1]='\0';
    lcd_putstr(11,0,str);
  }

  if(sec_flag==TRUE){ /* 1秒ごとの処理*/
    lcd_clear(); /* LCDをクリア */
    lcd_putstr(0,0,"CLOCK24");

    
	      sec1++;
	      if(sec1 == 10) {
		sec10++;
		sec1 = 0;
	      }
	      if(sec10 == 6) {
		min1++;
		sec10 = 0;
	      }
	      if(min1 == 10) {
		min10++;
		min1 = 0;
	      }
	      if(min10 == 6) {
		hour1++;
		min10 = 0;
	      }
	      if(hour1 == 10) {
		hour10++;
		hour1 = 0;
	      }
	      if(hour10 == 2 && hour1 == 4) {
		hour1 = 0;
		hour10 = 0;
	      }

    /* 24時間時計 */
    lcd_putchr(0, 1, '0' + hour10);
    lcd_putchr(1, 1, '0' + hour1);
    lcd_putstr(2, 1, ":  :");
    lcd_putchr(3, 1, '0' + min10);
    lcd_putchr(4, 1, '0' + min1);
    lcd_putchr(6, 1, '0' + sec10);
    lcd_putchr(7, 1, '0' + sec1);

    /*コメント：ここでは，LCDの再描画処理を1秒ごとに行っている。        */
    /*これは，万が一，予期せぬノイズで，LCDの表示に誤動作が発生しても， */
    /*1秒後には，回復させるという効果を期待している。ハードの世界では， */
    /*いくら工夫しても，防ぎようが無いノイズがあったりするのです…。    */
    /*不具合の発生確率は，「コストをある程度かければ」下げることが可能。*/

    sec_flag=FALSE;
  }

}

/* モード1: 24時間時計の時刻設定 */
void do_mode1(UI_DATA *ud) {
  
  static int matrix_scroll=FALSE;
  static int next_mode_data=MODE_0;
  int prev_next_mode_data;
  static char str[2];

  /*モード0で必ず実行するコードを記述*/
  prev_next_mode_data=next_mode_data;


  
  
  if(ud->prev_mode!=ud->mode){  /* 他のモードからモード0に遷移した時に実行 */
    /*必要なら，何らかのモードの初期化処理*/
    lcd_putstr(0,0,"MODE0->MODE1"); /*モード1の初期表示*/
    next_mode_data=MODE_0;
    prev_next_mode_data=MODE_0;
    matrix_scroll=FALSE;
  }

  
}
