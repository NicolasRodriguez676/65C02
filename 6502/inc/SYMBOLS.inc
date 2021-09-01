; Nicolas Rodriguez
; Symbols for Addresses and Constants of Peripherals
; December 17, 2020

;**********Peripheral Addresses**********

; --------------------------ACIA
ACIA        = $d000

ACIA_RT     = ACIA + $0  ; transmit and recieve registers
ACIA_SR     = ACIA + $1  ; programmed reset. status register
ACIA_CMD    = ACIA + $2  ; command register
ACIA_CNT    = ACIA + $3  ; control register

; --------------------------VIA1
VIA1        = $d200

VIA1_PORTB  = VIA1 + $0
VIA1_PORTA  = VIA1 + $1
VIA1_DDRB   = VIA1 + $2
VIA1_DDRA   = VIA1 + $3
VIA1_T1_LC  = VIA1 + $4	    ; Read T1 low order counter only
VIA1_T1_HC  = VIA1 + $5	    ; Read/Write T1 high order counter
VIA1_T1_LL  = VIA1 + $6	    ; Read/Write T1 low order latch
VIA1_T1_HL  = VIA1 + $7	    ; Read/Write T1 high order latch
VIA1_T2_L   = VIA1 + $8	    ; Read low order counter. Write low order latch
VIA1_T2_H   = VIA1 + $9     ; Read/Write high order counter
VIA1_SR     = VIA1 + $a	    ; Shift register. cb1 clk. cb2 data
VIA1_ACR    = VIA1 + $b     ; Auxillary Control Register
VIA1_PCR    = VIA1 + $c	    ; Peripheral Control Register
VIA1_IFR	= VIA1 + $d	    ; Interupt Flag Register
VIA1_IER    = VIA1 + $e     ; Interupt Enable Register

VIA2        = $d400

VIA2_PORTB  = VIA2 + $0
VIA2_PORTA  = VIA2 + $1
VIA2_DDRB   = VIA2 + $2
VIA2_DDRA   = VIA2 + $3
VIA2_T1_LC  = VIA2 + $4	    ; Read T1 low order counter only
VIA2_T1_HC  = VIA2 + $5	    ; Read/Write T1 high order counter
VIA2_T1_LL  = VIA2 + $6	    ; Read/Write T1 low order latch
VIA2_T1_HL  = VIA2 + $7	    ; Read/Write T1 high order latch
VIA2_T2_L   = VIA2 + $8	    ; Read low order counter. Write low order latch
VIA2_T2_H   = VIA2 + $9     ; Read/Write high order counter
VIA2_SR     = VIA2 + $a	    ; Shift register
VIA2_ACR    = VIA2 + $b     ; Auxillary Control Register
VIA2_PCR    = VIA2 + $c	    ; Peripheral Control Register
VIA2_IFR	= VIA2 + $d	    ; Interupt Flag Register
VIA2_IER    = VIA2 + $e     ; Interupt Enable Register

;**********Peripheral Constants**********

; ----------------------VIA CONSTANTS

;---- Interrupt Enable  Register

VIA_IRQ_EN7 = $80      ; Enables interrupts on VIA
VIA_IRQ_EN6 = $40      ; Timer 1
VIA_IRQ_EN5 = $20      ; Timer 2
VIA_IRQ_EN4 = $10      ; CB1
VIA_IRQ_EN3 = $08      ; CB2
VIA_IRQ_EN2 = $04      ; Shift register
VIA_IRQ_EN1 = $02      ; CA1
VIA_IRQ_EN0 = $01      ; CA2

;---- Auxillary Control Register

T1_MODE3    = $c0      ; Continous IRQ. PB7 squarewave
T1_MODE2    = $80      ; Single IRQ. Single PB7 pulse
T1_MODE1    = $40      ; Continous IRQ. No squarewave
T1_MODE0    = $00      ; Single IRQ. No pulse

                       ; this is flipped..
T2_MODE1    = $20      ; timed irq
T2_MODE0    = $00      ; count pulses on PORTB 6

SR_MODE0    = $00      ; disable shift register
SR_MODE1    = $04      ; shift in. t2
SR_MODE2    = $08      ; shift in. phi2
SR_MODE3    = $0c      ; shift in. cb1 (ext)
SR_MODE4    = $10      ; shift out. free running at t2
SR_MODE5    = $14      ; shift out. t2
SR_MODE6    = $18      ; shift out. phi2
SR_MODE7    = $1c      ; shift out cb1 (ext)

;---- Peripheral Control Register

CA2_MODE7   = $0e      ; high output
CA2_MODE6   = $0c      ; low output
CA2_MODE5   = $0a      ; pulse output
CA2_MODE4   = $08      ; handshake output
CA2_MODE3   = $06      ; input pos-active edge. independent
CA2_MODE2   = $04      ; input positive active edge
CA2_MODE1   = $02      ; input neg-active edge. independent
CA2_MODE0   = $00      ; input negative active edge
CA1_MODE1   = $01      ; irq on rising edge of CA1
CA1_MODE0   = $00      ; irq on falling edge of CA1

CB2_MODE7   = $e0      ; high output
CB2_MODE6   = $c0      ; low output
CB2_MODE5   = $a0      ; pulse output
CB2_MODE4   = $80      ; handshake output
CB2_MODE3   = $60      ; input pos-active edge. independent
CB2_MODE2   = $40      ; input positive active edge
CB2_MODE1   = $20      ; input neg-active edge. independent
CB2_MODE0   = $00      ; input negative active edge
CB1_MODE1   = $10      ; irq on rising edge of CB1
CB1_MODE0   = $00      ; irq on falling edge of CB1


; ----------------------ACIA constants
COMM_MODE0  = $09      ; No parity. No echo. RTS low. IRQ Tx no. Rx. Yes.
COMM_MODE1  = $01      ; No parity. No echo. RTS high. IRQ Tx no. Rx. Yes.
COMM_MODE2  = $0a      ; No parity. No echo. RTS low. IRQ Tx no. Rx. No.
COMM_MODE3  = $00      ; Turn ACIA off

CONT_MODE0  = $1e      ; 1 stop bit. No parity. 8 bits. Baud. 9600.
CONT_MODE1  = $1f      ; 1 stop bit. No parity. 8 bits. Baud. 19200.


; ----------------------LCD Symbols for Macros
                           
                            ; entry mode set (option 1)
LCD_MODE_SET	=	$01
NTRY_MDE_SET00	=	$40		; cursor dec (move left). no display shift
NTRY_MDE_SET10	=	$00
NTRY_MDE_SET01	=	$50		; cursor dec. yes display shift
NTRY_MDE_SET11	=	$00
NTRY_MDE_SET02	=	$60		; cursor inc (move right). no display shift
NTRY_MDE_SET12	=	$00
NTRY_MDE_SET03	=	$70		; cursor inc. yes display shift
NTRY_MDE_SET13	=	$00

                            ; display on/off/cursor (option 2)
LCD_DISP_CSR	=	$02
DIS_CSR_CNT00	= 	$80		; display off. cursor off. blink off
DIS_CSR_CNT10	= 	$00
DIS_CSR_CNT01	= 	$90		; display off. cursor off. blink on
DIS_CSR_CNT11	= 	$00
DIS_CSR_CNT02	= 	$a0		; display off. cursor on. blink off
DIS_CSR_CNT12	= 	$00
DIS_CSR_CNT03	= 	$b0		; display off. cursor on. blink on
DIS_CSR_CNT13	= 	$00
DIS_CSR_CNT04	= 	$c0		; display on. cursor off. blink off
DIS_CSR_CNT14	= 	$00
DIS_CSR_CNT05	= 	$d0		; display on. cursor off. blink on
DIS_CSR_CNT15	= 	$00
DIS_CSR_CNT06	= 	$e0		; display on. cursor on. blink off
DIS_CSR_CNT16	= 	$00
DIS_CSR_CNT07	= 	$f0		; display on. cursor on. blink on
DIS_CSR_CNT17	= 	$00
 
                            ; cursor/display shift (option 3)
LCD_SHIFT		=	$03
SHFT_DIS_CSR00	= 	$00		; cursor move. shift left
SHFT_DIS_CSR10	= 	$10
SHFT_DIS_CSR01	= 	$40		; cursor move. shift right
SHFT_DIS_CSR11	= 	$10
SHFT_DIS_CSR02	= 	$80		; display move. shift left
SHFT_DIS_CSR12	= 	$10
SHFT_DIS_CSR03	= 	$c0		; display move. shift right
SHFT_DIS_CSR13	= 	$10

LINE_STRT_ONE   =   $00     ; ddram address starts at
LINE_STRT_TWO   =   $40     ; this hex value
LINE_STRT_THR   =   $14     ; for each line
LINE_STRT_FOU   =   $54     ; use offsets to target
                            ; different positions

;----------------------Joystick symbols for reading position

JYSTK_MODE10    = $80       ; channel 0. single ended
JYSTK_MODE11    = $90       ; channel 1. single ended
JYSTK_MODE12    = $a0
JYSTK_MODE13    = $b0
JYSTK_MODE14    = $c0
JYSTK_MODE15    = $d0
JYSTK_MODE16    = $e0
JYSTK_MODE17    = $f0

JYSTK_MODE00    = $00
JYSTK_MODE01    = $10
JYSTK_MODE02    = $20
JYSTK_MODE03    = $30
JYSTK_MODE04    = $40
JYSTK_MODE05    = $50
JYSTK_MODE06    = $60
JYSTK_MODE07    = $70

;----------------------VGA Symbols

VGA_CS          = $04
VGA_X0          = $8c
VGA_Y0          = $22
VGA_END         = $0a
VGA_RED         = $0f
VGA_GREEN       = $f0
VGA_BLUE        = $0f

;----------------------Sound Symbols

OCTAVE_CLK      = $80
NOTE_CLK        = $40

OCTAVE_ONE      = 15
OCTAVE_TWO      = 31

OCT_CHNG_NULL   = 00
OCT_CHNG_ONE    = 15
OCT_CHNG_TWO    = 31

TEMPO_DIV_02    = $00
TEMPO_DIV_04    = $02
TEMPO_DIV_08    = $04
TEMPO_DIV_16    = $06
TEMPO_DIV_32    = $08

NOTE_NULL       = $00

NOTE_A          = 130 
NOTE_BB         = 246
NOTE_B          = 232
NOTE_C          = 219
NOTE_Cc         = 207
NOTE_D          = 195
NOTE_EE         = 184
NOTE_E          = 175
NOTE_F          = 165
NOTE_Ff         = 156
NOTE_G          = 147
NOTE_Gg         = 139