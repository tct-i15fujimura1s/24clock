#include "libs.h"
#include "main.h"
#include "mode10.h"

volatile TIME time = {0, 0, 0};

volatile int cursor_position = 0;

/* モード1: 24時間時計の時刻設定 */
void do_mode10(UI_DATA* ud){
  if(ud->prev_mode!=ud->mode){  /* 他のモードからモード10に遷移した時に実行 */
    /*必要なら，何らかのモードの初期化処理*/
    lcd_putstr(0,0,"MODE1:SET24CLOCK"); /*モード10の初期表示*/
    lcd_putstr(10,1,"-->\xc4\xb9\xb2");
    lcd_clear();
    lcd_cursor(TRUE);
    lcd_blink(TRUE);
  }

  switch(ud->sw){  /*モード内でのキー入力別操作*/
  case KEY_SHORT_U: /* 上短押し */
  case KEY_SHORT_D: /* 下短押し */
  {
    int delta = (ud->sw == KEY_SHORT_U ? 1 : -1) * (cursor_position & 1 ? 1 : 10);
    if(cursor_position < 2){
      time_add_hours(&time, delta);
    }else if(cursor_position < 4){
      time_add_minutes(&time, delta);
    }else{
      time_add_seconds(&time, delta);
    }
  }
    break;
  
  case KEY_SHORT_L: /* 左短押し */
    cursor_position = (cursor_position + 5) % 6;
    break;

  case KEY_SHORT_R: /* 右短押し */
    cursor_position = (cursor_position + 1) % 6;
    break;

  case KEY_LONG_R: /* 右長押し */
    lcd_cursor(FALSE);
    lcd_blink(FALSE);
    ud->mode = MODE_11; /* 時計画面に */
    break;

  case KEY_LONG_C: /* 中央キーの長押し */
    lcd_cursor(FALSE);
    lcd_blink(FALSE);
    ud->mode=MODE_0; /* メニューに移動 */
    break;

  default: /*上記以外*/
    break;
  }

  if(sec_flag==TRUE){
    /* 24時間時計 */
    lcd_putstr(0,1,"  :  :");
    lcd_putudec(0,1,2,time.hours);
    lcd_putudec(3,1,2,time.minutes);
    lcd_putudec(6,1,2,time.seconds);

    /* カーソルの表示 */
    int value, real_cursor_position;
    if(cursor_position < 2){
      value = time.hours;
      real_cursor_position = cursor_position;
    }else if(cursor_position < 4){
      value = time.minutes;
      real_cursor_position = cursor_position + 1;
    }else{
      value = time.seconds;
      real_cursor_position = cursor_position + 2;
    }
    if((cursor_position & 1) == 0){
      value *= 10;
    }
    lcd_putudec(real_cursor_position,1,1,value);

    /*コメント：ここでは，LCDの再描画処理を1秒ごとに行っている。        */
    /*これは，万が一，予期せぬノイズで，LCDの表示に誤動作が発生しても， */
    /*1秒後には，回復させるという効果を期待している。ハードの世界では， */
    /*いくら工夫しても，防ぎようが無いノイズがあったりするのです…。    */
    /*不具合の発生確率は，「コストをある程度かければ」下げることが可能。*/

    sec_flag=FALSE;
  }

}

/* モード1: 24時間時計 */
void do_mode11(UI_DATA *ud) {
  if(ud->prev_mode != ud->mode){
    lcd_clear(); /* LCDをクリア */
    lcd_putstr(0,0,"MODE1:24CLOCK");
    //lcd_putstr(0,2,"\xbe\xaf\xc3\xb2<--");
  }
  
  // 1秒ごと
  if(sec_flag==TRUE) { /* 1秒ごとの処理*/
    
    time_add_seconds(&time, 1); /* 時刻を一秒増やす */

    /* 24時間時計 */
    lcd_putstr(0,1,"  :  :");
    lcd_putudec(0,1,2,time.hours);
    lcd_putudec(3,1,2,time.minutes);
    lcd_putudec(6,1,2,time.seconds);
    
    sec_flag = FALSE;
  }
  
}
