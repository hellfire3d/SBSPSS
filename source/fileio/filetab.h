/******************/
/*** File Table ***/
/******************/

#ifndef	__FILE_TABLE___
#define	__FILE_TABLE___

/*******************************************************************************/
enum	FILEPOS_ENUM
{
	FILEPOS_BIGLUMP,
	FILEPOS_TRACK1,
	FILEPOS_THQ_STR,
	FILEPOS_CLIMAX_STR,
	FILEPOS_INTRO_STR,
#if defined(__TERRITORY_EUR__)
	FILEPOS_DEMO_STR,
#endif
//	FILEPOS_LEVEL1_STR,
//	FILEPOS_LEVEL2_STR,
//	FILEPOS_LEVEL3_STR,
//	FILEPOS_LEVEL4_STR,
//	FILEPOS_LEVEL5_STR,
//	FILEPOS_LEVEL6_STR,
//	FILEPOS_LEVEL7_STR,
//	FILEPOS_ZORD1_STR,
//	FILEPOS_ZORD2_STR,
/*
	FILEPOS_DEMO2_STR,
	FILEPOS_DEMO3_STR,
	FILEPOS_DEMO4_STR,
	FILEPOS_DEMO5_STR,
	FILEPOS_DEMO6_STR,
*/
	FILEPOS_MAX
};

/*******************************************************************************/

#endif