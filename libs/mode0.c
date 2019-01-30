#define MODE0_C
#include "libs.h"
#include "main.h"

char mode0_title[] = HK_ME HK_NI HK_YU_SMALL HK_ONBIKI;

static int mode_add(int mm, int n){
  mm += n;
  if(mm < MODE_10) mm += MODE_OUT_OF_MAX / 10 * 10; // MODE10より小さくなりそうならMODEの10刻み最大値を足す
  else if(mm > MODE_OUT_OF_MAX) mm -= MODE_OUT_OF_MAX / 10 * 10; // MODEの最大値よりはみ出そうならMODEのの10刻み最大値を引く
  return mm;
}

void do_mode0(UI_DATA* ud){
  static int next_mode_data=MODE_10, next_mode_data2 = MODE_20;
  static int cycle = 0;
  int i;

  if(ud->prev_mode!=ud->mode){  /* 他のモードからモード0に遷移した時に実行 */
    /*必要なら，何らかのモードの初期化処理*/
    for(i = 0; i < 8; i++) matrix_led_pattern[i] = 0x0000;
    DISABLE_LED_RED();
    DISABLE_LED_GREEN();
    DISABLE_LED_BLUE();
    /*ロゴを表示*/
    matrix_led_pattern[0] = 0x1FF0;
    matrix_led_pattern[1] = 0x0210;
    matrix_led_pattern[2] = 0x0410;
    matrix_led_pattern[3] = 0x02E0;
    matrix_led_pattern[4] = 0x1F0E;
    matrix_led_pattern[5] = 0x6015;
    matrix_led_pattern[6] = 0x8015;
    matrix_led_pattern[7] = 0xE006;
  }

  if(tma_flag==TRUE){
    if(cycle == 0){
      lcd_clear();
      lcd_putstr(0,0,"-> MODE");
      lcd_putstr(9,0,tithe_mode_titles[next_mode_data/10]);
      lcd_putstr(9,1,tithe_mode_titles[next_mode_data2/10]);
      lcd_putstr(0,1,"   MODE");
      lcd_putudec(7,0,1,next_mode_data/10);
      lcd_putudec(7,1,1,next_mode_data2/10);
    }
    cycle = (cycle + 1) & 3;
    tma_flag=FALSE;
  }

  switch(ud->sw){  /*モード内でのキー入力別操作*/

  case KEY_SHORT_U: /* 上短押し */
    next_mode_data2 = next_mode_data;
    next_mode_data = mode_add(next_mode_data,-10);
    break;

  case KEY_SHORT_D: /* 下短押し */
    next_mode_data = mode_add(next_mode_data,+10);
    next_mode_data2 = mode_add(next_mode_data,+10);
    break;
    
  case KEY_LONG_C: /* 中央キーの長押し */
    ud->mode=next_mode_data;/*次は，モード変更*/
    break;

  default: /*上記以外*/
    break;
  }

}
