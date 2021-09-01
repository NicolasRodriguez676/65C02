; Nicolas Rodriguez
; Test Program
; February 28, 2021

	.setcpu "65C02"
	.segment "CODE2"

begin_code:
	sei
    ldx #$ff
    txs
	
    ;jsr lcd_init
	;lcd_clr_dspl
;-----------------------------------------------------------------

    lda #0                      ; setup via1 port A
    sta VIA1_PORTA              ; for i2c communication
    sta VIA1_DDRA
dlvi0:
    delay_cc $ff, $ff           ; a little bit of downtime
    jsr detect_lcd              ; check for lcd
    cpy #0
    beq dlvi0

;---- initialize LCD via I2C

    jsr lcd_init

    lcd_print_sline1 hw_str1    ; display message
    lcd_print_sline2 hw_str2
    lcd_print_sline3 hw_str3

;---- initialize CompuJoy

    jsr jystk_init              ; sets VIA1 irq for
                                ; pushbutton

;---- detect CompuJoy

    jsr jystk_get_pos           ; ensure the joystick
    bne dcj0                    ; is present by checking
                                ; the position of the
                                ; joystick
    bra dcj2
dcj0:
    lcd_print_line no_str, LINE_STRT_TWO + 10
    lcd_print_sline4 hw_str4
dcj1:
    delay_cc 0, $ff
    jsr jystk_get_pos
    bne dcj1

dcj2:
    lcd_print_line yes_str, LINE_STRT_TWO + 10
    lcd_print_sline4 clear_line

;---- detect serial port

    lda ACIA_SR
    and #$40
    bne detsp0
    bra detsp2
detsp0:
    lcd_print_line no_str,  LINE_STRT_THR + 13
    lcd_print_sline4 hw_str5

detsp1:
    delay_cc 0, $ff
    lda ACIA_SR
    and #$40
    bne detsp1
detsp2:
    lcd_print_line yes_str, LINE_STRT_THR + 13
    lcd_print_sline4 clear_line


;-----------------------------------------------------------------
	lda VIA1_DDRA
	ora #2
	sta VIA1_DDRA

	lda VIA1_PORTA
	ora #2
	and #$7e
	sta VIA1_PORTA

mini_lp:
	bra mini_lp

hw_str1:	.asciiz "Hardware Detect"
hw_str2:	.asciiz "CompuJoy:"
hw_str3:	.asciiz "Serial Port:"
hw_str4:    .asciiz "Connect CompuJoy"
hw_str5:    .asciiz "Connect Serial Port"

clear_line: .asciiz "                   "
yes_str:	.asciiz "Yes"
no_str:		.asciiz "No "
j_str:		.asciiz "J"
a_str:		.asciiz "A"

	.include "LCD_I2C_LIB.inc"
	.include "I2C_LIB.inc"
	.include "MATH_LIB.inc"
	.include "JOYSTICK_LIB.inc"
	.include "STRING_LIB.inc"

	.segment "FILLER"
	.byte $0

	