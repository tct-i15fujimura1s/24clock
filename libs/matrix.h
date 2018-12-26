#ifndef _MATRIX_H
#define _MATRIX_H

#include "libsCore.h"
#include "3664.h"

/* 便利そうなmatrix関係の定義 */

   /* #defineを使う時 (実は，この行のPDR8は，ここでは，評価されない)*/
   /* #include "3664.h"は，インクルード不要で，展開の場所で書く     */
   //#define  ENABLE_MATRIX_BLANK(a)   BIT_SET(PDR8,0)
   //#define DISABLE_MATRIX_BLANK(a)  BIT_CLR(PDR8,0)

   /* inline関数を使う時 (このヘッダファイルをコンパイルする時点で */
   /* 評価されるため，#include "3664.h"のインクルードが必要 */
   // extern inline void  ENABLE_MATRIX_BLANK(void)・・・
   // extern inline void DISABLE_MATRIX_BLANK(void)・・・

/* util/libsCore.hの DEFINE_PORT_ACTIVE_HIGH マクロを使い，１行で書く      */
/*メリット１：ハードに変更があった時に,下記の一箇所だけ書き換えれば良い    */
/*メリット２：ここに記述した直後に，PDR8などの定義がコンパイル時に確認可能 */
/*デメリット１: grepを書けても，該当する場所が分からない時が出る*/

DEFINE_PORT_ACTIVE_HIGH(MATRIX_BLANK,PDR8,0)

     //ACTIVE_LOW版の例
     //#define  ENABLE_MATRIX_LATCH(a)   BIT_CLR(PDR5,5)
     //#define DISABLE_MATRIX_LATCH(a)  BIT_SET(PDR5,5)

DEFINE_PORT_ACTIVE_LOW(MATRIX_LATCH,PDR5,5)

DEFINE_PORT_ACTIVE_HIGH(MATRIX_SIN,PDR5,4)

DEFINE_PORT_ACTIVE_HIGH(MATRIX_SCLK,PDR8,1)

extern inline void SELECT_MATRIX_COLUMN(int i){
  PDR7 = (i<<4);  /*ハード的に最短手の方法(P7は，P76〜P74までしか無い)*/
}

extern unsigned const char matrix_fonts[];

extern void matrix_font_debug_out_sample(unsigned int matrix_led_pattern[]);

#endif
