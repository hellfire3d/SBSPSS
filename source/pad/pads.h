/*****************/
/*** Pad Stuff ***/
/*****************/

#ifndef __PADS_H__
#define __PADS_H__

/*****************************************************************************/
#define DUALSHOCKMODE ((u_long *)0x80010000)

/*****************************************************************************/
#define	PsxPadTypeNone				0
#define	PsxPadTypeMouse				1
#define	PsxPadTypeNegiCon 			2
#define	PsxPadTypeKonamiGun			3
#define	PsxPadTypeStandard			4
#define	PsxPadTypeAnalogStick		5
#define	PsxPadTypeNamcoGun 			6
#define	PsxPadTypeAnalogController	7

#define PAD_UP			PADLup
#define PAD_DOWN		PADLdown
#define PAD_LEFT		PADLleft
#define PAD_RIGHT		PADLright
#define PAD_SELECT		PADselect
#define PAD_START		PADstart
#define PAD_CROSS		PADRdown
#define PAD_SQUARE		PADRleft
#define PAD_CIRCLE		PADRright
#define PAD_TRIANGLE	PADRup
#define PAD_L1			PADL1
#define PAD_L2			PADL2
#define PAD_R1			PADR1
#define PAD_R2			PADR2

#define	PAD_BUTTONS 	(PAD_CROSS|PAD_SQUARE|PAD_CIRCLE|PAD_TRIANGLE|PAD_L1|PAD_L2|PAD_R1|PAD_R2)
#define	PAD_ALL_DIRS 	(PAD_UP|PAD_DOWN|PAD_LEFT|PAD_RIGHT)
#define	PAD_DIR_SHIFT 	12

#define	BUTTON_SELECT	PAD_CROSS
#define	BUTTON_BACK		PAD_TRIANGLE


/*****************************************************************************/
typedef struct
	{
	u16		Up,Held,Down,Old;
	int		XPos, YPos,	XPos1, YPos1;	// For analog
	int		Dx,Dy, Dx1,Dy1;
	u8		Status;			// These 8 values are obtained
	u8		Type;			// directly from the controller
	u8		Button1;		// buffer we installed with InitPAD.
	u8		Button2;
	u8		Analog0;
	u8		Analog1;
	u8		Analog2;
	u8		Analog3;
	u8		Motor0,Motor1;	// Analog
	u8		Send;
	u8		IsAnalogue;
	bool 	Active;
} sPadData;

struct VIBE_DATA
{
	int	frame;
	int	amplitude;
};

#define VIBE( a, b )	{ a, b }

/*****************************************************************************/
void		PadsInit();
void		PadClear(int Port);
void		PadUpdate();

sPadData 	*PadGet(int Port);

u16 		PadGetUp(int Port);
u16 		PadGetDown(int Port);
u16 		PadGetHeld(int Port);
u16 		PadGetTick(int Port);

int 		PadIsDualShock(int Port);

u16			PadGetPadAngle(u16 Pad, s16 angleAdjust);

bool		PadIsConnected(int port);

bool		PAD_IsRumbling( int port );
void		PAD_SetVibrationEnvelope( int Port, int count, const VIBE_DATA * data, int ferocity=255 );


/*****************************************************************************/
#define	PAD_ACCEPT		PAD_CROSS
#define	PAD_CANCEL		PAD_TRIANGLE
#define	PAD_UI_LEFT		PAD_LEFT
#define	PAD_UI_RIGHT	PAD_RIGHT
#define	PAD_UI_UP		PAD_UP
#define	PAD_UI_DOWN		PAD_DOWN

enum	PAD_CFG
{
	PAD_CFG_NONE=0,

	PAD_CFG_LEFT,
	PAD_CFG_RIGHT,
	PAD_CFG_UP,
	PAD_CFG_DOWN,

	PAD_CFG_START,
	PAD_CFG_SELECT,

	PAD_CFG_KICK,		// CIRCLE
	PAD_CFG_PUNCH,		// CROSS
	PAD_CFG_JUMP,		// SQUARE
	PAD_CFG_BLOCK,		// TRIANGLE
	PAD_CFG_RUN,		// L2
	PAD_CFG_SPECIAL,	// R2
	PAD_CFG_STEPLEFT,	// L1
	PAD_CFG_STEPRIGHT,	// R1

	PAD_CFG_MAX
};

struct	sPadConfigTable
{
	u16	Buttons[PAD_CFG_MAX];
};

class	CPadConfig
{
public:
	CPadConfig();
	void	SetConfig(sPadConfigTable *NewCfg);
	int		GetButton(PAD_CFG But);

private:
	sPadConfigTable	*Cfg;

};

extern sPadConfigTable *getPadConfigTable(int _padConfig);



/*****************************************************************************/

#endif