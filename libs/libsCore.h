#ifndef _LIBSCORE_H
#define _LIBSCORE_H

#include "3664.h"

/*便利そうな定義色々*/
#define FALSE 0
#define TRUE 1
#define OFF 0
#define ON  1

#define NULL ((void*)0)

/*機械語コード関連*/
#define EI()    asm("andc.b  #0x7f,ccr")     /*割り込み許可*/
#define DI()    asm("orc.b #0xc0,ccr")       /*割り込み禁止*/
#define SLEEP() asm("sleep");   /*割込が入るまで，スリープ(消費電力低減!)*/

/* 便利そうなbit操作の定義 */
#define  BIT_SET(a,b)   ((a) |=  (1<<(b)))
#define  BIT_CLR(a,b)   ((a) &= ~(1<<(b)))
#define  BIT_FLP(a,b)   ((a) ^=  (1<<(b)))

/* 少々，トリッキーな#defineの使い方を２例。defineで inline関数を使用して */
/* いるのは，#defineの中で,更に#defineはできないためである                */
/* ##は，defineの引数を結合するために用いている。                         */
/* この##は，難点があって，これを使うと，コードの変更を伴う保守性は良くな */
/* るのだが，どこに書かれているのか分からないという保守性悪化や可読性悪化 */
/* がある。*/

#define DEFINE_PORTS(NAME,b,c) \
extern inline void    FLIP_##NAME(void){BIT_FLP( b , c);}  \
extern inline void   SET_H_##NAME(void){BIT_SET( b , c);}  \
extern inline void   SET_L_##NAME(void){BIT_CLR( b , c);}  

#define DEFINE_PORT_ACTIVE_HIGH(NAME,b,c) \
extern inline void  ENABLE_##NAME(void){BIT_SET( b , c);}  \
extern inline void DISABLE_##NAME(void){BIT_CLR( b , c);}  \
DEFINE_PORTS(NAME,b,c)

#define DEFINE_PORT_ACTIVE_LOW(NAME,b,c) \
extern inline void  ENABLE_##NAME(void){BIT_CLR( b , c);}  \
extern inline void DISABLE_##NAME(void){BIT_SET( b , c);}  \
DEFINE_PORTS(NAME,b,c)

/* 保守性が良くなるのは，早速活用した。最初は,ENABLEとDISABLEしか作って*/
/* いなかったが，ふと思い付いて，FLIPを追加した。各定義では，変更無しに*/
/* ここの2行を追加するだけで，BIT_FLIP機能を使用することができる*/

/* で，更に追加したが，機能(ENABLEやDISABLE)でなくて，電位で書きたい時 */
/* (エッジ部分の表現とか…)もあるので，SET_LとSET_H の機能を付け加えた */

/* で，更に追加したが，ACTIVE_HIGHとACTIVE_LOWとのどちらとも同じことを */
/* 実現する部分は，DEFINE_PORTSで，一まとめにした(実は，この統合作業を */
/* 行ったことによって，コンパイルや実行前にバグが見付かった。具体的には*/
/* ACTIVE_LOWのSET_HとSET_Lが，FLIP動作の記述になっていた)             */

/* 間違いのFIX，機能追加は，極力，1箇所で済むようにコードを書くと良い*/

/*上記のマクロ(define)は，以下のように使用する*/
//DEFINE_PORT_ACTIVE_HIGH(MATRIX_BLANK,PDR8,0)

/*LEDの割り当てビット(昔の定義は，DEFINE_PORT_...の作成により捨てた。 */
/*下記の考え方は，led.hやled.cへ移行させる*/
//#define LED_R   (1<<7)
//#define LED_G   (1<<6)
//#define LED_B   (1<<5)


#endif

