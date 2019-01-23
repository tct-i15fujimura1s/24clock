#include "libs.h"
#include "main.h"

char mode30_title[] = HK_YA HK_SI HK_DAKUTEN HK_RU HK_SI;

static unsigned int matrix_led_pattern[8];
static unsigned int l=0,u=0,c=0,i;
static unsigned int matrix_pattern[10][8]={
  {0x1800,0x3c00,0x7e00,0xdb00,0x9900,0x1800,0x1800,0x1800},
  {0x0018,0x003c,0x007e,0x00db,0x0099,0x0018,0x0018,0x0018},
  {0x1800,0x1800,0x1800,0x9900,0xdb00,0x7e00,0x3c00,0x1800},
  {0x0018,0x0018,0x0018,0x0099,0x00db,0x007e,0x003c,0x0018},
  {0x1800,0x0c00,0x0600,0xff00,0xff00,0x0600,0x0c00,0x1800},
  {0x0018,0x000c,0x0006,0x00ff,0x00ff,0x0006,0x000c,0x0018},
  {0x1800,0x3000,0x6000,0xff00,0xff00,0x6000,0x3000,0x1800},
  {0x0018,0x0030,0x0060,0x00ff,0x00ff,0x0060,0x0030,0x0018},
  {0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff},
  {0xff00,0xff00,0xff00,0xff00,0xff00,0xff00,0xff00,0xff00}};
void do_mode30(UI_DATA* ud){
  switch(ud->sw){  /*モード内でのキー入力別操作*/
  case KEY_LONG_C:  /* 中央キーの長押し */
    ud->mode=MODE_0; /* 次は，モード0に戻る */
    break;
  case KEY_SHORT_L:
    if(c==0){
      //コース1
      if(l<=1 && u==2){
	l++;
	for(i=0;i<8;i++)
	  matrix_led_pattern[i]=matrix_pattern[0][i];
      }else{
	for(i=0;i<8;i++)
	  matrix_led_pattern[i]=matrix_pattern[1][i];
      }
    }else{
      //コース2
      if((l==1 && (u==0 || u==3)) || (l==2 && (u==1 || u==3))){
	//進めた時
	l--;
	//スタート位置判定
	if(u==0 && l==0){
	  for(i=0;i<8;i++)
	    matrix_led_pattern[i]=matrix_pattern[8][i];
	}else{
	  for(i=0;i<8;i++)
	    matrix_led_pattern[i]=matrix_pattern[0][i];
	}
      }else{
	for(i=0;i<8;i++)
	  matrix_led_pattern[i]=matrix_pattern[1][i];
      }
    }
    break;
  case KEY_SHORT_R:
    if(c==0){
      if(0<l && u==2){
	l--;
	for(i=0;i<8;i++)
	  matrix_led_pattern[i]=matrix_pattern[2][i];
      }else{
	for(i=0;i<8;i++)
	  matrix_led_pattern[i]=matrix_pattern[3][i];
      }
    }else{
      if((l==0 && (u==0 || u==3)) || (l==1 && (u==1 || u==3))){	  
	for(i=0;i<8;i++)
	  matrix_led_pattern[i]=matrix_pattern[2][i];
	l++;
      }else{
	for(i=0;i<8;i++)
	  matrix_led_pattern[i]=matrix_pattern[3][i];
      }
    }
    break;
  case KEY_SHORT_U:
    if(c==0){
      if(u<=1){
	u++;
	for(i=0;i<8;i++)
	  matrix_led_pattern[i]=matrix_pattern[4][i];
      }else{
	for(i=0;i<8;i++)
	  matrix_led_pattern[i]=matrix_pattern[5][i];
      }
    }else{
      if((l==1 && u==1) || (l==2 && (u==2 || u==3))){
	for(i=0;i<8;i++)
	  matrix_led_pattern[i]=matrix_pattern[4][i];
	u--;
      }else{
	for(i=0;i<8;i++)
	  matrix_led_pattern[i]=matrix_pattern[5][i];
      }
    }
    break;
  case KEY_SHORT_D:
    if(c==0){
      if(0<u && l!=1){
	u--;
	if(u==0){
	  if(l==2){
	    for(i=0;i<8;i++)
	      matrix_led_pattern[i]=matrix_pattern[9][i];
	  }
	  if(l==0){
	    for(i=0;i<8;i++)
	      matrix_led_pattern[i]=matrix_pattern[8][i];
	  }
	}else{
	  for(i=0;i<8;i++)
	    matrix_led_pattern[i]=matrix_pattern[6][i];
	}
      }else{
	for(i=0;i<8;i++)
	  matrix_led_pattern[i]=matrix_pattern[7][i];
      }
    }else{
      if((l==2 && (u==1 || u==2)) || (l==1 && u==0) || (l==0 && u==3)){
	u++;
	if(u==4){
	  for(i=0;i<8;i++)
	    matrix_led_pattern[i]=matrix_pattern[9][i];
	}else{
	  for(i=0;i<8;i++)
	    matrix_led_pattern[i]=matrix_pattern[6][i];
	}
      }else{
	for(i=0;i<8;i++)
	  matrix_led_pattern[i]=matrix_pattern[7][i];
      }
      break;
      case KEY_SHORT_C:
	for(i=0;i<8;i++)
	  matrix_led_pattern[i]=matrix_pattern[8][i];
	if(c==0)
	  c=1;
	else c=0;
	l=0;
	u=0;
	break;
    }
  }
  if(ud->prev_mode!=ud->mode || sec_flag==TRUE){ 
    /* 他のモード遷移した時に実行 もしくは，1秒ごとに表示*/
    /*必要なら，何らかのモードの初期化処理*/
    lcd_clear();  //0123456789ABCDEF
    lcd_putstr(4,0,"ﾐﾁｦﾐﾂｹﾃﾈ"); /*モード2の初期表示*/
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
