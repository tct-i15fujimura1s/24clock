#include "key.h"
#include "libsCore.h"
#include "lcd_util.h"

/*
 * support.c : H8/3446 ボード用デジタル時計プログラム
 *             LCD の制御ルーチンとスイッチの監視ルーチン
 *
 * Copyright (C) 2005 by
 *                      Dept. of Computer Science and Electronic Engineering,
 *                      Tokuyama College of Technology, JAPAN
 *
 *   上記著作権者は，Free Software Foundation によって公開されている GNU 一般公
 * 衆利用許諾契約書バージョン２に記述されている条件を満たす場合に限り，本ソース
 * コード(本ソースコードを改変したものを含む．以下同様)を使用・複製・改変・再配
 * 布することを無償で許諾する．
 *
 *   本ソースコードは＊全くの無保証＊で提供されるものである。上記著作権者および
 * 関連機関・個人は本ソースコードに関して，その適用可能性も含めて，いかなる保証
 * も行わない．また，本ソースコードの利用により直接的または間接的に生じたいかな
 * る損害に関しても，その責任を負わない．
 */

/*
  2005年:重村先生作
  2010年:新田研卒業研究生が,ポートの改編に合わせた変更。
         (4つのキーから5つのキーのサポート)＋詳細コメントの作成(Mr. Misaka.ら)
  2011年:新田がコメントを80カラムに納め直し＋キー処理だけ独立させ，
         ライブラリ化して実装
 */


/*
 * スイッチの状態を調べて返します。
 *   使用方法：トップハーフから 1/32 秒毎に呼び出します。
 *   注意事項：複数のスイッチによる同時押下をサポートしません。
 *   戻り値：
 *     +--+--+--+--+--+--+--+--+
 *     |b7|b6|b5|b4|b3|b2|b1|b0|
 *     +--+--+--+--+--+--+--+--+
 *       b7:長押し判定ビット（1:長押し/0:短押し）
 *       b6:未定義（初期値0）
 *       b5:未定義（初期値0）
 *       b4:sw1が押された（1:押されている/0:押されていない,以下同様）
 *       b3:sw2が押された
 *       b2:sw3が押された
 *       b1:sw4が押された
 *       b0:sw5が押された
 */

unsigned char sw_mng(void) {
	const char mask = 0x1f;
	static char presw = 0;			       /* 直前の状態の記憶 */
	static int swtimer = 0;			     /* 長押し計測タイマー */
	char cursw = (~PDRB >> 3) & mask;/* 現在のスイッチの状態を読み取る */
	char ret = 0;
	
	if (swtimer > 0 && --swtimer == 0) {  /* 長押しの時刻に達したなら  */
		ret = cursw | 0x80;                /* 長押しフラグを立てる */
		swtimer = -1;
	} else if(((cursw ^ presw) & ~presw) != 0){
	  /* 新しく押しされたスイッチがあれば */
		swtimer = 32 * 2;	/* 長押しタイマーを起動(2秒)する */
	} else if (cursw == 0) {	/* スイッチが全部離されていたなら */
		if (presw != 0 && swtimer >= 0) {
 		      /* 前回はどれかが押されていてかつ長押しではないなら */
			ret = presw;      /* 前回の状態が押されたスイッチ */
		}
		swtimer = 0;
	}

	presw = cursw;		/* 今回の状態を次の処理での前回の状態へ */
	return ret;
}


//キーデータのデバックアウトのサンプル
void key_debug_out_sample(void)
{
  int key_data = ~(PDRB);           /*負論理から正論理に変換し代入*/
  lcd_key_debug_out(9,0,key_data);  /*キーの状態をLCDに出力*/
}
