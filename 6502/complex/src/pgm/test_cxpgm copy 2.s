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


	
;-----------------------------------------------------------------

end_loop:
	bra end_loop

	; data i.e. messages, look up tables and include files
	
hello_there:	.asciiz " Hello there"
general:		.asciiz "General Kenobi"


