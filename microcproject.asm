
_main:

;microcproject.c,17 :: 		void main() {
;microcproject.c,18 :: 		Lcd_Init(); // Initialize LCD
	CALL       _Lcd_Init+0
;microcproject.c,19 :: 		Lcd_Cmd(_LCD_CLEAR); // Clear display
	MOVLW      1
	MOVWF      FARG_Lcd_Cmd_out_char+0
	CALL       _Lcd_Cmd+0
;microcproject.c,20 :: 		Lcd_Cmd(_LCD_CURSOR_OFF); // Cursor off
	MOVLW      12
	MOVWF      FARG_Lcd_Cmd_out_char+0
	CALL       _Lcd_Cmd+0
;microcproject.c,21 :: 		Lcd_Out(1,1,text); // Write "Bienvenu" in first row
	MOVLW      1
	MOVWF      FARG_Lcd_Out_row+0
	MOVLW      1
	MOVWF      FARG_Lcd_Out_column+0
	MOVLW      _text+0
	MOVWF      FARG_Lcd_Out_text+0
	CALL       _Lcd_Out+0
;microcproject.c,22 :: 		while(1){}; // Endless loop
L_main0:
	GOTO       L_main0
;microcproject.c,23 :: 		}
L_end_main:
	GOTO       $+0
; end of _main
