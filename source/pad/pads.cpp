/*****************/
/*** Pad Stuff ***/
/*****************/

#include	<LibPad.H>
#include	"system/global.h"
#include	"pad/pads.H"

#ifndef __SYSTEM_GSTATE_H__
#include "system\gstate.h"
#endif

#ifndef __PAD_VIBE_H__
#include "pad\vibe.h"
#endif


/*****************************************************************************/
// Yay! Automatic key-repeat stuff!
#define TYPOMATIC_DELAY	30
#define TYPOMATIC_RATE	15


/*****************************************************************************/
/*****************************************************************************/
sPadData	PadData[2];
u8 			PadBuffer[2][34];
u8 			PadAlign[6]={0,1,0xFF,0xFF,0xFF,0xFF};
u8			PadMotor[2][2];
int			PadRepeatTimers[2][16];
int			PadRescan=0;

typedef struct
{
	int	m_vibrationTurnedOn;
	u8	m_intensityValue;
} PadVibeData;
#if		defined(__USER_CDBUILD__ )
static PadVibeData	s_padVibeData[2]={{true,0},{true,0}};
#else
static PadVibeData	s_padVibeData[2]={{false,0},{false,0}};
#endif
int					PadVibrationActive[2]={false,false};
u8					PadVibrationIntensityValues[2];

/*****************************************************************************/
// 701
// 6 2
// 543
u16		PADAngeDirTable[16]=
{
0,				//  0
(ONE/8)*0,		//  1 U
(ONE/8)*2,		//  2 R
(ONE/8)*1,		//  3 UR
(ONE/8)*4,		//  4 D
0,				//  5 UD !!
(ONE/8)*3,		//  6 DR
0,				//  7 DUR !!
(ONE/8)*6,		//  8 L
(ONE/8)*7,		//  9 UL
0,				// 10 LR !!
0,				// 11 LUR !!
(ONE/8)*5,		// 12 DL
0,				// 13 LUD
0,				// 14 LDR
0,				// 15 LDUR
};

/*****************************************************************************/

void VibrationHandler(int _port);

/*****************************************************************************/

void	PadInitShock(int Port)
{
int 	offs,maxID,set;

	if(!PadData[Port].Active)return;

// ReInit Dual Shock if required
	VSync(20);
	maxID = PadInfoMode(0x00,InfoModeIdTable,-1);
	VSync(5);
	for(offs=0;offs<maxID;offs++)
		{
		if( PadInfoMode(0x00,InfoModeIdTable,offs) ==7)
			{
			VSync(6);
			PadSetMainMode(0x00,offs,2);
			break;
			}
		}

// Wait till it's stable
	set = 0;
	while(!set)
		{
		if(PadGetState(Port<<4) == PadStateStable)
			{
			if(PadInfoMode(Port,InfoModeCurExID,0))
				{
				PadSetAct(0x00,&PadMotor[Port][0],2);
				VSync(20);
				while( PadSetActAlign(0x00,PadAlign) == 0)
					{
					for(int ctr=0;ctr<6;ctr++)
						VSync(0);
					}
				}
			set = 1;
			}
		if(PadGetState(Port<<4) == PadStateFindCTP1)
			{
			set = 1;
			}
		}

}

/*****************************************************************************/
sPadData	*PadGet(int Port)
{
	return(&PadData[Port]);
}

/*****************************************************************************/

void	PadsInit()
{
	PadClear(0); 
	PadClear(1);
	PadInitDirect(PadBuffer[0],PadBuffer[1]);
	PadStartCom();
	PadInitShock(0);
	PadInitShock(1);

	for(int i=0;i<2;i++)
		for(int j=0;j<16;j++)
			PadRepeatTimers[i][j]=0;

	CPadConfig::setConfig(0);		
}

/*****************************************************************************/
u16		TranslatePadVals(u16 PadVal)
{
u16 RetVal;

	RetVal  = PadVal&PADLup ? 		PAD_UP			: 0;
	RetVal |= PadVal&PADLdown ? 	PAD_DOWN		: 0;
	RetVal |= PadVal&PADLleft ? 	PAD_LEFT		: 0;
	RetVal |= PadVal&PADLright ?	PAD_RIGHT		: 0;
	RetVal |= PadVal&PADstart ?		PAD_START		: 0;
	RetVal |= PadVal&PADselect ?	PAD_SELECT		: 0;
	RetVal |= PadVal&PADRdown ?		PAD_CROSS		: 0;
	RetVal |= PadVal&PADRleft ?		PAD_SQUARE		: 0;
	RetVal |= PadVal&PADRright ?	PAD_CIRCLE		: 0;
	RetVal |= PadVal&PADRup ?		PAD_TRIANGLE	: 0;
	RetVal |= PadVal&PADL1 ? 		PAD_L1 : 0;
	RetVal |= PadVal&PADL2 ? 		PAD_L2 : 0;
	RetVal |= PadVal&PADR1 ? 		PAD_R1 : 0;
	RetVal |= PadVal&PADR2 ? 		PAD_R2 : 0;
	return(RetVal);
}

/*****************************************************************************/
void	PadSetVals(sPadData *Pad,u16 Val)
{
u16 Held,Old;
	Val=TranslatePadVals(Val);

	Old=Pad->Old=Pad->Held;
	Held=Pad->Held=Val;
	Pad->Down=(Held^Old)&Held;
	Pad->Up=(Held^0xffff)&Old;
}


/*****************************************************************************/
const s16	AnalogThresh=64;
void 	Pad2Digital(sPadData *Pad)
{
u16		Button;

// Buttons
		Button = ~((Pad->Button1<<8) | Pad->Button2);
// Motion
		if (Pad->IsAnalogue)
			{
s16		XOfs=Pad->Analog2-127;
s16		YOfs=Pad->Analog3-127;

			if (XOfs<-AnalogThresh) Button|=PAD_LEFT;
			else
			if (XOfs>+AnalogThresh) Button|=PAD_RIGHT;
			if (YOfs<-AnalogThresh) Button|=PAD_UP;
			else
			if (YOfs>+AnalogThresh) Button|=PAD_DOWN;
			}
		PadSetVals(Pad,Button);
}

/*****************************************************************************/
int 	PadIsDualShock(int Port)
{
	return (PadData[Port].IsAnalogue==2);
}

/*****************************************************************************/
int 	PadCanVibrate(int Port)
{
	return PadIsConnected(Port)&&PadData[Port].CanVibrate;
}

/*****************************************************************************/
void	SetPadRescan()
{
		PadRescan=1;
}

/*****************************************************************************/
void	ReadController(int Port)
{
u8			*PadBuf=&PadBuffer[Port][0];
sPadData	*Pad=&PadData[Port];
int			PortShift=Port<<4;
int			intensity;

	Pad->IsAnalogue=0;
	Pad->Status=PadGetState(PortShift);

	if ((Pad->Status==PadStateDiscon) || PadBuf[0])
		{
		Pad->Active=0;
		Pad->Type=0;
		return;
		}
	Pad->Active=1;
	Pad->Type = PadBuf[1]>>4;
	Pad->Button1 = PadBuf[2];
	Pad->Button2 = PadBuf[3];
	Pad->Analog0 = PadBuf[4];
	Pad->Analog1 = PadBuf[5];
	Pad->Analog2 = PadBuf[6];
	Pad->Analog3 = PadBuf[7];

	switch( Pad->Type )
		{
		case PsxPadTypeNone:
			break;

		case PsxPadTypeMouse:			// Sony Mouse
			Pad->Dx = Pad->Analog0;
			Pad->Dy = Pad->Analog1;
			if ( Pad->Dx & 0x80) Pad->Dx|= 0xffffff80;
			if ( Pad->Dy & 0x80) Pad->Dy|= 0xffffff80;
			break;

		case PsxPadTypeNegiCon: 		// Namco negCon
										// Steering wheel
										// Sankyo Pachinko controler
			break;

		case PsxPadTypeKonamiGun:		// Light Gun
			break;

		case PsxPadTypeStandard:		// Standard Sony PAD controller
			intensity=s_padVibeData[Port].m_intensityValue;
			Pad->Motor0=intensity&1;
			Pad->Motor1=intensity&0xff;
			Pad2Digital(Pad);
			break;

		case PsxPadTypeAnalogStick:
		case PsxPadTypeAnalogController:// Analog 2-stick

			if (PadInfoMode(0,InfoModeCurExID,0)==7)
				Pad->IsAnalogue=2;
			else
				Pad->IsAnalogue=1;

			// Set vibration values ( motor 0 is 0..1, motor 1 is 0..255 )
			intensity=s_padVibeData[Port].m_intensityValue;
			Pad->Motor0=intensity&1;
			Pad->Motor1=intensity&0xff;

			Pad2Digital(Pad);
			break;

		case PsxPadTypeNamcoGun:
			break;

		default:
			break;
		}

	if (Pad->Status == PadStateFindPad)
	{
		Pad->Send = 0;
		Pad->CanVibrate=false;
	}
	if ( Pad->Send==0 || PadRescan)
		{
		PadRescan=0;
		PadSetAct(PortShift,&(Pad->Motor0),2);

		if (Pad->Status == PadStateFindCTP1)
		{
			Pad->Send = 1;
		}
		if (Pad->Status == PadStateStable)
			{
			if (PadSetActAlign(PortShift,PadAlign))
				{
				Pad->Send = 1;
				Pad->CanVibrate=true;
				}
			}
		}

}

/*****************************************************************************/
u16		PadGetUp(int Port)
{
	return(PadData[Port].Up);
}

/*****************************************************************************/
u16		PadGetDown(int Port)
{
	return(PadData[Port].Down);
}

/*****************************************************************************/
u16 	PadGetHeld(int Port)
{
	return(PadData[Port].Held);
}

/*****************************************************************************/
u16 	PadGetRepeat(int Port)
{
	return(PadData[Port].Repeat);
}

/*****************************************************************************/
void 	PadClear(int Port)
{
	PadData[Port].Up=PadData[Port].Down=PadData[Port].Held=PadData[Port].Old=0;
	PadData[Port].Dx=PadData[Port].Dy=0;
}


/*****************************************************************************/

void UpdateRepeats(int _port)
{
	int	frames,pad,i,mask,*repeatTimers;
	u16 *repeatFlags;


	frames=GameState::getFramesSinceLast();
	pad=PadGetHeld(_port);
	mask=1;
	repeatTimers=PadRepeatTimers[_port];
	repeatFlags=&PadData[_port].Repeat;
	*repeatFlags=0;
	for(i=0;i<16;i++)
	{
		if(pad&mask)
		{
			if(*repeatTimers)
			{
				*repeatTimers-=frames;
				if(*repeatTimers<=0)
				{
					*repeatFlags|=mask;
					*repeatTimers=TYPOMATIC_RATE;
				}
			}				
			else
			{
				*repeatFlags|=mask;
				*repeatTimers=TYPOMATIC_DELAY;
			}
		}
		else
		{
			*repeatTimers=0;
		}
		mask<<=1;
		repeatTimers++;
	}
}


void 	PadUpdate()
{
		PadData[0].Dx= PadData[1].Dx=0;
		PadData[0].Dy= PadData[1].Dy=0;
		PadData[0].Dx1=PadData[1].Dx1=0;
		PadData[0].Dy1=PadData[1].Dy1=0;

		ReadController(0);
		ReadController(1);

		UpdateRepeats(0);
		UpdateRepeats(1);

		VibrationHandler(0);
		VibrationHandler(1);
}


/*****************************************************************************/
u16		PadGetPadAngle(u16 Pad, s16 angleAdjust)
{
	return((PADAngeDirTable[Pad>>PAD_DIR_SHIFT]+((ONE/4)*2)+angleAdjust)&(ONE-1));
}


/*****************************************************************************/
bool	PadIsConnected(int port)
{
	sPadData	*pad=&PadData[port];
	
	return pad->Status!=PadStateDiscon&&pad->Type;
}

/*****************************************************************************/
void		PadSetVibrationIsTurnedOn(int _port,int _status)
{
	s_padVibeData[_port].m_vibrationTurnedOn=_status;
}
int			PadGetVibrationIsTurnedOn(int _port)
{
	return s_padVibeData[_port].m_vibrationTurnedOn;
}


/*----------------------------------------------------------------------
	Function:	VibrationHandler
	Purpose:	
	Params:		
	Returns:	
  ---------------------------------------------------------------------- */
void VibrationHandler(int _port)
{
	PadVibeData	*pvd;
	int			i;

	pvd=s_padVibeData;
	for(i=0;i<2;i++)
	{
		pvd->m_intensityValue=pvd->m_vibrationTurnedOn?CPadVibrationManager::getCurrentVibrationIntensity(i):0;
		pvd++;
	}
}







/*****************************************************************************/
int							CPadConfig::s_configNumber=-1;
CPadConfig::sPadConfigTable	*CPadConfig::s_cfg=NULL;
CPadConfig::sPadConfigTable	CPadConfig::s_padConfigs[NUM_PAD_CONFIGS]=
{
	// Config A
	{{
		0,					// PAD_CFG_NONE
		PAD_LEFT,			// PAD_CFG_LEFT
		PAD_RIGHT,			// PAD_CFG_RIGHT
		PAD_UP,				// PAD_CFG_UP
		PAD_DOWN,			// PAD_CFG_DOWN

		PAD_SQUARE,			// PAD_CFG_FIRE
		PAD_CIRCLE,			// PAD_CFG_CATCH
		PAD_CROSS,			// PAD_CFG_JUMP
		PAD_TRIANGLE,		// PAD_CFG_WEAPONCHANGE
	}},
	// Config B
	{{
		0,					// PAD_CFG_NONE
		PAD_LEFT,			// PAD_CFG_LEFT
		PAD_RIGHT,			// PAD_CFG_RIGHT
		PAD_UP,				// PAD_CFG_UP
		PAD_DOWN,			// PAD_CFG_DOWN

		PAD_CROSS,			// PAD_CFG_FIRE
		PAD_TRIANGLE,		// PAD_CFG_CATCH
		PAD_SQUARE,			// PAD_CFG_JUMP
		PAD_CIRCLE,			// PAD_CFG_WEAPONCHANGE
	}},
	// Config C
	{{
		0,					// PAD_CFG_NONE
		PAD_LEFT,			// PAD_CFG_LEFT
		PAD_RIGHT,			// PAD_CFG_RIGHT
		PAD_UP,				// PAD_CFG_UP
		PAD_DOWN,			// PAD_CFG_DOWN

		PAD_SQUARE,			// PAD_CFG_FIRE
		PAD_TRIANGLE,		// PAD_CFG_CATCH
		PAD_CROSS,			// PAD_CFG_JUMP
		PAD_CIRCLE,			// PAD_CFG_WEAPONCHANGE
	}},
};




void CPadConfig::setConfig(int _config)
{
	ASSERT(_config<NUM_PAD_CONFIGS);
	s_configNumber=_config;
	s_cfg=&s_padConfigs[_config];
}

int CPadConfig::getConfig()
{
	return s_configNumber;
}

int CPadConfig::getButton(PAD_CFG _but)
{
	return(s_cfg->m_buttons[_but]);
}
