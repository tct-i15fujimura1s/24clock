#include "libs.h"
#include "main.h"

char mode30_title[] = HK_ME HK_I HK_RO; /* メイロ */

typedef enum {
  LEFT = 0,
  RIGHT = 1,
  UP = 2,
  DOWN = 3
} DIR;

typedef enum {
  RED = 1,
  GREEN = 2,
  ORANGE = 3
} COLOR;

static void copy(const unsigned char dest[8], unsigned char src[8]);
static void transpose(unsigned char matrix[8]);
static void invert_cols(unsigned char matrix[8]);
static void matrix_led_clear();
static void paint_red(const unsigned char matrix[8]);
static void paint_green(const unsigned char matrix[8]);
static void paint(const unsigned char pattern, DIR dir, COLOR color);

static unsigned int l=0,u=0,c=0,i; /* c:コース番号 */
static unsigned char arrow_pattern[8] = {0x18,0x3c,0x7e,0xdb,0x99,0x18,0x18,0x18}; /* ← */
static unsigned char rect_pattern[8] = {0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff}; /* ■ */
//static unsigned int matrix_pattern[10][8]={
//  {0x1800,0x3c00,0x7e00,0xdb00,0x9900,0x1800,0x1800,0x1800}, /* 緑 ← */
//  {0x0018,0x003c,0x007e,0x00db,0x0099,0x0018,0x0018,0x0018}, /* 赤 ← */
//  {0x1800,0x1800,0x1800,0x9900,0xdb00,0x7e00,0x3c00,0x1800}, /* 緑 → */
//  {0x0018,0x0018,0x0018,0x0099,0x00db,0x007e,0x003c,0x0018}, /* 赤 → */
//  {0x1800,0x0c00,0x0600,0xff00,0xff00,0x0600,0x0c00,0x1800}, /* 緑 ↑ */
//  {0x0018,0x000c,0x0006,0x00ff,0x00ff,0x0006,0x000c,0x0018}, /* 赤 ↑ */
//  {0x1800,0x3000,0x6000,0xff00,0xff00,0x6000,0x3000,0x1800}, /* 緑 ↓ */
//  {0x0018,0x0030,0x0060,0x00ff,0x00ff,0x0060,0x0030,0x0018}, /* 赤 ↓ */
//  {0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff,0xffff}, /* オレンジ ■ */
//  {0xff00,0xff00,0xff00,0xff00,0xff00,0xff00,0xff00,0xff00}}; /* 緑 ■ */
void do_mode30(UI_DATA* ud){
  unsigned char matrix[8];
  switch(ud->sw){  /*モード内でのキー入力別操作*/
  case KEY_LONG_C:  /* 中央キーの長押し */
    ud->mode=MODE_0; /* 次は，モード0に戻る */
    break;
  case KEY_SHORT_L:
    if(c==0){
      //コース1
      if(l<=1 && u==2){
        l++;
        paint(arrow_pattern, LEFT, GREEN);
      }else{
        paint(arrow_pattern, LEFT, RED);
      }
    }else{
      //コース2
      if((l==1 && (u==0 || u==3)) || (l==2 && (u==1 || u==3))){
        //進めた時
        l--;
        //スタート位置判定
        if(u==0 && l==0){
          paint(rect_pattern, LEFT, ORANGE);
        }else{
          paint(arrow_pattern, LEFT, GREEN);
        }
      }else{
        paint(arrow_pattern, LEFT, RED);
      }
    }
    break;
  case KEY_SHORT_R:
    if(c==0){
      if(0<l && u==2){
        l--;
        paint(arrow_pattern, RIGHT, GREEN);
      }else{
        paint(arrow_pattern, RIGHT, RED);
      }
    }else{
      if((l==0 && (u==0 || u==3)) || (l==1 && (u==1 || u==3))){
        paint(arrow_pattern, RIGHT, GREEN);
        l++;
      }else{
        paint(arrow_pattern, RIGHT, RED);
      }
    }
    break;
  case KEY_SHORT_U:
    if(c==0){
      if(u<=1){
        u++;
        paint(arrow_pattern, UP, GREEN);
      }else{
        paint(arrow_pattern, UP, RED);
      }
    }else{
      if((l==1 && u==1) || (l==2 && (u==2 || u==3))){
        paint(arrow_pattern, UP, GREEN);
        u--;
      }else{
        paint(arrow_pattern, UP, RED);
      }
    }
    break;
  case KEY_SHORT_D:
    if(c==0){
      if(0<u && l!=1){
        u--;
        if(u==0){
          if(l==2){
            paint(rect_pattern, LEFT, GREEN);
          }
          if(l==0){
            paint(rect_pattern, LEFT, ORANGE);
          }
        }else{
          paint(arrow_pattern, DOWN, GREEN);
        }
      }else{
        paint(arrow_pattern, DOWN, RED);
      }
    }else{
      if((l==2 && (u==1 || u==2)) || (l==1 && u==0) || (l==0 && u==3)){
        u++;
        if(u==4){
          paint(rect_pattern, LEFT, GREEN);
        }else{
          paint(arrow_pattern, DOWN, GREEN);
        }
      }else{
        paint(arrow_pattern, DOWN, RED);
      }
      break;
      case KEY_SHORT_C:
        paint(rect_pattern, LEFT, ORANGE);
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
    lcd_putstr(4,0,HK_MI HK_TI HK_WO HK_MI HK_TU HK_KE HK_TE HK_NE); /*モード2の初期表示*/
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


static void copy(const unsigned char dest[8], unsigned char src[8]){
  int i;
  for(i = 0; i < 8; i++) dest[i] = src[i];
}

static void transpose(unsigned char matrix[8]){
  int x, y;
  int bit;
  for(x = 0; x < 8; x++) for(y = 0; y < 8; y++) {
    bit = (matrix[x] >> y) & 1;
    matrix[x] = (matrix[x] & ~(1 << y)) | (((matrix[y] >> x) & 1) << y);
    matrix[y] = (matrix[x] & ~(1 << x)) | (bit << x);
  }
}

static void invert_cols(unsigned char matrix[8]){
  int i;
  char col;
  for(i = 0; i < 4; i++){
    col = matrix[i];
    matrix[i] = matrix[7 - i];
    matrix[7 - i] = col;
  }
}

static void matrix_led_clear(){
  int i;
  for(i = 0; i < 8; i++) matrix_led_pattern[i] = 0x0000;
}

static void paint_red(const unsigned char matrix[8]){
  int i;
  for(i = 0; i < 8; i++) matrix_led_pattern[i] |= (int) matrix[i];
}

static void paint_green(const unsigned char matrix[8]){
  int i;
  for(i = 0; i < 8; i++) matrix_led_pattern[i] |= ((int) matrix[i]) << 8;
}

static void paint(const unsigned char pattern, DIR dir, COLOR color){
  unsigned char matrix[8];
  copy(matrix, pattern);
  if(dir & RIGHT)
    invert_cols(matrix);
  if(dir & UP)
    transpose(matrix);
  matrix_led_clear();
  if(color & RED)
    paint_red(matrix);
  if(color & GREEN)
    paint_green(matrix);
}
