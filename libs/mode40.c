#include "libs.h"
#include "main.h"
#include "mode40.h"

static unsigned int matrix_led_pattern1[8][8];
static unsigned int matrix_led_pattern2[8][8];
static unsigned int matrix_led_pattern3[8][8];
static unsigned int matrix_led_pattern4[8];
unsigned int i;

void do_mode40(UI_DATA* ud){
   if(ud->prev_mode!=ud->mode){  /* 他のモード遷移した時に実行 */
    /*必要なら，何らかのモードの初期化処理*/
    lcd_clear();
    lcd_putstr(0,0,"painting"); /*モード1の初期表示*/

    //赤色に光る
    for(i=0;i<8;i++){
	matrix_led_pattern1[0][i] = 0x0001;
	matrix_led_pattern1[1][i] = 0x0002;
	matrix_led_pattern1[2][i] = 0x0004;
	matrix_led_pattern1[3][i] = 0x0008;
	matrix_led_pattern1[4][i] = 0x0010;
	matrix_led_pattern1[5][i] = 0x0020;
	matrix_led_pattern1[6][i] = 0x0040;
	matrix_led_pattern1[7][i] = 0x0080;
    }
    //どこを光らせるか 
    
    /*モード4は，真中ボタンが押されたら，MODE0に戻る*/
  switch(ud->sw){    /*モード内でのキー入力別操作*/
  case KEY_LONG_C:   /* 中央キーの長押し */
    ud->mode=MODE_0; /* 次は，モード0に戻る */
    break;
  }
}
}
