	opt	at-,c+

	xref	exc_c

	xdef	install_exc
	xdef	uninstall_exc
	xdef	reg_lst
	xdef	dev_kit

	rsreset

OFS_AT	rw	1
OFS_V0	rw	1
OFS_V1	rw	1
OFS_A0	rw	1
OFS_A1	rw	1
OFS_A2	rw	1
OFS_A3	rw	1
OFS_T0	rw	1
OFS_T1	rw	1
OFS_T2	rw	1
OFS_T3	rw	1
OFS_T4	rw	1
OFS_T5	rw	1
OFS_T6	rw	1
OFS_T7	rw	1
OFS_S0	rw	1
OFS_S1	rw	1
OFS_S2	rw	1
OFS_S3	rw	1
OFS_S4	rw	1
OFS_S5	rw	1
OFS_S6	rw	1
OFS_S7	rw	1
OFS_T8	rw	1
OFS_T9	rw	1
OFS_GP	rw	1
OFS_SP	rw	1
OFS_FP	rw	1
OFS_RA	rw	1
OFS_HI	rw	1
OFS_LO	rw	1
OFS_SR	rw	1
OFS_CA	rw	1
OFS_EPC	rw	1

	section .text

;-------------------------------- install_exc ----------------------------
install_exc

	la	t0,exc_onoff
	lw	t1,0(t0)
	nop
	beqz	t1,exc_off
	nop
	jr	ra
	nop
exc_off	li	t1,1
	sw	t1,0(t0)

;detect dev_kit
	la	t0,dev_kit
	lw	t1,0(t0)
	nop
	bgez	t1,dev_2	;just chk once
	nop
	lw	t1,$80(zero)
	move	t2,zero
	lui	t3,$3c1a
	or	t3,t3,$1fa0
	beq	t1,t3,dev_0	;DTL-H2000
	lui	t3,$3c1a
	or	t3,t3,$1fc2
	beq	t1,t3,dev_0	;DTL-H2500
	nop
	b	dev_1
	nop
dev_0	li	t2,1
dev_1	sw	t2,0(t0)
dev_2

;install new exception-handler

;	mfc0	v0,SR
	dw	$40026000
	lui	at,$ffff
	or	at,at,$00fc	;mask off IM,KUc,IEc
	and	at,at,v0
;	mtc0	at,SR
	dw	$40816000
	nop

	li	t0,$80
	la	t1,old_handler

	lw	t2,0(t0)
	lw	t3,4(t0)
	lw	t4,8(t0)
	lw	t5,12(t0)
	sw	t2,0(t1)
	sw	t3,4(t1)
	sw	t4,8(t1)
	sw	t5,12(t1)
	la	t1,new_exc_code
	lw	t2,0(t1)
	lw	t3,4(t1)
	lw	t4,8(t1)
	lw	t5,12(t1)
	sw	t2,0(t0)
	sw	t3,4(t0)
	sw	t4,8(t0)
	sw	t5,12(t0)

;	mtc0	v0,SR		;restore irq
	dw	$40826000
	nop
	jr	ra
	nop

new_exc_code
	la	k0,exc_asm
	jr	k0
	nop

;------------------------------- uninstall_exc ----------------------------
uninstall_exc

	la	t0,exc_onoff
	lw	t1,0(t0)
	nop
	bnez	t1,exc_on
	nop
	jr	ra
	nop
exc_on	li	t1,0
	sw	t1,0(t0)

;	mfc0	v0,SR
	dw	$40026000
	lui	at,$ffff
	or	at,at,$00fc	;mask off IM,KUc,IEc
	and	at,at,v0
;	mtc0	at,SR
	dw	$40816000
	nop

	la	t0,old_handler
	li	t1,$80

	lw	t2,0(t0)
	lw	t3,4(t0)
	lw	t4,8(t0)
	lw	t5,12(t0)
	sw	t2,0(t1)
	sw	t3,4(t1)
	sw	t4,8(t1)
	sw	t5,12(t1)

;	mtc0	v0,SR		;restore irq
	dw	$40826000
	nop
	jr	ra
	nop

;------------------------------- exc_asm ----------------------------
exc_asm

;	mfc0	k0,Cause
	dw	$401a6800
	nop
	li	k1,%1111011111110

	srl	k0,k0,2
	andi	k0,k0,$1f
	srlv	k1,k1,k0

	andi	k1,k1,1
	beqz	k1,old_handler
	nop

;chk break
	li	k1,9
	bne	k0,k1,go_on
	nop
;	mfc0	k0,EPC
	dw	$401a7000
	nop
	lw	k0,0(k0)
	nop
	srl	k0,k0,16
	li	k1,6
	beq	k0,k1,go_on
	li	k1,7
	beq	k0,k1,go_on
	nop

old_handler

	nop
	nop
	nop
	nop

go_on	la	k0,reg_save

	sw	at,OFS_AT(k0)
	sw	v0,OFS_V0(k0)
	sw	v1,OFS_V1(k0)
	sw	a0,OFS_A0(k0)
	sw	a1,OFS_A1(k0)
	sw	a2,OFS_A2(k0)
	sw	a3,OFS_A3(k0)
	sw	t0,OFS_T0(k0)
	sw	t1,OFS_T1(k0)
	sw	t2,OFS_T2(k0)
	sw	t3,OFS_T3(k0)
	sw	t4,OFS_T4(k0)
	sw	t5,OFS_T5(k0)
	sw	t6,OFS_T6(k0)
	sw	t7,OFS_T7(k0)
	sw	s0,OFS_S0(k0)
	sw	s1,OFS_S1(k0)
	sw	s2,OFS_S2(k0)
	sw	s3,OFS_S3(k0)
	sw	s4,OFS_S4(k0)
	sw	s5,OFS_S5(k0)
	sw	s6,OFS_S6(k0)
	sw	s7,OFS_S7(k0)
	sw	t8,OFS_T8(k0)
	sw	t9,OFS_T9(k0)
	sw	gp,OFS_GP(k0)
	sw	sp,OFS_SP(k0)
	sw	fp,OFS_FP(k0)
	sw	ra,OFS_RA(k0)
	mfhi	k1
	nop
	sw	k1,OFS_HI(k0)
	mflo	k1
	nop
	sw	k1,OFS_LO(k0)
;	mfc0	k1,SR
	dw	$401b6000
	nop
	sw	k1,OFS_SR(k0)

;	mfc0	k1,Cause
	dw	$401b6800
	nop
	sw	k1,OFS_CA(k0)

;	mfc0	k1,EPC
	dw	$401b7000
	nop
	nop
	sw	k1,OFS_EPC(k0)

	la	k0,exc_c
	jr	k0
	nop

	section	.data

dev_kit		dw	-1	;-1=undefined, 0=playstation, 1=devkit
exc_onoff	dw	0	;internal exc-handler status onoff-flag
reg_lst		dw	reg_save

	section	.bss

reg_save	dsw	34

