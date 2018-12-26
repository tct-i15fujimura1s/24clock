/*
 * crt0.s : H8/3664 ボード用デジタル時計プログラム
 *          C言語用スタートアップルーチン等
 *
 * Copyright (C) 2005 - 2010 by
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

/*-------------------------------------------------------------------------
   スタートアップルーチン
  -------------------------------------------------------------------------*/
	.h8300hn			/* H8/300H Tiny 用のプログラム */
	.section .text
	.global _start
	.align 1
_start:
	mov.w	#__stack,sp		/* スタックポインタの初期化 */

/* .data セグメントの準備 */
	mov.w	#__etext,r1		/* ROM 上の初期化データ先頭 */
	mov.w	#__data,r2		/* RAM 上の初期化データ先頭 */
	mov.w	#__edata,r3		/* RAM 上で BSS の先頭      */

	cmp.w	r2,r3 		        /* DATAセクションがある？　*/
	beq	bssclr			/* DATA無しはBSSクリアへ　 */
copy:	
	mov.b	@r1,r0l			/* ROM からデータを取り出し */
	mov.b	r0l,@r2			/* RAM に書き込む           */
	inc.w	#1,r1			/* ROM アドレスを進める     */
	inc.w	#1,r2			/* RAM アドレスを進める     */
	cmp.w	r2,r3			/* BSS まで来たか           */
	bhi	copy			/* まだなら繰り返す         */

bssclr: /* .bss セグメントのクリア */
	xor.b	r0l,r0l			/* 0 を準備する             */
	mov.w	#__stack,r3		/* RAM 領域の最後           */
clear:
	mov.b	r0l,@r2			/* RAM に 0 を書き込む      */
	inc.w	#1,r2			/* RAM アドレスを進める     */
	cmp.w	r2,r3			/* RAM の最後まできたか     */
	bhi	clear			/* まだなら繰り返す         */

/* C言語プログラム起動 */
	jsr	@_main			/* C言語の main 関数を呼び出す */
	bra	_start			/* 万一 main から返って来たら    */

