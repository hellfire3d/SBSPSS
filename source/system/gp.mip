

	global	SaveGP
	global	ReloadGP
	global	SetGP

	section		.data
GpSave	dw		0

	section		.text

SaveGP
	la	t8,GpSave
	j	ra
	sw	gp,0(t8)

ReloadGP
	add	v0,gp,r0
	la	t8,GpSave
	j	ra
	lw	gp,0(t8)




SetGP
	j	ra
	add	gp,a0,r0
