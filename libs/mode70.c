#include "libs.h"
#include "main.h"

char mode70_title[] = HK_TA HK_N HK_KE HK_N;

typedef enum {
	STATE_SELECT_COMMAND,
	STATE_SELECT_COMMAND_2,
	STATE_MOVE,
	STATE_ATTACK,
	STATE_ITEM
} STATE;

typedef enum {
	SUBSTATE_NONE,
	SUBSTATE_SHOW_MESSAGE,
	SUBSTATE_SHOW_MENU,
	SUBSTATE_ASK_DIR
} SUBSTATE;

typedef enum {
	MSG_MODAL = 1
} MESSAGE_FLAG;

typedef enum {
	DIR_NONE = 0x0,
	DIR_LEFT = 0x1,
	DIR_RIGHT = 0x3,
	DIR_UP = 0x4,
	DIR_DOWN = 0xC
} DIR;

static void show_message(const char *message, MESSAGE_FLAG flags);
static void show_menu(const char *prompt, const char **menu_items);
static void ask_dir();

static void do_select_command(UI_DATA *);
static void do_select_command_2(UI_DATA *);
static void do_move(UI_DATA *);
static void do_attack(UI_DATA *);
static void do_item(UI_DATA *);
static void do_show_message(UI_DATA *);
static void do_show_menu(UI_DATA *);
static void do_ask_dir(UI_DATA *);

static STATE state = 0;
static SUBSTATE substate = 0;

// show_menu
static int current_selection, n_menu_items;
static char **menu_items;
static char *label;

// ask_dir
static DIR direction;

void do_mode70(UI_DATA *ud){
	switch(substate){
	case SUBSTATE_NONE:
		switch(state){
		case STATE_SELECT_COMMAND:
			do_select_command(ud);
			break;
		case STATE_SELECT_COMMAND_2:
			do_select_command_2(ud);
			break;
		case STATE_MOVE:
			do_move(ud);
			break;
		case STATE_ATTACK:
			do_attack(ud);
		case STATE_ITEM:
			do_item(ud);
		}
		break;
	case SUBSTATE_SHOW_MESSAGE:
		do_show_message(ud);
		break;
	case SUBSTATE_SHOW_MENU:
		do_show_menu(ud);
		break;
	case SUBSTATE_ASK_DIR:
		do_ask_dir(ud);
	}
	if(ud->sw == KEY_LONG_C) {
		ud->mode = MODE_0;
	}
}

static void do_show_message(UI_DATA *ud){
	if(ud->sw == KEY_SHORT_C){
		substate = SUBSTATE_NONE;
	}
}

/*
0x0123456789ABCDEF
           a/n UDC
*/
static void do_show_menu(UI_DATA *ud){
	switch(ud->sw){
	case KEY_SHORT_C:
		label = menu_items[current_selection];
		substate = SUBSTATE_NONE;
		break;
	case KEY_SHORT_U:
		current_selection = (current_selection - 1) % n_menu_items;
		lcd_putdec(9, 0, 1, current_selection);
		lcd_putstr(2, 1, menu_items[current_selection]);
		break;
	case KEY_SHORT_D:
		current_selection = (current_selection + 1) % n_menu_items;
		lcd_putdec(9, 0, 1, current_selection);
		lcd_putstr(2, 1, menu_items[current_selection]);
	}
}

static void do_ask_dir(UI_DATA *ud){
	DIR dir = DIR_NONE;
	if(tma_flag==TRUE){
		if(ud->sw){
			if(ud->sw & KEY_SHORT_L) dir |= DIR_LEFT;
			else if(ud->sw & KEY_SHORT_R) dir |= DIR_RIGHT;
			if(ud->sw & KEY_SHORT_U) dir |= DIR_UP;
			else if(ud->sw & KEY_SHORT_D) dir |= DIR_DOWN;
			direction = dir;
			substate = SUBSTATE_NONE;
		}
		tma_flag=FALSE;
	}
}

static const char *command_labels[] = {
	HK_I HK_TO HK_DAKUTEN HK_U,
	HK_
};

static void do_select_command(UI_DATA *ud){
	state = STATE_SELECT_COMMAND_2;
	show_menu(HK_KO HK_MA HK_N HK_TO HK_DAKUTEN "?", command_labels);
}
