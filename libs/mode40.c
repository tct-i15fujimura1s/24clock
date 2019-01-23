#include "libs.h"
#include "main.h"
#include "mode40.h"

static unsigned int pattern[8];
static unsigned int x=0,y=0;

void do_mode40(UI_DATA* ud){
  int i, col;
   if(ud->prev_mode!=ud->mode){  /* 他のモード遷移した時に実行 */
    /*必要なら，何らかのモードの初期化処理*/
    lcd_clear();
    lcd_putstr(0,0,"MODE4:OEKAKI");
   }
   
   if(tma_flag == TRUE){
     for(i = 0; i < 8; i++){//コピー
       matrix_led_pattern[i] = pattern[i];
     }
     matrix_led_pattern[x] |= 0x0101 << y; //(x,y)をオレンジ色にする
     tma_flag = FALSE;
   }
    
    /*モード4は，真中ボタンが押されたら，MODE0に戻る*/
  switch(ud->sw){    /*モード内でのキー入力別操作*/
  case KEY_LONG_C:/* 中央キーの長押し */
    for(i = 0; i < 8; i++)
      matrix_led_pattern[i] = 0x0000;
    ud->mode=MODE_0; /* 次は，モード0に戻る */
    break;
  case KEY_SHORT_L:
    x=(x+7)%8;
    break;
  case KEY_SHORT_R:
   x=(x+1)%8;
    break;
  case KEY_SHORT_D:
    y=(y+1)%8;
    break;
  case KEY_SHORT_U:
    y=(y+7)%8;
    break;
  case KEY_SHORT_C:
    col = pattern[x];
    lcd_putdec(0,1,5,col);
    if(col & (0x0001<<y)){//もし赤なら
      col &= ~(0x0001<<y);//赤を消す
      col |= (0x0100<<y);//緑をつける
    }
    else if(col & 0x0100<<y)//緑なら
      col &= ~(0x0100 <<y);//緑を消す
    else col |= (0x0001<<y);//ついてなかったら赤をつける
    pattern[x] = col;
  }
}

