/*****************/
/*** Pad Stuff ***/
/*****************/

#ifndef __PAD_PADS_H__
#define __PAD_PADS_H__

#ifndef _GLOBAL_HEADER_
#include "system\global.h"
#endif


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
	u16		Up,Held,Down,Old,Repeat;
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
	bool	CanVibrate;
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

void		SetPadRescan();
sPadData 	*PadGet(int Port);

u16 		PadGetUp(int Port);
u16 		PadGetDown(int Port);
u16 		PadGetHeld(int Port);
u16			PadGetRepeat(int Port);


int 		PadIsDualShock(int Port);
int			PadCanVibrate(int Port);

u16			PadGetPadAngle(u16 Pad, s16 angleAdjust);

bool		PadIsConnected(int port);

void		PadSetVibrationIsTurnedOn(int _port,int _status);
int			PadGetVibrationIsTurnedOn(int _port);

/*****************************************************************************/


class	CPadConfig
{
public:
	enum
	{
		NUM_PAD_CONFIGS=3,
	};

	enum	PAD_CFG
	{
		PAD_CFG_NONE=0,

		PAD_CFG_LEFT,
		PAD_CFG_RIGHT,
		PAD_CFG_UP,
		PAD_CFG_DOWN,

		PAD_CFG_FIRE,
		PAD_CFG_CATCH,
		PAD_CFG_JUMP,
		PAD_CFG_WEAPONCHANGE,

		PAD_CFG_MAX
	};

	static void				setConfig(int _config);
	static int				getConfig();
	static int				getButton(PAD_CFG _but);

private:
	struct	sPadConfigTable
	{
		u16	m_buttons[PAD_CFG_MAX];
	};

	static int				s_configNumber;
	static sPadConfigTable	*s_cfg;
	static sPadConfigTable	s_padConfigs[NUM_PAD_CONFIGS];

};


/*****************************************************************************/

#endif