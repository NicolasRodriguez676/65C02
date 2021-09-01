; Nicolas Rodrigez
; Complex Boot Loader
; *for my current hardware setup*
; April 1, 2021

; Dependencies
	.setcpu  "65C02"

	.include "RAM.inc"
    .include "SYMBOLS.inc"
	.include "MACROS.inc"

;--HARDWARE RESET---------------------------------------------------

	.segment "INIT"

reset:
    sei

;--INIT MEM---------------------------------------------------------

init:
    lda #0
    ldy #0
in0:                 	; clear stack
    pha
    iny
    bne in0
in1:					; clear zeropage
    sta $0, y
    iny
    bne in1

    ldx #$ff			; set stack pointer to $ff
    txs

    ldy #0
in2:
    lda data_lib, y
    cmp #$aa
    beq in3
    sta lcd_set_opt, y
    iny
    bra in2
in3:


;--INIT HW----------------------------------------------------------

;---- setup ACIA
    lda #COMM_MODE0     ; receive irq
    sta ACIA_CMD        ; no transmit irq
    lda #CONT_MODE1     ; baud at 19200
    sta ACIA_CNT
    

;---- detect & initialize LCD via I2C

    lda #0
    sta VIA1_PORTA
    sta VIA1_DDRA
    sta VIA1_PORTB
    sta VIA1_DDRB
    sta VIA2_PORTA
    sta VIA2_DDRA
    sta VIA2_PORTB
    sta VIA2_DDRB

    jsr lcd_init

serial_gone:

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
    lcd_print_sline4 clear_str16

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
    lcd_print_sline4 clear_str16

    delay_cc $ff, $ff
    delay_cc $ff, $ff
    delay_cc $ff, $ff
    delay_cc $ff, $ff
    delay_cc $ff, $ff
    delay_cc $ff, $ff
    delay_cc $ff, $ff
    delay_cc $ff, $ff

;--INIT BL----------------------------------------------------------
    
    .segment "BL"

;---- welcome message
    lcd_clr_dspl
    lcd_print_sline1 menu_str1
    lcd_print_sline2 menu_str2

;---- set up irq locations

    ; ignore CA1/CA2
	;lda #(VIA_IRQ_EN1 | VIA_IRQ_EN0)
    ;trb VIA1_IER

    ; respond to timer 2 in via1
    lda #(VIA_IRQ_EN7 | VIA_IRQ_EN5)
    tsb VIA1_IER            ; enable t2 irq
    lda #T2_MODE1
    trb VIA1_ACR            ; t2 set to timed irq

    set_address IRQ_ACIA +  0, serv_acia_rcv1
    set_address IRQ_ACIA +  2, serv_acia_dsr

    set_address IRQ_VIA1 +  0, srvi1
    set_address IRQ_VIA1 +  2, serv_via1_t2
    set_address IRQ_VIA1 +  4, srvi1
    set_address IRQ_VIA1 +  6, srvi1
    set_address IRQ_VIA1 +  8, srvi1
    set_address IRQ_VIA1 + 10, srvi1
    set_address IRQ_VIA1 + 12, srvi1

    set_address IRQ_VIA2 +  0, srvi1
    set_address IRQ_VIA2 +  2, srvi1
    set_address IRQ_VIA2 +  4, srvi1
    set_address IRQ_VIA2 +  6, srvi1
    set_address IRQ_VIA2 +  8, srvi1
    set_address IRQ_VIA2 + 10, srvi1
    set_address IRQ_VIA2 + 12, srvi1

;---- set up location to store data
	stz PGM_BEGIN
	lda #$10
	sta PGM_BEGIN + 1

;---- wait for data loop
    ldy #0
    sty WAIT_VAR

    cli

bl_loop:
    ; regular stuff
    lda WAIT_VAR
    beq bl_loop  
    sei
    bpl bll0
    lcd_clr_dspl
    jmp serial_gone

bll0:
    ;---- house cleaning
    lda #(VIA_IRQ_EN1 | VIA_IRQ_EN0)
    tsb VIA1_IER

    set_address IRQ_ACIA +  0, serv_acia_rcv2
    set_address IRQ_VIA1 +  2, srvi1

    lcd_print_sline4 menu_str3

    delay_cc $ff, $ff
    delay_cc $ff, $ff
    delay_cc $ff, $ff
    delay_cc $ff, $ff
    delay_cc $ff, $ff

    ;---- jump to loaded program
    jmp $1000

;--MESSAGES---------------------------------------------------------
hw_str1:	.asciiz "Hardware Detect"
hw_str2:	.asciiz "CompuJoy:"
hw_str3:	.asciiz "Serial Port:"
hw_str4:	.asciiz "Connect CompuJoy"
hw_str5:	.asciiz "Connect Serial Port"

clear_str16:.asciiz "                   "

yes_str:	.asciiz "Yes"
no_str:		.asciiz "No "

menu_str1:  .asciiz "CompuTerm ver. 1.0"
menu_str2:  .asciiz "Load a program!"
menu_str3:  .asciiz "Program loaded..."

data_lib:   .byte $09, $0d, $09, $0d, $00
            .byte $88, $8c, $08, $0c, $00
            .byte $fa, $fe, $fa, $fe, $00
            .byte $fb, $ff, $fb, $ff, $00
            .byte $01, $00, $00
            .byte $11, $00
            .byte $06, $00
            .byte $aa

;--ROM LIB----------------------------------------------------------

    .segment "ROM_LIB"

	.include "LCD_I2C_LIB.inc"
	.include "I2C_LIB.inc"
	.include "MATH_LIB.inc"
	.include "JOYSTICK_LIB.inc"
	.include "STRING_LIB.inc"

;--IRQ VECTORS------------------------------------------------------
   
    .segment "ROM_IRQ"

serv_irq:
    pha
    phx
    phy

;---------------------------- Check ACIA
	lda ACIA_SR
	bpl srvi0
    and #$08
    bne rcv_reg_full
    ldx #2
    jmp (IRQ_ACIA, x)      ; DSR level change
rcv_reg_full:
    jmp (IRQ_ACIA)

;---------------------------- Check VIA1
srvi0:
	lda VIA1_IFR
    bpl srvi3              ; when via2 is present physically
    asl
    ldx #$fe
srvi2:
    inx
    inx
    cpx #14
    beq srvi1
    asl
    bcc srvi2
	jmp (IRQ_VIA1, x)

;---------------------------- Check VIA2
srvi3:
    lda VIA2_IFR
    bpl srvi1
    asl
    ldx #$fe
srvi4:
    inx
    inx
    cpx #14
    beq srvi1
    asl
    bcc srvi4
	jmp (IRQ_VIA2, x)

;---------------------------- No irq
srvi1:

	ply
	plx
	pla
	rti

;---------------------------- acia recieve register full
serv_acia_rcv1:
	lda ACIA_RT

    ;---- store data byte
	ply
	sta (PGM_BEGIN), y
	iny
	beq inc_pgm_begin
	bra arm_t2
inc_pgm_begin:
	inc PGM_BEGIN + 1
	lda PGM_BEGIN + 1
arm_t2:
    lda #$ff
    sta VIA1_T2_L
    sta VIA1_T2_H

	plx
	pla
	rti

;---------------------------- acia dsr input changed
serv_acia_dsr:

    ; if dsr is high, reset the system
    ; also useful for resetting the 6502 to load
    ; a new program. the rest of the system is only
    ; affected by the reset point

    lda ACIA_SR
    asl
    bcs dsr_is_lo
    lda #$80
    sta WAIT_VAR
dsr_is_lo:
	ply
	plx
	pla
	rti

;---------------------------- acia recieve register full
serv_acia_rcv2:
    lda ACIA_RT
    sta ACIA_RT
    sta ACIA_BYTE

    ply
    plx
    pla
    rti

;---------------------------- via1 timer 2 timeout
serv_via1_t2:
    lda VIA1_T2_L      ; clr irq

    ;---- timeout has occured
    ;     jump to $1000 after exiting the
    ;     irq routine
    lda #1
    sta WAIT_VAR

	ply
    plx
    pla
    rti

	.segment "VECTORS"
	.word reset
	.word reset
	.word serv_irq

    ; binary data placed in seperate file
    .include "test_cxpgm.s"
