/***********************/
/*** Selection Class ***/
/***********************/

#ifndef	__SELECTION_CLASS_HEADER__
#define	__SELECTION_CLASS_HEADER__


/*****************************************************************************/
class CSelect  
{
public:
		CSelect();
		virtual ~CSelect(){};

		void	Invalidate();
		bool	Handle(CPoint &Pnt,UINT Flags);
		bool	IsValid();
		bool	HasSelection();

		CRect	&GetRect()		{return(Sel);}

private:
		void	Start(CPoint &Pnt, UINT Flags);
		void	End(CPoint &Pnt, UINT Flags);

		CPoint	Pos,Pos0;
		CRect	Sel;
		UINT	Mode;

};

/*****************************************************************************/
#endif
