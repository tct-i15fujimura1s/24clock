#include "libs.h"
#include "main.h"
#include "mode30.h"
static unsigned int matrix_led_pattern[8];
static unsigned int l=0,r=0,u=0,d=0;
#pragma interrupt
void int_timerv(void){         /* 約1[msec] ごとに呼び出されるようにする*/
  static unsigned int column=0;
      unsigned int i,p;
       /*スピーカ周りの値*/

       //卒業研究で作成してくれたコードから改変。
       //tempoを変更できるように改造 by T.nitta
       //下記のtempoやtempo_compareをconstでなく，変数にする。
       //但し，この計算をボトムハーフで毎回実行すると,処理が重いので
       //トップハーフで計算を行う。
       //       const int tempo=120;
       //       const int tempo_compare = 1000/(tempo * 16 / 60);
       //       const int tempo_compare = 3735 / tempo ;
       static int tempo_count=0;

       TCSRV &= ~0xe0;         /* タイマVの割り込みフラグクリア*/
       tmv_flag=TRUE;
       /*int_timervは，割込禁止のまま,全力動作*/

       /* 1ms ごとにハードリアルタイムで動作させたい処理(matrix_led周り)*/

       column=(column+1) & 0x0007; /* column=(column++)&0x0007は，NGです…  */ 
       ENABLE_MATRIX_LATCH();      /*ラッチを有効にし，舞台裏でD-FFをセット */

       /*16bit (1列分) をシリアル転送*/
       for(p=matrix_led_pattern[column],i=0;i<16;i++,p<<=1){        
	 if((p&0x8000)==0){
	   DISABLE_MATRIX_SIN();
	 }else{
	   ENABLE_MATRIX_SIN();
	 }
	 SET_H_MATRIX_SCLK();SET_L_MATRIX_SCLK();   /* posエッジ一つ*/
       }

       ENABLE_MATRIX_BLANK();   /*以下の筒抜けと列変更は表示させない*/
       DISABLE_MATRIX_LATCH();  ENABLE_MATRIX_LATCH();  /*一瞬筒抜け*/
       SELECT_MATRIX_COLUMN(column);               /*j番目の列に変更*/
       DISABLE_MATRIX_BLANK();                          /*点灯させる*/


       /*スピーカ関連処理*/
       if(snd_check()==TRUE){
	 if(tempo_compare < ++tempo_count){
	   tempo_count = 0;
	   tempo_flag = TRUE;
	 }
       }


#ifdef DEBUG
       { /*ボトムハーフ処理の余力チェック用コード*/
	 volatile int loop;
	 for(loop=0;loop<10;loop++);
	 /*とある条件下での挙動(但し，その後にキー入力のコードを追加)*/
	 /*loop<461 :10分程度では,破綻を確認できず(但し，検証回数は1回だけ)*/
       }

       if((TCSRV & 0xe0)!=0){ /* 割り込みフラグの状態チェック */
	 ENABLE_LED_RED();    /* もし割り込みフラグが立ってたら赤色LED点灯*/
       }
#endif
}
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
