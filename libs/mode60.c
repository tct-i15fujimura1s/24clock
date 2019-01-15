#include "mode60.h"

enum TURN {
	TURN_RED,
	TURN_GREEN
};

typedef struct {
	char reserved:1;
	char is_simulation:1;
	char y:3;
	char x:3;
} POS;

enum TURN current_turn = TURN_RED;

static unsigned int matrix[8] = {0, 0, 0, 0, 0, 0, 0, 0};

#define IS_RED(p) (matrix[(p.y)] & 1 << (p.x))
#define IS_GREEN(p) (matrix[(p.y)] & 0x100 << (p.x))

inline void SET_RED(POS p){
	matrix[p.y] = matrix[p.y] & ~(0x100 << p.x) | (0x1 << p.x);
}

inline void SET_GREEN(POS p){
	matrix[p.y] = matrix[p.y] & ~(0x1 << p.x) | (0x100 << p.x);
}

POS pointer;
int show_pointer = FALSE;

static void draw_board();
static void lcd_refresh();
static int put(enum TURN turn, POS p);
static int flip(enum TURN turn, POS p, POS d);
static int set(enum TURN turn, POS p);
static void com_routine();

void do_mode60(UI_DATA *ui_data){
	if(ui_data->prev_mode != ui_data->mode) {
		//マトリックスLEDの準備
		//オセロ盤の初期化
		//((unsigned short *) red)[0] = 0x0000; 
		red[0] = red[1] = red[2] = 0x00;
		green[0] = green[1] = green[2] = 0x00;
		red[3] = green[4] = 0x10;
		green[3] = red[4] = 0x08;
		red[5] = red[6] = red[7] = 0x00;
		green[5] = green[6] = green[7] = 0x00;

		each_turn_begins();
	}

	switch(ui_data->key){//FIXME
	case KEY_SHORT_U:
		pointer.y = pointer.y + 7 & 7;
	case KEY_SHORT_D:
		pointer.y = pointer.y + 1 & 7;
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
		ui_data->next_mode = MODE_0;
		break;
	default:
	}

	if(tmv_flag){
		draw_board();
		lcd_refresh();
		tmv_flag = FALSE;
	}

	if(sec_flag){
		if(current_turn == TURN_GREEN){
			com_routine();
			current_turn = TURN_RED;
		}
		sec_flag = FALSE;
	}
}

static void draw_board() {
	static int col = 0;
	/*16bit (1列分)をシリアル転送*/
	col = col + 1 & 7;
	unsigned int p;
	for(p = matrix[col], i = 0; i < 16; i++, p <<= 1){
		if((p&0x8000)==0){
			DISABLE_MATRIX_SIN();
		}else{
			ENABLE_MATRIX_SIN();
		}
	}
	SET_H_MATRIX_SCLK();
	SET_L_MATRIX_SCLK();
	ENABLE_MATRIX_BLANK();
	DISABLE_MATRIX_LATCH();
	ENABLE_MATRIX_LATCH();
	SELECT_MATRIX_COLUMN(col);
	DISABLE_MATRIX_BLANK();
}

static void lcd_refresh(){
	lcd_clear();
	lcd_putstr(0,0,"MODE7:OTHELLO");
	if(current_turn == TURN_RED){
		lcd_putstr(0,1,"  put  pass");
		if(show_pointer){
			lcd_putstr(0, 1, "\xc6\xb1put      UDLRC");
		}else{
			lcd_putstr(13, 1, "LRC");
			lcd_putstr(pointer.x == 0 ? 4 : 9, 1, "\xc6\xb1"); //「ﾆｱ」
		}
		//TODO: make the full-color led red
		DISABLE_LED_GREEN();
		ENABLE_LED_RED();
	}else{
		//TODO: make the full-color led green
		DISABLE_LED_RED();
		ENABLE_LED_GREEN();
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

static int flip(enum TURN turn, POS p, POS d){
	//TODO
}

static int set(enum TURN turn, POS p){
	//TODO
}

static void com_routine(){
	static int candidate[]; //
	//評価値=-開放度*10+ひっくり返る石の数
	//TODO
}
