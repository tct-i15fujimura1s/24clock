#include "3664.h"
#include "wait.h"
#include "libsCore.h"

/*
 * LCD にコマンドやデータを入出力する基本ルーチン/データ
 */

void lcd_init(void);
static char lcd_initialized=FALSE;
static char stat;

static void outLcdC(char x) {

        /*LCDのイニシャライズチェック*/
        if(lcd_initialized==FALSE){
           lcd_init();
        }

	PCR5 |= 0x0f;				/* 出力ビット指定 */
	PDR1 &= ~0x07;
	PDR5 = (x & 0x0f) | (PDR5 & ~0x0f);
	PDR1 = 0x04 | (PDR1 & ~0x07);		/* 書き込み */
	PDR1 &= ~0x07;
//	PCR5 &= ~0x0f;				/* 全ビット入力へ */
}

static void outLcdD(char x) {
	PCR5 |= 0x0f;				/* 出力ビット指定 */
	PDR1 = 0x01 | (PDR1 & ~0x07);
	PDR5 = (x & 0x0f) | (PDR5 & ~0x0f);
	PDR1 = 0x05 | (PDR1 & ~0x07);		/* 書き込み */
	PDR1 = 0x01 | (PDR1 & ~0x07);
	PDR1 &= ~0x07;
//	PCR5 &= ~0x0f;				/* 全ビット入力へ */
}

static unsigned char inLcdC(void) {
	unsigned char x;
	PCR5 &= ~0x0f;				/* 全ビット入力へ */
	PDR1 = 0x02 | (PDR1 & ~0x07);
	PDR1 = 0x06 | (PDR1 & ~0x07);
	x = PDR5 & 0x0f;
	PDR1 = 0x02 | (PDR1 & ~0x07);		/* 上位4ビット読み込み */
	PDR1 = 0x06 | (PDR1 & ~0x07);
	x = (x << 4) | (PDR5 & 0x0f);
	PDR1 = 0x02 | (PDR1 & ~0x07);		/* 下位4ビット読み込み */
	PDR1 &= ~0x07;
	return x;
}

/* LCD の BUSY 待ち */
static void waitLcd(void) {
	while (inLcdC()&0x80);	/* Busy のチェック */
}

static void writeLcdC(unsigned char x) {
	waitLcd();
	outLcdC(x>>4);
	outLcdC(x);
}

static void writeLcdD(unsigned char x) {
	waitLcd();
	outLcdD(x>>4);
	outLcdD(x);
}

/* LCDの初期化を行います。
 *   使用方法：LCDを利用する前に必ず呼び出します。
 *   #外部からLCD関連の関数を呼び出したら
 *   #outLcdC()関数が呼び出されるので，
 *   #そこで，初期化済かどうか判定して，未初期化ならば，
 *   #この関数を呼び出すことも可能。
 */
void lcd_init(void) {
	int i;

	lcd_initialized=TRUE;

	for (i=0; i<100; i++)		/* パワーオンから 15ms 以上待つ */
		wait1m();

	outLcdC(0x03);			/* 初期化１ */
	for (i=0; i<5; i++)		/* 4.1ms 以上待つ */
		wait1m();

	outLcdC(0x03);			/* 初期化２ */
	wait1m();			/* 100us 以上待つ */

	outLcdC(0x03);			/* 初期化３ */

	/* ここから Busy のチェックができる */
	while ((inLcdC()&0x80)==0x80);
	outLcdC(0x02);			/* ファンクションセット */

	/* ここから 4bit 動作 */
	writeLcdC(0x28);		/* 表示モード */
	writeLcdC(0x08);		/* 表示オフ   */
	writeLcdC(0x01);		/* 表示クリア */
	writeLcdC(0x06);		/* エントリーモードセット */
	writeLcdC(0x80);		/* アドレスセット */

	stat = 0x0f;			/* 表示ON,カーソルON,ブリンクON */
	writeLcdC(stat);		/* 液晶の表示モード設定 */
}

/* LCDの表示ON/OFFを切り替えます。
 *   使用方法：LCDを使用するときとしないときにそれぞれ呼び出します。
 *   注意事項：アナログ入力で可変抵抗の値を読み取るときはOFFにしてください。
 *   引数：定数定義されたON/OFFを使用します。
 */
void lcd_enable(int x) {
	stat = (stat & ~0x04) | ((x & 0x01)<<2);
	writeLcdC(stat);
}

/*
 * LCDのカーソルON/OFFを切り替えます。
 *   使用方法：カーソルを使用するときとしないときにそれぞれ呼び出します。
 *   引数：定数定義されたON/OFFを使用します。
 */
void lcd_cursor(int x) {
	stat = (stat & ~0x02) | ((x & 0x01)<<1);
	writeLcdC(stat);
}

/*
 * LCDのカーソルブリンク（点滅）ON/OFFを切り替えます。
 *   使用方法：点滅を使用するときとしないときにそれぞれ呼び出します。
 *   引数：定数定義されたON/OFFを使用します。
 */
void lcd_blink(int x) {
	stat = (stat & ~0x01) | (x & 0x01);
	writeLcdC(stat);
}

/*
 * LCDの指定された座標へ文字を出力します。
 *   使用方法：座標と文字列（文字コード）を使用し、LCDへ文字列を出力します。
 *   注意事項：半角カナとASCII文字のみ使用できます。
 *   引数：
 *     x: x座標（0〜15までの16キャラクタ）
 *     y: y座標（0〜1までの2行）
 *     s: 文字列

 */
void lcd_putstr(int x, int y, char *s) {
	char c;
	int a = x;			/* A は DDRAM のアドレス        */
	if (y!=0) a += 0x40;		/* ２行目のアドレスは 0x40 から */
	writeLcdC(0x80 | a);		/* アドレスをセットする         */
	while ((c=*s++)!=0) {		/* NULL文字が来るまで，文字列出力 */
		if (c!=0x8e)		/* 0x8eはEUCの半角カナエスケープ */
		writeLcdD(c);
	}
}

