/*************************************************************************************
*
*
*   'New' polygon types, and associated macros, for non-textured primitives (and 
*   sprites) with per-poly ABR control. Also includes tpage control for sprites.
*   Courtesty of Dead Ashton
*
*
*************************************************************************************/

#ifndef	__PRIMPLUS_H
#define	__PRIMPLUS_H

//------------------------------------------------------------------------------------
//	Defines
//------------------------------------------------------------------------------------

#define	GPUCODE_MODE_SHADE_TEX	(1<<0)
#define	GPUCODE_MODE_SEMI_TRANS	(1<<1)

// GPU Primitive codes. Please remember that 3 and 4 point lines require the pad field setting to 0x55555555.
#define	GPUCODE_POLY_F3 		(0x20)
#define	GPUCODE_POLY_FT3		(0x24)
#define	GPUCODE_POLY_G3 		(0x30)
#define	GPUCODE_POLY_GT3		(0x34)
#define	GPUCODE_POLY_F4 		(0x28)
#define	GPUCODE_POLY_FT4		(0x2c)
#define	GPUCODE_POLY_G4 		(0x38)
#define	GPUCODE_POLY_GT4		(0x3c)

#define	GPUCODE_SPRT			(0x64)
#define	GPUCODE_SPRT_8			(0x74)
#define	GPUCODE_SPRT_16			(0x7c)

#define	GPUCODE_TILE			(0x60)
#define	GPUCODE_TILE_1			(0x68)
#define	GPUCODE_TILE_8			(0x70)
#define	GPUCODE_TILE_16			(0x78)

#define	GPUCODE_LINE_F2			(0x40)
#define	GPUCODE_LINE_G2			(0x50)
#define	GPUCODE_LINE_F3			(0x48)
#define	GPUCODE_LINE_G3			(0x58)	
#define	GPUCODE_LINE_F4			(0x4c)
#define	GPUCODE_LINE_G4			(0x5c)
											   
// GPU Primitive sizes excluding the tag word. This is NOT the same as sizeof(<primitive type>)
#define	GPUSIZE_POLY_F3 		(4)
#define	GPUSIZE_POLY_FT3		(7)
#define	GPUSIZE_POLY_G3 		(6)
#define	GPUSIZE_POLY_GT3		(9)
#define	GPUSIZE_POLY_F4 		(5)
#define	GPUSIZE_POLY_FT4		(9)
#define	GPUSIZE_POLY_G4 		(8)
#define	GPUSIZE_POLY_GT4		(12)

#define	GPUSIZE_SPRT			(4)
#define	GPUSIZE_SPRT_8			(3)
#define	GPUSIZE_SPRT_16			(3)

#define	GPUSIZE_TILE			(3)
#define	GPUSIZE_TILE_1			(2)
#define	GPUSIZE_TILE_8			(2)
#define	GPUSIZE_TILE_16			(2)

#define	GPUSIZE_LINE_F2			(3)
#define	GPUSIZE_LINE_G2			(4)
#define	GPUSIZE_LINE_F3			(5)
#define	GPUSIZE_LINE_G3			(7)
#define	GPUSIZE_LINE_F4			(6)
#define	GPUSIZE_LINE_G4			(9)

#define	GPUSIZE_DR_TPAGE		(1)

#define	GPUSIZE_TPOLY_F3		(GPUSIZE_DR_TPAGE+GPUSIZE_POLY_F3+1)
#define	GPUSIZE_TPOLY_F4		(GPUSIZE_DR_TPAGE+GPUSIZE_POLY_F4+1)
#define	GPUSIZE_TPOLY_G3		(GPUSIZE_DR_TPAGE+GPUSIZE_POLY_G3+1)
#define	GPUSIZE_TPOLY_G4		(GPUSIZE_DR_TPAGE+GPUSIZE_POLY_G4+1)

#define	GPUSIZE_TLINE_F2		(GPUSIZE_DR_TPAGE+GPUSIZE_LINE_F3+1)
#define	GPUSIZE_TLINE_G2		(GPUSIZE_DR_TPAGE+GPUSIZE_LINE_G2+1)
#define	GPUSIZE_TLINE_F3		(GPUSIZE_DR_TPAGE+GPUSIZE_LINE_F3+1)
#define	GPUSIZE_TLINE_G3		(GPUSIZE_DR_TPAGE+GPUSIZE_LINE_G3+1)
#define	GPUSIZE_TLINE_F4		(GPUSIZE_DR_TPAGE+GPUSIZE_LINE_F4+1)
#define	GPUSIZE_TLINE_G4		(GPUSIZE_DR_TPAGE+GPUSIZE_LINE_G4+1)

#define	GPUSIZE_TSPRT			(GPUSIZE_DR_TPAGE+GPUSIZE_SPRT+1)
#define	GPUSIZE_TSPRT_8			(GPUSIZE_DR_TPAGE+GPUSIZE_SPRT_8+1)
#define	GPUSIZE_TSPRT_16		(GPUSIZE_DR_TPAGE+GPUSIZE_SPRT_16+1)

#define	GPUSIZE_TTILE			(GPUSIZE_DR_TPAGE+GPUSIZE_TILE+1)
#define	GPUSIZE_TTILE_1			(GPUSIZE_DR_TPAGE+GPUSIZE_TILE_1+1)
#define	GPUSIZE_TTILE_8			(GPUSIZE_DR_TPAGE+GPUSIZE_TILE_8+1)
#define	GPUSIZE_TTILE_16		(GPUSIZE_DR_TPAGE+GPUSIZE_TILE_16+1)


//------------------------------------------------------------------------------------
//	Macros
//------------------------------------------------------------------------------------

#define	setTDrawTPageSize(p,s)	setlen((p), (s)), (p)->t_code[0] = _get_mode(0,1,0)
#define	setTDrawTPage(p)		setTDrawTPageSize((p), 1)

#define	setTPolyF3(p)			setTDrawTPageSize((p), GPUSIZE_TPOLY_F3), setcode(&((p)->tag_poly), GPUCODE_POLY_F3), ((p)->tag_poly)=0
#define	setTPolyF4(p)			setTDrawTPageSize((p), GPUSIZE_TPOLY_F4), setcode(&((p)->tag_poly), GPUCODE_POLY_F4), ((p)->tag_poly)=0
#define	setTPolyG3(p)			setTDrawTPageSize((p), GPUSIZE_TPOLY_G3), setcode(&((p)->tag_poly), GPUCODE_POLY_G3), ((p)->tag_poly)=0
#define	setTPolyG4(p)			setTDrawTPageSize((p), GPUSIZE_TPOLY_G4), setcode(&((p)->tag_poly), GPUCODE_POLY_G4), ((p)->tag_poly)=0

#define	setTLineF2(p)			setTDrawTPageSize((p), GPUSIZE_TLINE_F2), setcode(&((p)->tag_poly), GPUCODE_LINE_F2), ((p)->tag_poly)=0
#define	setTLineG2(p)			setTDrawTPageSize((p), GPUSIZE_TLINE_G2), setcode(&((p)->tag_poly), GPUCODE_LINE_G2), ((p)->tag_poly)=0
#define	setTLineF3(p)			setTDrawTPageSize((p), GPUSIZE_TLINE_F3), setcode(&((p)->tag_poly), GPUCODE_LINE_F3), ((p)->tag_poly)=0, ((p)->pad)=0x55555555
#define	setTLineG3(p)			setTDrawTPageSize((p), GPUSIZE_TLINE_G3), setcode(&((p)->tag_poly), GPUCODE_LINE_G3), ((p)->tag_poly)=0, ((p)->pad)=0x55555555
#define	setTLineF4(p)			setTDrawTPageSize((p), GPUSIZE_TLINE_F4), setcode(&((p)->tag_poly), GPUCODE_LINE_F4), ((p)->tag_poly)=0, ((p)->pad)=0x55555555
#define	setTLineG4(p)			setTDrawTPageSize((p), GPUSIZE_TLINE_G4), setcode(&((p)->tag_poly), GPUCODE_LINE_G4), ((p)->tag_poly)=0, ((p)->pad)=0x55555555

#define	setTSprt(p)				setTDrawTPageSize((p), GPUSIZE_TSPRT),    setcode(&((p)->tag_poly), GPUCODE_SPRT),    ((p)->tag_poly)=0
#define	setTSprt8(p)			setTDrawTPageSize((p), GPUSIZE_TSPRT_8),  setcode(&((p)->tag_poly), GPUCODE_SPRT_8),  ((p)->tag_poly)=0
#define	setTSprt16(p)			setTDrawTPageSize((p), GPUSIZE_TSPRT_16), setcode(&((p)->tag_poly), GPUCODE_SPRT_16), ((p)->tag_poly)=0

#define	setTTile(p)				setTDrawTPageSize((p), GPUSIZE_TTILE),    setcode(&((p)->tag_poly), GPUCODE_TILE),    ((p)->tag_poly)=0
#define	setTTile1(p)			setTDrawTPageSize((p), GPUSIZE_TTILE_1),  setcode(&((p)->tag_poly), GPUCODE_TILE_1),  ((p)->tag_poly)=0
#define	setTTile8(p)			setTDrawTPageSize((p), GPUSIZE_TTILE_8),  setcode(&((p)->tag_poly), GPUCODE_TILE_8),  ((p)->tag_poly)=0
#define	setTTile16(p)			setTDrawTPageSize((p), GPUSIZE_TTILE_16), setcode(&((p)->tag_poly), GPUCODE_TILE_16), ((p)->tag_poly)=0

#define	setTSemiTrans(p, abe)	setSemiTrans(&((p)->tag_poly), (abe))
#define	setTABRMode(p,abr)		(p)->t_code[0] = _get_mode(0,1,(abr<<5))
#define	setTSprtTPage(p,tp)		(p)->t_code[0] = _get_mode(0,1,(tp))
#define	setTSprtTPageABR(p,t,a)	(p)->t_code[0] = _get_mode(0,1,(((t)&0x19f)|((a)<<5)))

#define	addPrimSize(ot,p,size)	(p)->tag = ((*(ot))|((size)<<24)), *((ot)) = (((u_long)(p)<<8)>>8)


//------------------------------------------------------------------------------------
//	Structures
//------------------------------------------------------------------------------------

typedef struct	__tpoly_f3
		{
		u_long	tag;
		u_long	t_code[1];
		u_long	tag_poly;
		u_char	r0, g0, b0, code;
		short	x0, 	y0;
		short	x1,	y1;
		short	x2,	y2;
		}	TPOLY_F3;				// Flat Triangle with ABR control 

typedef struct	__tpoly_f4
		{
		u_long	tag;
		u_long	t_code[1];
		u_long	tag_poly;
		u_char	r0, g0, b0, code;
		short	x0, 	y0;
		short	x1,	y1;
		short	x2,	y2;
		short	x3,	y3;
		}	TPOLY_F4;				// Flat Quadrangle with ABR control

typedef struct	__tpoly_g3
		{
		u_long	tag;
		u_long	t_code[1];
		u_long	tag_poly;
		u_char	r0, g0, b0, code;
		short	x0, 	y0;
		u_char	r1, g1, b1, pad1;
		short	x1,	y1;
		u_char	r2, g2, b2, pad2;
		short	x2,	y2;
		}	TPOLY_G3;				// Gouraud Triangle with ABR control

typedef struct	__tpoly_g4
		{
		u_long	tag;
		u_long	t_code[1];
		u_long	tag_poly;
		u_char	r0, g0, b0, code;
		short	x0, 	y0;
		u_char	r1, g1, b1, pad1;
		short	x1,	y1;
		u_char	r2, g2, b2, pad2;
		short	x2,	y2;
		u_char	r3, g3, b3, pad3;
		short	x3,	y3;
		}	TPOLY_G4;				// Gouraud Quadrangle with ABR control

//
// Line Primitive Definitions
//

typedef struct	__tline_f2
		{
		u_long	tag;
		u_long	t_code[1];
		u_long	tag_poly;
		u_char	r0, g0, b0, code;
		short	x0, 	y0;
		short	x1,	y1;
		}	TLINE_F2;				// Unconnected Flat Line with ABR control

typedef struct	__tline_g2
		{
		u_long	tag;
		u_long	t_code[1];
		u_long	tag_poly;
		u_char	r0, g0, b0, code;
		short	x0, 	y0;
		u_char	r1, g1, b1, p1;
		short	x1,	y1;
		}	TLINE_G2;				// Unconnected Gouraud Line with ABR control

typedef struct	__tline_f3
		{
		u_long	tag;
		u_long	t_code[1];
		u_long	tag_poly;
		u_char	r0, g0, b0, code;
		short	x0, 	y0;
		short	x1,	y1;
		short	x2,	y2;
		u_long	pad;
		}	TLINE_F3;				// 2 connected Flat Line with ABR control

typedef struct	__tline_g3
		{
		u_long	tag;
		u_long	t_code[1];
		u_long	tag_poly;
		u_char	r0, g0, b0, code;
		short	x0, 	y0;
		u_char	r1, g1, b1, p1;
		short	x1,	y1;
		u_char	r2, g2, b2, p2;
		short	x2,	y2;
		u_long	pad;
		}	TLINE_G3;				// 2 connected Gouraud Line with ABR control

typedef struct	__tline_f4
		{
		u_long	tag;
		u_long	t_code[1];
		u_long	tag_poly;
		u_char	r0, g0, b0, code;
		short	x0, 	y0;
		short	x1,	y1;
		short	x2,	y2;
		short	x3,	y3;
		u_long	pad;
		}	TLINE_F4;				// 3 connected Flat Line Quadrangle with ABR control

typedef struct	__tline_g4
		{
		u_long	tag;
		u_long	t_code[1];
		u_long	tag_poly;
		u_char	r0, g0, b0, code;
		short	x0, 	y0;
		u_char	r1, g1, b1, p1;
		short	x1,	y1;
		u_char	r2, g2, b2, p2;
		short	x2,	y2;
		u_char	r3, g3, b3, p3;
		short	x3,	y3;
		u_long	pad;
		}	TLINE_G4;				// 3 connected Gouraud Line with ABR control

//
// Sprite Primitive Definitions
//

typedef struct 	__tsprt
		{
		u_long	tag;
		u_long	t_code[1];
		u_long	tag_poly;
		u_char	r0, g0, b0, code;
		short	x0, 	y0;
		u_char	u0, v0;	u_short	clut;
		short	w,	h;
		}	TSPRT;					// Free size Sprite with TPage/ABR control 

typedef struct	__tsprt_16
		{
		u_long	tag;
		u_long	t_code[1];
		u_long	tag_poly;
		u_char	r0, g0, b0, code;
		short	x0, 	y0;
		u_char	u0, v0;	u_short	clut;
		}	TSPRT_16;				// 16x16 Sprite with TPage/ABR control
	       
typedef struct	__tsprt_8
		{
		u_long	tag;
		u_long	t_code[1];
		u_long	tag_poly;
		u_char	r0, g0, b0, code;
		short	x0, 	y0;
		u_char	u0, v0;	u_short	clut;
		}	TSPRT_8;				// 8x8 Sprite with TPage/ABR control
	       
//
// Tile Primitive Definitions
//

typedef struct	__ttile
		{
		u_long	tag;
		u_long	t_code[1];
		u_long	tag_poly;
		u_char	r0, g0, b0, code;
		short	x0, 	y0;
		short	w,	h;
		}	TTILE;					// free size Tile with ABR control
	
typedef struct	__ttile16
		{
		u_long	tag;
		u_long	t_code[1];
		u_long	tag_poly;
		u_char	r0, g0, b0, code;
		short	x0, 	y0;
		}	TTILE_16;				// 16x16 Tile with ABR control

typedef struct	__ttile_8
		{
		u_long	tag;
		u_long	t_code[1];
		u_long	tag_poly;
		u_char	r0, g0, b0, code;
		short	x0, 	y0;
		}	TTILE_8;				// 8x8 Tile with ABR control

typedef struct	__ttile_1
		{
		u_long	tag;
		u_long	t_code[1];
		u_long	tag_poly;
		u_char	r0, g0, b0, code;
		short	x0, 	y0;
		}	TTILE_1;				// 1x1 Tile with ABR control

#endif
