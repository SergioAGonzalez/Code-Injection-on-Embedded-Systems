; main.s
; Runs on any Cortex M processor
; A very simple first project implementing a random number generator
; Daniel Valvano
; May 4, 2012

;  This example accompanies the book
;  "Embedded Systems: Introduction to Arm Cortex M Microcontrollers",
;  ISBN: 978-1469998749, Jonathan Valvano, copyright (c) 2012
;  Section 3.3.10, Program 3.12
;
;Copyright 2012 by Jonathan W. Valvano, valvano@mail.utexas.edu
;   You may use, edit, run or distribute this file
;   as long as the above copyright notice remains
;THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
;OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
;MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
;VALVANO SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL,
;OR CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
;For more information about my classes, my research, and my books, see
;http://users.ece.utexas.edu/~valvano/


       THUMB
       AREA    DATA, ALIGN=2
       ALIGN          
       AREA    |.text|, CODE, READONLY, ALIGN=2
       EXPORT  Start
	   ;unlock 0x4C4F434B
	   
	   ;PF4 is SW1
	   ;PF0 is SW2
	   ;PF1 is RGB Red
	   ;Enable Clock RCGCGPIO p338
	   ;Set direction 1 is out 0 is in. GPIODIR
	   ;DEN 
	   ; 0x3FC
	   
	   
Start  
	mov32 R0, #0x400fe608
	mov R1, #0x20
	str R1, [R0]		;enable GPIOF clock
	nop
	nop
	mov32 R0, #0x40025000
	;mov32 R1, #0x4c4f434b	;unlock value
	;str R1, [R0, #0x520]	;unlock GPIOF
	mov R1, #4
	str R1, [R0, #0x400]	;set dir as output
	str R1, [R0, #0x510]	;enable pull-up resistor
	str R1, [R0, #0x51c]	;enable digital enable 
	mov32 R2, #0xd9039	;for 16 MHz, 8M cycles ls half a second
				;divided by 9 cycles per loop

loop				;cycles: 9 cycles from simulator
	subs R2, R2, #1		;decrement counter
	mov32eq R2, #0xd9039	;reset counter if hit zero
	mvneq R1, R1		;invert R1
	;andeq R1, #4		;and with pin 2
	streq R1, [R0, #0x3fc]	;store led state in led reg
	b loop

       ALIGN      
       END  
           