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
							           -1,           -1,           -1,           -1,           -1,           -1,           -1,           -1,
							           -1,           -1,           -1,           -1,           -1,           -1,           -1,           -1,
							           -1,           -1,           -1,           -1,           -1,           -1,           -1,           -1,
							           -1,           -1,           -1,           -1,           -1,           -1,           -1,           -1,
	/*   ! " # $ % & ' */	           -1,FRM_FONT_0037,FRM_FONT_0038,           -1,FRM_FONT_0052,FRM_FONT_0039,FRM_FONT_0040,FRM_FONT_0036,
	/* ( ) * + , - . / */	FRM_FONT_0042,FRM_FONT_0043,FRM_FONT_0041,FRM_FONT_0152,FRM_FONT_0047,FRM_FONT_0044,FRM_FONT_0053,FRM_FONT_0050,
	/* 0 1 2 3 4 5 6 7 */	FRM_FONT_0026,FRM_FONT_0027,FRM_FONT_0028,FRM_FONT_0029,FRM_FONT_0030,FRM_FONT_0031,FRM_FONT_0032,FRM_FONT_0033,
	/* 8 9 : ; < = > ? */	FRM_FONT_0034,FRM_FONT_0035,FRM_FONT_0045,FRM_FONT_0046,FRM_FONT_0149,FRM_FONT_0153,FRM_FONT_0150,FRM_FONT_0048,
	/* @ A B C D E F G */	           -1,FRM_FONT_0000,FRM_FONT_0001,FRM_FONT_0002,FRM_FONT_0003,FRM_FONT_0004,FRM_FONT_0005,FRM_FONT_0006,
	/* H I J K L M N O */	FRM_FONT_0007,FRM_FONT_0008,FRM_FONT_0009,FRM_FONT_0010,FRM_FONT_0011,FRM_FONT_0012,FRM_FONT_0013,FRM_FONT_0014,
	/* P Q R S T U V W */	FRM_FONT_0015,FRM_FONT_0016,FRM_FONT_0017,FRM_FONT_0018,FRM_FONT_0019,FRM_FONT_0020,FRM_FONT_0021,FRM_FONT_0022,
	/* X Y Z [ \ ] ^ _ */	FRM_FONT_0023,FRM_FONT_0024,FRM_FONT_0025,FRM_FONT_0054,FRM_FONT_0049,FRM_FONT_0055,           -1,FRM_FONT_0151,
	/* ` a b c d e f g */	           -1,FRM_FONT_0056,FRM_FONT_0057,FRM_FONT_0058,FRM_FONT_0059,FRM_FONT_0060,FRM_FONT_0061,FRM_FONT_0062,
	/* h i j k l m n o */	FRM_FONT_0063,FRM_FONT_0064,FRM_FONT_0065,FRM_FONT_0066,FRM_FONT_0067,FRM_FONT_0068,FRM_FONT_0069,FRM_FONT_0070,
	/* p q r s t u v w */	FRM_FONT_0071,FRM_FONT_0072,FRM_FONT_0073,FRM_FONT_0074,FRM_FONT_0075,FRM_FONT_0076,FRM_FONT_0077,FRM_FONT_0078,
	/* x y z { | } ~ . */	FRM_FONT_0079,FRM_FONT_0080,FRM_FONT_0081,           -1,           -1,           -1,           -1,           -1,
	/* Ä Å Ç É Ñ Ö Ü á */	           -1,           -1,           -1,           -1,           -1,           -1,           -1,           -1,
	/* à â ä ã å ç é è */	           -1,           -1,           -1,           -1,           -1,           -1,           -1,           -1,
	/* ê ë í ì î ï ñ ó */	           -1,           -1,           -1,           -1,           -1,           -1,           -1,           -1,
	/* ò ô ö õ ú ù û ü */	           -1,           -1,           -1,           -1,           -1,           -1,           -1,           -1
	/* † ° ¢ £ § • ¶ ß */	           -1,           -1,           -1,           -1,           -1,           -1,           -1,           -1,
	/* ® © ™ ´ ¨ ≠ Æ Ø */	           -1,           -1,           -1,           -1,           -1,           -1,           -1,           -1,
	/* ∞ ± ≤ ≥ ¥ µ ∂ ∑ */	           -1,           -1,           -1,           -1,           -1,           -1,           -1,           -1,
	/* ∏ π ∫ ª º Ω æ ø */	           -1,           -1,           -1,           -1,           -1,           -1,           -1,           -1,
							-1,-1,
	/* ¿ ¡ ¬ √ ƒ ≈ ∆ « */	FRM_FONT_0084,FRM_FONT_0085,FRM_FONT_0086,FRM_FONT_0087,FRM_FONT_0088,FRM_FONT_0089,FRM_FONT_0090,FRM_FONT_0091,
	/* » …   À Ã Õ Œ œ */	FRM_FONT_0092,FRM_FONT_0093,FRM_FONT_0094,FRM_FONT_0095,FRM_FONT_0096,FRM_FONT_0097,FRM_FONT_0098,FRM_FONT_0099,
	/* – — “ ” ‘ ’ ÷ ◊ */	FRM_FONT_0100,FRM_FONT_0101,FRM_FONT_0102,FRM_FONT_0103,FRM_FONT_0104,FRM_FONT_0105,FRM_FONT_0106,FRM_FONT_0107,
	/* ÿ Ÿ ⁄ € ‹ › ﬁ ﬂ */	FRM_FONT_0108,FRM_FONT_0109,FRM_FONT_0110,FRM_FONT_0111,FRM_FONT_0112,FRM_FONT_0113,FRM_FONT_0114,FRM_FONT_0115,
	/* ‡ · ‚ „ ‰ Â Ê Á */	FRM_FONT_0116,FRM_FONT_0117,FRM_FONT_0118,FRM_FONT_0119,FRM_FONT_0120,FRM_FONT_0121,FRM_FONT_0122,FRM_FONT_0123,
	/* Ë È Í Î Ï Ì Ó Ô */	FRM_FONT_0124,FRM_FONT_0125,FRM_FONT_0126,FRM_FONT_0127,FRM_FONT_0128,FRM_FONT_0129,FRM_FONT_0130,FRM_FONT_0131,
	/*  Ò Ú Û Ù ı ˆ ˜ */	FRM_FONT_0132,FRM_FONT_0133,FRM_FONT_0134,FRM_FONT_0135,FRM_FONT_0136,FRM_FONT_0137,FRM_FONT_0138,FRM_FONT_0139,
	/* ¯ ˘ ˙ ˚ ¸ ˝ ˛ ˇ */	FRM_FONT_0140,FRM_FONT_0141,FRM_FONT_0142,FRM_FONT_0143,FRM_FONT_0144,FRM_FONT_0145,FRM_FONT_0146,FRM_FONT_0147,
};
FontData	standardFont( UI_UIGFX_SPR,	standardFontTab,	9,	1,1, 4 );

s16	largeFontTab[]=
{
							           -1,           -1,           -1,           -1,           -1,           -1,           -1,           -1,
							           -1,           -1,           -1,           -1,           -1,           -1,           -1,           -1,
							           -1,           -1,           -1,           -1,           -1,           -1,           -1,           -1,
							           -1,           -1,           -1,           -1,           -1,           -1,           -1,           -1,
	/*   ! " # $ % & ' */	           -1,FRM_SBFONTAN_0037,FRM_SBFONTAN_0038,           -1,FRM_SBFONTAN_0052,FRM_SBFONTAN_0039,FRM_SBFONTAN_0040,FRM_SBFONTAN_0036,
	/* ( ) * + , - . / */	FRM_SBFONTAN_0042,FRM_SBFONTAN_0043,FRM_SBFONTAN_0041,               -1,FRM_SBFONTAN_0047,FRM_SBFONTAN_0044,FRM_SBFONTAN_0053,FRM_SBFONTAN_0050,
	/* 0 1 2 3 4 5 6 7 */	FRM_SBFONTAN_0026,FRM_SBFONTAN_0027,FRM_SBFONTAN_0028,FRM_SBFONTAN_0029,FRM_SBFONTAN_0030,FRM_SBFONTAN_0031,FRM_SBFONTAN_0032,FRM_SBFONTAN_0033,
	/* 8 9 : ; < = > ? */	FRM_SBFONTAN_0034,FRM_SBFONTAN_0035,FRM_SBFONTAN_0045,FRM_SBFONTAN_0046,               -1,               -1,               -1,FRM_SBFONTAN_0048,
	/* @ A B C D E F G */	           -1,FRM_SBFONTAN_0000,FRM_SBFONTAN_0001,FRM_SBFONTAN_0002,FRM_SBFONTAN_0003,FRM_SBFONTAN_0004,FRM_SBFONTAN_0005,FRM_SBFONTAN_0006,
	/* H I J K L M N O */	FRM_SBFONTAN_0007,FRM_SBFONTAN_0008,FRM_SBFONTAN_0009,FRM_SBFONTAN_0010,FRM_SBFONTAN_0011,FRM_SBFONTAN_0012,FRM_SBFONTAN_0013,FRM_SBFONTAN_0014,
	/* P Q R S T U V W */	FRM_SBFONTAN_0015,FRM_SBFONTAN_0016,FRM_SBFONTAN_0017,FRM_SBFONTAN_0018,FRM_SBFONTAN_0019,FRM_SBFONTAN_0020,FRM_SBFONTAN_0021,FRM_SBFONTAN_0022,
	/* X Y Z [ \ ] ^ _ */	FRM_SBFONTAN_0023,FRM_SBFONTAN_0024,FRM_SBFONTAN_0025,FRM_SBFONTAN_0054,FRM_SBFONTAN_0049,FRM_SBFONTAN_0055,           -1,               -1,
	/* ` a b c d e f g */	           -1,FRM_SBFONTAN_0056,FRM_SBFONTAN_0057,FRM_SBFONTAN_0058,FRM_SBFONTAN_0059,FRM_SBFONTAN_0060,FRM_SBFONTAN_0061,FRM_SBFONTAN_0062,
	/* h i j k l m n o */	FRM_SBFONTAN_0063,FRM_SBFONTAN_0064,FRM_SBFONTAN_0065,FRM_SBFONTAN_0066,FRM_SBFONTAN_0067,FRM_SBFONTAN_0068,FRM_SBFONTAN_0069,FRM_SBFONTAN_0070,
	/* p q r s t u v w */	FRM_SBFONTAN_0071,FRM_SBFONTAN_0072,FRM_SBFONTAN_0073,FRM_SBFONTAN_0074,FRM_SBFONTAN_0075,FRM_SBFONTAN_0076,FRM_SBFONTAN_0077,FRM_SBFONTAN_0078,
	/* x y z { | } ~ . */	FRM_SBFONTAN_0079,FRM_SBFONTAN_0080,FRM_SBFONTAN_0081,           -1,           -1,           -1,           -1,           -1,
	/* Ä Å Ç É Ñ Ö Ü á */	           -1,           -1,           -1,           -1,           -1,           -1,           -1,           -1,
	/* à â ä ã å ç é è */	           -1,           -1,           -1,           -1,           -1,           -1,           -1,           -1,
	/* ê ë í ì î ï ñ ó */	           -1,           -1,           -1,           -1,           -1,           -1,           -1,           -1,
	/* ò ô ö õ ú ù û ü */	           -1,           -1,           -1,           -1,           -1,           -1,           -1,           -1
	/* † ° ¢ £ § • ¶ ß */	           -1,           -1,           -1,           -1,           -1,           -1,           -1,           -1,
	/* ® © ™ ´ ¨ ≠ Æ Ø */	           -1,           -1,           -1,           -1,           -1,           -1,           -1,           -1,
	/* ∞ ± ≤ ≥ ¥ µ ∂ ∑ */	           -1,           -1,           -1,           -1,           -1,           -1,           -1,           -1,
	/* ∏ π ∫ ª º Ω æ ø */	           -1,           -1,           -1,           -1,           -1,           -1,           -1,           -1,
							-1,-1,
	/* ¿ ¡ ¬ √ ƒ ≈ ∆ « */	FRM_SBFONTAN_0084,FRM_SBFONTAN_0085,FRM_SBFONTAN_0086,FRM_SBFONTAN_0087,FRM_SBFONTAN_0088,FRM_SBFONTAN_0089,FRM_SBFONTAN_0090,FRM_SBFONTAN_0091,
	/* » …   À Ã Õ Œ œ */	FRM_SBFONTAN_0092,FRM_SBFONTAN_0093,FRM_SBFONTAN_0094,FRM_SBFONTAN_0095,FRM_SBFONTAN_0096,FRM_SBFONTAN_0097,FRM_SBFONTAN_0098,FRM_SBFONTAN_0099,
	/* – — “ ” ‘ ’ ÷ ◊ */	FRM_SBFONTAN_0100,FRM_SBFONTAN_0101,FRM_SBFONTAN_0102,FRM_SBFONTAN_0103,FRM_SBFONTAN_0104,FRM_SBFONTAN_0105,FRM_SBFONTAN_0106,FRM_SBFONTAN_0107,
	/* ÿ Ÿ ⁄ € ‹ › ﬁ ﬂ */	FRM_SBFONTAN_0108,FRM_SBFONTAN_0109,FRM_SBFONTAN_0110,FRM_SBFONTAN_0111,FRM_SBFONTAN_0112,FRM_SBFONTAN_0113,FRM_SBFONTAN_0114,FRM_SBFONTAN_0115,
	/* ‡ · ‚ „ ‰ Â Ê Á */	FRM_SBFONTAN_0116,FRM_SBFONTAN_0117,FRM_SBFONTAN_0118,FRM_SBFONTAN_0119,FRM_SBFONTAN_0120,FRM_SBFONTAN_0121,FRM_SBFONTAN_0122,FRM_SBFONTAN_0123,
	/* Ë È Í Î Ï Ì Ó Ô */	FRM_SBFONTAN_0124,FRM_SBFONTAN_0125,FRM_SBFONTAN_0126,FRM_SBFONTAN_0127,FRM_SBFONTAN_0128,FRM_SBFONTAN_0129,FRM_SBFONTAN_0130,FRM_SBFONTAN_0131,
	/*  Ò Ú Û Ù ı ˆ ˜ */	FRM_SBFONTAN_0132,FRM_SBFONTAN_0133,               -1,               -1,               -1,               -1,               -1,               -1,
	/* ¯ ˘ ˙ ˚ ¸ ˝ ˛ ˇ */	               -1,               -1,               -1,               -1,               -1,               -1,               -1,               -1,
};
FontData	largeFont( UI_UIGFX_SPR,	largeFontTab,	19,	1,1, 6 );


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