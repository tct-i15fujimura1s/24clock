#include "3664.h"
#include "libsCore.h"
#include "matrix.h"
#include "lcd.h"

/*
 * main.cにあった，io_init()をここに追い出した。
 */
/*I/O関係のイニシャライズルーチン*/
void io_init(void){
  int i;

        DI();         /*色々変更する前に，念のためにDisable Interrupt*/

	              /* I:入力として使用する(0)。O:出力として使用する(1)*/
                      /* i:端子が無いのでとりあえず，デフォルト値で入力(0)*/
	              /* o:端子の先に何も接続していないので出力とする(1)*/

        PCR1 = 0x37;  /* ポート1の入出力設定(IIooiOOO)*/
                      /* P10:LCD RS端子*/
                      /* P11:LCD RW端子*/	
	              /* P12:LCD E端子 */
                      /* P13:本マイコンでは，端子無し*/
                      /* P14:JP2-2(学生実験では未使用:拡張ハード用IRQ0)*/
                      /* P15:JP2-1(学生実験では未使用:拡張ハード用IRQ1)*/
                      /* P16:IR受光モジュール(IRQ2)*/
                      /* P17:IR受光モジュール(IRQ2)*/

	PCR2 = 0x05;  /* ポート2の入出力設定(iiiiiOIo) */
                      /* P20:JP1-1(学生実験では未使用)*/
                      /* P21:シリアル受信(RXD)*/
                      /* P22:シリアル送信(TXD)*/
	              /* P23〜P27は，本マイコンでは，端子無し*/

	              /* ポート3,ポート4は，このマイコンでは，実装が無い*/

        PCR5 = 0xff;  /* ポート5の入出力設定(ooOOOOOO)*/
                      /* P50:LCD(DB4 LCDのデータバス)*/
                      /* P51:LCD(DB5 LCDのデータバス)*/
                      /* P52:LCD(DB6 LCDのデータバス)*/
                      /* P53:LCD(DB7 LCDのデータバス)*/
                      /* P54:マトリクスLEDドライバのSIN(シリアルデータ)*/
                      /* P55:マトリクスLEDドライバの~LATCH*/
                      /* P56:学生実験では未使用 I2C(SDA)用*/
                      /* P57:学生実験では未使用 I2C(SCL)用*/
	              
	              /* ポート6は，このマイコンでは，実装が無い*/

        PCR7 = 0x70;  /*ポート7の入出力設定(iOOOiiii)*/
	              /* P70〜P73:本マイコンでは，端子無し*/
	              /* P74:マトリクスLEDドライバのCSEL0*/
	              /* P75:マトリクスLEDドライバのCSEL1*/
	              /* P76:マトリクスLEDドライバのCSEL2*/
	              /* P77:本マイコンでは，端子無し*/

        PDR8 = 0xf9;  /* ポート設定前に，初期値を先に書き込む(LED消灯) */
                      /* マトリクスLEDも消灯させる(BLANK有効)*/

        PCR8 = 0xff;  /* ポート8の入出力設定(OOOOOOOO)*/
	              /* P80:マトリクスLEDドライバのBLANK */
	              /* P81:マトリクスLEDドライバのSCLK  */
	              /* P82:スピーカ*/
	              /* P83:赤外発光LED(負論理)*/
	              /* P84:赤外発光LED(負論理)*/
	              /* P85:3色LED B青(負論理)*/
	              /* P86:3色LED G緑(負論理)*/
	              /* P87:3色LED R赤(負論理)*/

 	/*ポートBは，入力専用ピンなので，入出力方向の設定は無い*/

	/*なおマトリクスLEDドライバICは，TI社のTLC5920である*/
	/*このヒントだけで，マトリクスLEDを駆動できたら，この実験は，*/
	/*受講しなくても良いレベルかも・・・*/


	ENABLE_MATRIX_BLANK();  /* matrixledの表示を消す(舞台裏を見せない)*/
	ENABLE_MATRIX_LATCH();  /* 基本は，ラッチ有効。 必要時だけ筒抜け*/
	SET_L_MATRIX_SCLK();    /* 基本は，SCLK=L。必要時だけposエッジ発生 */

	DISABLE_MATRIX_SIN();   /* 消灯データの設定 */
	for(i=0;i<16;i++){      /* SIN=0のままで，SCLKに16回のトリガを入力*/
	  SET_H_MATRIX_SCLK();  /* SCLK=Lを前提として，posエッジの発生*/
	  SET_L_MATRIX_SCLK();  /* SCLK=Lに戻す */
	}

	ENABLE_MATRIX_BLANK();  /*以下の筒抜け時と列の変更時は表示させない*/
	DISABLE_MATRIX_LATCH();  ENABLE_MATRIX_LATCH();   /*一瞬だけ筒抜け*/
	SELECT_MATRIX_COLUMN(0);                               /*0列を選択*/
	DISABLE_MATRIX_BLANK();  /*点灯させる(注:消灯データのため光らない)*/
	/* matrix周りの初期化 (ここまで)*/

}

