/*********************/
/*** PSX Bootstrap ***/
/*** Dave 190697   ***/
/*********************/

#ifndef PSXBootHeader
#define PSXBootHeader

/*****************************************************************************/
typedef	unsigned char	byte;
typedef	unsigned int	word;

/*****************************************************************************/
// Screen position and dimensions. 
#define	FRAME_X			512

#ifdef	__TERRITORY_USA__
#define	FRAME_Y			240
#define	SCREEN_X		0 //-24
#define	SCREEN_Y		-8 
#else
#define	FRAME_Y			256
#define	SCREEN_X		0 //-8
#define	SCREEN_Y		8
#endif // NTSC

/*---------------------------------------------------------------------------------------------------------*/
#define         LoadBuffer		0x80010000              // psx boot
#define         BinLoadAddr		0x80010000              // psx boot

/*****************************************************************************/
typedef struct 
{
	DRAWENV		draw;
	DISPENV		disp;
} 	DB;

#endif
