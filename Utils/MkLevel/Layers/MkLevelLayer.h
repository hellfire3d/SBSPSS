/******************/
/*** Layer Core ***/
/******************/

#ifndef	__MKLEVEL_LAYER_HEADER__
#define	__MKLEVEL_LAYER_HEADER__

#include	<Vector>

#include	"..\MkLevel.h"
#include	"..\..\MapEdit\LayerDef.h"
#include	"..\..\mapedit\ExportHdr.h"


/*****************************************************************************/
class	CMkLevelLayer
{
public:

virtual		void	PreProcess(CMkLevel *Core)=0;
virtual		void	Process(CMkLevel *Core)=0;
virtual		int		Write(CMkLevel *Core,FILE *File,const char *LayerName)=0;

			bool	IsType(int _Type,int _SubType)			{return(Type==_Type && SubType==_SubType);}

			void	SetSize(int S)	{Size=S;}
			int		GetSize()		{return(Size);}
			int		GetType()		{return(Type);}
			int		GetSubType()	{return(SubType);}
protected:
			int		Type;
			int		SubType;
			int		Width;
			int		Height;
			int		Size;
};


/*****************************************************************************/
#endif
