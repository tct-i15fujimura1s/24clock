#include "libs.h"
#include "main.h"
#include "mode50.h"

char mode50_title[] = HK_HI HK_HANDAKUTEN HK_A HK_NO;

void do_mode50(UI_DATA* ud){
  static int tempo=120;
  static int sound=1;
  if(cnt[0]==16) cnt[0]=0;
  else if(cnt[0]!=0) cnt[0]++;
  
  if(cnt[1]==16) cnt[1]=0;
  else if(cnt[1]!=0) cnt[1]++;
  
  if(cnt[2]==16) cnt[2]=0;
  else if(cnt[2]!=0) cnt[2]++;
  
  if(cnt[3]==16) cnt[3]=0;
  else if(cnt[3]!=0) cnt[3]++;
  
  if(cnt[4]==16) cnt[4]=0;
  else if(cnt[4]!=0) cnt[4]++;
  
  if(cnt[5]==16) cnt[5]=0;
  else if(cnt[5]!=0) cnt[5]++;
  
  if(cnt[6]==16) cnt[6]=0;
  else if(cnt[6]!=0) cnt[6]++;
  
  if(cnt[7]==16) cnt[7]=0;
  else if(cnt[7]!=0) cnt[7]++;
  
  if(cnt[8]==16) cnt[8]=0;
  else if(cnt[8]!=0) cnt[8]++;
  
  if(cnt[9]==16) cnt[9]=0;
  else if(cnt[9]!=0) cnt[9]++;
  
  if(cnt[10]==16) cnt[10]=0;
  else if(cnt[10]!=0) cnt[10]++;

  if(ud->prev_mode!=ud->mode){  /* 他のモード遷移した時に実行 */
    /*必要なら，何らかのモードの初期化処理*/
    lcd_clear();
    lcd_putstr(0,0,"MODE50"); /*モード1の初期表示*/
    lcd_putstr(0,1,"SOUND="); /*モード1の初期表示*/
    lcd_putdec(6,1,1,sound);
    tempo=120;
    tempo_compare = 3735 / tempo; /* 1000/(tempo * 16 / 60) を展開 */
  }

  /*モード1は，真中ボタンが押されたら，MODE0に戻るだけの単純な処理*/
  /*それに，beta2のバージョンでは，音楽再生機能の起動部分を追加*/
  /*但し，main関数内で，キーのデバッグ表示を行っている*/
  switch(ud->sw){  /*モード内でのキー入力別操作*/
  case KEY_LONG_C:  /* 中央キーの長押し */
    ud->mode=MODE_0; /* 次は，モード0に戻る */
    break;

  case KEY_SHORT_C:
    if(sound==1) sound=2;
    else sound=1;
    lcd_putdec(6,1,1,sound);

  default:

    if((PDRB & 0x28) == 0x00 ){
      if(cnt[4]==0){
	if(sound==1) snd_play("#C");
	else snd_play("#G");
	cnt[4]++;
      }
    }
    
    else if((PDRB & 0x48) == 0x00 ){
      if(cnt[5]==0){
	if(sound==1) snd_play("#D");
	else snd_play("#^A_");
	cnt[5]++;
      }
    }
    
    else if((PDRB & 0x18) == 0x00 ){
      if(cnt[6]==0){
	if(sound==1) snd_play("#E");
	else snd_play("#^B_");
	cnt[6]++;
      }
    }
    
    else if((PDRB & 0x88) == 0x00 ){
      if(cnt[7]==0){
	if(sound==1) snd_play("#F");
	else snd_play("#^C_");
	cnt[7]++;
      }
    }

    else if((PDRB & 0x60) == 0x00 ){
      if(cnt[8]==0){
	if(sound==2) snd_play("^D_");
	cnt[8]++;
      }
    }
    
    else if((PDRB & 0x30) == 0x00 ){
      if(cnt[9]==0){
	if(sound==2) snd_play("^E_");
	cnt[9]++;
      }
    }
    
    else if((PDRB & 0xa0) == 0x00 ){
      if(cnt[10]==0){
	if(sound==2) snd_play("^F_");
	cnt[10]++;
      }
    }
    else if((PDRB & 0x20) == 0x00 ){
      if(cnt[0]==0 && cnt[8]==0 && cnt[9]==0 && cnt[10]==0 && cnt[4]==0){
	if(sound==1) snd_play("C");
	else snd_play("G");
	cnt[0]++;
      }
    }
    else if((PDRB & 0x40) == 0x00 ){
      if(cnt[1]==0 && cnt[8]==0 && cnt[5]==0){
	if(sound==1) snd_play("D");
	else snd_play("^A_");
	cnt[1]++;
      }
    }
    
    else if((PDRB & 0x10) == 0x00 ){ 
      if(cnt[2]==0 && cnt[9]==0 && cnt[6]==0){
	if(sound==1) snd_play("E");
	else snd_play("^B_");
	cnt[2]++;
      }
    }
    else if((PDRB & 0x80) == 0x00 ){
      if(cnt[3]==0 && cnt[10]==0 && cnt[7]==0){
	if(sound==1) snd_play("F");
	else snd_play("^C_");
	cnt[3]++;
      }
    }
    else{
      int ln=0;
      while(ln<=10){
	cnt[ln]=0;
	ln++;
      }
    }
  }
}
