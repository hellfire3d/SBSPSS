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

#include	"Layer.h"
#include	"MapEdit.h"
#include	"GUIElemList.h"

/*****************************************************************************/
struct	sTriFace
{
	int				Mat;
	int				Flags;
	Vector3			vtx[3];
	int				pts[3];
	sUV				uvs[3];
};

enum	TRI_FLAGS
{
		TRI_FLAGS_TRANS_ADD=1<<0,
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
		enum CentreMode
		{
			CentreModeL	=1<<0,
			CentreModeR	=1<<1,
			CentreModeLR=1<<2,
			CentreModeT	=1<<3,
			CentreModeB	=1<<4,
			CentreModeTB=1<<5,
		};
		enum
		{
			UnitSize=16,
		};

		CElem(int Width,int Height);																			// Blank (2d)
		CElem(CCore *Core,const char *Filename,int TexId,int XOfs,int YOfs,int Width,int Height,int CentreMode=0);	// 2d elem
		CElem(CCore *Core,const char *Filename,CScene &ThisScene,int Node);										// 3d elem
		void	CleanUp();

		void	Purge();
		void	Render(int Flags,bool Render3d);

		int		GetElemWidth()				{return(ElemWidth);}
		int		GetElemHeight()				{return(ElemHeight);}
		u8		*GetElemRGB()				{return(ElemRGB);}

		bool	IsValid()					{return(ValidFlag);}

		bool	IsBlank()					{return(BlankFlag);}
		bool	IsElem3d()					{return(Type==ElemType3d);}
		int		GetTexXOfs()				{return(TexXOfs);}
		int		GetTexYOfs()				{return(TexYOfs);}

static	void	CreateDefaultTileGfx();
static	int		BlankID;
static	int		InvalidID;
static	bool	DefTexFlag;
		
		std::vector<sTriFace> 	&GetTriList() {return(TriList);}

		void	SetBlank();
		void	SetInvalid();
		GLint	GetTexID()	{return(TexID);}

		void		Report(FILE *File);

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
		GString					ElemName;

		Vector3					Ofs;

		GLint					DrawList[ElemTypeMax];
		ElemType				Type;
		bool					ValidFlag,BlankFlag;
		int						TexXOfs,TexYOfs;
		
		int						ElemWidth,ElemHeight;
		float					UnitWidth,UnitHeight;
		int						ElemID;
		u8						*ElemRGB;
		GLint					TexID;
};

/*****************************************************************************/
class	CElemSet
{
public:
		enum
		{
			DEF_ELEMBROWSWEWIDTH=8,
		};

		CElemSet(const char *_Filename,int Idx,int MaxWidth,int MaxHeight,bool CreateBlank,int Centre=0);
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
		void		Report(FILE *File);

private:
		void			Load2d(CCore *Core);
		void			Load3d(CCore *Core);
		
		CPoint			GetElemPos(int ID);

		GString			Filename,Name;
		
		int				SetNumber;
		int				MaxWidth,MaxHeight;
		int				BmpW,BmpH;
		int				CentreMode;
		CList<CElem>	ElemList;
		bool			Loaded;
		int				ElemBrowserWidth;
};

/*****************************************************************************/
class	CElemBank : public CLayer
{
public:
// Local
		CElemBank(int MaxWidth,int MaxHeight,bool Blank,int Centre=0);
virtual	~CElemBank();
		void		CleanUp();

virtual	void		Load(CFile *File,int Version);
virtual	void		Save(CFile *File);

		int			AddSet(const char *Filename);
		void		LoadAllSets(CCore *Core);
		void		LoadNewSet(CCore *Core);
		void		ReloadAllSets();
		void		DeleteSet(int Set);
		int			NeedLoad()							{return(LoadFlag);}
		CElem		&GetElem(int Set,int Elem)			{return(SetList[Set].GetElem(Elem));}
		bool		IsValid(int Set,int Elem);
		void		RenderElem(int Set,int Elem,int Flags,bool Is3d);
		void		RenderInvalid()						{glCallList(CElem::InvalidID);}
		int			GetSetCount()						{return(SetList.size());}

const	char		*GetSetName(int Set)									{return(SetList[Set].GetName());}
const	char		*GetSetFilename(int Set)								{return(SetList[Set].GetFilename());}

		void		Report();
// Overloads
virtual	int			GetType()							{return(0);}

virtual	void		RenderGrid(CCore *Core,Vector3 &CamPos,bool Active);
virtual	void		RenderSelection(CCore *Core,Vector3 &ThisCam){};
virtual	void		FindCursorPos(CCore *Core,Vector3 &CamPos,CPoint &MousePos);

virtual	void		GUIInit(CCore *Core);
virtual	void		GUIKill(CCore *Core);
virtual	void		GUIUpdate(CCore *Core);
virtual	void		GUIChanged(CCore *Core);

virtual	void		Export(CCore *Core,CExport &Exp){};

virtual	bool		LButtonControl(CCore *Core,UINT nFlags, CPoint &CursorPos,bool DownFlag)	{return(false);}
virtual	bool		RButtonControl(CCore *Core,UINT nFlags, CPoint &CursorPos,bool DownFlag)	{return(false);}
virtual	bool		MouseMove(CCore *Core,UINT nFlags, CPoint &CursorPos)						{return(false);}
virtual	bool		Command(int CmdMsg,CCore *Core,int Param0=0,int Param1=0)					{return(false);}
		CPoint		GetElemPos(int ID,int Width);

virtual	void			LoadGfx(CCore *Core){}

// Local
virtual	bool		CanClose()												{return(true);}

protected:
		CList<CElemSet>	SetList;
		
		bool			LoadFlag;
		int				CurrentSet;
		int				CursorPos;

		int				MaxWidth,MaxHeight;
		bool			BlankFlag;
		int				CentreMode;

		CGUIElemList	GUIElemList;

};

/*****************************************************************************/
#endif