void do_mode20(UI_DATA* ud){

  if(ud->prev_mode!=ud->mode || sec_flag==TRUE){ 
    /* 他のモード遷移した時に実行 もしくは，1秒ごとに表示*/
    /*必要なら，何らかのモードの初期化処理*/
    lcd_clear();  //0123456789ABCDEF
    lcd_putstr(0,0,"MODE2:secｦ ﾋｮｳｼﾞ"); /*モード2の初期表示*/
    show_sec();
    sec_flag=FALSE;
  }

  /*モード2は，真中ボタンが押されたら，MODE0に戻る*/
  switch(ud->sw){    /*モード内でのキー入力別操作*/
  case KEY_LONG_C:   /* 中央キーの長押し */
    ud->mode=MODE_0; /* 次は，モード0に戻る */
    break;
  }
}
