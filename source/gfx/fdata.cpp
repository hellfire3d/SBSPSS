/*=========================================================================

	fdata.cpp

	Author:		PKG
	Created: 
	Project:	PRLSR
	Purpose: 

	Copyright (c) 2000 Climax Development Ltd

===========================================================================*/


/*----------------------------------------------------------------------
	Includes
	-------- */

#include "gfx\fdata.h"

#ifndef		__FILE_EQUATES_H__
#include	"biglump.h"
#endif

#include	<UiGfx.h>


/*	Std Lib
	------- */

/*----------------------------------------------------------------------
	Tyepdefs && Defines
	------------------- */

/*----------------------------------------------------------------------
	Structure defintions
	-------------------- */

/*----------------------------------------------------------------------
	Positional Vars
	--------------- */

/*----------------------------------------------------------------------
	Function Prototypes
	------------------- */

/*----------------------------------------------------------------------
	Vars
	---- */

s16	standardFontTab[]=
{
							-1,-1,-1,-1,-1,-1,-1,-1,
							-1,-1,-1,-1,-1,-1,-1,-1,
							-1,-1,-1,-1,-1,-1,-1,-1,
							-1,-1,-1,-1,-1,-1,-1,-1,
	/*   ! " # $ % & ' */	-1,FRM_FONT_0037,FRM_FONT_0038,-1,FRM_FONT_0052,FRM_FONT_0039,FRM_FONT_0040,FRM_FONT_0036,
	/* ( ) * + , - . / */	FRM_FONT_0042,FRM_FONT_0043,FRM_FONT_0041,-1,FRM_FONT_0047,FRM_FONT_0044,FRM_FONT_0053,FRM_FONT_0050,
	/* 0 1 2 3 4 5 6 7 */	FRM_FONT_0026,FRM_FONT_0027,FRM_FONT_0028,FRM_FONT_0029,FRM_FONT_0030,FRM_FONT_0031,FRM_FONT_0032,FRM_FONT_0033,
	/* 8 9 : ; < = > ? */	FRM_FONT_0034,FRM_FONT_0035,FRM_FONT_0045,FRM_FONT_0046,FRM_FONT_0149,-1,FRM_FONT_0150,FRM_FONT_0048,
	/* @ A B C D E F G */	-1,FRM_FONT_0000,FRM_FONT_0001,FRM_FONT_0002,FRM_FONT_0003,FRM_FONT_0004,FRM_FONT_0005,FRM_FONT_0006,
	/* H I J K L M N O */	FRM_FONT_0007,FRM_FONT_0008,FRM_FONT_0009,FRM_FONT_0010,FRM_FONT_0011,FRM_FONT_0012,FRM_FONT_0013,FRM_FONT_0014,
	/* P Q R S T U V W */	FRM_FONT_0015,FRM_FONT_0016,FRM_FONT_0017,FRM_FONT_0018,FRM_FONT_0019,FRM_FONT_0020,FRM_FONT_0021,FRM_FONT_0022,
	/* X Y Z [ \ ] ^ _ */	FRM_FONT_0023,FRM_FONT_0024,FRM_FONT_0025,FRM_FONT_0054,FRM_FONT_0049,FRM_FONT_0055,-1,FRM_FONT_0151,
	/* ` a b c d e f g */	-1,FRM_FONT_0056,FRM_FONT_0057,FRM_FONT_0058,FRM_FONT_0059,FRM_FONT_0060,FRM_FONT_0061,FRM_FONT_0062,
	/* h i j k l m n o */	FRM_FONT_0063,FRM_FONT_0064,FRM_FONT_0065,FRM_FONT_0066,FRM_FONT_0067,FRM_FONT_0068,FRM_FONT_0069,FRM_FONT_0070,
	/* p q r s t u v w */	FRM_FONT_0071,FRM_FONT_0072,FRM_FONT_0073,FRM_FONT_0074,FRM_FONT_0075,FRM_FONT_0076,FRM_FONT_0077,FRM_FONT_0078,
	/* x y z { | } ~ . */	FRM_FONT_0079,FRM_FONT_0080,FRM_FONT_0081,-1,-1,-1,-1,-1,
	/* Ä Å Ç É Ñ Ö Ü á */	-1,-1,-1,-1,-1,-1,-1,-1,
	/* à â ä ã å ç é è */	-1,-1,-1,-1,-1,-1,-1,-1,
	/* ê ë í ì î ï ñ ó */	-1,-1,-1,-1,-1,-1,-1,-1,
	/* ò ô ö õ ú ù û ü */	-1,-1,-1,-1,-1,-1,-1,-1,
	/* † ° ¢ £ § • ¶ ß */	-1,-1,-1,-1,-1,-1,-1,-1,
	/* ® © ™ ´ ¨ ≠ Æ Ø */	-1,-1,-1,-1,-1,-1,-1,-1,
	/* ∞ ± ≤ ≥ ¥ µ ∂ ∑ */	-1,-1,-1,-1,-1,-1,-1,-1,
	/* ∏ π ∫ ª º Ω æ ø */	-1,-1,-1,-1,-1,-1,-1,-1,
	/* ¿ ¡ ¬ √ ƒ ≈ ∆ « */	-1,-1,-1,-1,-1,-1,-1,-1,
	/* » …   À Ã Õ Œ œ */	-1,-1,-1,-1,-1,-1,-1,-1,
	/* – — “ ” ‘ ’ ÷ ◊ */	-1,-1,-1,-1,-1,-1,-1,-1,
	/* ÿ Ÿ ⁄ € ‹ › ﬁ ﬂ */	-1,-1,-1,-1,-1,-1,-1,-1,
	/* ‡ · ‚ „ ‰ Â Ê Á */	-1,-1,-1,-1,-1,-1,-1,-1,
	/* Ë È Í Î Ï Ì Ó Ô */	-1,-1,-1,-1,-1,-1,-1,-1,
	/*  Ò Ú Û Ù ı ˆ ˜ */	-1,-1,-1,-1,-1,-1,-1,-1,
	/* ¯ ˘ ˙ ˚ ¸ ˝ ˛ ˇ */	-1,-1,-1,-1,-1,-1,-1,-1,
};
FontData	standardFont( UI_UIGFX_SPR,	standardFontTab,	9,	1,1, 4 );

/*----------------------------------------------------------------------
	Function:
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
FontData::FontData( FileEquate _fontFileId, s16 *_fontTab, s16 _charHeight, s16 _charGapX, s16 _charGapY, s16 _spaceWidth )
{
	fontFileId=_fontFileId;
	fontTab=_fontTab;
	charHeight=_charHeight;
	charGapX=_charGapX;
	charGapY=_charGapY;
	spaceWidth=_spaceWidth;
}


/*===========================================================================
 end */