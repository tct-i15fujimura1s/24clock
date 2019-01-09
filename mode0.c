#include "libs.h"
#include "main.h"
#include "mode0.h"

TIME time = {0, 0, 0};

void do_mode0(UI_DATA* ud){
  static int matrix_scroll=FALSE;
  static int next_mode_data=MODE_0;
  int prev_next_mode_data;

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

  if(sec_flag==TRUE){ /* 1秒ごとの処理*/
    lcd_clear(); /* LCDをクリア */
    lcd_putstr(0,0,"CLOCK24");

    time_add_seconds(&time, 1);

    /* 24時間時計 */
    lcd_putchr(0, 1, time_to_string(&time));

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
