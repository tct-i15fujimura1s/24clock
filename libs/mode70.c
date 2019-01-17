#include "libs.h"
#include "main.h"

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
    lcd_putstr(1,0,"2" HK_TO HK_DAKUTEN HK_TU_SMALL HO_TO HK_HU HK_DAKUTEN HK_RA HK_SA HK_DAKUTEN HK_ONBIKI HK_SU HK_DAKUTEN); /*モード1の初期表示*/
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
  	ud->mode=MODE_71; /*モード72に行く*/
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
 *     マップは1列毎に疑似乱数とマルコフ連鎖を用いて自動生成される。
 *     また、全体マップは実際には存在しないが、あるとしたときのプレイヤーの現在の位置がLCDの2行目に表示される。
 *     絶対位置は(playerX+mapX-7)で計算される。プレイヤーを表す"I"が表示される位置はそれを>>3した位置となる。
 *     0から14番目までのマップは自動生成であるが、15番目のマップは決まっている。
 *** 地面
 *     地面はオレンジ色で表現される。
 *     地面は、一つ前のmapから生成される。また、ある高さに地面がある場合、それより下はすべて地面になる。また、地面の高さは0以上4以下である。
 *     プレイヤーは地面がないと落ちる。そしてplayerY < -1になると死ぬ(SE要件)。
 *** アイテムの生成
 *     アイテムは、緑のドットで表現され、地面から1~3ドット離れた高さに生成される。
 *     アイテムを取るとスコアに加算される(SE要件)。
 ** ゴール
 *     ゴールは赤い縦長の旗であらわされる。
 *     ゴールに触れるとクリアとなり、ハイスコアの更新処理をしてMODE_71に移行する。
 */
//static const unsigned int goalMap = {0x8080, 0x8080, 0x8080, 0x8080, 0x8080, 0x8080, 0xE080, 0xC080};

static unsigned int nextFrame(){
	static unsigned int count = 0;
	count = (count + 1) & 0x7;
	return !count;
}

#define IS_GROUND(x, y) (map[x] && (0x0101 << (y)))

static int score = 0, time = 0;
static unsigned int map[8];
void do_mode72(UI_DATA *ud){
	static int playerX = 0, playerVY = 1, playerY = 1, isJumping = FALSE, mapX = 7;


	int i;

	if(ud->prev_mode!=ud->mode){
		//MODE71の初期表示
		lcd_clear();
		lcd_putstr(1,0,HK_SU HK_KO HK_A "    0 " HK_TA HK_I HK_MU "100");
		lcd_putstr(0,1,"I...............");
		score = 0;
		time = 100;
	}

	if(ud->sw == KEY_LONG_C){//終了
		ud->mode = MODE_0;
	}
	if(tma_flag==TRUE && nextFrame()){
		if(isJumping){//ジャンプ、または落下中
			//足元に地面があるか？
			for(i = 0; i < playerVY; i++) if(IS_GROUND(playerX, playerY + i)) break;
			if(i != playerVY){
				playerY += i;
				isJumping = FALSE;
			}else{
				playerY += playerVY++;
			}
		}
		if(!isJumping && /*TODO*/){//ジャンプ
			isJumping = TRUE;
			playerVY = 4;
		}
		if(/*TODO*/){//右へ
			if(playerX > 4 && mapX < 63){//スクロール
				for(i = 0; i < 7; i++) map[i] = map[i + 1]; //ずらす
				generateMapColumn();
			}else if(playerX < 7){//動ける
				playerX++;
				if(playerX == 126){//ゴール
					if(score > highscore){
						highscore = score;
					}
					ud->mode = MODE_73;//クリア画面
					return;
				}
			}
		}
		if(/*TODO*/){//左へ
			if(playerX >= 0){//動ける
				playerX--;
			}
		}
		tma_flag=FALSE;
	}
}

//これで合ってるかはわからないけれど・・・。まぁまぁかな
unsigned int rand(){
	static unsigned int next = 0x0001;
	next = next * 17 + 91;
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
