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
	/*   ! " # $ % & ' */	-1,FRM_PR_FONTINGAME_0037,FRM_PR_FONTINGAME_0038,-1,FRM_PR_FONTINGAME_0052,FRM_PR_FONTINGAME_0039,FRM_PR_FONTINGAME_0040,FRM_PR_FONTINGAME_0036,
	/* ( ) * + , - . / */	FRM_PR_FONTINGAME_0042,FRM_PR_FONTINGAME_0043,FRM_PR_FONTINGAME_0041,-1,FRM_PR_FONTINGAME_0047,FRM_PR_FONTINGAME_0044,FRM_PR_FONTINGAME_0053,FRM_PR_FONTINGAME_0050,
	/* 0 1 2 3 4 5 6 7 */	FRM_PR_FONTINGAME_0026,FRM_PR_FONTINGAME_0027,FRM_PR_FONTINGAME_0028,FRM_PR_FONTINGAME_0029,FRM_PR_FONTINGAME_0030,FRM_PR_FONTINGAME_0031,FRM_PR_FONTINGAME_0032,FRM_PR_FONTINGAME_0033,
	/* 8 9 : ; < = > ? */	FRM_PR_FONTINGAME_0034,FRM_PR_FONTINGAME_0035,FRM_PR_FONTINGAME_0045,FRM_PR_FONTINGAME_0046,FRM_PR_FONTINGAME_0149,-1,FRM_PR_FONTINGAME_0150,FRM_PR_FONTINGAME_0048,
	/* @ A B C D E F G */	-1,FRM_PR_FONTINGAME_0000,FRM_PR_FONTINGAME_0001,FRM_PR_FONTINGAME_0002,FRM_PR_FONTINGAME_0003,FRM_PR_FONTINGAME_0004,FRM_PR_FONTINGAME_0005,FRM_PR_FONTINGAME_0006,
	/* H I J K L M N O */	FRM_PR_FONTINGAME_0007,FRM_PR_FONTINGAME_0008,FRM_PR_FONTINGAME_0009,FRM_PR_FONTINGAME_0010,FRM_PR_FONTINGAME_0011,FRM_PR_FONTINGAME_0012,FRM_PR_FONTINGAME_0013,FRM_PR_FONTINGAME_0014,
	/* P Q R S T U V W */	FRM_PR_FONTINGAME_0015,FRM_PR_FONTINGAME_0016,FRM_PR_FONTINGAME_0017,FRM_PR_FONTINGAME_0018,FRM_PR_FONTINGAME_0019,FRM_PR_FONTINGAME_0020,FRM_PR_FONTINGAME_0021,FRM_PR_FONTINGAME_0022,
	/* X Y Z [ \ ] ^ _ */	FRM_PR_FONTINGAME_0023,FRM_PR_FONTINGAME_0024,FRM_PR_FONTINGAME_0025,FRM_PR_FONTINGAME_0054,FRM_PR_FONTINGAME_0049,FRM_PR_FONTINGAME_0055,-1,FRM_PR_FONTINGAME_0151,
	/* ` a b c d e f g */	-1,FRM_PR_FONTINGAME_0056,FRM_PR_FONTINGAME_0057,FRM_PR_FONTINGAME_0058,FRM_PR_FONTINGAME_0059,FRM_PR_FONTINGAME_0060,FRM_PR_FONTINGAME_0061,FRM_PR_FONTINGAME_0062,
	/* h i j k l m n o */	FRM_PR_FONTINGAME_0063,FRM_PR_FONTINGAME_0064,FRM_PR_FONTINGAME_0065,FRM_PR_FONTINGAME_0066,FRM_PR_FONTINGAME_0067,FRM_PR_FONTINGAME_0068,FRM_PR_FONTINGAME_0069,FRM_PR_FONTINGAME_0070,
	/* p q r s t u v w */	FRM_PR_FONTINGAME_0071,FRM_PR_FONTINGAME_0072,FRM_PR_FONTINGAME_0073,FRM_PR_FONTINGAME_0074,FRM_PR_FONTINGAME_0075,FRM_PR_FONTINGAME_0076,FRM_PR_FONTINGAME_0077,FRM_PR_FONTINGAME_0078,
	/* x y z { | } ~ . */	FRM_PR_FONTINGAME_0079,FRM_PR_FONTINGAME_0080,FRM_PR_FONTINGAME_0081,-1,-1,-1,-1,-1,
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