/*******************/
/*** Level Class ***/
/*******************/

#ifndef __LEVEL_LEVEL_H__
#define __LEVEL_LEVEL_H__


/*****************************************************************************/
class CLevel
{
public:
			CLevel(){}
	virtual ~CLevel(){}


	void	init();
	void	shutdown();
	void	render();
	void	think(int _frames);
	
private:
	sLvlHdr	*LvlData;
};

/*****************************************************************************/

#endif