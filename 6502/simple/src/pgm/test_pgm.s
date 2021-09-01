; Nicolas Rodriguez
; Test Program
; February 28, 2021

	.setcpu "65C02"
	.segment "CODE2"

begin_code:
	sei
    ldx #$ff
    txs
	
;-----------------------------------------------------------------
	
	.asciiz "Hello there"

;-----------------------------------------------------------------
	lda #2
	tsb VIA1_DDRA

	lda VIA1_PORTA
	ora #2
	and #$7e
	sta VIA1_PORTA

mini_lp:
	bra mini_lp




clear_line: .asciiz "                   "

	.include "LCD_I2C_LIB.inc"
	.include "I2C_LIB.inc"
	.include "MATH_LIB.inc"
	.include "JOYSTICK_LIB.inc"
	.include "STRING_LIB.inc"

	.segment "FILLER"
	.byte $0

	