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

		CElem(int Width,int Height);																			// Blank (2d)
		CElem(CCore *Core,const char *Filename,int TexId,int XOfs,int YOfs,int Width,int Height,bool Centre);	// 2d elem
		CElem(CCore *Core,const char *Filename,CScene &ThisScene,int Node);										// 3d elem
		void	CleanUp();

		void	Purge();
		void	Render(int Flags,bool Render3d);
		void	RenderInvalid();

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
			DEF_ELEMBROWSWEWIDTH=8,
		};

		CElemSet(const char *_Filename,int Idx,int MaxWidth,int MaxHeight,bool CreateBlank,bool Centre);
		~CElemSet();
	
		void		CleanUp();
		int			IsLoaded()										{return(Loaded);}
		int			GetCount()										{return(ElemList.size());}

		void		Load(CCore *Core);

const	char		*GetFilename()									{return(Filename);}
const	char		*GetName()										{return(Name);}
		int			GetBrowserWidth()								{return(ElemBrowserWidth);}

		void		RenderElem(int Elem,int Flags,bool Render3d)	{ElemList[Elem].Render(Flags,Render3d);}
		void		RenderInvalid()									{ElemList[0].RenderInvalid();}
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
		int				MaxWidth,MaxHeight;
		bool			CentreFlag;
		CList<CElem>	ElemList;
		bool			Loaded;
		int				ElemBrowserWidth;
};

/*****************************************************************************/
class	CElemBank : public CLayer
{
public:
// Local
		CElemBank(int MaxWidth,int MaxHeight,bool Blank,bool Centre);
virtual	~CElemBank();
		void		CleanUp();

virtual	void		Load(CFile *File,int Version);
virtual	void		Save(CFile *File);

		void		AddSet(const char *Filename);
		void		LoadAllSets(CCore *Core);
		void		LoadNewSet(CCore *Core);
		void		ReloadAllSets();
		void		DeleteSet(int Set);
		int			NeedLoad()							{return(LoadFlag);}
		CElem		&GetElem(int Set,int Elem)			{return(SetList[Set].GetElem(Elem));}
		bool		IsValid(int Set,int Elem);
		void		RenderElem(int Set,int Elem,int Flags,bool Is3d);

		int			GetSetCount()						{return(SetList.size());}

const	char		*GetSetName(int Set)									{return(SetList[Set].GetName());}
const	char		*GetSetFilename(int Set)								{return(SetList[Set].GetFilename());}

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

// Local
virtual	bool		CanClose()												{return(true);}

protected:
		CList<CElemSet>	SetList;
		
		bool			LoadFlag;
		int				CurrentSet;
		int				CursorPos;

		int				MaxWidth,MaxHeight;
		bool			BlankFlag,CentreFlag;

		CGUIElemList	GUIElemList;

};

/*****************************************************************************/
#endif