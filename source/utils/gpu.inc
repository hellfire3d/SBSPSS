;*******************
;*** GPU Defines ***
;*******************

;******************************************************************************
;	SVECTOR
;	-------
;	short	vx,vy,vz,vw

	rsreset
SVECTOR_vx		rh	1
SVECTOR_vy		rh	1
SVECTOR_vz		rh	1
SVECTOR_vw		rh	1
SVECTOR_size	rb	0

;******************************************************************************
;	VECTOR
;	-------
;	long	vx,vy,vz,vw

	rsreset
VECTOR_vx		rw	1
VECTOR_vy		rw	1
VECTOR_vz		rw	1
VECTOR_vw		rw	1
VECTOR_size		rb	0

;******************************************************************************
;	POLY_F3
;	-------
;	u_long	tag;
;	u_char	r0, g0, b0, code;
;	short	x0, y0;
;	short	x1,	y1;
;	short	x2,	y2;

	rsreset
F3_tag		rb	3
F3_len		rb	1

F3_rgb0		rb	0
F3_r0		rb	1
F3_g0		rb	1
F3_b0		rb	1
F3_code		rb	1

F3_xy0		rh	0
F3_x0		rh	1
F3_y0		rh	1

F3_xy1		rh	0
F3_x1		rh	1
F3_y1		rh	1

F3_xy2		rh	0
F3_x2		rh	1
F3_y2		rh	1

F3_size		rb	0
F3_PrimSize	equ	4
F3_PrimCode	equ	$20

;------------------------------------------------------------------------------
;	POLY_FT3
;	--------
;	u_long	tag;
;	u_char	r0, g0, b0, code;
;	short	x0, y0;
;	u_char	u0, v0;	
;	u_short	clut;
;	short	x1,	y1;
;	u_char	u1, v1;	
;	u_short	tpage;
;	short	x2,	y2;
;	u_char	u2, v2;	
;	u_short	pad1;

	rsreset
FT3_tag			rb	3
FT3_len			rb	1

FT3_rgb0		rb	0
FT3_r0			rb	1
FT3_g0			rb	1
FT3_b0			rb	1
FT3_code		rb	1

FT3_xy0			rb	0
FT3_x0			rh	1
FT3_y0			rh	1

FT3_uv0			rb	0
FT3_u0			rb	1
FT3_v0			rb	1
FT3_clut		rh	1

FT3_xy1			rb	0
FT3_x1			rh	1
FT3_y1			rh	1

FT3_uv1			rb	0
FT3_u1			rb	1
FT3_v1			rb	1
FT3_tpage		rh	1

FT3_xy2			rb	0
FT3_x2			rh	1
FT3_y2			rh	1

FT3_uv2			rb	0
FT3_u2			rb	1
FT3_v2			rb	1
FT3_pad1		rh	1

FT3_size		rb	0
FT3_PrimSize	equ	7
FT3_PrimCode	equ	$24

;******************************************************************************
;	POLY_G3
;	-------
;	u_long	tag;
;	u_char	r0, g0, b0, code;
;	short	x0, 	y0;
;	u_char	r1, g1, b1, pad1;
;	short	x1,	y1;
;	u_char	r2, g2, b2, pad2;
;	short	x2,	y2;

	rsreset
G3_tag			rb	3
G3_len			rb	1

G3_rgb0			rb	0
G3_r0			rb	1
G3_g0			rb	1
G3_b0			rb	1
G3_code			rb	1

G3_xy0			rh	0
G3_x0			rh	1
G3_y0			rh	1

G3_rgb1			rb	0
G3_r1			rb	1
G3_g1			rb	1
G3_b1			rb	1
G3_p1			rb	1

G3_xy1			rh	0
G3_x1			rh	1
G3_y1			rh	1

G3_rgb2			rb	0
G3_r2			rb	1
G3_g2			rb	1
G3_b2			rb	1
G3_p2			rb	1

G3_xy2			rh	0
G3_x2			rh	1
G3_y2			rh	1

G3_size			rb	0
G3_PrimSize		equ	6
G3_PrimCode		equ	$30

;------------------------------------------------------------------------------
;	POLY_GT3
;	--------
;	u_long	tag;
;	u_char	r0, g0, b0, code;
;	short	x0, y0;
;	u_char	u0, v0;	
;	u_short	clut;
;	u_char	r1, g1, b1, p1;
;	short	x1,	y1;
;	u_char	u1, v1;	
;	u_short	tpage;
;	u_char	r2, g2, b2, p2;
;	short	x2,	y2;
;	u_char	u2, v2;	
;	u_short	pad2;

	rsreset
GT3_tag			rb	3
GT3_len			rb	1

GT3_rgb0		rb	0
GT3_r0			rb	1
GT3_g0			rb	1
GT3_b0			rb	1
GT3_code		rb	1

GT3_xy0			rb	0
GT3_x0			rh	1
GT3_y0			rh	1

GT3_uv0			rb	0
GT3_u0			rb	1
GT3_v0			rb	1
GT3_clut		rh	1

GT3_rgb1		rb	0
GT3_r1			rb	1
GT3_g1			rb	1
GT3_b1			rb	1
GT3_p1			rb	1

GT3_xy1			rb	0
GT3_x1			rh	1
GT3_y1			rh	1

GT3_uv1			rb	0
GT3_u1			rb	1
GT3_v1			rb	1
GT3_tpage		rh	1

GT3_rgb2		rb	0
GT3_r2			rb	1
GT3_g2			rb	1
GT3_b2			rb	1
GT3_p2			rb	1

GT3_xy2			rb	0
GT3_x2			rh	1
GT3_y2			rh	1

GT3_uv2			rb	0
GT3_u2			rb	1
GT3_v2			rb	1
GT3_pad2		rh	1

GT3_size		rb	0
GT3_PrimSize	equ	9
GT3_PrimCode	equ	$34

;------------------------------------------------------------------------------
;	POLY_F4
;	-------
;	u_long	tag;
;	u_char	r0, g0, b0, code;
;	short	x0, y0;
;	short	x1,	y1;
;	short	x2,	y2;
;	short	x3,	y3;

	rsreset
F4_tag			rb	3
F4_len			rb	1

F4_rgb0			rb	0
F4_r0			rb	1
F4_g0			rb	1
F4_b0			rb	1
F4_code			rb	1

F4_xy0			rh	0
F4_x0			rh	1
F4_y0			rh	1

F4_xy1			rh	0
F4_x1			rh	1
F4_y1			rh	1

F4_xy2			rh	0
F4_x2			rh	1
F4_y2			rh	1

F4_size			rb	0
F4_PrimSize		equ	5
F4_PrimCode		equ	$28

;------------------------------------------------------------------------------
;	POLY_FT4
;	--------
;	u_long	tag;
;	u_char	r0, g0, b0, code;
;	short	x0, y0;
;	u_char	u0, v0;	
;	u_short	clut;
;	short	x1,	y1;
;	u_char	u1, v1;	
;	u_short	tpage;
;	short	x2,	y2;
;	u_char	u2, v2;	
;	u_short	pad1;
;	short	x3,	y3;
;	u_char	u3, v3;	
;	u_short	pad2;

	rsreset
FT4_tag			rb	3
FT4_len			rb	1

FT4_rgb0	   	rb	0
FT4_r0			rb	1
FT4_g0			rb	1
FT4_b0			rb	1
FT4_code		rb	1

FT4_xy0			rb	0
FT4_x0			rh	1
FT4_y0			rh	1

FT4_uv0		   	rb	0
FT4_u0			rb	1
FT4_v0			rb	1
FT4_clut		rh	1

FT4_xy1			rb	0
FT4_x1			rh	1
FT4_y1			rh	1

FT4_uv1		   	rb	0
FT4_u1			rb	1
FT4_v1			rb	1
FT4_tpage		rh	1

FT4_xy2			rb	0
FT4_x2			rh	1
FT4_y2			rh	1

FT4_uv2		   	rb	0
FT4_u2			rb	1
FT4_v2			rb	1
FT4_pad2		rh	1

FT4_xy3			rb	0
FT4_x3			rh	1
FT4_y3			rh	1

FT4_uv3		   	rb	0
FT4_u3			rb	1
FT4_v3			rb	1
FT4_pad3		rh	1

FT4_size		rb	0
FT4_PrimSize	equ	9
FT4_PrimCode	equ	$2c


;------------------------------------------------------------------------------
;	POLY_G4
;	-------
;	u_long	tag;
;	u_char	r0, g0, b0, code;
;	short	x0, y0;
;	u_char	r1, g1, b1, pad1;
;	short	x1,	y1;
;	u_char	r2, g2, b2, pad2;
;	short	x2,	y2;
;	u_char	r3, g3, b3, pad3;
;	short	x3,	y3;


	rsreset
G4_tag			rb	3
G4_len			rb	1

G4_rgb0			rb	0
G4_r0			rb	1
G4_g0			rb	1
G4_b0			rb	1
G4_code			rb	1

G4_xy0			rh	0
G4_x0			rh	1
G4_y0			rh	1

G4_rgb1			rb	0
G4_r1			rb	1
G4_g1			rb	1
G4_b1			rb	1
G4_p1			rb	1

G4_xy1			rh	0
G4_x1			rh	1
G4_y1			rh	1

G4_rgb2			rb	0
G4_r2			rb	1
G4_g2			rb	1
G4_b2			rb	1
G4_p2			rb	1

G4_xy2			rh	0
G4_x2			rh	1
G4_y2			rh	1

G4_rgb3			rb	0
G4_r3			rb	1
G4_g3			rb	1
G4_b3			rb	1
G4_p3			rb	1

G4_xy3			rh	0
G4_x3			rh	1
G4_y3			rh	1

G4_size			rb	0
G4_PrimSize		equ	8
G4_PrimCode		equ	$38

;------------------------------------------------------------------------------
;	POLY_GT4
;	--------
;	u_long	tag;
;	u_char	r0, g0, b0, code;
;	short	x0, y0;
;	u_char	u0, v0;	
;	u_short	clut;
;	u_char	r1, g1, b1, p1;
;	short	x1,	y1;
;	u_char	u1, v1;	
;	u_short	tpage;
;	u_char	r2, g2, b2, p2;
;	short	x2,	y2;
;	u_char	u2, v2;	
;	u_short	pad2;
;	u_char	r3, g3, b3, p3;
;	short	x3,	y3;
;	u_char	u3, v3;	
;	u_short	pad3;

	rsreset
GT4_tag			rb	3
GT4_len			rb	1

GT4_rgb0	   	rb	0
GT4_r0			rb	1
GT4_g0			rb	1
GT4_b0			rb	1
GT4_code		rb	1

GT4_xy0		   	rb	0
GT4_x0			rh	1
GT4_y0			rh	1

GT4_uv0		   	rb	0
GT4_u0			rb	1
GT4_v0			rb	1
GT4_clut		rh	1

GT4_rgb1	   	rb	0
GT4_r1			rb	1
GT4_g1			rb	1
GT4_b1			rb	1
GT4_p1			rb	1

GT4_xy1		   	rb	0
GT4_x1			rh	1
GT4_y1			rh	1

GT4_uv1		   	rb	0
GT4_u1			rb	1
GT4_v1			rb	1
GT4_tpage		rh	1

GT4_rgb2	   	rb	0
GT4_r2			rb	1
GT4_g2			rb	1
GT4_b2			rb	1
GT4_p2			rb	1

GT4_xy2		   	rb	0
GT4_x2			rh	1
GT4_y2			rh	1

GT4_uv2		   	rb	0
GT4_u2			rb	1
GT4_v2			rb	1
GT4_pad2		rh	1

GT4_rgb3	   	rb	0
GT4_r3			rb	1
GT4_g3			rb	1
GT4_b3			rb	1
GT4_p3			rb	1

GT4_xy3		   	rb	0
GT4_x3			rh	1
GT4_y3			rh	1

GT4_uv3		   	rb	0
GT4_u3			rb	1
GT4_v3			rb	1
GT4_pad3		rh	1

GT4_size		rb	0
GT4_PrimSize	equ	12
GT4_PrimCode	equ	$3c

; =========================================================================
; end
