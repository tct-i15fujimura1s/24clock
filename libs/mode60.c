#include "libs.h"
#include "main.h"
#include "mode60.h"

enum TURN {
	TURN_RED,
	TURN_GREEN
};

typedef struct {
	char computes_openness:1;
	char is_simulation:1;
	char y:3;
	char x:3;
} POS;

static enum TURN current_turn = TURN_RED;

static unsigned int matrix[8] = {0, 0, 0, 0, 0, 0, 0, 0};

#define IS_RED(p) (matrix[(p.y)] & 0x100 << (p.x))
#define IS_GREEN(p) (matrix[(p.y)] & 0x1 << (p.x))

static inline void SET_RED(POS p){
	matrix[p.y] = matrix[p.y] & ~(0x1 << p.x) | (0x100 << p.x);
}

static inline void SET_GREEN(POS p){
	matrix[p.y] = matrix[p.y] & ~(0x100 << p.x) | (0x1 << p.x);
}

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
	        refresh();
	}

	switch(ui_data->sw){//FIXME
	case KEY_SHORT_U:
		pointer.y = pointer.y + 7 & 7;
		break;
	case KEY_SHORT_D:
		pointer.y = pointer.y + 1 & 7;
		break;
	case KEY_SHORT_L:
		pointer.x = show_pointer ? pointer.x + 7 & 7 : !pointer.x;
		break;
	case KEY_SHORT_R:
		pointer.x = show_pointer ? pointer.x + 1 & 7 : !pointer.x;
		break;
	case KEY_SHORT_C:
		if(show_pointer){
			//TODO: put
			if(put(TURN_RED, pointer)){
				current_turn = TURN_GREEN;
				show_pointer = FALSE;
			}
		}else{
			if(pointer.x == 0){
				pointer.x = pointer.y = 0;
				show_pointer = TRUE;
			}else{
				//TODO: pass
			}
		}
		break;
	case KEY_LONG_C:
		ui_data->mode = MODE_0;
		break;
	default:
	  break;
	}

	if(tma_flag){
		refresh();
		if(current_turn == TURN_GREEN){
			com_routine();
		}
		tma_flag = FALSE;
	}
}

static void refresh(){
	lcd_clear();
	lcd_putstr(0,0,"MODE6:\xB5\xBE\xDB"); // 「オセロ」
	if(current_turn == TURN_RED){
	  lcd_putstr(0,1,"   \xB3\xC2    \xCA\xDF\xBD"); // 「ウツ」「パス」
		if(show_pointer){
			lcd_putstr(0, 1, "\xc6\xb1 \xB3\xC2      UDLRC");
		}else{
			lcd_putstr(13, 1, "LRC");
			lcd_putstr(pointer.x == 0 ? 0 : 6, 1, "\xc6\xb1"); //「ﾆｱ」
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
	  matrix_led_pattern[pointer.x] |= 0x101 << pointer.y;
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
	if(!p.is_simulation && flipped != 0) set(turn, p); //もし1つでもひっくりかえせていれば置く
	return flipped;
}

#define IS_WITHIN(x, a, b) ((x) >= (a) && (x) <= (b))
#define IS_WITHIN_RECT8(p) (IS_WITHIN(p.x, 0, 7) && IS_WITHIN(p.y, 0, 7))
#define P_ADD(p, d) (p).x += (d).x, (p).y -= (d).y
#define P_SUB(p, d) (p).x -= (d).x, (p).y -= (d).y
static int flip(enum TURN turn, POS p, POS d){
	int flipped = 0, i;
	POS q, r, s, t;
	if(turn == TURN_RED){
		for(q = p, P_ADD(q, d); IS_WITHIN_RECT8(q); flipped++, P_ADD(q, d)){
			if(IS_RED(q)) break;
			if(!IS_GREEN(q)) return 0;
		}
		if(!p.is_simulation) for(q = p, P_ADD(q, d), i = 0; i < flipped; i++, P_ADD(q, d)) SET_RED(q);
		//REDはプレイヤーなのでcomputes_opennessはいらない
	}else{
		for(q = p, P_ADD(q, d); IS_WITHIN_RECT8(q); flipped++, P_ADD(q, d)){
			if(IS_GREEN(q)) break;
			if(!IS_RED(q)) return 0;
		}
		if(!p.is_simulation) for(q = p, P_ADD(q, d), i = 0; i < flipped; i++, P_ADD(q, d)) SET_GREEN(q);
		if(p.computes_openness){
			q = p;
			P_ADD(q, d);
			s = q;
			P_ADD(s, d);
			t = q;
			t.x = q.x + 1;
			if(IS_WITHIN_RECT8(t) && !IS_RED(t) && !IS_GREEN(t)) flipped += 0x100;
			t.x = q.x - 1;
			if(IS_WITHIN_RECT8(t) && !IS_RED(t) && !IS_GREEN(t)) flipped += 0x100;
			
		}
	}
	return flipped;
}

static void set(enum TURN turn, POS p){
	if(turn == TURN_RED) SET_RED(p);
	else SET_GREEN(p);
}

struct CANDIDATE {
	unsigned char flips: 4;
	unsigned char openness: 4;
};
static void com_routine(){
	static struct CANDIDATE candidates[64];
	static int n_candidates;
	POS p;
	if(cycle >= 9){
		cycle = 0;
		current_turn = TURN_RED;
		n_candidates = 0;
		return;
	}
	if(cycle < 8){
		p.y = cycle;
		for(p.x = 0; p.x < 8; p.x++) {
			
		}
	}
	//評価値=-開放度*10+ひっくり返る石の数
	//TODO
	//nサイクルかけてゆっくり処理
	cycle++;
}
