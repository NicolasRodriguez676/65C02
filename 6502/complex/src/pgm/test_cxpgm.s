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
	set_octave OCT_CHNG_TWO

    lda #SR_MODE7
    tsb VIA2_ACR

lsdf:
	jsr jystk_get_pos
	sta VIA2_SR
	bmi skip_note
	tax
	lda note_lut, x
	jsr sound_write_note
skip_note:
	bra lsdf
	
;-----------------------------------------------------------------

end_loop:
	bra end_loop

	; data i.e. messages, look up tables and include files
	
hello_there:	.asciiz " Hello there"
general:		.asciiz "General Kenobi"

note_lut:		.byte NOTE_NULL, NOTE_E, NOTE_E, NOTE_C, NOTE_E
				.byte NOTE_Gg, NOTE_E, NOTE_Cc, NOTE_E
				.byte NOTE_Gg, NOTE_E, NOTE_Cc, NOTE_E
				.byte NOTE_Gg, NOTE_E, NOTE_Cc, NOTE_E
				.byte NOTE_Gg, NOTE_E, NOTE_Cc, NOTE_E
				.byte NOTE_Gg, NOTE_E, NOTE_Cc, NOTE_E
				.byte NOTE_Gg, NOTE_E, NOTE_Cc, NOTE_E

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
