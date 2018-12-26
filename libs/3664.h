/*
 * 3664.h : H8/3664 の入出力レジスタの定義
 * ルネサスのホームページより入手したものを改変したものである。
 */
#ifndef __3664_H__
#define __3664_H__

/*
 * addr をバイト(8bit)／ワード(16bit)アクセスするためのマクロ
 */
#define	VOLATILE_BYTE(addr)		(*(volatile unsigned char*)(addr))
#define	VOLATILE_WORD(addr)		(*(volatile unsigned short*)(addr))

/** タイマW関係のレジスタ */
#define	TMRW	VOLATILE_BYTE(0xff80)
#define	TCRW	VOLATILE_BYTE(0xff81)
#define	TIERW	VOLATILE_BYTE(0xff82)
#define	TSRW	VOLATILE_BYTE(0xff83)
#define	TIOR0	VOLATILE_BYTE(0xff84)
#define	TIOR1	VOLATILE_BYTE(0xff85)
#define	TCNT	VOLATILE_WORD(0xff86)
#define	GRA		VOLATILE_WORD(0xff88)
#define	GRB		VOLATILE_WORD(0xff8a)
#define	GRC		VOLATILE_WORD(0xff8c)
#define	GRD		VOLATILE_WORD(0xff8e)

/** フラッシュメモリ(ROM)関係のレジスタ */
#define	FLMCR1	VOLATILE_BYTE(0xff90)
#define	FLMCR2	VOLATILE_BYTE(0xff91)
#define	FLPWCR	VOLATILE_BYTE(0xff92)
#define	EBR1	VOLATILE_BYTE(0xff93)
#define	FENR	VOLATILE_BYTE(0xff9b)

/** タイマV関係のレジスタ */
#define	TCRV0	VOLATILE_BYTE(0xffa0)
#define	TCSRV	VOLATILE_BYTE(0xffa1)
#define	TCORA	VOLATILE_BYTE(0xffa2)
#define	TCORB	VOLATILE_BYTE(0xffa3)
#define	TCNTV	VOLATILE_BYTE(0xffa4)
#define	TCRV1	VOLATILE_BYTE(0xffa5)

/** タイマA関係のレジスタ */
#define	TMA		VOLATILE_BYTE(0xffa6)
#define	TCA		VOLATILE_BYTE(0xffa7)

/** SCI3(RS-232C)関係のレジスタ */
#define	SMR		VOLATILE_BYTE(0xffa8)
#define	BRR		VOLATILE_BYTE(0xffa9)
#define	SCR3	VOLATILE_BYTE(0xffaa)
#define	TDR		VOLATILE_BYTE(0xffab)
#define	SSR		VOLATILE_BYTE(0xffac)
#define	RDR		VOLATILE_BYTE(0xffad)

/** A/D変換器関係のレジスタ */
#define	ADDRA	VOLATILE_WORD(0xffb0)
#define	ADDRB	VOLATILE_WORD(0xffb2)
#define	ADDRC	VOLATILE_WORD(0xffb4)
#define	ADDRD	VOLATILE_WORD(0xffb6)
#define	ADCSR	VOLATILE_BYTE(0xffb8)
#define	ADCR	VOLATILE_BYTE(0xffb9)

/** ウオッチドッグ関係のレジスタ */
#define	TCSRWD	VOLATILE_BYTE(0xffc0)
#define	TCWD	VOLATILE_BYTE(0xffc1)
#define	TMWD	VOLATILE_BYTE(0xffc2)

/** I2Cバス関係のレジスタ */
#define	ICCR	VOLATILE_BYTE(0xffc4)
#define	ICSR	VOLATILE_BYTE(0xffc5)
#define	ICDR	VOLATILE_BYTE(0xffc6)
#define	SARX	VOLATILE_BYTE(0xffc6)
#define	ICMR	VOLATILE_BYTE(0xffc7)
#define	SAR		VOLATILE_BYTE(0xffc7)
#define	TSCR	VOLATILE_BYTE(0xfffc)

/** アドレスブレーク関係のレジスタ */
#define	ABRKCR	VOLATILE_BYTE(0xffc8)
#define	ABRKSR	VOLATILE_BYTE(0xffc9)
#define	BARH	VOLATILE_BYTE(0xffca)
#define	BARL	VOLATILE_BYTE(0xffcb)
#define	BDRH	VOLATILE_BYTE(0xffcc)
#define	BDRL	VOLATILE_BYTE(0xffcd)

/** I/Oポート関係のレジスタ */
#define	PUCR1	VOLATILE_BYTE(0xffd0)
#define	PUCR5	VOLATILE_BYTE(0xffd1)
#define	PDR1	VOLATILE_BYTE(0xffd4)
#define	PDR2	VOLATILE_BYTE(0xffd5)
#define	PDR5	VOLATILE_BYTE(0xffd8)
#define	PDR7	VOLATILE_BYTE(0xffda)
#define	PDR8	VOLATILE_BYTE(0xffdb)
#define	PDRB	VOLATILE_BYTE(0xffdd)
#define	PMR1	VOLATILE_BYTE(0xffe0)
#define	PMR5	VOLATILE_BYTE(0xffe1)
#define	PCR1	VOLATILE_BYTE(0xffe4)
#define	PCR2	VOLATILE_BYTE(0xffe5)
#define	PCR5	VOLATILE_BYTE(0xffe8)
#define	PCR7	VOLATILE_BYTE(0xffea)
#define	PCR8	VOLATILE_BYTE(0xffeb)

/** 省エネ関係のレジスタ */
#define	SYSCR1	VOLATILE_BYTE(0xfff0)
#define	SYSCR2	VOLATILE_BYTE(0xfff1)
#define	MSTCR1	VOLATILE_BYTE(0xfff9)

/** 割り込み関係のレジスタ */
#define	IEGR1	VOLATILE_BYTE(0xfff2)
#define	IEGR2	VOLATILE_BYTE(0xfff3)
#define	IENR1	VOLATILE_BYTE(0xfff4)
#define	IRR1	VOLATILE_BYTE(0xfff6)
#define	IWPR	VOLATILE_BYTE(0xfff8)

/** EEPROM のレジスタ */
#define	EKR		VOLATILE_BYTE(0xff10)

#endif /* __3664_H__ */

