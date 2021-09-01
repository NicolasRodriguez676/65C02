; Nicolas Rodriguez
; Macros
; March 15, 2021

; All macros are susceptible to IRQ, unless
; noted otherwise

;--Basic Details----------------------------------------------------
; 
;   rough estimates ahead. further testing needed!
;
;   @ F(clk) = 3MHz
;
;   Time = T(clk) * (16 * clock_cycles + 14 - integer(clock_cyles % 256))
;   Time(min)  = 4.67uS  (latency)
;   Time(max)  = 349.4mS
;   Time(step) = 16 * T(clk) = 5.33uS
;                               
;
;   clock_cycles = (Time / T(clk) - 14) / 16
;                   - 1 cc every 256 clock_cycles
;
;   number of bits = log2(clock_cyles)
;
;   the conditional statements are needed to prevent overflow
;   when adding one to the arguments of the delay macro.
;   adding one solves the rollover to $ff when setting the value
;   to $00. this lends the macro to being more intuitive but less
;   accurate overall.
;
;-------------------------------------------------------------------
    .macro delay_cc hi_cc, lo_cc

    .local delay
    .if lo_cc = $ff && hi_cc = $ff
        lda #lo_cc          ; 2 cc
        sta DLY_VAR0        ; 3 cc
        lda #hi_cc          ; 2 cc
        sta DLY_VAR1        ; 3 cc
    delay:
        dec DLY_VAR0        ; 5 cc
        bne delay           ; 2 + 1 cc
        dec DLY_VAR1        ; 5 cc
        bne delay           ; 2 + 1 cc

    .elseif lo_cc <> $ff && hi_cc <> $ff
        lda #(lo_cc + 1)    ; 2 cc
        sta DLY_VAR0        ; 3 cc
        lda #(hi_cc + 1)    ; 2 cc
        sta DLY_VAR1        ; 3 cc
    delay:
        dec DLY_VAR0        ; 5 cc
        bne delay           ; 2 + 1 cc
        dec DLY_VAR1        ; 5 cc
        bne delay           ; 2 + 1 cc

    .elseif lo_cc = $ff && hi_cc <> $ff
        lda #lo_cc          ; 2 cc
        sta DLY_VAR0        ; 3 cc
        lda #(hi_cc + 1)    ; 2 cc
        sta DLY_VAR1        ; 3 cc
    delay:
        dec DLY_VAR0        ; 5 cc
        bne delay           ; 2 + 1 cc
        dec DLY_VAR1        ; 5 cc
        bne delay           ; 2 + 1 cc
    
    .elseif lo_cc <> $ff && hi_cc = $ff
        lda #(lo_cc + 1)    ; 2 cc
        sta DLY_VAR0        ; 3 cc
        lda #hi_cc          ; 2 cc
        sta DLY_VAR1        ; 3 cc
    delay:
        dec DLY_VAR0        ; 5 cc
        bne delay           ; 2 + 1 cc
        dec DLY_VAR1        ; 5 cc
        bne delay           ; 2 + 1 cc
    .endif

    .endmacro; delay_cc

;--VGA Macros-------------------------------------------------------
    
    .macro vga_cs_enable
        lda #1
        tsb VIA2_PORTA
    .endmacro; vga_cs_enable
   
    .macro vga_cs_disable
        lda #1
        trb VIA2_PORTA
    .endmacro; vga_cs_disable

    .macro vga_set_data data
    	set_address vga_data_ptr, data
    .endmacro; vga_set_data

    .macro vga_data_red red_data
        lda vga_c0
        and #$f0
        ora #(red_data & $0f)
        sta vga_c0
    .endmacro; vga_data_red

    .macro vga_data_green green_data
        lda vga_c0
        and #$0f
        ora #(green_data & $f0)
        sta vga_c0
    .endmacro; vga_data_green

    .macro vga_data_blue blue_data
        lda #(blue_data & $0f)
        sta vga_c0 + 1        
    .endmacro; vga_data_blue
    
    .macro vga_data_y_axis y_data1, y_data2
        lda #<y_data1
        sta vga_y1
        lda #>y_data1
        sta vga_y1 + 1
        
        lda #<y_data2
        sta vga_y2
        lda #>y_data2
        sta vga_y2 + 1
    .endmacro; vga_data_y_axis
    
    .macro vga_data_x_axis x_data1, x_data2
        lda #<x_data1
        sta vga_x1
        lda #>x_data1
        sta vga_x1 + 1
        
        lda #<x_data2
        sta vga_x2
        lda #>x_data2
        sta vga_x2 + 1
    .endmacro; vga_data_x_axis
    
    ; "super" macros

    .macro vga_data_rgb red, green, blue
        vga_data_red red
        vga_data_green green
        vga_data_blue blue
    .endmacro; vga_data_rgb

    .macro vga_draw_box org_x, org_y, width, length, red, green, blue
        vga_data_rgb red, green, blue
        vga_data_x_axis org_x, width
        vga_data_y_axis org_y, length
    .endmacro; vga_draw_box

;--Joystick Macros (SPI/Parallel)-----------------------------------

    .macro jystk_configure mode
        lda #mode
        sta jystk_get_dat + 1
    .endmacro; jystk_configure

;--LCD Macros (I2C)-------------------------------------------------

;-------------------------------------------------------------------
;   polling will not help that much so I decided to do software 
;   delays
;   
;   delays are set for use with a 3MHz clock.
;   ~2mS and ~4.5mS, respectively
;
;-------------------------------------------------------------------

    .macro lcd_wait
        delay_cc $02, $80
    .endmacro; lcd_wait

    .macro lcd_wait_long
        delay_cc $06, $98
    .endmacro; lcd_wait_long


;-------------------------------------------------------------------
;   printing character macros
;
;   two steps for strings
;   1. fix the data to match 4 bit mode
;   2. fix the data sequence to match
;      the lcd data rows. not gonna happen lol
;
;-------------------------------------------------------------------

    .macro lcd_print_sline1 string
        lcd_set_ddram LINE_STRT_ONE
        lcd_print_str string
    .endmacro; lcd_print_sline1

    .macro lcd_print_sline2 string
        lcd_set_ddram LINE_STRT_TWO
        lcd_print_str string
    .endmacro; lcd_print_sline2

    .macro lcd_print_sline3 string
        lcd_set_ddram LINE_STRT_THR
        lcd_print_str string
    .endmacro; lcd_print_sline3
    
    .macro lcd_print_sline4 string
        lcd_set_ddram LINE_STRT_FOU
        lcd_print_str string
    .endmacro; lcd_print_sline4

    .macro lcd_print_line string, start
        lcd_set_ddram start
        lcd_print_str string
    .endmacro; lcd_print_line
    
    .macro lcd_print_char char
        lda #char
	    jsr lcd_print
    .endmacro; lcd_print_char

    .macro lcd_print_str string
    .local lps0, lps1
	    set_address DATA_PTR0, string    
        ldy #0
    lps0:
        lda (DATA_PTR0), y
        beq lps1
        iny
        phy
        jsr lcd_print
        ply
        bra lps0
    lps1:
    .endmacro; lcd_print_string

;-------------------------------------------------------------------
;   these macros will streamline changing the settings on the LCD
;
;   I made LCD_ENOT to include the backlight bit.
;   it is entirely not needed for normal operation.
;
;-------------------------------------------------------------------
    .macro lcd_clr_dspl
	    set_address DATA_PTR, clr_dspl
	    jsr write_i2c
	.endmacro; lcd_clr_dspl

    .macro lcd_rtn_home
	    set_address DATA_PTR, rtn_home
	    jsr write_i2c
        lcd_wait
    .endmacro; lcd_rtn_home

    .macro lcd_set_ddram address
        lda #address
        sta VAR000
        jsr lcd_set_ddram_sr        
    .endmacro; lcd_set_ddram

    .macro lcd_set_cgram address
        lda #address
        sta VAR000
        jsr lcd_set_cgram_sr
        .endmacro; lcd_set_cgram
    
    .macro lcd_options option, mode
    .if option = 1
        .if mode = 0
            lda #(NTRY_MDE_SET10 | LCD_ENOT)
            sta lcd_set_opt
            lda #(NTRY_MDE_SET10 | LCD_E)
            sta lcd_set_opt + 1
            lda #(NTRY_MDE_SET00 | LCD_ENOT)
            sta lcd_set_opt + 2
            lda #(NTRY_MDE_SET00 | LCD_E)
            sta lcd_set_opt + 3
            jsr write_i2c
        .elseif mode = 1
            lda #(NTRY_MDE_SET11 | LCD_ENOT)
            sta lcd_set_opt
            lda #(NTRY_MDE_SET11 | LCD_E)
            sta lcd_set_opt + 1
            lda #(NTRY_MDE_SET01 | LCD_ENOT)
            sta lcd_set_opt + 2
            lda #(NTRY_MDE_SET01 | LCD_E)
            sta lcd_set_opt + 3
            jsr write_i2c
        .elseif mode = 2
            lda #(NTRY_MDE_SET12 | LCD_ENOT)
            sta lcd_set_opt
            lda #(NTRY_MDE_SET12 | LCD_E)
            sta lcd_set_opt + 1
            lda #(NTRY_MDE_SET02 | LCD_ENOT)
            sta lcd_set_opt + 2
            lda #(NTRY_MDE_SET02 | LCD_E)
            sta lcd_set_opt + 3
            jsr write_i2c
        .elseif mode = 3
            lda #(NTRY_MDE_SET13 | LCD_ENOT)
            sta lcd_set_opt
            lda #(NTRY_MDE_SET13 | LCD_E)
            sta lcd_set_opt + 1
            lda #(NTRY_MDE_SET03 | LCD_ENOT)
            sta lcd_set_opt + 2
            lda #(NTRY_MDE_SET03 | LCD_E)
            sta lcd_set_opt + 3
            jsr write_i2c
        .else
            .error  "lcd_options: Only modes 3-0 are defined for option 1."
        .endif
    .elseif option = 2
        .if mode = 0
            lda #(DIS_CSR_CNT10 | LCD_ENOT)
            sta lcd_set_opt
            lda #(DIS_CSR_CNT10 | LCD_E)
            sta lcd_set_opt + 1
            lda #(DIS_CSR_CNT00 | LCD_ENOT)
            sta lcd_set_opt + 2
            lda #(DIS_CSR_CNT00 | LCD_E)
            sta lcd_set_opt + 3
            jsr write_i2c
        .elseif mode = 1
            lda #(DIS_CSR_CNT11 | LCD_ENOT)
            sta lcd_set_opt
            lda #(DIS_CSR_CNT11 | LCD_E)
            sta lcd_set_opt + 1
            lda #(DIS_CSR_CNT01 | LCD_ENOT)
            sta lcd_set_opt + 2
            lda #(DIS_CSR_CNT01 | LCD_E)
            sta lcd_set_opt + 3
            jsr write_i2c
        .elseif mode = 2
            lda #(DIS_CSR_CNT12 | LCD_ENOT)
            sta lcd_set_opt
            lda #(DIS_CSR_CNT12 | LCD_E)
            sta lcd_set_opt + 1
            lda #(DIS_CSR_CNT02 | LCD_ENOT)
            sta lcd_set_opt + 2
            lda #(DIS_CSR_CNT02 | LCD_E)
            sta lcd_set_opt + 3
            jsr write_i2c
        .elseif mode = 3
            lda #(DIS_CSR_CNT13 | LCD_ENOT)
            sta lcd_set_opt
            lda #(DIS_CSR_CNT13 | LCD_E)
            sta lcd_set_opt + 1
            lda #(DIS_CSR_CNT03 | LCD_ENOT)
            sta lcd_set_opt + 2
            lda #(DIS_CSR_CNT03 | LCD_E)
            sta lcd_set_opt + 3
            jsr write_i2c
        .elseif mode = 4
        lcd_wait_long
            lda #(DIS_CSR_CNT14 | LCD_ENOT)
            sta lcd_set_opt
            lda #(DIS_CSR_CNT14 | LCD_E)
            sta lcd_set_opt + 1
            lda #(DIS_CSR_CNT04 | LCD_ENOT)
            sta lcd_set_opt + 2
            lda #(DIS_CSR_CNT04 | LCD_E)
            sta lcd_set_opt + 3
            jsr write_i2c
        .elseif mode = 5
            lda #(DIS_CSR_CNT15 | LCD_ENOT)
            sta lcd_set_opt
            lda #(DIS_CSR_CNT15 | LCD_E)
            sta lcd_set_opt + 1
            lda #(DIS_CSR_CNT05 | LCD_ENOT)
            sta lcd_set_opt + 2
            lda #(DIS_CSR_CNT05 | LCD_E)
            sta lcd_set_opt + 3
            jsr write_i2c
        .elseif mode = 6
            lda #(DIS_CSR_CNT16 | LCD_ENOT)
            sta lcd_set_opt
            lda #(DIS_CSR_CNT16 | LCD_E)
            sta lcd_set_opt + 1
            lda #(DIS_CSR_CNT06 | LCD_ENOT)
            sta lcd_set_opt + 2
            lda #(DIS_CSR_CNT06 | LCD_E)
            sta lcd_set_opt + 3
            jsr write_i2c
        .elseif mode = 7
            lda #(DIS_CSR_CNT17 | LCD_ENOT)
            sta lcd_set_opt
            lda #(DIS_CSR_CNT17 | LCD_E)
            sta lcd_set_opt + 1
            lda #(DIS_CSR_CNT07 | LCD_ENOT)
            sta lcd_set_opt + 2
            lda #(DIS_CSR_CNT07 | LCD_E)
            jsr write_i2c
        .else
            .error  "lcd_options: Only modes 7-0 are defined for option 2."
        .endif
    .elseif option = 3
        .if mode = 0
            lda #(SHFT_DIS_CSR10 | LCD_ENOT)
            sta lcd_set_opt
            lda #(SHFT_DIS_CSR10 | LCD_E)
            sta lcd_set_opt + 1
            lda #(SHFT_DIS_CSR00 | LCD_ENOT)
            sta lcd_set_opt + 2
            lda #(SHFT_DIS_CSR00 | LCD_E)
            sta lcd_set_opt + 3
            jsr write_i2c
        .elseif mode = 1
            lda #(SHFT_DIS_CSR11 | LCD_ENOT)
            sta lcd_set_opt
            lda #(SHFT_DIS_CSR11 | LCD_E)
            sta lcd_set_opt + 1
            lda #(SHFT_DIS_CSR01 | LCD_ENOT)
            sta lcd_set_opt + 3
            sta lcd_set_opt + 2
            lda #(SHFT_DIS_CSR01 | LCD_E)
            sta lcd_set_opt + 3
            jsr write_i2c
        .elseif mode = 2
            lda #(SHFT_DIS_CSR12 | LCD_ENOT)
            sta lcd_set_opt
            lda #(SHFT_DIS_CSR12 | LCD_E)
            sta lcd_set_opt + 1
            lda #(SHFT_DIS_CSR02 | LCD_ENOT)
            sta lcd_set_opt + 2
            lda #(SHFT_DIS_CSR02 | LCD_E)
            sta lcd_set_opt + 3
            jsr write_i2c
        .elseif mode = 3
            lda #(SHFT_DIS_CSR13 | LCD_ENOT)
            sta lcd_set_opt
            lda #(SHFT_DIS_CSR13 | LCD_E)
            sta lcd_set_opt + 1
            lda #(SHFT_DIS_CSR03 | LCD_ENOT)
            sta lcd_set_opt + 2
            lda #(SHFT_DIS_CSR03 | LCD_E)
            sta lcd_set_opt + 3
            jsr write_i2c
        .else
            .error  "lcd_options: Only modes 3-0 are defined for option 3."
        .endif
    .else
        .error  "lcd_options: There are only three options in this macro."
    .endif
    .endmacro; lcd_options
    


;--Update Address---------------------------------------------------

;-------------------------------------------------------------------
;   Fetch high and low byte of an address
;   and place it at the desired location, like a pointer
;
;-------------------------------------------------------------------

    .macro set_address pointer, address
        lda #<address
	    sta pointer
	    lda #>address
	    sta pointer + 1
    .endmacro; set_address

;--Math Macros------------------------------------------------------

    .macro dvd_imm dividend, divider
    	lda #divider
	    sta VAR002
        stz VAR003
        lda #dividend
	    sta VAR000
        stz VAR001
        jsr div_16x16
    .endmacro; dvd_imm

    .macro dvd_mem10 dividend
    	dvd_imm dividend, 10
    .endmacro; dvd_mem10

;--String Macros----------------------------------------------------

    .macro concat_str string1, string2

    ; not worth it right now
        
    .endmacro; concat_str

;--Virtual Stack----------------------------------------------------

    .macro vs_init
	    set_address v_stack_ptr, v_stack
    .endmacro; vs_init

    .macro vs_pha
        sta (v_stack_ptr)
        inc v_stack_ptr
    .endmacro; vs_pha

    .macro vs_phx
        pha
        txa
        sta (v_stack_ptr)
        inc v_stack_ptr
        pla
    .endmacro; vs_phx

    .macro vs_phy
        pha
        tya
        sta (v_stack_ptr)
        inc v_stack_ptr
        pla
    .endmacro; vs_phy
    
    .macro vs_pla
        lda (v_stack_ptr)
        dec v_stack_ptr
    .endmacro; vs_pla

    .macro vs_plx
        pha
        lda (v_stack_ptr)
        tax
        dec v_stack_ptr
        pla
    .endmacro; vs_plx

    .macro vs_ply
        pha
        lda (v_stack_ptr)
        tay
        dec v_stack_ptr
        pla
    .endmacro; vs_ply

;--Sound Macros----------------------------------------------------

    .macro set_octave octave
        lda #octave
        jsr sound_write_octave
    .endmacro; snd_octave

    .macro set_note note
        lda #note
        jsr sound_write_note
    .endmacro; snd_note