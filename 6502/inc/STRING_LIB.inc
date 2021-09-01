; Nicolas Rodriguez
; String Macros and Subroutines
; RAM Usage will be in RAM used by the terminal program
; January 9, 2021


;----------------------------------------------------------------
;	compare strings by Nicolas Rodriguez
;
;	reference strings (254 bytes to zero bytes)
;		ref1	STRING_PTR
;		ref2	STRING_PTR0
;
;		x register return codes
;			$ff	-	string length matches and
;					are equal to eachother
;			$fe	-	strings are not equal
;			$01	-	ref1 longer than ref2
;					strings are equal up to
;					length of ref1
;			$00	-	ref2 longer than ref1
;					strings are equal up to
;					length of ref1
;
;		y register will be left with index
;		of string end or the point where the
;		two strings are no longer equal
;
;	registers	y, x, a
;
;----------------------------------------------------------------

compare_strings:
	ldy #0					; set y index to zero
	ldx #$ff				; expect strings to be equal
cmp_str0:					; compare loop
	lda (STRING_PTR), y		; ref1 string
	beq cmp_str1			; exit loop when null found
	cmp (STRING_PTR0), y	; compare to ref2 string
	bne cmp_str2			; exit loop when compare fails
	iny						; increment index
	bra cmp_str0			; loop
cmp_str1:					; end of ref1
	lda (STRING_PTR0), y	; check for end of ref2
	bne cmp_str3			; if not end, branch
	rts						; if end, x = $ff
cmp_str2:					; comparison failed
	lda (STRING_PTR0), y	; check for end of
	beq cmp_str4			; ref2
	dex						; if not end, x = $fe
	rts						; string compare failed
cmp_str4:					; compare succed x = $01
	inx						; but ref1 longer than ref2
cmp_str3:					; compare succed
	inx						; ref2 longer than ref1
    rts						; x = $00

;-------------------------------------------------------send string
;
;	send string over serial port by Nicolas Rodriguez
;
;	does not check for serial port connection
;
;	sends a null terminated string.
;	one byte index counter. (string length - null < 255)
;  
;	T1 timer
;	ACIA will not fire irq on recieving data
;	uses VIA1 irq vector (restores previous vector)
;   y register
;	WAIT_VAR
;	VAR001/VAR000
;	STRING_PTR
;
;-------------------------------------------------------------------
serial_print_str:
	sei					; disable irq
						; prevents jumping to irq that
						; may not be ready yet

	ldy #0
	sty WAIT_VAR
	lda VIA1_ACR
	and #$3f
	sta VIA1_ACR
	

	lda #VIA_IRQ_EN6	; enable via1 T1 irq
    tsb VIA1_IER

    lda #COMM_MODE2		; disable ACIA recieve irq
    sta ACIA_CMD

	lda IRQ_VIA1		; save previous irq vector
	sta VAR000			; for via1
	lda IRQ_VIA1 + 1
	sta VAR001

						; update via1 irq vector
	set_address IRQ_VIA1, sps1

 	lda #$c0
    sta VIA1_T1_LC
    lda #$09
    sta VIA1_T1_HC
    
	cli					; enable irq

sps0:
	lda WAIT_VAR
	beq sps0

	sei

	lda #VIA_IRQ_EN6	; disable via1 T1 irq
    trb VIA1_IER

    lda #COMM_MODE0		; enable ACIA recieve irq
    sta ACIA_CMD
	
	lda VAR000			; restore previous irq vector
	sta IRQ_VIA1 		; for via1
	lda VAR001
	sta IRQ_VIA1 + 1

	rts

sps1:
	ply					; get index into string
	lda (STRING_PTR), y
	beq sps2
	sta ACIA_RT			; send one byte

 	lda #$c0			; arm T1
    sta VIA1_T1_LC
    lda #$09
    sta VIA1_T1_HC

	iny					; increment string index

	plx
	pla
	rti

sps2:
    lda VIA1_T1_LC
	inc WAIT_VAR
	plx
	pla
	rti

;*---------------------------------------------------------------
;	recieve string from serial port by Nicolas Rodriguez
;	
;	ACIA should be in mode 0 before calling this subroutine
;
;*-----------------------------------------------------------------
rcv_string:
	lda #<input_string	; update string pointer
	sta STRING_PTR0
	lda #>input_string
	sta STRING_PTR0 + 1

	ldy #0
	sty WAIT_VAR
	cli					; enable IRQ
wait_loop:				; wait for data
	lda WAIT_VAR
	beq wait_loop
	dec WAIT_VAR
	lda	ACIA_BYTE		; test for end of string
	beq string_end		; null
	cmp #$0a			; linefeed
	beq string_end
	cmp #$0d			; carriage return
	bne build_string
string_end:
	lda #0
	sta (STRING_PTR0), y
	rts
build_string:
	sta (STRING_PTR0), y
	iny
	cpy #$0f			; character limit of 16 bytes
	beq string_end		; protects memory
	bra wait_loop

;input_string:	.byte $11, $0


;*---------------------------------------------------------------
;	ASCII hex to 16 bit binary number by Nicolas Rodriguez
;
;	maximum 4 digit hex number
;
;	input string -  STRING_PTR0
;					null terminated
;					only lower case ASCII
;
;	registers	x, y, a
;	memory		VAR003\VAR002	hi\lo bytes
;				VAR004			index value for above bytes
;
;*---------------------------------------------------------------
Lo_HEX_BYTE	=	VAR002

ascii_hex2_16b:
	ldx #1
	ldy #0					; basic setup stuff
	sty Lo_HEX_BYTE
	sty Lo_HEX_BYTE + 1
	sty VAR004
ahb0:						; get length of string
	lda (STRING_PTR0), y	; get place values
	beq ahb1				; determine if
	iny						; string is too big
	bra ahb0
ahb1:						; reached end of string
	dey
	tya
	sec
	sbc #4 					; 4 place values max. null char
	beq get_next_char		; == 4
	bmi get_next_char		; <  4
	bra string_too_long		; >  4
get_next_char:
	lda (STRING_PTR0), y
	sec
	sbc #$57				; zero out ASCII char
	bpl	ahb2				; valid ASCII f-a
	lda (STRING_PTR0), y
	sec
	sbc #$30				; same as above but 9-0
	bmi input_string_invalid
ahb2:
	dex
	beq ahb3
	asl a
	bra ahb2
ahb3:
	lsr VAR004
	ldx VAR004
	rol VAR004
	ora Lo_HEX_BYTE, x
	sta Lo_HEX_BYTE, x
	dey
	bmi ahb5
	inc VAR004
	lda VAR004
	lsr a
	bcs shift_4_times
	ldx #1
	bra get_next_char
shift_4_times:
	ldx #5
	bra get_next_char
string_too_long:
	ldy #$fe
	rts
input_string_invalid:
	ldy #$fd
ahb5:
	rts

;*---------------------------------------------------------------
;   binary to decimal string by Nicolas Rodriguez
;       
;   stack       1x jsr
;   registers   y, x, a
;
;   memory      
;               VAR00b/VAR00a   16b number to be converted
;               DEC_STRING      6 bytes (including NULL)
;               VAR005-VAR000   (16b div 16b subroutine)
;               VAR00f\VAR00e   program flow
; 
;               two LUTs at end of subroutine: 18 bytes
;----------------------------------------------------------------
B2D_BYTE_Lo     =   VAR00a
B2D_BYTE_Hi     =   VAR00b

b2d_str:
    lda ascii_lut
    sta DEC_STRING
    ldx #0                  ; index for new decimal string
    stx DEC_STRING + 1      ; clear string
    stx DEC_STRING + 2
    stx DEC_STRING + 3
    stx DEC_STRING + 4
    stx DEC_STRING + 5
    stx VAR00e              ; prevent leading zeroes
    stx VAR00f              ; divisor index
    lda #$27                ; initialize divisor
    sta DIVISOR_Hi          ; divide by 10,000
    lda #$10                ; loop: 1,000 100 10 1
    sta DIVISOR_Lo
begin_conv:
    lda B2D_BYTE_Lo         ; fetch low byte to convert
    sta DIVIDEND_Lo         ; dividend == bin to dec bytes
    lda B2D_BYTE_Hi         ; fetch high byte convert
    sta DIVIDEND_Hi
    jsr div_16x16           ; division: reduce by power of 10
    tya                     ; test if division was 
    bne place_zero          ; not successful
    inc VAR00e              ; if successful,
    ldy RESULT_Lo           ; (RESULT_Lo == index to ASCII LUT)
    lda ascii_lut, y        ; use LUT to update
    sta DEC_STRING, x       ; decimal string
    inx                     ; increment string pointer
sub_divisor:                 ; subtract divisor by the number
    sec                     ; of times it fits in perfectly
    lda B2D_BYTE_Lo         ; into dividend
    sbc DIVISOR_Lo          ; new B2d = old B2D - divisor
    sta B2D_BYTE_Lo         ; update binary to decimal byte
    lda B2D_BYTE_Hi
    sbc DIVISOR_Hi
    sta B2D_BYTE_Hi         ; update byte binary to decimal byte
    dey                     ; repeat y times
    bne sub_divisor         
    bra get_next_divisor    ; end subtraction
place_zero:                 ; place zero if needed
    lda VAR00e
    beq get_next_divisor    
    lda ascii_lut
    sta DEC_STRING, x
    inx
get_next_divisor:
    ldy VAR00f              ; test if VAR00f is out-of-bounds
    cpy #$08                ; conversion is done
    beq end_conv            ; if VAR00f == 8
    lda pow_ten_divs, y
    sta DIVISOR_Lo
    lda pow_ten_divs + 1, y
    sta DIVISOR_Hi
    inc VAR00f              ; increment twice
    inc VAR00f              ; divisor is 16b
    bra begin_conv
end_conv:
    rts

ascii_lut:
    .byte "0", "1", "2", "3", "4", "5", "6", "7", "8", "9"

pow_ten_divs:
    .byte $e8, $03, $64, $00, $0a, $00, $01, $00

;DEC_STRING:	.byte $6, $0
