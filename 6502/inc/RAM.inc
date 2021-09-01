; Nicolas Rodriguez
; RAM Usage
; March 14, 2021

	.segment "ZEROPAGE"

PGM_BEGIN:		.word	$0000
STRING_PTR:		.word	$0000
STRING_PTR0:	.word	$0000
STRING_PTR1:	.word	$0000
DATA_PTR:		.word	$0000
ACIA_BYTE:		.byte	$00
WAIT_VAR:		.byte	$00
DLY_VAR0:		.byte	$00
DLY_VAR1:		.byte	$00
DATA_PTR0:		.word	$0000
v_stack_ptr:	.word	$0000
vga_data_ptr:	.word	$0000
counter_ovfl:	.byte	$00

	.segment "ROM_SPAD"

VAR000:			.byte	$00
VAR001:			.byte	$00
VAR002:			.byte	$00
VAR003:			.byte	$00
VAR004:			.byte	$00
VAR005:			.byte	$00
VAR006:			.byte	$00
VAR007:			.byte	$00
VAR008:			.byte	$00
VAR009:			.byte	$00
VAR00a:			.byte	$00
VAR00b:			.byte	$00
VAR00c:			.byte	$00
VAR00d:			.byte	$00
VAR00e:			.byte	$00
VAR00f:			.byte	$00
VAR010:			.byte	$00
VAR011:			.byte	$00
VAR012:			.byte	$00
VAR013:			.byte	$00
VAR014:			.byte	$00
VAR015:			.byte	$00
VAR016:			.byte	$00
T1_CNT_LO:		.byte	$00
T1_CNT_HI:		.byte	$00
T1_CNT_MODE:	.byte	$00
I2C_TEMP:		.byte	$00
JYSTK_Y_AXIS:	.byte	$00
JYSTK_X_AXIS:	.byte	$00
JYSTK_POSITION:	.byte	$00
Y_CENTERED:		.byte	$00
X_CENTERED:		.byte	$00
JYSTK_BTN:		.byte	$00

; lcd_i2c_lib
; read/write memory locations with default values

lcd_set_opt:	.byte $08, $0c, $08, $0c, $00
wrt_data:		.byte $09, $0d, $09, $0d, $00

lcd_set_addr:	.byte $88, $8c, $08, $0c, $00

lcd_rd_addr:	.byte $fa, $fe, $fa, $fe, $00
lcd_rd_data:	.byte $fb, $ff, $fb, $ff, $00

; joystick lib
jystk_get_dat:  .byte $01, $00, $00

; string lib
input_string:	.byte $11, $00
DEC_STRING:		.byte $06, $00

; The following is still compliant with the simple bootloader
IRQ_ACIA:		.byte	$00, $00	; only two type of irq are
				.byte	$00, $00	; distinguised

									; all seven irqs will be
									; distinguished
IRQ_VIA1:		.byte	$00, $00	; irq timer 1
				.byte	$00, $00	; irq timer 2
				.byte	$00, $00	; irq CB1
				.byte	$00, $00	; irq CB2
				.byte	$00, $00	; irq shift register
				.byte	$00, $00	; irq CA1
				.byte	$00, $00	; irq CA2

IRQ_VIA2:		.byte	$00, $00	; irq timer 1
				.byte	$00, $00	; irq timer 2
				.byte	$00, $00	; irq CB1
				.byte	$00, $00	; irq CB2
				.byte	$00, $00	; irq shift register
				.byte	$00, $00	; irq CA1
				.byte	$00, $00	; irq CA2


								
	.segment "PGM_RAM"

jmp_ptr:		.word $0000

vga:
vga_x1:			.byte $00, $00		; min value $008c
vga_x2:			.byte $00, $00		; max value $030c
vga_y1:			.byte $00, $00		; min value $0022
vga_y2:			.byte $00, $00		; max value $01e0
vga_c0:			.byte $00, $00 		; last four dont care

sound_temp:		.byte $00

;jystk_jmp_tbl:	.word joystick_down		;  0,  1
				;.word joystick_up		;  2,  3
				;.word joystick_left	;  4,  5
				;.word joystick_right	;  6,  7
				;.word joystick_q1		;  8,  9
				;.word joystick_q2		; 10, 11
				;.word joystick_q3		; 12, 13
				;.word joystick_q4		; 14, 15

	.segment "V_STACK"

v_stack:	.byte $0, $0, $0, $0, $0, $0, $0, $0
			.byte $0, $0, $0, $0, $0, $0, $0, $0
			.byte $0, $0, $0, $0, $0, $0, $0, $0
			.byte $0, $0, $0, $0, $0, $0, $0, $0
			.byte $0, $0, $0, $0, $0, $0, $0, $0
			.byte $0, $0, $0, $0, $0, $0, $0, $0
			.byte $0, $0, $0, $0, $0, $0, $0, $0
			.byte $0, $0, $0, $0, $0, $0, $0, $0