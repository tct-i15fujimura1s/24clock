#include "libs.h"
#include "main.h"
#include "mode60.h"

char mode60_title[] = HK_O HK_SE HK_RO;

/*よく使うかもしれない文字列を定義*/
#define S_ANATA HK_A HK_NA HK_TA
#define S_KONPYU_TA HK_KO HK_N HK_HI HK_HANDAKUTEN HK_YU_SMALL HK_ONBIKI HK_TA /*Are you happy?*/
#define S_PASU HK_HA HK_HANDAKUTEN HK_SU
#define S_UTU HK_U HK_TU
#define S_NIA HK_NI HK_A /*カーソルのメタファー*/
#define S_16 "                " /*スペース×16*/

enum TURN {
  TURN_RED,
  TURN_GREEN
};

typedef struct { /*オーバーフロー前提なので3bit×2では足りない*/
  char y;
  char x;
} POS;

static enum TURN current_turn = TURN_RED;

static unsigned int matrix[8];

#define IS_RED(p) (matrix[(int) (p.x)] & 0x1 << (p.y))
#define IS_GREEN(p) (matrix[(int) (p.x)] & 0x100 << (p.y))
#define IS_EITHER(p) (matrix[(int) (p.x)] & 0x101 << (p.y))

static inline void SET_GREEN(POS p){
  matrix[(int) p.x] = (matrix[(int) p.x] & ~(0x1 << p.y)) | (0x100 << p.y);
}

static inline void SET_RED(POS p){
  matrix[(int) p.x] = (matrix[(int) p.x] & ~(0x100 << p.y)) | (0x1 << p.y);
}

static int wait_frames = 0;

static POS pointer;
static int show_pointer = FALSE;

static void refresh();
static int put(enum TURN turn, POS p);
static int flip(enum TURN turn, POS p, POS d);
static void set(enum TURN turn, POS p);

static int cycle = 0;
static void com_routine();

void do_mode60(UI_DATA *ui_data){
  if(ui_data->prev_mode != ui_data->mode) {
    //マトリックスLEDの準備
    //オセロ盤の初期化
    //((unsigned short *) red)[0] = 0x0000;
    matrix[0] = matrix[1] = matrix[2] = 0x0000;
    matrix[3] = 0x1008;
    matrix[4] = 0x0810;
    matrix[5] = matrix[6] = matrix[7] = 0x0000;
  }
  
  switch(ui_data->sw){
  case KEY_LONG_R: 	//DEBUG
    set(TURN_RED, pointer);
    break;
  case KEY_LONG_L: 	//DEBUG
    set(TURN_GREEN, pointer);
    break;
  case KEY_SHORT_U:
    pointer.y = (pointer.y + 7) & 7;
    break;
  case KEY_SHORT_D:
    pointer.y = (pointer.y + 1) & 7;
    break;
  case KEY_SHORT_L:
    pointer.x = show_pointer ? (pointer.x + 7) & 7 : !pointer.x;
    break;
  case KEY_SHORT_R:
    pointer.x = show_pointer ? (pointer.x + 1) & 7 : !pointer.x;
    break;
  case KEY_SHORT_C:
    if(show_pointer){
      if(put(TURN_RED, pointer)){
	current_turn = TURN_GREEN;
      }else{
	lcd_putstr(0,1,S_16);
	lcd_putstr(0,1,HK_SO HK_KO HK_NI HK_HA " " HK_U HK_TE HK_MA HK_SE HK_N);
	wait_frames = 10;
      }
      show_pointer = FALSE;
      pointer.x = pointer.y = 0;
    }else{
      if(pointer.x == 0){
	pointer.x = pointer.y = 0;
	show_pointer = TRUE;
      }else{
	current_turn = TURN_GREEN;
	lcd_putstr(0, 1, S_16);
	lcd_putstr(0, 1, S_ANATA ":" S_PASU);
	wait_frames = 10;
	//TODO: pass
      }
    }
    break;
  case KEY_LONG_C:
    DISABLE_LED_RED();
    DISABLE_LED_GREEN();
    ui_data->mode = MODE_0;
    break;
  default:
    break;
  }
  
  static unsigned int frame = 0;
  if(tma_flag){
    frame++;
    if(wait_frames > 0){
      wait_frames--;
      return;
    }
    if(frame & 4) refresh();
    if(frame & 8 && current_turn == TURN_GREEN){
      com_routine();
    }
    tma_flag = FALSE;
  }
}

static void refresh(){
  lcd_clear();
  lcd_putstr(0,0,"MODE6:" HK_O HK_SE HK_RO); // 「オセロ」
  if(show_pointer)lcd_putstr(14,0,IS_RED(pointer)?IS_GREEN(pointer)?"RG":"R ":IS_GREEN(pointer)?" G":"  ");//debug: both/red/green/none
  if(current_turn == TURN_RED){
    lcd_putstr(0,1,"   " S_UTU "    " S_PASU); // 「ウツ」「パス」
    if(show_pointer){
      lcd_putstr(0, 1, S_NIA " " S_UTU "      UDLRC");
    }else{
      lcd_putstr(13, 1, "LRC");
      lcd_putstr(pointer.x == 0 ? 0 : 6, 1, S_NIA); //「ﾆｱ」
    }
    DISABLE_LED_GREEN();
    ENABLE_LED_RED();
  }else{
    DISABLE_LED_RED();
    ENABLE_LED_GREEN();
  }
  int i;
  for(i = 0; i < 8; i++) matrix_led_pattern[i] = matrix[i];
  if(show_pointer){
    matrix_led_pattern[(int) pointer.x] |= 0x101 << pointer.y;
  }
}

static int put(enum TURN turn, POS p){
  if(IS_RED(p) || IS_GREEN(p)) return FALSE; //すでにある
  int i, flipped = 0;
  POS d;
  // 8方向にひっくり返る石を計算
  for(i = 8; i != 0; i--){
    d.x = (i%3+1)%3-1;
    d.y = (i/3+1)%3-1;
    flipped += flip(turn, p, d);
  }
  if(flipped != 0) set(turn, p); //もし1つでもひっくりかえせていれば置く
  return flipped;
}

#define IS_WITHIN(x, a, b) ((x) >= (a) && (x) <= (b))
#define IS_WITHIN_RECT8(p) (IS_WITHIN(p.x, 0, 7) && IS_WITHIN(p.y, 0, 7))
#define P_ADD(p, d) (p).x += (d).x, (p).y -= (d).y
#define P_SUB(p, d) (p).x -= (d).x, (p).y -= (d).y
static int flip(enum TURN turn, POS p, POS d){
  int flipped = 0, i;
  POS q;
  if(turn == TURN_RED){
    for(q = p, P_ADD(q, d); IS_WITHIN_RECT8(q); flipped++, P_ADD(q, d)){
      if(IS_RED(q)) break;
      if(!IS_GREEN(q)) return 0;
    }
    if(!IS_WITHIN_RECT8(q)) return 0;
    for(i = 0, q = p, P_ADD(q, d); i < flipped; i++, P_ADD(q, d)) SET_RED(q);
  }else{
    for(q = p, P_ADD(q, d); IS_WITHIN_RECT8(q); flipped++, P_ADD(q, d)){
      if(IS_GREEN(q)) break;
      if(!IS_RED(q)) return 0;
    }
    if(!IS_WITHIN_RECT8(q)) return 0;
    for(i = 0, q = p, P_ADD(q, d); i < flipped; i++, P_ADD(q, d)) SET_GREEN(q);
  }
  return flipped;
}

static void set(enum TURN turn, POS p){
  if(turn == TURN_RED) SET_RED(p);
  else SET_GREEN(p);
}

typedef struct {
  unsigned char flips: 4;
  unsigned char openness: 4;
} CANDIDATE;

static void com_routine(){
  static POS cp;
  static CANDIDATE candidate;
  static int candidates;
  POS p, q, d, t;
  int i, flips, openness;
  if(cycle == 0){
    candidates = 0;
    candidate.openness = 0;
    candidate.flips = 0;
  }
  if(cycle < 8){//目に見える速度で思考する
    p.x = cycle;
    for(p.y = 0; p.y < 8; p.y++) if(IS_GREEN(p)) for(i = 8; i != 0; i--) {
	  d.x = (i%3+1)%3-1;
	  d.y = (i/3+1)%3-1;
	  flips = openness = 0;
	  for(q = p, P_ADD(q, d); IS_WITHIN_RECT8(q); P_ADD(q, d)){
	    if(IS_GREEN(q)){//自分の色に挟まれていると取れない
	      flips = 0;
	      break;
	    }
	    if(IS_RED(q)){//取れるかも？
	      flips++;
	      t = q; t.x -= d.x; if(!IS_EITHER(t)) openness++;
	      t.y += d.y; if(!IS_EITHER(t)) openness++;
	      t = q; t.y -= d.y; if(!IS_EITHER(t)) openness++;
	      t.x += d.x; if(!IS_EITHER(t)) openness++;
	    }else{//取れたかも？
	      t = q; t.x -= d.x; if(!IS_EITHER(t)) openness++;
	      t = q; t.y -= d.y; if(!IS_EITHER(t)) openness++;
	      break;
	    }
	  }
	  if(flips == 0) continue;//残念、1つも取れなかった。次
	  //どうやら何かしら取れたらしい
	  if(candidate.openness < openness || (candidate.openness == openness && candidate.flips < flips)){
	    cp = q;
	    candidate.openness = openness;
	    candidate.flips = flips;
	    candidates++;
	  }
	  
	  //debug
	  lcd_putstr(0,1,S_16);
#define S_KONPYU_TA_KOUHO "COM " HK_KO HK_U HK_HO
	  lcd_putstr(0,1,S_KONPYU_TA_KOUHO ":( , )");
	  lcd_putudec(sizeof(S_KONPYU_TA_KOUHO ":"),1,1,cp.x);
	  lcd_putudec(sizeof(S_KONPYU_TA_KOUHO ":( "),1,1,cp.y);
	  wait_frames = 20;
	}
  }else{
    if(candidates == 0){
      wait_frames = 10;
      lcd_putstr(0,1,S_16);
      lcd_putstr(0,1,S_KONPYU_TA ":" S_PASU);
      //TODO:pass
    }else{
      put(TURN_GREEN, cp);
    }
    cycle = 0;
    current_turn = TURN_RED;
    return;
  }
  //評価値=-開放度*10+ひっくり返る石の数
  //TODO
  //nサイクルかけてゆっくり処理
  cycle++;
}
