#include "libs.h"
#include "main.h"

char mode70_title[] = HK_TE HK_TO HK_RI HK_SU;

typedef struct {
	char x;
	char y;
} POS;

typedef struct {
	POS p1;
	POS p2;
	POS p3;
	POS p4;
} TETRIMINO;

typedef enum {
	BLACK = 0x0000,
	RED = 0x0001,
	GREEN = 0x0100,
	ORANGE = 0x0101
} COLOR;

static unsigned int rand();

static void rotateR(TETRIMINO *);
static void rotateL(TETRIMINO *);
static int is_laying(TETRIMINO *, POS);
static void paint_onto(TETRIMINO *, POS, COLOR, unsigned int *);

/* static定数 */
static const COLOR colors[] = {RED, GREEN, ORANGE};
static char *tetrimino_names[] = {"I", "J", "L", "O", "S", "Z", "T"};
static const TETRIMINO tetriminoes[] = {
	{{-1, 0}, {0, 0}, {1, 0}, {2, 0}},
	{{-2, 0}, {-1, 0}, {0, 0}, {0, 1}},
	{{0, 1}, {0, 0}, {1, 0}, {2, 0}},
	{{0, 0}, {1, 0}, {1, 1}, {0, 1}},
	{{0, 0}, {1, 0}, {-1, 1}, {0, 1}},
	{{-1, 0}, {0, 0}, {0, 1}, {1, 1}},
	{{-1, 0}, {0, 0}, {1, 0}, {0, 1}}
};
static const int difficulty_max = 12;

/* static変数 */
static unsigned int matrix[8];
static POS pos;
static TETRIMINO mino;
static COLOR color;
static int next;
static unsigned int score;
static int difficulty;
static int cycle;

void do_mode70(UI_DATA *ud){
	int i;
	
	/* モードの初期化 */
	if(ud->prev_mode != ud->mode){
		for(i = 0; i < 8; i++) matrix[i] = 0x0000;
		pos.x = pos.y = 0;
		color = BLACK;
		next = rand() % 7;
		score = 0;
		difficulty = 0;
		cycle = 0;
		
		lcd_clear();
		for(i = 0; i < 8; i++) matrix_led_pattern[i] = matrix[i] = 0x0000;
		lcd_putstr(0, 0, "MODE7:" HK_TE HK_TO HK_RI HK_SU);
		lcd_putstr(0, 1, HK_SU HK_KO HK_A ":00000 " HK_TU HK_KI HK_DAKUTEN ":");
		lcd_putstr(14, 1, tetrimino_names[next]);
	}
	
	if(tmv_flag == TRUE){
		switch(ud->sw){
		case KEY_SHORT_L:
			pos.x--;
			if(is_laying(&mino, pos)) pos.x++;
			break;
		case KEY_SHORT_R:
			pos.x++;
			if(is_laying(&mino, pos)) pos.x--;
			break;
		case KEY_SHORT_C:
			rotateR(&mino);
			if(is_laying(&mino, pos)) rotateL(&mino);
			break;
		case KEY_LONG_C:
			ud->mode = MODE_0;
		}
		tmv_flag = FALSE;
	}
	
	if(tma_flag == TRUE){
		if(cycle == 0){
		  if(color == BLACK){ /* テトリミノがない */
		    pos.x = 3;
		    pos.y = 0;
		    mino = tetriminoes[next];
		    color = colors[rand() % 3];
		    next = rand() % 7;
		    lcd_putstr(14, 1, tetrimino_names[next]);
		  }else{ /* テトリミノがある */
		    pos.y++;
		    lcd_putdec(14,0,1,is_laying(&mino, pos));//debug
		    if(is_laying(&mino, pos)){
		      pos.y--;
		      paint_onto(&mino, pos, color, matrix);
		      color = BLACK;

		      /* 行を消す処理を入れる */
		      for(i = 7; i >= 0; i--){
			/* もしその行が消せるなら？ */
		      }
		    }
		  }
		}
		cycle = (cycle + 1) % (difficulty_max - difficulty);
		
		for(i = 0; i < 8; i++) matrix_led_pattern[i] = matrix[i];
		if(color != BLACK){
			paint_onto(&mino, pos, color, matrix_led_pattern);
		}
		
		tma_flag = FALSE;
	}
}

static inline POS rotR(POS p){
	POS q = {-p.y, p.x};
	return q;
}
static void rotateR(TETRIMINO *t){
	t->p1 = rotR(t->p1);
	t->p2 = rotR(t->p2);
	t->p3 = rotR(t->p3);
	t->p4 = rotR(t->p4);
}

static inline POS rotL(POS p){
	POS q = {p.y, -p.x};
	return q;
}
static void rotateL(TETRIMINO *t){
	t->p1 = rotL(t->p1);
	t->p2 = rotL(t->p2);
	t->p3 = rotL(t->p3);
	t->p4 = rotL(t->p4);
}

#define within(x, a, b) ((x) >= a && (x) <= b)

static inline int within_screen(TETRIMINO *t, POS p){
  //FIXME
  return (
    within(t->p1.x + p.x, 0, 7) && within(t->p1.y + p.y, -2, 7) &&
    within(t->p2.x + p.x, 0, 7) && within(t->p2.y + p.y, -2, 7) &&
    within(t->p3.x + p.x, 0, 7) && within(t->p3.y + p.y, -2, 7) &&
    within(t->p4.x + p.x, 0, 7) && within(t->p4.y + p.y, -2, 7)) << 1;
}

static int is_laying(TETRIMINO *t, POS p){
  return !within_screen(t, p) ||
    (matrix[t->p1.x + p.x] & (ORANGE << (t->p1.y + p.y))) ||
    (matrix[t->p2.x + p.x] & (ORANGE << (t->p2.y + p.y))) ||
    (matrix[t->p3.x + p.x] & (ORANGE << (t->p3.y + p.y))) ||
    (matrix[t->p4.x + p.x] & (ORANGE << (t->p4.y + p.y)));
}

static void paint_onto(TETRIMINO *t, POS p, COLOR c, unsigned int *m){
  int i;
  m[t->p1.x + p.x] |= c << (t->p1.y + p.y);
  m[t->p2.x + p.x] |= c << (t->p2.y + p.y);
  m[t->p3.x + p.x] |= c << (t->p3.y + p.y);
  m[t->p4.x + p.x] |= c << (t->p4.y + p.y);
}

static unsigned int rand(){
	static unsigned int curr = 0x1234;
	unsigned int next = curr * 17 + 91;
	curr ^= (next >> 2) | ((next + curr) << 2 & 0xc000);
	return next;
}
