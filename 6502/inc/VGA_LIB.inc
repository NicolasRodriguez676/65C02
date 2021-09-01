; Nicolas Rodriguez
; VGA Library
; May 1, 2021

	;---- setup vga control line
vga_init:
    lda #1
	sta VIA2_DDRA
	vga_cs_disable
    rts
	
	;---- send vga data
vga_send_data:    
	vga_cs_enable
	ldy #0
vga_loop:
	lda (vga_data_ptr), y
	sta VIA2_SR
	nop
	nop
	iny
	cpy #VGA_END
	bne vga_loop
	vga_cs_disable
    rts