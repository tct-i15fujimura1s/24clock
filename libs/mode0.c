#include "libs.h"
#include "main.h"
#include "mode0.h"

static int mode_add(int mm, int n){
  mm += n;
  if(mm < MODE_10) mm += MODE_OUT_OF_MAX / 10 * 10; // MODE10より小さくなりそうならMODEの10刻み最大値を足す
  else if(mm > MODE_OUT_OF_MAX) mm -= MODE_OUT_OF_MAX / 10 * 10; // MODEの最大値よりはみ出そうならMODEのの10刻み最大値を引く
  return mm;
}

void do_mode0(UI_DATA* ud){
  static int next_mode_data=MODE_10;
  int i;

  if(ud->prev_mode!=ud->mode){  /* 他のモードからモード0に遷移した時に実行 */
    /*必要なら，何らかのモードの初期化処理*/
    for(i = 0; i < 8; i++) matrix_led_pattern[i] = 0x0000;
    DISABLE_LED_RED();
    DISABLE_LED_GREEN();
    DISABLE_LED_BLUE();
  }

  if(tma_flag==TRUE){
    lcd_clear();
    lcd_putstr(0,0,"-> MODE"); /*モード0の初期表示*/
    lcd_putstr(0,1,"   MODE");
    lcd_putudec(7,0,1,next_mode_data/10);
    lcd_putudec(7,1,1,mode_add(next_mode_data,10)/10);
    tma_flag=FALSE;
  }

  switch(ud->sw){  /*モード内でのキー入力別操作*/

  case KEY_SHORT_U: /* 上短押し */
    next_mode_data = mode_add(next_mode_data,-10);
    break;

  case KEY_SHORT_D: /* 下短押し */
    next_mode_data = mode_add(next_mode_data,+10);
    break;
    
  case KEY_LONG_C: /* 中央キーの長押し */
    ud->mode=next_mode_data;/*次は，モード変更*/
    break;

  default: /*上記以外*/
    break;
  }

}
