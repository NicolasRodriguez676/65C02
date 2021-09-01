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

	set_octave OCTAVE_ONE

lp:
	ldx #0	
sound_lp:
	lda sound_dat, x
	jsr sound_write_note
	delay_cc $c0, $00
	inx
	cpx #12
	beq change_dir
	bra sound_lp

change_dir:
	ldx #11
lp2:
	lda sound_dat, x
	jsr sound_write_note
	delay_cc $c0, $00
	dex
	beq lp
	bra lp2

;-----------------------------------------------------------------

end_loop:
	bra end_loop

	; data i.e. messages, look up tables and include files
	
hello_there:	.asciiz " Hello there"
general:		.asciiz "General Kenobi"


song_one:		.byte NOTE_A,   TEMP_DIV_FOUR,  OCT_CHNG_NULL
				.byte NOTE_D,   TEMP_DIV_FOUR,  OCT_CHNG_NULL
				.byte NOTE_A,   TEMP_DIV_FOUR,  OCT_CHNG_NULL
				.byte NOTE_D,   TEMP_DIV_FOUR,  OCT_CHNG_NULL
				.byte NOTE_A,   TEMP_DIV_FOUR,  OCT_CHNG_NULL
				.byte NOTE_D,   TEMP_DIV_FOUR,  OCT_CHNG_NULL
				.byte NOTE_A,   TEMP_DIV_EIGHT, OCT_CHNG_NULL
				.byte NOTE_Gg,  TEMP_DIV_EIGHT, OCT_CHNG_ONE

				.byte NOTE_A,   TEMP_DIV_FOUR,  OCT_CHNG_TWO
				.byte NOTE_A,   TEMP_DIV_EIGHT, OCT_CHNG_NULL
				.byte NOTE_Gg,  TEMP_DIV_EIGHT, OCT_CHNG_ONE
				.byte NOTE_A,   TEMP_DIV_EIGHT, OCT_CHNG_TWO
				.byte NOTE_G,   TEMP_DIV_EIGHT, OCT_CHNG_ONE
				.byte NOTE_NULL,TEMP_DIV_FOUR,  OCT_CHNG_NULL
				.byte NOTE_Ff,  TEMP_DIV_EIGHT, OCT_CHNG_NULL
				.byte NOTE_G,   TEMP_DIV_EIGHT, OCT_CHNG_NULL    

				.byte NOTE_Ff,  TEMP_DIV_EIGHT, OCT_CHNG_NULL
				.byte NOTE_F,   TEMP_DIV_TWO,   OCT_CHNG_NULL
				.byte NOTE_D,   TEMP_DIV_FOUR,  OCT_CHNG_NULL
				.byte NOTE_NULL,TEMP_DIV_FOUR,  OCT_CHNG_NULL        
				.byte NOTE_A,   TEMP_DIV_FOUR,  OCT_CHNG_TWO
				.byte NOTE_D,   TEMP_DIV_FOUR,  OCT_CHNG_NULL
				.byte NOTE_A,   TEMP_DIV_FOUR,  OCT_CHNG_NULL
				.byte NOTE_D,   TEMP_DIV_FOUR,  OCT_CHNG_NULL

				.byte NOTE_A,   TEMP_DIV_FOUR,  OCT_CHNG_NULL
				.byte NOTE_D,   TEMP_DIV_FOUR,  OCT_CHNG_NULL
				.byte NOTE_A,   TEMP_DIV_EIGHT, OCT_CHNG_NULL
				.byte NOTE_Gg,  TEMP_DIV_EIGHT, OCT_CHNG_ONE
				.byte NOTE_A,   TEMP_DIV_FOUR,  OCT_CHNG_TWO
				.byte NOTE_G,   TEMP_DIV_TWO,   OCT_CHNG_NULL
				.byte NOTE_G,   TEMP_DIV_TWO,   OCT_CHNG_NULL
				.byte NOTE_Ff,  TEMP_DIV_TWO,   OCT_CHNG_NULL

				.byte NOTE_G,   TEMP_DIV_FOUR,  OCT_CHNG_NULL
				.byte NOTE_C,   TEMP_DIV_FOUR,  OCT_CHNG_TWO
				.byte NOTE_BB,  TEMP_DIV_FOUR,  OCT_CHNG_NULL
				.byte NOTE_A,   TEMP_DIV_FOUR,  OCT_CHNG_NULL
				.byte NOTE_G,   TEMP_DIV_FOUR,  OCT_CHNG_ONE
				.byte NOTE_A,   TEMP_DIV_FOUR,  OCT_CHNG_TWO
				.byte NOTE_D,   TEMP_DIV_FOUR,  OCT_CHNG_NULL
				.byte NOTE_A,   TEMP_DIV_FOUR,  OCT_CHNG_NULL

				.byte NOTE_D,   TEMP_DIV_FOUR,  OCT_CHNG_NULL
				.byte NOTE_A,   TEMP_DIV_FOUR,  OCT_CHNG_NULL
				.byte NOTE_D,   TEMP_DIV_FOUR,  OCT_CHNG_NULL
				.byte NOTE_A,   TEMP_DIV_EIGHT, OCT_CHNG_NULL
				.byte NOTE_Gg,  TEMP_DIV_EIGHT, OCT_CHNG_ONE
				.byte NOTE_A,   TEMP_DIV_FOUR,  OCT_CHNG_TWO
				.byte NOTE_C,   TEMP_DIV_FOUR,  OCT_CHNG_NULL        
				.byte NOTE_C,   TEMP_DIV_FOUR,  OCT_CHNG_NULL        

				.byte NOTE_A,   TEMP_DIV_FOUR,  OCT_CHNG_NULL
				.byte NOTE_G,   TEMP_DIV_FOUR,  OCT_CHNG_ONE
				.byte NOTE_F,   TEMP_DIV_TWO,   OCT_CHNG_NULL
				.byte NOTE_D,   TEMP_DIV_TWO,   OCT_CHNG_NULL
				.byte NOTE_D,   TEMP_DIV_TWO,   OCT_CHNG_NULL
				.byte NOTE_F,   TEMP_DIV_TWO,   OCT_CHNG_NULL
				.byte NOTE_A,   TEMP_DIV_TWO,   OCT_CHNG_TWO
				.byte NOTE_C,   TEMP_DIV_TWO,   OCT_CHNG_NULL

				.byte NOTE_EE,  TEMP_DIV_FOUR,  OCT_CHNG_NULL
				.byte NOTE_D,   TEMP_DIV_FOUR,  OCT_CHNG_NULL
				.byte NOTE_Gg,  TEMP_DIV_FOUR,  OCT_CHNG_ONE
				.byte NOTE_A,   TEMP_DIV_FOUR,  OCT_CHNG_TWO

	.include "SOUND_LIB.inc"
