#include "libs.h"
#include "main.h"
#include "mode30.h"


void do_mode30(UI_DATA* ud){
  switch(ud->sw){  /*モード内でのキー入力別操作*/
  case KEY_LONG_C:  /* 中央キーの長押し */
    ud->mode=MODE_0; /* 次は，モード0に戻る */
    break;
    
  case KEY_SHORT_L:
    if(l==0){
      matrix_led_pattern[0]=0x0018;
      matrix_led_pattern[1]=0x003c;
      matrix_led_pattern[2]=0x007e;
      matrix_led_pattern[3]=0x00db;
      matrix_led_pattern[4]=0x0099;
      matrix_led_pattern[5]=0x0018;
      matrix_led_pattern[6]=0x0018;
      matrix_led_pattern[7]=0x0018;
      l=1;
    }else{
      matrix_led_pattern[0]=0x1800;
      matrix_led_pattern[1]=0x3c00;
      matrix_led_pattern[2]=0x7e00;
      matrix_led_pattern[3]=0xdb00;
      matrix_led_pattern[4]=0x9900;
      matrix_led_pattern[5]=0x1800;
      matrix_led_pattern[6]=0x1800;
      matrix_led_pattern[7]=0x1800;
      l=0;
    }
    break;
    
  case KEY_SHORT_R:
    if(r==0){
      matrix_led_pattern[0]=0x0018;
      matrix_led_pattern[1]=0x0018;
      matrix_led_pattern[2]=0x0018;
      matrix_led_pattern[3]=0x0099;
      matrix_led_pattern[4]=0x00db;
      matrix_led_pattern[5]=0x007e;
      matrix_led_pattern[6]=0x003c;
      matrix_led_pattern[7]=0x0018;
      r=1;
    }else{
      matrix_led_pattern[0]=0x1800;
      matrix_led_pattern[1]=0x1800;
      matrix_led_pattern[2]=0x1800;
      matrix_led_pattern[3]=0x9900;
      matrix_led_pattern[4]=0xdb00;
      matrix_led_pattern[5]=0x7e00;
      matrix_led_pattern[6]=0x3c00;
      matrix_led_pattern[7]=0x1800;
      r=0;
    }
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
    lcd_putstr(3,0,"ﾎﾞﾀﾝｦｵｼﾃﾈ"); /*モード2の初期表示*/
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
