; Nicolas Rodrigez
; Simple Default Boot Loader
; *for my current hardware setup*
; February 28, 2021

; Dependencies
	.setcpu  "65C02"
    .include "SYMBOLS.inc"
	.include "RAM.inc"
	.include "MACROS.inc"

;--HARDWARE RESET---------------------------------------------------

	.segment "CODE1"
reset:
    sei

;-------------------------------------------------------------------
;   INITIALIZE MEMORY
;
;       ERASE   ZP
;               STACK
;               VIA1 REGISTERS
;		 SETUP  STACK POINTER
;				STRING POINTER
;               IRQ VECTORS
;               
;-------------------------------------------------------------------

init_mem:
    lda #0
    ldy #0
im0:                 	; clear stack
    pha
    iny
    bne im0
im1:					; clear zeropage
    sta 0, y
    iny
    bne im1

    ldx #$ff			; set stack pointer to $ff
    txs

	lda #0
	sta VIA1_DDRA
	sta VIA1_PORTA
	sta VIA1_DDRB
	sta VIA1_PORTB

init_irq:				
    lda #<serv_via1		
    sta IRQ_VIA1		
    lda #>serv_via1 	
    sta IRQ_VIA1 + 1

    lda #<serv_acia		
    sta IRQ_ACIA		
    lda #>serv_acia 	
    sta IRQ_ACIA + 1

init_acia:
    lda #COMM_MODE0         ; receive irq
    sta ACIA_CMD            ; no transmit irq
	lda #CONT_MODE1         ; baud at 19200
    sta ACIA_CNT

init_str:
	lda #<trml_wel
	sta STRING_PTR
	lda #>trml_wel
	sta STRING_PTR + 1
	
	ldy #0
send_strng:
	lda (STRING_PTR), y
	beq done_strng
	sta ACIA_RT
	iny
	delay_cc $00, $f0
	bra send_strng
done_strng:

;-------------------------------------------------------------------
;   Receive Program
;	Execute Program
;
;-------------------------------------------------------------------

	ldy #0
	sty WAIT_VAR

	sty PGM_BEGIN
	lda #$10
	sta PGM_BEGIN + 1

	cli
receive_loop:
	lda WAIT_VAR
	beq receive_loop

	dec WAIT_VAR
	lda ACIA_BYTE
	sta (PGM_BEGIN), y
	iny
	beq inc_pgm_begin
	bra receive_loop
inc_pgm_begin:
	inc PGM_BEGIN + 1
	lda PGM_BEGIN + 1
	cmp #$20
	bne receive_loop
	jmp $1000

;--STRINGS--------------------------------------------------------
   
trml_wel:	.asciiz "Simple Bootloader by Nicolas Rodriguez\r\nVersion 1.0\r\nMaximum program size: 4kB\r\n"

;--DEPENDENCIES---------------------------------------------------

	;.include "DELAY_LIB.inc"
	
;-------------------------------------------------------------------
;   INTERUPTS
;
;-------------------------------------------------------------------

serv_irq:
    pha
    phx
    phy
	
	lda ACIA_SR
	bpl srvi0
	jmp (IRQ_ACIA)
srvi0:
	lda VIA1_IFR
    bpl srvi1
	jmp (IRQ_VIA1)
srvi1:
	ply
	plx
	pla
	rti

serv_acia:
	lda ACIA_RT
	sta ACIA_BYTE
	inc WAIT_VAR
	
	ply
	plx
	pla
	rti

serv_via1:

	ply
    plx
    pla
    rti

;-----------------INTERRUPT VECTORS---------------------------------

	.segment "VECTORS"
	.word reset
	.word reset
	.word serv_irq

	.include "test_pgm.s"