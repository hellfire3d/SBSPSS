/******************/
/*** Elem Stuph ***/
/******************/

#ifndef	__ELEM_HEADER__
#define	__ELEM_HEADER__

#include	"stdafx.h"
#include	<Vector3.h>
#include	<gl\gl.h>
#include	<gl\glu.h>
#include	<Vector>
#include	"TexCache.h"
#include	"GinTex.h"

struct	sTriFace
{
	int				Mat;
	Vector3			vtx[3];
	int				pts[3];
	sUV				uvs[3];
};


/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
class	CCore;
class	CElem
{
public:
		enum ElemType
		{
			ElemType2d=0,
			ElemType3d,
			ElemTypeMax
		};
		enum
		{
			UnitSize=16,
		};

		CElem(int Width,int Height);																	// Blank (2d)
		CElem(CCore *Core,const char *Filename,int TexId,int XOfs,int YOfs,int Width,int Height);		// 2d elem
		CElem(CCore *Core,const char *Filename,CScene &ThisScene,int Node);								// 3d elem
		void	CleanUp();

		void	Purge();
		void	Render(int Flags,bool Render3d);

		int		GetElemWidth()				{return(ElemWidth);}
		int		GetElemHeight()				{return(ElemHeight);}
		u8		*GetElemRGB()				{return(ElemRGB);}

		bool	IsValid()					{return(ValidFlag);}

		bool	IsElem3d()					{return(Type==ElemType3d);}
		int		GetTexXOfs()				{return(TexXOfs);}
		int		GetTexYOfs()				{return(TexYOfs);}

		void	CreateBlankTileGfx();
		void	CreateInvalidTileGfx();
		
		std::vector<sTriFace> 	&GetTriList() {return(TriList);}

protected:
		void	Build3dElem(CTexCache &TexCache,CScene &ThisScene,int Node);
		void	Build2dElem(CCore *Core,const char *Filename,int TexId);
		void	Build3dDrawList(CTexCache &TexCache,GLint &List);
		void	Build2dDrawList(CTexCache &TexCache,GLint &List);

		void	RenderElem4Texture(sRGBData &RGBData);
		void	Create2dTexture(CTexCache &TexCache,const char *Filename,int ID);
		bool	CheckHasData(sRGBData &RGBData);
		void	Calc3dSize();
		GString					SetPath;

		std::vector<sTriFace>	TriList;

		Vector3					Ofs;

		GLint					DrawList[ElemTypeMax];
		ElemType				Type;
		bool					ValidFlag;
		int						TexXOfs,TexYOfs;
		
		int						ElemWidth,ElemHeight;
		float					UnitWidth,UnitHeight;
		int						ElemID;
		u8						*ElemRGB;

};

/*****************************************************************************/
class	CElemSet
{
public:
		enum
		{
			DEF_ELEMWIDTH=16,
			DEF_ELEMHEIGHT=16,
			DEF_ELEMBROWSWEWIDTH=8,
		};

		CElemSet(const char *_Filename,int Idx,int Width=DEF_ELEMWIDTH,int Height=DEF_ELEMHEIGHT);
		~CElemSet();
	
		void		CleanUp();
		int			IsLoaded()										{return(Loaded);}
		int			GetCount()										{return(ElemList.size());}

		void		Load(CCore *Core);

const	char		*GetFilename()									{return(Filename);}
const	char		*GetName()										{return(Name);}
		int			GetBrowserWidth()								{return(ElemBrowserWidth);}

		void		RenderElem(int Elem,int Flags,bool Render3d)	{ElemList[Elem].Render(Flags,Render3d);}
		CElem		&GetElem(int No)								{return(ElemList[No]);}
		void		Purge();
		bool		IsValid(int No);

		bool		operator==(CElemSet const &v1)					{return (Name==v1.Name);}

private:
		void			Load2d(CCore *Core);
		void			Load3d(CCore *Core);
		
		CPoint			GetElemPos(int ID);

		GString			Filename,Name;
		
		int				SetNumber;
		int				DefWidth,DefHeight;
		CList<CElem>	ElemList;
		bool			Loaded;
		int				ElemBrowserWidth;
};

/*****************************************************************************/
class	CElemBank
{
public:
		CElemBank();
		~CElemBank();
		void	CleanUp();

		void		Load(CFile *File,int Version);
		void		Save(CFile *File);

		void		AddSet(const char *Filename);
		void		LoadAllSets(CCore *Core);
		void		LoadNewSet(CCore *Core);
		void		ReloadAllSets();
		void		DeleteSet(int Set);
		int			NeedLoad()							{return(LoadFlag);}
		CElem		&GetElem(int Set,int Elem);
		bool		IsValid(int Set,int Elem);

		int			GetSetCount()						{return(SetList.size());}
	

protected:

		CList<CElemSet>	SetList;
		bool			LoadFlag;

};

/*****************************************************************************/
#endif