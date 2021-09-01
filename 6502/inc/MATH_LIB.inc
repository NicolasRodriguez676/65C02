; Nicolas Rodriguez
; Math Subroutines
; RAM Usage will be in RAM used by the terminal program
; January 9, 2021

DIVIDEND_Lo     =   VAR000
DIVIDEND_Hi     =   VAR001
DIVISOR_Lo      =   VAR002
DIVISOR_Hi      =   VAR003
RESULT_Lo       =   VAR004
RESULT_Hi       =   VAR005
TEMP_Lo         =   VAR006
TEMP_Hi         =   VAR007

;----------------------------------------------------------
;   16b/16b subroutine by Nicolas Rodriguez
;   reference material used when writing
;       https://tinyurl.com/yy8uq8ed
;       http://www.obelisk.me.uk/65C02/reference.html#LDA
;
;   stack       not used at all
;   registers   y, a
;
;   memory
;   dividend    VAR001/VAR000
;   divisor     VAR003/VAR002
;   result      VAR005/VAR004
;   
;   error reported in register y
;   y == 0 if division successful
;----------------------------------------------------------

div_16x16:
    clc
    lda DIVISOR_Lo          ; TEST: DIVISOR == 0
    adc DIVISOR_Hi
    bne divs_nz
    ldy #$ff                ; return error code
    rts
divs_nz:
    sec                     ; TEST: DIVISOR > DIVIDEND
    lda DIVIDEND_Lo
    sbc DIVISOR_Lo
    lda DIVIDEND_Hi
    sbc DIVISOR_Hi
    bcs begin_div           ; carry == clear => result neg
    ldy #$fe                ; return error code
    rts
begin_div:
    ldy #0                  ; first need to setup divisor
    sty RESULT_Lo           ; clear result memory locations
    sty RESULT_Hi           ; 16 rotations not guaranteed
    clc                     ; make sure no extra bits are 
                            ; rotated into divisor
shift_divisor_left:
    rol DIVISOR_Lo
    rol DIVISOR_Hi
    iny                     ; y register holds amount shifted left   
    bcc shift_divisor_left  ; rotate divisor till carry is set
shift_divisor_right:
    ror DIVISOR_Hi
    ror DIVISOR_Lo
    lda DIVIDEND_Lo         ; save dividend
    sta TEMP_Lo
    lda DIVIDEND_Hi
    sta TEMP_Hi
    sec                     ; 16 bit subtraction       
    lda DIVIDEND_Lo
    sbc DIVISOR_Lo
    sta DIVIDEND_Lo
    lda DIVIDEND_Hi
    sbc DIVISOR_Hi
    sta DIVIDEND_Hi
    bcs form_result         ; branch if result is positive
    lda TEMP_Lo             ; undo subtraction if result is
    sta DIVIDEND_Lo         ; negative
    lda TEMP_Hi
    sta DIVIDEND_Hi
form_result:
    rol RESULT_Lo
    rol RESULT_Hi           ; shift in carry flag into result
    clc
    dey
    bne shift_divisor_right
    rts


