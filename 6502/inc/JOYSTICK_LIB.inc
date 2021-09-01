; Nicolas Rodriguez
; Joystick Library "CompuJoy"
; April 7, 2021

;-Details-----------------------------------------------------------
;
;   position data
;
;   $c0 - down only
;   $c2 - up only
;   $a0 - left only
;   $a1 - right only
;
;   $01 - quadrant 1
;   $03 - quadrant 2
;   $05 - quadrant 3
;   $07 - quadrant 4
;
;
;   Joystick
;   2 axis ~10k pots
;   1 button debounced with 555 timer. pullup 100kohms
;
;   MCP3008 ADC uses SPI interface
;   74HC595 for reading data from MCP3008
;
;   data 
;
;   0       1       2       3       4       5       6       7
;   0       0       0       0       0       0       0       start
;
;   8       9       10      11      12      13      14      15
;   CFG     D2      D1      D0      X       Null    B9      B8
;
;   16      17      18      19      20      21      22      23
;   B7      B6      B5      B4      B3      B2      B1      B0
;
;   B9-B0 will end up in the two 74HC595's
;   i can cut down on needed components by using one 74HC595 
;   and use only the first 8 bits (8 bit resolution instead of 10 bit)
;   *i cannot do this because the output is MSB*
;
;   CFG - 1 single ended input. 0 differenital pair inputs
;
;   Hardware Configuration
;
;   SCLK    - CB2
;   DATIN   - CB1
;   
;   LATCH   - PORTA
;   ~CS     - PORTA
;
;   Button  - CA1
;
;                   data to be processed
;   D9      - QB2   PORTB 7
;   D8      - QA2   PORTB 6
;   D7      - QH1   PORTB 5
;   D6      - QG1   PORTB 4
;   D5      - QF1   PORTB 3
;   D4      - QE1   PORTB 2
;   D3      - QD1   PORTB 1
;   D2      - QC1   PORTB 0
;
;                   ignored
;   D1      - QB1
;   D0      - QA1
;
;
;   Set CS low. Send 3 bytes of "data" to MCP3008
;   set CS high. Pulse latch.
;
;-------------------------------------------------------------------

                        ; change these values as needed
JYSTK_CS        = $40
JYSTK_OE        = $20
JYSTK_LATCH     = $10

Y_AXIS_CENTER   = 111
X_AXIS_CENTER   = 128

Y_AXIS_CRANGE   = 40
X_AXIS_CRANGE   = 40


;---Joystick Subroutines---------------------------------------------

jystk_init:
                        ; setup PORTB
    stz VIA1_DDRB       ; all inputs

                        ; setup shift reg. in VIA1
    lda #SR_MODE6
    tsb VIA1_ACR
    lda #$04            ; disable IRQ from shift register
    trb VIA1_IER
    
                        ; setup PORTA
    
    lda #(JYSTK_CS | JYSTK_OE)  ; active low
    tsb VIA1_PORTA
    lda #($81 | JYSTK_LATCH)    ; active high
    trb VIA1_PORTA

                                ; all are outputs
    lda #(JYSTK_LATCH | JYSTK_CS | JYSTK_OE)
    tsb VIA1_DDRA
 
    ; setup interrupt for joystick pushbutton

    ;lda #CA1_MODE1     ; active edge - rising
    ;tsb VIA1_PCR
    
                        ; enable irq for ca1 on via1
    ;lda #VIA_IRQ_EN1
    ;tsb VIA1_IER

    ; for simple bootloader, update the
    ; jump vector: IRQ_VIA1
    ;set_address IRQ_VIA1 + 10, jystk_button

    lda #1
    sta jystk_get_dat
    lda #0
    sta jystk_get_dat + 2
    rts


    ;------------------------------------------------- button IRQ
    ; this is a subroutine but needs to be terminated
    ; by rti since it is "called" by an interrupt
    ; in this instance, i only need to know if the 
    ; button was pressed. no other information is needed
;jystk_button:
    ;ply
    ;plx
    ;pla
    ;rti

jystk_get_pos:
    ; optimized code by setting this to zero
    lda #0
    sta JYSTK_POSITION
    sta Y_CENTERED
    sta X_CENTERED

    ; sample x-axis
    lda #$80
    sta jystk_get_dat + 1
    jsr jystk_sample
    sta JYSTK_X_AXIS

    ; sample y-axis
    lda #$90
    sta jystk_get_dat + 1
    jsr jystk_sample
    sta JYSTK_Y_AXIS

    ;----------------------------------- y and x magnitude test
    cmp JYSTK_X_AXIS
    bcc yxmt0               ; y >= x
    ror JYSTK_POSITION      ; y <  x
yxmt0:

    ;-----------------------------------test bounds (y)
    lda JYSTK_Y_AXIS
    cmp #(Y_AXIS_CENTER - Y_AXIS_CRANGE)
    bcs y_not_below
    bra end_y_bound_test
y_not_below:
    cmp #(Y_AXIS_CENTER + Y_AXIS_CRANGE)
    bcc y_at_center
    inc JYSTK_POSITION
    inc JYSTK_POSITION
    bra end_y_bound_test
y_at_center:
    lda #$82
    sta Y_CENTERED

end_y_bound_test:

    ;-----------------------------------test bounds (x)
    lda JYSTK_X_AXIS
    cmp #(X_AXIS_CENTER - X_AXIS_CRANGE)
    bcs x_not_left
    bra end_x_bound_test
x_not_left:
    cmp #(X_AXIS_CENTER + X_AXIS_CRANGE)
    bcc x_is_center
    inc JYSTK_POSITION
    bra end_x_bound_test
x_is_center:
    lda #$81
    sta X_CENTERED

end_x_bound_test:

    lda Y_CENTERED
    ora X_CENTERED 
    cmp #$83
    beq jystk_at_center   
    lda Y_CENTERED
    bmi x_only
    lda X_CENTERED
    bmi y_only
    lda JYSTK_POSITION
    and #3
    tax
    lda lut_q, x
    rts

x_only:
    lda JYSTK_POSITION
    and #1
    ora #$a0
    rts

y_only:
    lda JYSTK_POSITION
    and #2
    ora #$c0
    rts

jystk_at_center:
    lda #0
    rts

jystk_sample:
    lda #JYSTK_CS
    trb VIA1_PORTA
    ldy #$ff
jp0:
    iny
    cpy #3
    beq jp1
    lda jystk_get_dat, y
    sta VIA1_SR
    nop
    nop
    bra jp0
jp1:
    lda #JYSTK_CS
    tsb VIA1_PORTA
    lda #JYSTK_LATCH
    tsb VIA1_PORTA
    trb VIA1_PORTA
    lda #JYSTK_OE
    trb VIA1_PORTA
    lda VIA1_PORTB
    pha
    lda #JYSTK_OE
    tsb VIA1_PORTA
    lda #$81
    trb VIA1_PORTA
    pla
    rts

lut_q:   .byte $5, $7, $3, $1