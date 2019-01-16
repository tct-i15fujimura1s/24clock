#include "libs.h"
#include "main.h"
#include "mode20.h"

void do_mode20(UI_DATA* ud){
  static int tempo=120;

  if(ud->prev_mode!=ud->mode){  /* 他のモード遷移した時に実行 */
    /*必要なら，何らかのモードの初期化処理*/
    lcd_clear();
    lcd_putstr(0,0,"MODE1"); /*モード1の初期表示*/
    lcd_putstr(0,1,"A    =120"); /*モード1の初期表示*/
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

  case KEY_SHORT_L: /* 左のキーが押されたら,演奏開始*/
    snd_play("CDEFEDC EFG^A_GFE C C C C !C!C!D!D!E!E!F!FEDC");
    break;

  case KEY_SHORT_R: /* 右のキーが押されたら,演奏終了*/
    snd_stop();
    break;

  case KEY_SHORT_U: /* テンポUP */
    tempo+=10;
    if(tempo>=240)
      tempo=240;
    tempo_compare = 3735 / tempo;
    lcd_putdec(6,1,3,tempo);
    break;

  case KEY_SHORT_D: /* テンポDOWN */
    tempo-=10;
    if(tempo<=60)
      tempo=60;
    tempo_compare = 3735 / tempo;
    lcd_putdec(6,1,3,tempo);
    break;
  }

}
