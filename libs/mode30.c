#include "libs.h"
#include "main.h"
#include "mode30.h"
static unsigned int l=0,r=0,u=0,d=0;
static const unsigned int pattern[8] = {0x0018, 0x003c, 0x007e, 0x00db, 0x0099, 0x0018, 0x0018, 0x0018};

void do_mode30(UI_DATA* ud){
  int i;
  switch(ud->sw){  /*モード内でのキー入力別操作*/
  case KEY_LONG_C:  /* 中央キーの長押し */
    ud->mode=MODE_0; /* 次は，モード0に戻る */
    break;
    
  case KEY_SHORT_L:
    if(l==0){
      for(i = 0; i < 8; i++){
	matrix_led_pattern[i] = pattern[i];
      }
    }else{
      for(i = 0; i < 8; i++){
	matrix_led_pattern[i] = ((pattern[i] & 0xff) << 8) | (pattern[i] >> 8);
      }
    }
    l=!l;
    break;
    
  case KEY_SHORT_R:
    if(r==0){
      for(i = 0; i < 8; i++){
	matrix_led_pattern[7 - i] = pattern[i];
      }
    }else{
      for(i = 0; i < 8; i++){
	matrix_led_pattern[7 - i] = ((pattern[i] & 0xff) << 8) | (pattern[i] >> 8);
      }
    }
    r=!r;
    break;
    
  case KEY_SHORT_U:
    if(u==0){
      matrix_led_pattern[0]=0x0018;
      matrix_led_pattern[1]=0x000c;
      matrix_led_pattern[2]=0x0006;
      matrix_led_pattern[3]=0x00ff;
      matrix_led_pattern[4]=0x00ff;
      matrix_led_pattern[5]=0x0006;
      matrix_led_pattern[6]=0x000c;
      matrix_led_pattern[7]=0x0018;
      u=1;
    }else{
      matrix_led_pattern[0]=0x1800;
      matrix_led_pattern[1]=0x0c00;
      matrix_led_pattern[2]=0x0600;
      matrix_led_pattern[3]=0xff00;
      matrix_led_pattern[4]=0xff00;
      matrix_led_pattern[5]=0x0600;
      matrix_led_pattern[6]=0x0c00;
      matrix_led_pattern[7]=0x1800;
      u=0;
    }
    break;
    
  case KEY_SHORT_D:
    if(d==0){
      matrix_led_pattern[0]=0x0018;
      matrix_led_pattern[1]=0x0030;
      matrix_led_pattern[2]=0x0060;
      matrix_led_pattern[3]=0x00ff;
      matrix_led_pattern[4]=0x00ff;
      matrix_led_pattern[5]=0x0060;
      matrix_led_pattern[6]=0x0030;
      matrix_led_pattern[7]=0x0018;
      d=1;
    }else{
      matrix_led_pattern[0]=0x1800;
      matrix_led_pattern[1]=0x3000;
      matrix_led_pattern[2]=0x6000;
      matrix_led_pattern[3]=0xff00;
      matrix_led_pattern[4]=0xff00;
      matrix_led_pattern[5]=0x6000;
      matrix_led_pattern[6]=0x3000;
      matrix_led_pattern[7]=0x1800;
      d=0;
    }
    break;
  }
  if(ud->prev_mode!=ud->mode || sec_flag==TRUE){ 
    /* 他のモード遷移した時に実行 もしくは，1秒ごとに表示*/
    /*必要なら，何らかのモードの初期化処理*/
    lcd_clear();  //0123456789ABCDEF
    lcd_putstr(3,0,"\xCE\xDE\xC0\xDD\xA6\xB5\xBC\xC3\xC8"); /*モード2の初期表示: ボタンヲオシテネ*/
    //show_sec();
    sec_flag=FALSE;
  }
  
  /*モード2は，真中ボタンが押されたら，MODE0に戻る*/
  switch(ud->sw){    /*モード内でのキー入力別操作*/
  case KEY_LONG_C:   /* 中央キーの長押し */
    ud->mode=MODE_0; /* 次は，モード0に戻る */
   break;
  }
}
