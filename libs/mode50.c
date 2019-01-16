void do_mode50(UI_DATA* ud){
  static int tempo=120;
  static int sound=1;

  if(ud->prev_mode!=ud->mode){  /* 他のモード遷移した時に実行 */
    /*必要なら，何らかのモードの初期化処理*/
    lcd_clear();
    lcd_putstr(0,0,"MODE1"); /*モード1の初期表示*/
    lcd_putstr(0,1,"SOUND=1"); /*モード1の初期表示*/
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
    break;
    
  case KEY_SHORT_L: 
    snd_stop();
    if(sound==1) snd_play("C");
    else snd_play("G");
    break;

  case KEY_SHORT_D: 
    snd_stop();
    if(sound==1) snd_play("D");
    else snd_play("^A_");
    break;

  case KEY_SHORT_R: 
    snd_stop();
    if(sound==1) snd_play("E");
    else snd_play("^B_");
    break;

  case KEY_SHORT_U: 
    snd_stop();
    if(sound==1) snd_play("F");
    else snd_play("^C_");
    break;
  }

}
