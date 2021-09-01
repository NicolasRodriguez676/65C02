; Deanna Rodriguez
; Sound Library
; May 12, 2021

; tempo - 458ms
; calculation (tempo * T2 * zero page counter) / 1/3E6

; 1.8432MHZ

; count down from 
;                zp v1_t2    decimal value
; divide by  2: $0a 7b98      687000
; divide by  4: $05 3dcc      343500
; divide by  8: $02 9ee6      171750
; divide by 16: $01 4f73       85875
; divide by 32: $00 a7ba       42938


sound_write_octave:
    sta VIA2_PORTB
 
    nop
    nop

    lda #OCTAVE_CLK
    tsb VIA2_PORTA
    nop
    nop
    trb VIA2_PORTA
    
    rts

sound_write_note:
    sta VIA2_PORTB
    
    nop
    nop

    lda #NOTE_CLK
    tsb VIA2_PORTA    
    nop
    nop
    trb VIA2_PORTA

    rts


sound_init:
    stz VIA2_PORTA
    
    lda #$ff
    sta VIA2_DDRA
    sta VIA2_DDRB

    rts


    ;---- interrupts

snd_irq_v1_t2:
	lda counter_ovfl
	beq arm_t2_sound
    cmp #$ff
    beq no_delay
	dec counter_ovfl

arm_t2_max:
	lda #$ff
	sta VIA1_T2_L
	sta VIA1_T2_H
	
    ply
	plx
	pla
	rti

arm_t2_sound:
    ldx sound_temp
    lda sound_temp, x
	sta VIA1_T2_L
    inx
    lda sound_temp, x
	sta VIA1_T2_H
    lda #$ff
    dec counter_ovfl

	ply
	plx
	pla
	rti

no_delay:
    lda VIA1_T2_L
    lda #$80
    sta counter_ovfl

    ply
	plx
	pla
	rti

tempo_lut:  .byte   $98, $7b
            .byte   $cc, $3d
            .byte   $e6, $9e
            .byte   $73, $4f
            .byte   $ba, $a7

ovfl_lut:   .byte   $0a, $00
            .byte   $05, $00
            .byte   $02, $00
            .byte   $01, $00
            .byte   $00, $00