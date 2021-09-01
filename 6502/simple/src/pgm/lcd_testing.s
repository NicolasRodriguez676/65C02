; Nicolas Rodrigez
; LCD Library Test (I2C)
; February 28, 2021

	.setcpu "65C02"
	.segment "CODE2"

begin_code:
	sei
    ldx #$ff
    txs

	lda #$ff
	sta VIA1_PORTB
	sta VIA1_DDRB
	lda #0
	sta VIA1_PORTA
	sta VIA1_DDRA
	sta VIA1_PORTB
	sta VIA1_DDRB

	jsr lcd_init

	;lcd_clr_dspl
	;lcd_rtn_home

	lcd_print_str widowmaker

	lda VIA1_DDRA
	ora #2
	sta VIA1_DDRA

	lda VIA1_PORTA
	ora #2
	sta VIA1_PORTA

mini_lp:
	bra mini_lp

widowmaker:	.asciiz "widowmaker"

	.include "LCD_I2C_LIB.inc"
	.include "I2C_LIB.inc"
	;.include "MATH_LIB.inc"
	;.include "STRING_LIB.inc"
	.segment "FILLER"
	.byte $0