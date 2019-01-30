#define MAIN_C
#include "libs.h"       /* 前のmy3664hの内容は，libs/libs.hへ統合した */
#include "main.h"

volatile int tma_flag=FALSE;
volatile int sec_flag=FALSE;
volatile int tmv_flag=FALSE;
volatile int sec=0;

volatile int tempo_flag=FALSE;
int tempo_compare=0;


unsigned int matrix_led_pattern[8]=
  //{0x007e,0x0011,0x0011,0x0011,0x007e,0x7f00,0x4900,0x4900};
{0x7e7e,0x1111,0x1111,0x0011,0x007e,0x7f00,0x4900,0x4900};
/*列0～7のデータ(詳細は，過去のリストを読め)*/

/* int_timera() や int_imterv() の割込ルーチン(ボトムハーフの処理) */

#pragma interrupt                   /*割込処理ルーチンであることの指定*/
void int_timera(void){              
        volatile static int count=0;
	BIT_CLR(IRR1,6);      /* 本来のタイマA割り込みフラグクリア */
	tma_flag = TRUE;
	//	EI();         /* 必要に応じて EI()を実行  */
	/*32回呼び出されたら，if文の中が実行されて，sec_flagが有効になる*/
	if(++count >= 32){
	      count=0;
	      sec_flag = TRUE;
	      /* 1秒増やす */
	      sec++;
	      
	}

#ifdef DEBUG
       { /*ボトムハーフ処理の余力チェック用コード*/
	 volatile long int loop;
	 for(loop=0;loop<1;loop++); 
       }

	if((IRR1 & 0x40)!=0){ /* 割り込みフラグの状態チェック */
	 ENABLE_LED_GREEN();  /* もし割り込みフラグが立ってたら緑色LED点灯 */
       }
#endif
}

#pragma interrupt
void int_timerv(void){         /* 約1[msec] ごとに呼び出されるようにする*/
       static unsigned int column=0;
       unsigned int i,p;
       /*スピーカ周りの値*/

       //卒業研究で作成してくれたコードから改変。
       //tempoを変更できるように改造 by T.nitta
       //下記のtempoやtempo_compareをconstでなく，変数にする。
       //但し，この計算をボトムハーフで毎回実行すると,処理が重いので
       //トップハーフで計算を行う。
       //       const int tempo=120;
       //       const int tempo_compare = 1000/(tempo * 16 / 60);
       //       const int tempo_compare = 3735 / tempo ;
       static int tempo_count=0;

       TCSRV &= ~0xe0;         /* タイマVの割り込みフラグクリア*/
       tmv_flag=TRUE;
       /*int_timervは，割込禁止のまま,全力動作*/

       /* 1ms ごとにハードリアルタイムで動作させたい処理(matrix_led周り)*/

       column=(column+1) & 0x0007; /* column=(column++)&0x0007は，NGです…  */ 
       ENABLE_MATRIX_LATCH();      /*ラッチを有効にし，舞台裏でD-FFをセット */

       /*16bit (1列分) をシリアル転送*/
       for(p=matrix_led_pattern[column],i=0;i<16;i++,p<<=1){        
	 if((p&0x8000)==0){
	   DISABLE_MATRIX_SIN();
	 }else{
	   ENABLE_MATRIX_SIN();
	 }
	 SET_H_MATRIX_SCLK();SET_L_MATRIX_SCLK();   /* posエッジ一つ*/
       }

       ENABLE_MATRIX_BLANK();   /*以下の筒抜けと列変更は表示させない*/
       DISABLE_MATRIX_LATCH();  ENABLE_MATRIX_LATCH();  /*一瞬筒抜け*/
       SELECT_MATRIX_COLUMN(column);               /*j番目の列に変更*/
       DISABLE_MATRIX_BLANK();                          /*点灯させる*/


       /*スピーカ関連処理*/
       if(snd_check()==TRUE){
	 if(tempo_compare < ++tempo_count){
	   tempo_count = 0;
	   tempo_flag = TRUE;
	 }
       }


#ifdef DEBUG
       { /*ボトムハーフ処理の余力チェック用コード*/
	 volatile int loop;
	 for(loop=0;loop<10;loop++);
	 /*とある条件下での挙動(但し，その後にキー入力のコードを追加)*/
	 /*loop<461 :10分程度では,破綻を確認できず(但し，検証回数は1回だけ)*/
       }

       if((TCSRV & 0xe0)!=0){ /* 割り込みフラグの状態チェック */
	 ENABLE_LED_RED();    /* もし割り込みフラグが立ってたら赤色LED点灯*/
       }
#endif
}

/*タイマ関係の値設定*/
void timer_init(void){
	/*タイマAの設定(1/32[sec]ごとのタイマ割り込みを生成) */
	TMA = 0x9f; /* タイマA TCAとプリスケーラWのリセット(2011/12/15追加) */
	TMA = 0x9b; /* タイマA 1/32[sec]毎の割込の設定                      */
	/* 0x9b => 1001 1011 の内訳 (解説は，過去のリストを見よ)            */

	IENR1 |= 0x40; 	     /* タイマAの割り込み生成を有効にする*/

	/*タイマVの設定(マニュアルを見て,以下の3行の右辺を書き換えよ)*/
	TCORA = 124;    /*割り込み間隔が約1msになるようにする(125分周)*/
	TCRV0 = 0x4b;  /*コンペアマッチAで割り込み。その際にカウンタクリア*/
	TCRV1 = 0x01;  /*クロックは，内部のΦの128分周(16MHz/128)を用いる*/

	/*timervも必ず0からスタートさせる */
	TCNTV = 0x00;          /* タイマVの内部カウンタを0から開始*/
	BIT_CLR(IRR1,6);       /* タイマAの割り込みフラグクリア   */
	TCSRV &= ~0xe0;        /* タイマVの割り込みフラグクリア   */

	/* タイマWの設定(スピーカ用)*/
	TCRW = 0xbc;
}

/* main() などの割り込みルーチン以外の処理は，トップハーフの処理*/

/* User Interface のステートマシン (ここを作り込む)*/

int main(void){
  //UI_DATA* ui_data;
	unsigned char sw=KEY_NONE;

	//	char test[]="hoge";

	DI();           /* 念のために速やかにDI() */ 
	io_init();      /* I/Oポートの初期化ルーチン(matrix,lcd周りも含む) */
	lcd_init();     /* LCDの初期化ルーチンを実行 */

	/*LCDの初期状態は，カーソルOFFで,ブリンクもOFFと仮にしておく*/
	lcd_cursor(OFF);
	lcd_blink(OFF); 

	/*スピーカ用のテンポ計算(とりあえずの初期値を設定)*/
	{
	  int tempo=120;
	  tempo_compare = 3735 / tempo; /* 1000/(tempo * 16 / 60) を展開 */
	}

	timer_init();   /* タイマの初期設定を実行(EIの直前に実行) */
        EI();	        /* 割り込みを許可  */

	//半角カタカナ一覧:入力できないなら，ここからコピペせよ。
	//ｱｲｳｴｵ ｶｷｸｹｺ ｻｼｽｾｿ ﾀﾁﾂﾃﾄ ﾅﾆﾇﾈﾉ ﾊﾋﾌﾍﾎ ﾏﾐﾑﾒﾓ ﾗﾘﾙﾚﾛ ﾔﾕﾖ ﾜｦﾝ ﾞﾟ ｯ ｧｨｩｪｫ

	for(;;){	  /* 組み込みシステムは，基本的には無限ループで実行 */


	  /* 以下のif文の中は，多分，1/32秒ごとに処理を行う*/
	  if(tma_flag == TRUE){ 
	    sw = sw_mng();       /* スイッチの入力チェック libs/key.c */

	    
	    
	    /*ui_data=*/ui(sw);           /* ユーザインタフェースの実行 */
	    
	    tma_flag=FALSE;
	  }

	  /* 以下の処理は，1msごとに行う */
	  if(tmv_flag==TRUE){

#if 0
	    /*スピーカのダイレクトコントロールは，beta2から*/
	    /*できないようになりました。*/
	    /*スピーカ用のポートは，タイマWというタイマから*/
	    /*タイマ出力を直接出力することになります*/
	    /*スピーカ用の関数 snd_mng(),snd_play(),snd_stop()などを*/
	    /*活用すること*/

	    /*とりあえず，モード1の時に，上のキーが短押しされたら，   */
	    /*(正確には，短押しの離された時に)，500Hzの音を短く出す   */
	    /*ようなスピーカのサンプルコード。                        */
	    /*スマートにスピーカを鳴らすのだったら，どこかで， フラグ */
	    /*が立ったら， FLIP_SPEAKER_DIRECT_CONTROL();を実行する   */
	    /*ようにするのだろうなぁ。きっと。多分。                  */
	    if(ui_data!=NULL && ui_data->mode==MODE_1 && sw==KEY_SHORT_U){
		FLIP_SPEAKER_DIRECT_CONTROL();
	    }
#endif
	    tma_flag=FALSE;
	  }

	  if(tempo_flag){
	    snd_mng();
	    tempo_flag = FALSE;
	  }


	  SLEEP(); /*割込が入るまで，スリープさせる(消費電力低減!)*/
	}
	return 0;  /*この行は実行されないが、警告を出さないおまじない*/
}

/*************************************************************
 (1)  同様の指示で省略
 (2)  afp://iemac.ie.tokuyama.ac.jp/i0/nitta/2014exp4/9_2014beta_sample/ の
     (以下も同様で省略)
 (3) 同様の指示で省略
 (4)～(9) 初回と同様。
 ************************************************************/
