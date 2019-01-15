#include "mode60.h"

enum TURN {
	TURN_RED,
	TURN_GREEN
};

enum TURN current_turn = TURN_RED;

static unsigned int matrix[8] = {0, 0, 0, 0, 0, 0, 0, 0};

int pointer_x = 0, pointer_y = 0;
int show_pointer = FALSE;

void draw_board() {
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

void lcd_refresh(){
	lcd_clear();
	lcd_putstr(0,0,"MODE7:OTHELLO");
	if(current_turn == TURN_RED){
		lcd_putstr(0,1,"  put  pass");
		if(show_pointer){
			lcd_putstr(0, 1, "\xc6\xb1put      UDLRC");
		}else{
			lcd_putstr(13, 1, "LRC");
			lcd_putstr(pointer_x == 0 ? 4 : 9, 1, "\xc6\xb1"); //「ﾆｱ」
		}
		//TODO: make the full-color led red
		ENABLE_LED_RED();
	}else{
		//TODO: make the full-color led green
		ENABLE_LED_GREEN();
	}
}

void do_mode60(UI_DATA *ui_data){
	if(ui_data->prev_mode != ui_data->mode) {
		//マトリックスLEDの準備
		//オセロ盤の初期化
		//((unsigned short *) red)[0] = 0x0000; 
		red[0] = red[1] = red[2] = 0x00;
		red[5] = red[6] = red[7] = 0x00;
		green[0] = green[1] = green[2] = 0x00;
		green[5] = green[6] = green[7] = 0x00;
		red[3] = green[4] = 0x10;
		green[3] = red[4] = 0x08;

		each_turn_begins();
	}

	switch(ui_data->key){//FIXME
	case KEY_SHORT_U:
		pointer_y = pointer_y + 7 & 7;
	case KEY_SHORT_D:
		pointer_y = pointer_y + 1 & 7;
	case KEY_SHORT_L:
		pointer_x = show_pointer ? pointer_x + 7 & 7 : !pointer_x;
		break;
	case KEY_SHORT_R:
		pointer_x = show_pointer ? pointer_x + 1 & 7 : !pointer_x;
		break;
	case KEY_SHORT_C:
		if(show_pointer){
			//TODO: put
		}else{
			if(pointer_x == 0){
				pointer_x = pointer_y = 0;
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
		tmv_flag = FALSE;
	}

	if(sec_flag){
		sec_flag = FALSE;
	}
}
