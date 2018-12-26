#include "lcd_util.h"
#include "lcd.h"

/*
 * LCDの指定された座標へ整数を出力する関数
 *   使用方法：座標と整数を使用し、LCDへ整数を出力します。
 *   注意事項：表示させる値は，integerの整数だけ使用できます。
 *   引数：
 *     x: x座標（0〜15までの16キャラクタ）
 *     y: y座標（0〜1までの2行）
 *     l: 表示する桁数
 *     v: 表示したい数値
 */
void lcd_putdec(int x, int y, int l, int v) {

  char buf[l+1];
  int  i;

  for (i=l-1; i>=0 && v!=0; i--) {
    buf[i] =(v % 10) + '0';
    v /= 10;
  }

  for (; i>=0; i--) {
    buf[i] = ' ';
  }
  buf[l] = '\0';
  lcd_putstr(x, y, buf);

}

void lcd_putudec(int x, int y, int l, unsigned int v) {

  char buf[l+1];
  int  i;

  for (i=l-1; i>=0 && v!=0; i--) {
    buf[i] =(v % 10) + '0';
    v /= 10;
  }

  for (; i>=0; i--) {
    buf[i] = ' ';
  }
  buf[l] = '\0';
  lcd_putstr(x, y, buf);

}


void lcd_key_debug_out(int x,int y,unsigned char key_data)
{
  char key_state[6];

  /*参考として，LCDにキーの状態をデバッグ表示*/
  key_state[0]=((key_data & (1<<7) )==0) ? '*':'U' ; /*Up*/
  key_state[1]=((key_data & (1<<6) )==0) ? '*':'D' ; /*Down*/
  key_state[2]=((key_data & (1<<5) )==0) ? '*':'L' ; /*Left*/
  key_state[3]=((key_data & (1<<4) )==0) ? '*':'R' ; /*Right*/
  key_state[4]=((key_data & (1<<3) )==0) ? '*':'C' ; /*Center*/
  key_state[5]='\0';
  lcd_putstr(x,y,key_state);
}


void lcd_clear(void){
                //0123456789ABCDEF
  lcd_putstr(0,0,"                ");
  lcd_putstr(0,1,"                ");
}
