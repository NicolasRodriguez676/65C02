; Nicolas Rodriguez
; Test Program (Complex)
; April 24, 2021

	.setcpu "65C02"
	.segment "PGM_ROM"
	
begin_code:
	sei
    ldx #$ff
    txs

;-----------------------------------------------------------------
	
	lcd_clr_dspl
	lcd_print_sline1 hello_there
	lcd_print_sline2 general

	jsr sound_init

	set_octave OCTAVE_TWO

	lda song_data
	jsr sound_write_note

	; set up via1
	set_address IRQ_VIA1 + 2, snd_irq_v1_t2
    
	lda #(VIA_IRQ_EN7 | VIA_IRQ_EN5)
    sta VIA1_IER            ; enable t2 irq
	stz VIA1_ACR

repeat_song:
	ldy #$ff
song_loop:
	
	;---- get note
	iny
	lda song_data, y
	cmp #$ff
	beq song_done
	jsr sound_write_note
	
	;--- get delay
	iny
	ldx song_data, y
	stx sound_temp
	lda ovfl_lut, x
	sta counter_ovfl

	;--- check octave
	iny
	lda song_data, y
	beq skip_octave
	jsr sound_write_octave
skip_octave:

	; note delay
	cli
	lda #16
	sta VIA1_T2_L
	stz VIA1_T2_H
note_loop:
	lda counter_ovfl
	cmp #$80
	bne note_loop
	sei
	bra song_loop
song_done:
	lda #NOTE_NULL
	jsr sound_write_note
	delay_cc $ff, $ff
	delay_cc $ff, $ff
	delay_cc $ff, $ff
	delay_cc $ff, $ff
	delay_cc $ff, $ff
	delay_cc $ff, $ff
	delay_cc $ff, $ff
	delay_cc $ff, $ff
	delay_cc $ff, $ff
	jmp repeat_song
	
;-----------------------------------------------------------------

end_loop:
	bra end_loop

	; data i.e. messages, look up tables and include files
	
hello_there:	.asciiz " Hello there"
general:		.asciiz "General Kenobi"


song_data:		.byte NOTE_A,   TEMPO_DIV_04, OCT_CHNG_NULL
				.byte NOTE_D,   TEMPO_DIV_04, OCT_CHNG_NULL
				.byte NOTE_A,   TEMPO_DIV_04, OCT_CHNG_NULL
				.byte NOTE_D,   TEMPO_DIV_04, OCT_CHNG_NULL
				.byte NOTE_A,   TEMPO_DIV_04, OCT_CHNG_NULL
				.byte NOTE_D,   TEMPO_DIV_04, OCT_CHNG_NULL
				.byte NOTE_A,   TEMPO_DIV_08, OCT_CHNG_NULL
				.byte NOTE_Gg,  TEMPO_DIV_08, OCT_CHNG_ONE
				.byte NOTE_A,   TEMPO_DIV_04, OCT_CHNG_TWO
				.byte NOTE_A,   TEMPO_DIV_08, OCT_CHNG_NULL
				.byte NOTE_Gg,  TEMPO_DIV_08, OCT_CHNG_ONE
				.byte NOTE_A,   TEMPO_DIV_08, OCT_CHNG_TWO
				.byte NOTE_G,   TEMPO_DIV_08, OCT_CHNG_ONE
				.byte NOTE_NULL,TEMPO_DIV_04, OCT_CHNG_NULL
				.byte NOTE_Ff,  TEMPO_DIV_08, OCT_CHNG_NULL
				.byte NOTE_G,   TEMPO_DIV_08, OCT_CHNG_NULL    
				.byte NOTE_Ff,  TEMPO_DIV_08, OCT_CHNG_NULL
				.byte NOTE_F,   TEMPO_DIV_02, OCT_CHNG_NULL
				.byte NOTE_D,   TEMPO_DIV_04, OCT_CHNG_NULL
				.byte NOTE_NULL,TEMPO_DIV_04, OCT_CHNG_NULL        
				.byte NOTE_A,   TEMPO_DIV_04, OCT_CHNG_TWO
				.byte NOTE_D,   TEMPO_DIV_04, OCT_CHNG_NULL
				.byte NOTE_A,   TEMPO_DIV_04, OCT_CHNG_NULL
				.byte NOTE_D,   TEMPO_DIV_04, OCT_CHNG_NULL
				.byte NOTE_A,   TEMPO_DIV_04, OCT_CHNG_NULL
				.byte NOTE_D,   TEMPO_DIV_04, OCT_CHNG_NULL
				.byte NOTE_A,   TEMPO_DIV_08, OCT_CHNG_NULL
				.byte NOTE_Gg,  TEMPO_DIV_08, OCT_CHNG_ONE
				.byte NOTE_A,   TEMPO_DIV_04, OCT_CHNG_TWO
				.byte NOTE_G,   TEMPO_DIV_02, OCT_CHNG_NULL
				.byte NOTE_G,   TEMPO_DIV_02, OCT_CHNG_NULL
				.byte NOTE_Ff,  TEMPO_DIV_02, OCT_CHNG_NULL
				.byte NOTE_G,   TEMPO_DIV_04, OCT_CHNG_NULL
				.byte NOTE_C,   TEMPO_DIV_04, OCT_CHNG_TWO
				.byte NOTE_BB,  TEMPO_DIV_04, OCT_CHNG_NULL
				.byte NOTE_A,   TEMPO_DIV_04, OCT_CHNG_NULL
				.byte NOTE_G,   TEMPO_DIV_04, OCT_CHNG_ONE
				.byte NOTE_A,   TEMPO_DIV_04, OCT_CHNG_TWO
				.byte NOTE_D,   TEMPO_DIV_04, OCT_CHNG_NULL
				.byte NOTE_A,   TEMPO_DIV_04, OCT_CHNG_NULL
				.byte NOTE_D,   TEMPO_DIV_04, OCT_CHNG_NULL
				.byte NOTE_A,   TEMPO_DIV_04, OCT_CHNG_NULL
				.byte NOTE_D,   TEMPO_DIV_04, OCT_CHNG_NULL
				.byte NOTE_A,   TEMPO_DIV_08, OCT_CHNG_NULL
				.byte NOTE_Gg,  TEMPO_DIV_08, OCT_CHNG_ONE
				.byte NOTE_A,   TEMPO_DIV_04, OCT_CHNG_TWO
				.byte NOTE_C,   TEMPO_DIV_04, OCT_CHNG_NULL
				.byte NOTE_C,   TEMPO_DIV_04, OCT_CHNG_NULL
				.byte NOTE_A,   TEMPO_DIV_04, OCT_CHNG_NULL
				.byte NOTE_G,   TEMPO_DIV_04, OCT_CHNG_ONE
				.byte NOTE_F,   TEMPO_DIV_02, OCT_CHNG_NULL
				.byte NOTE_D,   TEMPO_DIV_02, OCT_CHNG_NULL
				.byte NOTE_D,   TEMPO_DIV_02, OCT_CHNG_NULL
				.byte NOTE_F,   TEMPO_DIV_02, OCT_CHNG_NULL
				.byte NOTE_A,   TEMPO_DIV_02, OCT_CHNG_TWO
				.byte NOTE_C,   TEMPO_DIV_02, OCT_CHNG_NULL
				.byte NOTE_EE,  TEMPO_DIV_04, OCT_CHNG_NULL
				.byte NOTE_D,   TEMPO_DIV_04, OCT_CHNG_NULL
				.byte NOTE_Gg,  TEMPO_DIV_04, OCT_CHNG_ONE
				.byte NOTE_A,   TEMPO_DIV_04, OCT_CHNG_TWO
				.byte $ff

	.include "SOUND_LIB.inc"
