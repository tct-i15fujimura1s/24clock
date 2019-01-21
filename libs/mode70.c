#include "libs.h"
#include "main.h"

#define S_16 "                "

char mode70_title[] = HK_A HK_KU HK_SI HK_YO_SMALL HK_N;

//モード70: スタート画面
//モード71: ハイスコア閲覧
//モード72: ゲーム
//モード73: クリア画面

static unsigned int highscore = 0;

void do_mode70(UI_DATA* ud){
  if(ud->prev_mode!=ud->mode){  /* 他のモード遷移した時に実行 */
    /*必要なら，何らかのモードの初期化処理*/
    lcd_clear();
    lcd_putstr(1,0,"2" HK_TO HK_DAKUTEN HK_TU_SMALL HK_TO HK_HU HK_DAKUTEN HK_RA HK_SA HK_DAKUTEN HK_ONBIKI HK_SU HK_DAKUTEN); /*モード1の初期表示*/
    lcd_putstr(3,1,"> " HK_HA HK_SI HK_DAKUTEN HK_ME HK_RU " <  >>");
  }

  switch(ud->sw){  /*モード内でのキー入力別操作*/
  case KEY_LONG_C:  /* 中央キーの長押し */
    ud->mode=MODE_0; /* 次は，モード0に戻る */
    break;
    
  case KEY_SHORT_R:
    ud->mode=MODE_71; /*モード71に行く*/
    break;
    
  case KEY_SHORT_C:
    ud->mode=MODE_72; /*モード72に行く*/
    break;
    
  default:
    break;
  }
}

void do_mode71(UI_DATA *ud){
  if(ud->prev_mode!=ud->mode){
    lcd_clear();
    lcd_putstr(3,0,HK_HA HK_I HK_SU HK_KO HK_A);
    lcd_putstr(0,1,"<<");
    lcd_putdec(5,1,5,highscore);
  }
  
  switch(ud->sw){  /*モード内でのキー入力別操作*/
  case KEY_LONG_C:  /* 中央キーの長押し */
    ud->mode=MODE_0; /* 次は，モード0に戻る */
    break;
    
  case KEY_SHORT_L:
    ud->mode=MODE_70; /*モード70に戻る*/
    break;
  default:
    break;
  }
}

/*
 * 設計
 ** プレイヤー(2dot)
 *     プレイヤーは上半身、下半身の2つの赤いドットで表現される。
 *     プレイヤーの下半身の座標はstatic int playerX, playerYで制御される。
 *     playerXは0が一番左、playerYは0が一番上である。
 *     ↑ボタンを押すとジャンプする(SE要件)。←ボタンを押すと左に動く。→ボタンを押すと右に動く。
 ** スクロール
 *     playerX > 4かつmapX < 127の状態で右に動こうとするとスクロールする。左にはスクロールしない。
 *     現在の画面の右端のx座標はmapXで表される。
 *     スクロールすると、現在画面に表示されているものが1列分左にずれ、一番左の列は消える。playerXは変化しない。
 *     マップの次の列(mapX+1)が読み込まれ、マップが無かった(((mapX+1)&3)==0)場合は次を生成する。
 ** マップ
 *     マップはunsigned int[8]の配列に格納される。
 *     マップは1列毎に疑似乱数を用いて自動生成される。
 *     また、全体マップは実際には存在しないが、あるとしたときのプレイヤーの現在の位置がLCDの2行目に表示される。
 *     絶対位置は(playerX+mapX-7)で計算される。プレイヤーを表す"I"が表示される位置はそれを>>3した位置となる。
 *     0から14番目までのマップは自動生成であるが、15番目のマップは決まっている。
 *** 地面
 *     地面はオレンジ色で表現される。
 *     地面は、一つ前のmapから生成される。また、ある高さに地面がある場合、それより下はすべて地面になる。また、地面の高さは0以上4以下である。
 *     プレイヤーは地面がないと落ちる。そしてplayerY < -1になると死ぬ(SE要件)。
 *** アイテムの生成
 *     アイテムは、緑のドットで表現され、地面から1~4ドット離れた高さに生成される。
 *     アイテムを取るとスコアに加算される(SE要件)。
 ** ゴール
 *     ゴールは赤い縦長の旗であらわされる。
 *     ゴールに触れるとクリアとなり、ハイスコアの更新処理をしてMODE_71に移行する。
 */
static const unsigned int goalMap[] = {0x8080, 0x8080, 0xE080, 0xC080};

typedef struct {
  unsigned char U:1;
  unsigned char D:1;
  unsigned char L:1;
  unsigned char R:1;
  unsigned char C:1;
} KEY_DATA;

static int wait_frames = 0;
static int next_mode_data = 0;

static inline void showMessage(int frames, int x, char *message){
  lcd_putstr(0,1,S_16);
  lcd_putstr(x,1,message);
  wait_frames = frames;
}

static unsigned int count = 0;
static unsigned int nextFrame(){
  count = (count + 1) & 0x7;
  return !count;
}
static void die();
static unsigned int generateMapColumn();
static KEY_DATA getKeyData();
static unsigned int rand();

static inline void check_jumping();
static inline void check_motion();
static inline void paint();
static inline void goal();

#define RED(y) (0x0100 << (y))
#define GREEN(y) (0x0001 << (y))
#define ORANGE(y) (0x0101 << (y))
#define GROUND(p) ((((p) & 0xff00) >> 8) & (p))
#define IS_GROUND(x, y) (map[x] & ORANGE(y))
#define IS_ITEM(x, y) (map[x] & GREEN(y) & ~RED(y))
#define PLAYER_ABSOLUTE_X (playerX+mapX-7)


static const KEY_DATA key_null = {0, 0, 0, 0, 0};
static int score = 0, time = 0;
static int playerX = 0, playerVY = 1, playerY = 1, isJumping = FALSE, mapX = 7;
static unsigned int map[8];
static KEY_DATA key = {0, 0, 0, 0, 0};
void do_mode72(UI_DATA *ud){
  int i;
  key = getKeyData(key);
  
  if(ud->prev_mode!=ud->mode){
    //MODE71の初期表示
    lcd_clear();
    lcd_putstr(1,0,HK_SU HK_KO HK_A "    0 " HK_TA HK_I HK_MU "100");
    lcd_putstr(0,1,S_16);
    score = 0;
    time = 100;
    playerX = 0;
    playerVY = 1;
    playerY = 1;
    isJumping = FALSE;
    mapX = 7;
    key = key_null;
    
    //初期マップを生成
    map[0] = ORANGE(6) |  ORANGE(7);
    map[1] = ORANGE(6) | ORANGE(7);
    for(i = 2; i < 8; i++) map[i] = generateMapColumn(i);
  }
  
  if(ud->sw == KEY_LONG_C){//終了
    ud->mode = MODE_0;
  }

  //タイムを減らす
  if(sec_flag==TRUE){
    time--;
    if(time < 0){
      showMessage(20, 5, "TIME UP!");
      die();
    }
    sec_flag=FALSE;
  }
  
  if(tma_flag==TRUE && nextFrame()){
    //処理を止める (メッセージ表示など)
    if(wait_frames > 0){
      wait_frames--;
      return;
    }
    
    //遅延モード遷移
    if(next_mode_data!=0){
      ud->mode = next_mode_data;
      return;
    }
    
    check_jumping();
    check_motion();
    
    //描画など
    paint();

    key = key_null;

    tma_flag=FALSE;
  }
}

//これで合ってるかはわからないけれど・・・。まぁまぁかな
static unsigned int rand(){
  static unsigned int next = 0x0001;
  next = next * 17 + 91;
  return next;
}

static KEY_DATA getKeyData(KEY_DATA kd){
  int ki = ~(PDRB);
  kd.U |= ki & 0x80 ? 1 : 0;
  kd.D |= ki & 0x40 ? 1 : 0;
  kd.L |= ki & 0x20 ? 1 : 0;
  kd.R |= ki & 0x10 ? 1 : 0;
  kd.C |= ki & 0x08 ? 1 : 0;
  return kd;
}

//落下中の判定とジャンプしたかの判定
static void check_jumping(){
  int i;
  if(isJumping){//ジャンプ、または落下中
    if(playerY >= 9){//落ちた[足元の座標が9以上だった]ら死ぬ
      die();
      return;
    }

    //足元に地面があったらそこで止まる
    for(i = 0; i <= playerVY; i++){
      if(IS_GROUND(playerX, playerY + i + 1)){//足元が地面
	playerY += i;
	isJumping = FALSE;
	return;
      }
    }
    
    playerY += playerVY++;//加速度1[dot/frame^2]
    return;
  }

  if(!IS_GROUND(playerX, playerY + 1)){//足元に地面がない
    isJumping = TRUE;
    playerVY = 0;//速度0[dots/frame]
    return;
  }
  if(key.U){//ジャンプ
    isJumping = TRUE;
    playerVY = -2;//速度-2[dots/frame]
  }
}

//左右に動く判定とアイテム取得、スクロール
static void check_motion(){
  int i;
  if(key.R){//右に動く
    if(playerX == 7 || IS_GROUND(playerX + 1, playerY) || IS_GROUND(playerX + 1, playerY - 1)){//右は地面か画面端
      return;
    }

    //アイテム
    if(IS_ITEM(playerX + 1, playerY)){
      score += 100;
      map[playerX + 1] ^= GREEN(playerY);
    }
    if(IS_ITEM(playerX + 1, playerY - 1)){
      score += 100;
      map[playerX + 1] ^= GREEN(playerY - 1);
    }
    
    if(playerX > 4 && mapX < 127){//スクロールする
      mapX++;
      for(i = 0; i < 7; i++) map[i] = map[i + 1];//ずらす
      if(mapX < 124){//最後のマップ以外
	map[7] = generateMapColumn(7);//生成
      }else{//最後のマップ
	map[7] = goalMap[mapX & 3];//既定
      }
      return;
    }
    
    playerX++;
    
    if(PLAYER_ABSOLUTE_X >= 126){//ゴール
      goal();
    }

    return;
  }

  if(key.L){//左に動く
    if(playerX == 0 || IS_GROUND(playerX - 1, playerY) || IS_GROUND(playerX - 1, playerY - 1)){//左は地面か画面端
      return;
    }
    //アイテム
    if(IS_ITEM(playerX - 1, playerY)){
      score += 100;
      map[playerX - 1] ^= GREEN(playerY);
    }
    if(IS_ITEM(playerX - 1, playerY - 1)){
      score += 100;
      map[playerX - 1] ^= GREEN(playerY - 1);
    }
    playerX--;
  }
}

static void goal(){
  score += time * 10;
  if(score > highscore){
    highscore = score;
  }
  next_mode_data = MODE_73;
  showMessage(20, 0, "CONGRATURATIONS!");
}

static void paint(){
  int i;
  for(i = 0; i < 8; i++) matrix_led_pattern[i] = map[i];
  if(playerY >= 0 && playerY < 8) matrix_led_pattern[playerX] |= RED(playerY); //下半身
  if(playerY >= 1 && playerY < 8) matrix_led_pattern[playerX] |= RED(playerY - 1); //上半身
    
    lcd_putstr(0,1,"................");
    lcd_putstr(PLAYER_ABSOLUTE_X>>3,1,"I");
    lcd_putdec(4,0,5,score);
    lcd_putdec(13,0,3,time);
}

static unsigned int generateMapColumn(int n){
  unsigned int r = rand();
  unsigned int ground, col = 0;
  
  //地面
  if(GROUND(map[n - 2]) == 0x00){ //2つ前が穴なら
    if((r & 7) < 1){ //[0:2]1/8の確率で高さ4
      ground = 0x10;
    }else if((r & 7) < 3){ //[0:2]2/8の確率で高さ3
      ground = 0x20;
    }else if((r & 7) < 6){ //[0:2]3/8の確率で高さ2
      ground = 0x40;
    }else{ //[0]2/8の確率で高さ1
      ground = 0x80;
    }
  }else if(GROUND(map[n - 2]) == GROUND(map[n - 1])){ //平らなら
    if((r & 7) != 0){ //[0:2]:7/8の確率で平ら
      ground = GROUND(map[7]);
    }else{ //[0:2]:1/8の確率で斜め
      if(r & 8){ //[3]:1/2の確率で上がる
	ground = GROUND(map[7]) >> 1;
      }else{ //[3]:1/2の確率で下がる
	ground = GROUND(map[7]) << 1;
      }
    }
  }else if(GROUND(map[n - 2]) < GROUND(map[n - 1])){ //上りなら
    if((r & 7) < 3){//[0:2]3/8の確率で上り
      ground = GROUND(map[7]) >> 1;
    }else if((r & 7) < 5){//[0:2]2/8の確率で平ら
      ground = GROUND(map[7]);
    }else if((r & 7) < 7){//[0:2]2/8の確率で下り
      ground = GROUND(map[7]) << 1;
    }else{//[0:2]1/8の確率で2つ下
      ground = GROUND(map[7]) << 2;
    }
  }else{ //下りなら
    if((r & 7) < 4){//[0:2]4/8の確率で平ら
      ground = GROUND(map[7]);
    }else if((r & 7) < 6){//[0:2]2/8の確率で下り
      ground = GROUND(map[7]) << 1;
    }else{//[0:2]2/8の確率で上り
      ground = GROUND(map[7]) >> 1;
    }
  }
  ground &= 0xf8; //地面の高さは0から4
  while(ground != 0x00 && (ground & 0x80) == 0) ground |= ground << 1;//地面のライン以下を埋める
  col |= ground | (ground << 8);
  
  //アイテム
  if((r & 0x38) < 0x18){//[4:5]3/8の確率でアイテムを生成する
    col |= 0x0001 << (0 + ((r >> 6) & 3)); //[6]アイテムの高さは0から3
  }
  
  return col;
}

static void die(){
  time = -1;
  next_mode_data = MODE_73;
  tempo_compare = 3735 / 120;
  snd_play("CCAAGGF");
}

// time<0ならゲームオーバー、そうでなければクリア(score==highscoreならNEW RECORD!)
void do_mode73(UI_DATA *ud){
  if(ud->prev_mode!=ud->mode){
    lcd_clear();
    if(time < 0){
      lcd_putstr(0,0,"<< GAME OVER...");
      lcd_putstr(0,1,HK_MA HK_TA HK_NO HK_O HK_KO HK_SI HK_WO HK_O HK_MA HK_TI HK_SI HK_TE HK_O HK_RI HK_MA HK_SU);
    }else{
      lcd_putstr(0,0,"<< CLEAR!");
      lcd_putdec(10,0,5,score);
      if(score == highscore) lcd_putstr(5,1,"NEW RECORD!");
    }
  }
  
  switch(ud->sw){  /*モード内でのキー入力別操作*/
  case KEY_LONG_C:  /* 中央キーの長押し */
    ud->mode=MODE_0; /* 次は，モード0に戻る */
    break;
    
  case KEY_SHORT_L:
    ud->mode=MODE_70; /*モード70に戻る*/
    break;
  default:
    break;
  }
}
