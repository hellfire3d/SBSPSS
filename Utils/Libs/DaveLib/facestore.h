/****************************************/
/*** Generic Face Compilation Storage ***/
/****************************************/

#ifndef __FACESTORE_HEADER__
#define __FACESTORE_HEADER__

#include <ginio.h>
#include <gintex.h>
#include <Vector>
#include <TexGrab.h>

#include "..\..\..\tools\data\include\dStructs.h"

using namespace std;

//***************************************************************************
/*
struct	sFaceTexList
{
	GString		Name;
	int			TexGrabId;
};
*/
//***************************************************************************
/*
class	CFace
{
public:
	int				PntCount;
	int				Mat;
	Vector3			vtx[4];
	int				pts[4];
//	int				idx[4];
	sUV				uvs[4];
//	int				vis[4];
	Vector3			Normal;
	bool			Avail;
//	int				ID;
	GString			TexName;
	int				TPageFlag;

};
*/

class	CFace
{
public:
	CFace()
	{
	vtx.resize(3);
	pts.resize(3);
	uvs.resize(3);
	}

	int				Mat;
	vector<Vector3>	vtx;
	vector<int>		pts;
	vector<sUV>		uvs;
	Vector3			Normal;
	bool			Avail;
	GString			TexName;
	int				TPageFlag;
	int				OtOfs;
};


//***************************************************************************
#ifndef	sTriFace
struct  sTriFace
{
	int			Mat;
	int			Flags;
	Vector3		vtx[3];
	int			pts[3];
	sUV			uvs[3];
};
#endif

//***************************************************************************
class CFaceStore
{

public:
		CFaceStore()			{MaxStrip = 3;TexGrab=0;}
		CFaceStore(int Max)		{MaxStrip=Max;TexGrab=0;}
		~CFaceStore(){};

		void					SetTPageFlag(CFace &F,int MatFlag);
		CFace					&AddFace(vector<Vector3> const &P, const sGinTri &T, const sUVTri &uv,GString const &Tex,int MatFlag=0,bool ProcessTexFlag=false);
		CFace					&AddFace(CFace &F,bool TexFlag=true);
		void					AddFaces(vector<CFace>&Faces,bool TexFlag=true);
		void					AddFaces(CFaceStore &Faces,bool TexFlag=true);

		CFace					&AddFace(sTriFace &Face,int ID=0);

		void					SetTexGrab(CTexGrab &NewGrab)						{TexGrab=&NewGrab;}
		int						AddTex(GString const &TexName);

		void					Process(vector<sTri> &OutTriList,vector<sQuad> &OutQuadList,vector<sVtx> &OutVtxList);

		int						WriteTriList(FILE *File,vector<sTri> &List);
		int						WriteQuadList(FILE *File,vector<sQuad> &List);
		int						WriteVtxList(FILE *File,vector<sVtx> &List);
		
			
		vector<CFace> const		&GetFaceList()					{return(FaceList);}
		int						GetFaceCount()					{return(FaceList.size());}

		vector<CFace> const		&GetTriFaceList()				{return(TriFaceList);}
		int						GetTriFaceCount()				{return(TriFaceList.size());}

		vector<CFace> const		&GetQuadFaceList()				{return(QuadFaceList);}
		int						GetQuadFaceCount()				{return(QuadFaceList.size());}
static	int						AddVtx(vector<sVtx> &OutVtxList,Vector3 &Vtx);
static	int						AddVtx(vector<sVtx> &OutVtxList,sVtx &ThisVtx);
		void					ParseVtx4BBox(sVtx &Vtx);

		void					setMaxStripLength(int v)		{MaxStrip = v;}

		CFace& operator []( int nIndex )	{return(FaceList[nIndex]);}

		sBBox					&GetBBox()	{return(BBox);}
private:
		void					Quad();
		void					SetupUV(CFace const &In, sTri &Out);
		void					SetupUV(CFace const &In, sQuad &Out);

		void					BuildOutTriList(vector<sTri> &OutTriList,vector<sVtx> &OutVtxList);
		void					BuildOutQuadList(vector<sQuad> &OutQuadList,vector<sVtx> &OutVtxList);

		int						QuadGetAttached(int FaceNo);
		void					QuadGetPnts(CFace &F,int *Join0,int *Join1,int *Pnt);
		void					QuadGetUVs(CFace &F,int *Join0,int *Join1,int *Pnt);
		void					OrderPnts( CFace &F ,int unc);

		bool					CanConnect( int f0, int f1 );
		int						CountFacesAttached ( int f );
		void					FollowFace( int id, CFace &F );
		int						GetUnconnectedPoint( int f0, int f1, int &f0p0, int &f0p1 );
		bool					GetFace( CFace & F );

		int						MaxStrip;

		vector<CFace>			FaceList;
		CTexGrab				*TexGrab;

		vector<CFace>			TriFaceList;
		vector<CFace>			QuadFaceList;
		vector<sVtx>			VtxList;
		sBBox					BBox;

};

#endif