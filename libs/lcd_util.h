#ifndef _LCD_UTIL_H
#define _LCD_UTIL_H

/* LCDの指定された座標へ整数を出力する関数のプロトタイプ宣言 */
extern void lcd_putdec(int, int, int, int);
extern void lcd_putudec(int, int, int, unsigned int);

extern void lcd_key_debug_out(int x,int y,unsigned char key_data);

extern void lcd_clear(void);

#endif
