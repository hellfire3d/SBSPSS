/***********************/
/*** Map Editor Core ***/
/***********************/

#ifndef	__CORE_HEADER__
#define	__CORE_HEADER__

#include	<Vector>


/*****************************************************************************/
//struct	sLayer;
struct	sLayer
{
	char	Name[32];
//	sLayer	*Next,*Prev;
};

/*****************************************************************************/
class	CCore
{
public:
		CCore();
		~CCore();

// Layers
		void			LayerAdd(char *Name=0);
		void			LayerSetActive(int Layer);
		int				LayerGetActive();
		int				LayerGetCount();
		sLayer	const	&LayerGet(int i);
		void			LayerDelete(int Layer);
		void			LayerMoveUp(int Layer);
		void			LayerMoveDown(int Layer);



private:

	std::vector<sLayer>	Layers;
	int					ActiveLayer;
};

/*****************************************************************************/
#endif
