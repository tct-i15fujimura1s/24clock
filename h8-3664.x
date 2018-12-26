ENTRY("_start")

OUTPUT_FORMAT("elf32-h8300");
OUTPUT_ARCH("h8300:h8300hn")

MEMORY {
	vectors (r)  : ORIGIN = 0x0000, LENGTH = 0x0034
	rom     (rx) : ORIGIN = 0x0034, LENGTH = 0x7fcc
	ram     (rw) : ORIGIN = 0xf780, LENGTH = 0x0400
	stack   (rw) : ORIGIN = 0xff80, LENGTH = 0x0000
}

SECTIONS {
	.vectors : {
		SHORT(ABSOLUTE(_start))
		SHORT(ABSOLUTE(_start))
		SHORT(ABSOLUTE(_start))
		SHORT(ABSOLUTE(_start))
		SHORT(ABSOLUTE(_start))
		SHORT(ABSOLUTE(_start))
		SHORT(ABSOLUTE(_start))
		SHORT(DEFINED(_int_nmi)?ABSOLUTE(_int_nmi):ABSOLUTE(_start))
		SHORT(DEFINED(_int_trap0)?ABSOLUTE(_int_trap0):ABSOLUTE(_start))
		SHORT(DEFINED(_int_trap1)?ABSOLUTE(_int_trap1):ABSOLUTE(_start))
		SHORT(DEFINED(_int_trap2)?ABSOLUTE(_int_trap2):ABSOLUTE(_start))
		SHORT(DEFINED(_int_trap3)?ABSOLUTE(_int_trap3):ABSOLUTE(_start))
		SHORT(DEFINED(_int_break)?ABSOLUTE(_int_break):ABSOLUTE(_start))
		SHORT(DEFINED(_int_sleep)?ABSOLUTE(_int_sleep):ABSOLUTE(_start))
		SHORT(DEFINED(_int_irq0)?ABSOLUTE(_int_irq0):ABSOLUTE(_start))
		SHORT(DEFINED(_int_irq1)?ABSOLUTE(_int_irq1):ABSOLUTE(_start))
		SHORT(DEFINED(_int_irq2)?ABSOLUTE(_int_irq2):ABSOLUTE(_start))
		SHORT(DEFINED(_int_irq3)?ABSOLUTE(_int_irq3):ABSOLUTE(_start))
		SHORT(DEFINED(_int_wkp)?ABSOLUTE(_int_wkp):ABSOLUTE(_start))
		SHORT(DEFINED(_int_timera)?ABSOLUTE(_int_timera):ABSOLUTE(_start))
		SHORT(ABSOLUTE(_start))
		SHORT(DEFINED(_int_timerw)?ABSOLUTE(_int_timerw):ABSOLUTE(_start))
		SHORT(DEFINED(_int_timerv)?ABSOLUTE(_int_timerv):ABSOLUTE(_start))
		SHORT(DEFINED(_int_sci3)?ABSOLUTE(_int_sci3):ABSOLUTE(_start))
		SHORT(DEFINED(_int_iic)?ABSOLUTE(_int_iic):ABSOLUTE(_start))
		SHORT(DEFINED(_int_adi)?ABSOLUTE(_int_adi):ABSOLUTE(_start))
	} > vectors
	.text : {
		*(.text)
		*(.text.*)
		*(.rodata)
		*(.rodata.*)
		__etext = . ;
	} > rom
	.data : AT(ADDR(.text) + SIZEOF(.text) ) {
		__data = .;
		*(.data)
		__edata = .;
	} > ram
	.bss  : {
		*(.bss)
		*(.COMMON)
		__end = . ;
	} >ram
	.stack : {
		__stack = . ;
		*(.stack)
	} > stack
}
