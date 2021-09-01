; Nicolas Rodriguez
; Game
; May 3, 2021


joystick_down:
    set_address STRING_PTR, string_down
	jsr serial_print_str
	lda #$80
    rts
;----------------

joystick_up:
    set_address STRING_PTR, string_up
	jsr serial_print_str
	lda #$80
	rts
;----------------

joystick_left:
    set_address STRING_PTR, string_left
	jsr serial_print_str
	lda #$80
	rts
;----------------

joystick_right:
    set_address STRING_PTR, string_right
	jsr serial_print_str
	lda #$80
	rts
;----------------

joystick_q1:
    set_address STRING_PTR, string_q1
	jsr serial_print_str
	lda #$80
	rts
;----------------

joystick_q2:
    set_address STRING_PTR, string_q2
	jsr serial_print_str
	lda #$80
	rts
;----------------

joystick_q3:
    set_address STRING_PTR, string_q3
	jsr serial_print_str
	lda #$80
	rts
;----------------

joystick_q4:
    set_address STRING_PTR, string_q4
	jsr serial_print_str
	lda #$80
	rts
;----------------

scan_jystk:
    jsr jystk_get_pos
    bne jystk_not_zero
    rts
	;---- test negative value
jystk_not_zero:
    bmi x_or_y_only

	;---- quadrant
	clc
	adc #7
	tax
	jmp (jystk_jmp_tbl, x)

x_or_y_only:
	asl
	asl
	bcs y_only_dir
	and #4
	beq x_left_dir
	ldx #6
	jmp (jystk_jmp_tbl, x)
x_left_dir:
	ldx #4
	jmp (jystk_jmp_tbl, x)
y_only_dir:
	lsr
	lsr
	and #2
	tax 
	jmp (jystk_jmp_tbl, x)