; Nicolas Rodrigez
; LCD Library via I2C controller (PCF8574T)
; February 28, 2021


; Hardware Configuration
; 
; I2C		LCD
; 
; P0	-	RS
; P1	-	R/W
; P2	-	E
; P3	-	backlight. active high.
; 
; P4	-	DB4/DB0
; P5	-	DB5/DB1
; P6	-	DB6/DB2
; P7	-	DB7/DB3
; 
; Writing a high to P is driving the output high
; with a weak current source, which can be externally
; forced low or high. (input mode)
; 
; Writing a low to P is driving the output low
; 
; Page 33 in datasheet is a goodstart
; https://eater.net/datasheets/HD44780.pdf


;--Instructions-----------------------------------------

; $08 -- enable backlight
; $04 -- toggle E
;
; new data can be sent when E goes low
; removes one byte and one i2c write routine

; function set		(set_func)
; display on/off	(ctr_dspl)
; entry mode set	(set_mode)
; write data

; these first three rows are read only!!
set_lcd:		.byte $28, $2c, $28, $2c, $88, $8c, $00
rtn_home:		.byte $08, $0c, $28, $2c, $00
clr_dspl:		.byte $08, $0c, $18, $1c, $00

; read/write memory locations with default values
;lcd_set_opt:	.byte $08, $0c, $08, $0c, $00
;wrt_data:		.byte $09, $0d, $09, $0d, $00
;
;lcd_set_addr:	.byte $88, $8c, $08, $0c, $00
;
;lcd_rd_addr:	.byte $fa, $fe, $fa, $fe, $00
;lcd_rd_data:	.byte $fb, $ff, $fb, $ff, $00

;--Constants-----------------------------------------

LCD_R			=	$4f		; I2C address for reading
LCD_W			=	$4e		; I2C address for writing

LCD_ENOT		=	$08
LCD_E			=	$0c
LCD_RW			=	$0a
LCD_RS			=	$09

;--Control Words--------------------------------------
;
;	no options for clear display and return home
;
;	data split in advance to accommodate I2C and
;	four bit mode
;
;	option 3 - display data is unchanged
;			   cursor or display is shifted
;
;-----------------------------------------------------


; a few symbols are defined in SYMBOLS.inc

; function set
; defined as 4 bit mode. 5x8 font. 2 lines (4 for current lcd)

; set cgram address (address 1)
; lower 6 bits are the new address
W_CGRAM_ADR		=	$40	

; set ddram address (address 2)
; lower 7 bits are the new address
W_DDRAM_ADR		=	$80

; read busy flag and address
; includes backlight.
LCD_RFLAG		=	LCD_RW

; write data to cg/dd ram
; includes backlight
LCD_WDFLAG		=	LCD_RS

; read data from cg/dd ram
LCD_RDFLAG		=   LCD_RW | LCD_RS


;--LCD SUBROUTINES------------------------------------
;
;	DISABLES IRQ AT BEGINNING OF EACH
;	SUBROUTINE
;	WILL MESS UP LCD, OTHERWISE

lcd_init:
	; start 4 bit mode
	set_address DATA_PTR, set_lcd
	jsr write_i2c

	; configure display
	set_address DATA_PTR, lcd_set_opt
	lcd_options LCD_DISP_CSR, $04
	lcd_options LCD_MODE_SET, $02
	lcd_rtn_home

	lcd_wait_long
	rts

lcd_print:
	sei
	tay
	and #$f0
	ora #LCD_RS
	sta wrt_data
	ora #(LCD_RS | LCD_E)
	sta wrt_data + 1		
	tya
	and #$0f
	asl
	asl
	asl
	asl
	ora #LCD_RS
	sta wrt_data + 2
	ora #(LCD_RS | LCD_E)
	sta wrt_data + 3		

	set_address DATA_PTR, wrt_data
	jmp write_i2c

detect_lcd:
	sei
	ldy #9					; try eight times
dlcd0:
	dey
	beq dlcd1
	jsr i2c_start
	lda #LCD_W
	jsr send_i2c_byte
	txa
	and #$80
	php
	jsr i2c_stop
	plp
	bmi dlcd0
dlcd1:
	rts						; that it was initialized

lcd_set_ddram_sr:
    lda VAR000
    and #$f0
    ora #$88
    sta lcd_set_addr
    ora #$04
    sta lcd_set_addr + 1
    lda VAR000
    asl
    asl
    asl
    asl
    ora #$08
    sta lcd_set_addr + 2
    ora #$04
    sta lcd_set_addr + 3
    set_address DATA_PTR, lcd_set_addr
    jsr write_i2c
	rts

lcd_set_cgram_sr:
    lda VAR000
    and #$70
    ora #$48
    sta lcd_set_addr
    ora #$04
    sta lcd_set_addr + 1
    lda VAR000
    asl
    asl
    asl
    asl
    ora #$08
    sta lcd_set_addr + 2
    ora #$04
    sta lcd_set_addr + 3
    set_address DATA_PTR, lcd_set_addr
    jsr write_i2c
	rts

lcd_read_data:
	jsr i2c_start
	lda #LCD_W
	jsr send_i2c_byte
	lda lcd_rd_data
	jsr send_i2c_byte
	jsr i2c_stop

	jsr i2c_start
	lda #LCD_W
	jsr send_i2c_byte
	lda lcd_rd_data + 1
	jsr send_i2c_byte
	jsr i2c_stop

    jsr i2c_start
    lda #LCD_R
    jsr send_i2c_byte
    jsr rcv_i2c_byte
    jsr i2c_nak
    jsr i2c_stop
    lda I2C_TEMP
    sta VAR000

	jsr i2c_start
	lda #LCD_W
	jsr send_i2c_byte
	lda lcd_rd_data + 2
	jsr send_i2c_byte
	jsr i2c_stop

	jsr i2c_start
	lda #LCD_W
	jsr send_i2c_byte
	lda lcd_rd_data + 3
	jsr send_i2c_byte
	jsr i2c_stop

    jsr i2c_start
    lda #LCD_R
    jsr send_i2c_byte
    jsr rcv_i2c_byte
    jsr i2c_nak
    jsr i2c_stop
    lda I2C_TEMP
    sta VAR001

    jsr i2c_start
    lda #LCD_W
    jsr send_i2c_byte
    lda #$08
    jsr send_i2c_byte
	jsr i2c_stop
    rts

lcd_read_addr:
	jsr i2c_start
	lda #LCD_W
	jsr send_i2c_byte
	lda lcd_rd_addr
	jsr send_i2c_byte
	jsr i2c_stop

	jsr i2c_start
	lda #LCD_W
	jsr send_i2c_byte
	lda lcd_rd_addr + 1
	jsr send_i2c_byte
	jsr i2c_stop

    jsr i2c_start
    lda #LCD_R
    jsr send_i2c_byte
    jsr rcv_i2c_byte
    jsr i2c_nak
    jsr i2c_stop
    lda I2C_TEMP
    sta VAR000

	jsr i2c_start
	lda #LCD_W
	jsr send_i2c_byte
	lda lcd_rd_addr + 2
	jsr send_i2c_byte
	jsr i2c_stop

	jsr i2c_start
	lda #LCD_W
	jsr send_i2c_byte
	lda lcd_rd_addr + 3
	jsr send_i2c_byte
	jsr i2c_stop

    jsr i2c_start
    lda #LCD_R
    jsr send_i2c_byte
    jsr rcv_i2c_byte
    jsr i2c_nak
    jsr i2c_stop
    lda I2C_TEMP
    sta VAR001

    jsr i2c_start
    lda #LCD_W
    jsr send_i2c_byte
    lda #$08
    jsr send_i2c_byte
	jsr i2c_stop
    rts

;--LCD I2C SUBROUTINES--------------------------------

write_i2c:
	sei
	jsr i2c_start
	lda #LCD_W
	jsr send_i2c_byte
	ldy #0
wib0:
	lda (DATA_PTR), y
	beq wib1
	iny
	jsr send_i2c_byte
	bra wib0
wib1:
	lda #$08
	jsr send_i2c_byte
	jsr i2c_stop
	rts


