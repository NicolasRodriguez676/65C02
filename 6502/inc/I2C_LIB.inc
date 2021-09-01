; Nicolas Rodriguez
; Following code adapted from online: http://wilsonminesco.com/6502primer/GENRLI2C.ASM
; Code changed to fit Retro Assembler and to actually work
; January 4, 2021

; Slight changes to macro syntax when moved to cc65. March 2021

.macro  I2C_DATA_UP
	lda #$80				; Two instructions here.  Clear bit 7 of the DDR
	trb VIA1_DDRA			; to make PA7 an input and let it float up.
.endmacro

;-----------------------
.macro I2C_DATA_DN
	lda #$80				; Two instructions here.  Set bit 7 of the DDR
	tsb VIA1_DDRA			; to make PA7 an output and pull it down since
.endmacro					; bit 7 of the output register is a 0

;-----------------------
.macro I2C_CLK_UP			; (as above)
	lda #1
	trb VIA1_DDRA
.endmacro

;-----------------------
.macro I2C_CLK_DN			; (as above)
	lda #1
	tsb VIA1_DDRA
.endmacro

;-----------------------
i2c_start:
	I2C_DATA_DN
	I2C_CLK_DN
	rts

;-----------------------
i2c_stop:
	I2C_DATA_DN	
	I2C_CLK_UP
	I2C_DATA_UP
	rts

;-----------------------
i2c_ack:   
	I2C_DATA_DN					; Acknowledge. The ACK bit in I2C is the 9th bit of a "byte"
ia1:							;              and acknowledging consists of pulling it down.
	I2C_CLK_UP        
	inc VIA1_DDRA				; Clk down. We know the bit val, so just inc.
	I2C_DATA_UP
	rts

;-----------------------
i2c_nak:     
	I2C_DATA_UP					; Not acknowledge.
	bra ia1

;-----------------------
i2c_ack_q:
	I2C_DATA_UP					; At end, N=0 means ACK.  N=1 means NAK.
	I2C_CLK_UP
    bit VIA1_PORTA				; bit 7 (the data line) gets put in the N flag.
	tsb VIA1_DDRA				; Clk down.  Accum still has 1 from I2C_CLK_UP.  Take advantage.
	php							; get ACK bit
	plx							; into x register
	rts

;-----------------------            
clr_i2c:						; This clears any unwanted transaction that might be in progress, by giving
	jsr i2c_stop				;    enough clock pulses to finish a byte and not acknowledging it.
	jsr i2c_start
	I2C_DATA_UP					; Keep data line released so we don't ACK any byte sent by a device.
	ldx #9						; Loop 9x to send 9 clock pulses to finish any byte a device might send.
	ci2c: 
	dec VIA1_DDRA				; Like I2C_CLK_UP since we know i2c_start left clock down (VIA1_DDRA bit 0 high).
	inc VIA1_DDRA				; Like I2C_CLK_DN since we know the state from the above instruction.
	dex
	bne ci2c
	jsr i2c_start
	jmp i2c_stop				; (jsr, rts)

;------------------
; data & clk start lo
send_i2c_byte:					; Start with byte in A, and clock low.  Ends with i2c_ack_q.
	sta I2C_TEMP				; Store the byte in a variable so we can use A with tsb & trb for data line
	lda #$80					; Init A for mask for trb & tsb below.  A does not get disturbed below.
	ldx #8						; Send 8 bits
sib0:
	asl I2C_TEMP				; Bit 7 to carry flag
	bcs data_bit_set			; Branch if bit 7 was set
	tsb VIA1_DDRA				; Otherwise set data line to zero
	bra toggle_clk				; toggle clock
data_bit_set:
	trb VIA1_DDRA				; set data line to one
toggle_clk:
	dec VIA1_DDRA				; Set clk
	inc VIA1_DDRA				; Reset clk
	dex							; Count number of bits sent
	bne sib0					; Loop until 8 bits have been sent
	jmp i2c_ack_q				; Read slave's ack bit

;------------------
rcv_i2c_byte:					; Start with clock low.  Ends with byte in I2C_TEMP.  Do ACK bit separately.
	I2C_DATA_UP					; Make sure we're not holding the data line down.  Be ready to input data.
	ldx #8						; We will do 8 bits.  There's no need to init I2C_TEMP.
rib1:
	dec VIA1_DDRA				; Set clock line high.
	asl I2C_TEMP				; Get the forming byte's next bit position ready to accept the bit.
	bit VIA1_PORTA				; Read the data line value into N flag.
	bpl rib2					; If the data line was high,
	inc I2C_TEMP				; increment the 1's place to a 1 in the forming byte.  (asl made bit 0 = 0.)
rib2:
	inc VIA1_DDRA				; Put clock line back low.
	dex
	bne rib1					; Go back for next bit if there is one.
	rts
