/*
 * $PSLibId: Run-time Library Release 4.7$
 */
#ifndef _MCGUI_H_
#define _MCGUI_H_


#define NEGICON_A	(0x20)
#define NEGICON_B	(0x10)

#define MOUSE_LBUTTON	(0x08)
#define MOUSE_RBUTTON	(0x04)

#define MCGUI_INTERNAL_FONT	(0)
#define MCGUI_EXTERNAL_FONT	(1)

/* Texture Information Structure */

typedef struct {
	unsigned long* addr;
} sMcGuiTexture;


/* Memory Card Information Structure */

typedef struct {
	char file[21];
	char title[65];
	char frame;
	char block;
	long dataBytes;
	unsigned long* iconAddr;
	unsigned long* dataAddr;
} sMcGuiCards;

/* BG Information Structure */

typedef struct {
	short mode;
	signed char scrollDirect;	/* 0:Up 1:Up&Left 2:Left 3:Down&left 4:Down ...*/
	signed char scrollSpeed;	/* 0:no scroll 1:1/60 2:1/30 3:1/20 */
	unsigned long* timadr;
} sMcGuiBg;

/* Cursor Information Structure */

typedef struct {
	char type;
	unsigned char r;
	unsigned char g;
	unsigned char b;
} sMcGuiCursor;

/* BGM,Sound Effects Information Structure */

typedef struct {
	int MVOL;
	struct {
		int isbgm;
		unsigned long* seq;
		unsigned long* vh;
		unsigned long* vb;
		int SVOL;
		int isReverb;
		int reverbType;
		int reverbDepth;
	} bgm;
	struct {
		int isse;
		unsigned long* vh;
		unsigned long* vb;
		int vol;
		int prog;
		int TONE_OK;
		int TONE_CANCEL;
		int TONE_CURSOR;
		int TONE_ERROR;
	} se;
} sMcGuiSnd;

/* Controller Related Information Structure */

typedef struct {
	volatile unsigned char* buf[2];
	struct {
		int flag;
		unsigned long BUTTON_OK;
		unsigned long BUTTON_CANCEL;
	} type1;
	struct {
		int flag;
		unsigned long BUTTON_OK;
		unsigned long BUTTON_CANCEL;
	} type2;
	struct {
		int flag;
		unsigned long BUTTON_OK;
		unsigned long BUTTON_CANCEL;
	} type3;
	struct {
		int flag;
		unsigned long BUTTON_OK;
		unsigned long BUTTON_CANCEL;
	} type4;
} sMcGuiController;


/* Memory Card Screen Configuration Structure */

typedef struct {
	sMcGuiCards cards;		/* Memory Card Information */
	sMcGuiBg bg;			/* BG Information */
	sMcGuiController controller;	/* Controller Related Information */
	sMcGuiSnd sound;		/* BGM Sound Effects Information */
	sMcGuiTexture texture;		/* Texture Information */
	sMcGuiCursor cursor;		/* Cursor Information */
} McGuiEnv;

#if defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
extern "C" {
#endif

extern int McGuiSave(McGuiEnv* env);
extern int McGuiLoad(McGuiEnv* env);

int McGuiSetExternalFont(McGuiEnv* env, int mode);

#if defined(_LANGUAGE_C_PLUS_PLUS)||defined(__cplusplus)||defined(c_plusplus)
}
#endif

#endif /* _MCGUI_H_ */
