;*******************
;*** MATHMIP.MIP ***
;****************************

	include	gtereg.h


	opt	at-
	section	.text

;******************************************************************************
; s32 FixedMul(s32 a, s32 b)
	global	FixedMul
FixedMul

	mult    a0,a1
	mfhi    a3
	mflo    a2
	srl     v0,a2,12
	sll     a0,a3,20
	or      v0,v0,a0
	j      ra
	nop

