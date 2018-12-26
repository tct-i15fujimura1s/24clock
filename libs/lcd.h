#ifndef _LCD_H
#define _LCD_H
/*
 * ファイル名：lcd.c
 * 用途：LCDの操作を簡単に行う機能を内蔵する。
 */
extern void lcd_init(void);           /* LCDの初期化を行います。 */

extern void lcd_enable(int);          /* LCDの表示ON/OFFを切り替えます。 */

extern void lcd_cursor(int);          /* LCDのカーソルON/OFFを切り替えます。*/

extern void lcd_blink(int);           /* LCDのカーソルブリンク（点滅）ON/OFF*/
                                      /* を切り替えます。 */

extern void lcd_putstr(int, int, char *);     /* LCDの指定された座標へ文字を*/
					      /*出力します。 */


#endif
