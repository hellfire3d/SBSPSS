/******************/
/*** PSX Timer  ***/
/******************/

#ifndef __SYSTEM_CLICKCOUNT_H__
#define __SYSTEM_CLICKCOUNT_H__

/*****************************************************************************/
class CClickCount
{
private:
	u32		m_lastTime;

	static	u32		s_currentTime;
	static	bool	s_initialised;
	static	bool	s_paused;

	static	u32		getCurrentTime();

	friend void		clockTicker();

public:
			CClickCount(){};

	u32		timeSinceLast();

	static	void	initialise();
	static	void	pauseClickCount();
	static	void	restartClickCount();
	static	void	updateCurrentTime();
};

/*****************************************************************************/

#endif
