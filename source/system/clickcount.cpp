/******************/
/*** PSX Timer  ***/
/******************/


#include <libapi.h>

#include "system\global.h"
#include "system\clickcount.h"
#include "system/gp.h"


/*****************************************************************************/
u32		CClickCount::s_currentTime=0;
bool	CClickCount::s_initialised=false;
bool	CClickCount::s_paused=false;

static const int COUNT_DOWN_VAL = 17200;
static const int COUNTS_PER_FRAME_INTERNAL = 2;
static const int COUNTS_PER_FRAME_EXTERNAL = 4096;


/*****************************************************************************/
void clockTicker()
{
	u32	thisGp;
	thisGp=ReloadGP();

	CClickCount::updateCurrentTime();
	SetGP(thisGp);
}

/*****************************************************************************/
u32 CClickCount::timeSinceLast()
{
	if (!s_initialised)
		{
		initialise();
		m_lastTime=getCurrentTime();
		s_initialised=true;
		}

	u32		timeSince;
	u32		currentTime;
	u32		lastTime;

	lastTime=m_lastTime;
	currentTime=getCurrentTime();
	timeSince=currentTime-m_lastTime;

	m_lastTime=currentTime;

	return((timeSince*COUNTS_PER_FRAME_EXTERNAL)/COUNTS_PER_FRAME_INTERNAL);
}

/*****************************************************************************/
void CClickCount::initialise()
{
	unsigned long eventHandle;

	// set up variables and environment

	EnterCriticalSection();
	eventHandle = OpenEvent( RCntCNT2, EvSpINT, EvMdINTR,(long (*)(...)) clockTicker);
	EnableEvent( eventHandle );
//	SetRCnt( RCntCNT2, COUNT_DOWN_VAL, RCntMdINTR|RCntMdSP);
	SetRCnt( RCntCNT2, COUNT_DOWN_VAL, RCntMdINTR);
	StartRCnt( RCntCNT2 );
	ExitCriticalSection();
}

/*****************************************************************************/
u32 CClickCount::getCurrentTime()
{
	return(s_currentTime);
}

/*****************************************************************************/
void CClickCount::pauseClickCount()
{
	ASSERT(!s_paused);
	s_paused = true;
}

/*****************************************************************************/
void CClickCount::restartClickCount()
{
	ASSERT(s_paused);
	s_paused = false;
}

/*****************************************************************************/
void CClickCount::updateCurrentTime()
{
	if (!s_paused)
	{
		s_currentTime++;
	}

}

