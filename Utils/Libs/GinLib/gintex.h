/********************/
/*** Daves GinTex ***/
/********************/

#pragma warning( disable : 4786 )

#ifndef __GINTEX_H__
#define __GINTEX_H__

#include <vector>

#include "ginio.h"
#include "vector3.h"
#include "matrix4x4.h"
#include "quat.h"

//#include "IniClass.h"


/*****************************************************************************/
int		IsStrSame(char *Str0,char *Str1,int Len=-1);

/*****************************************************************************/
/*** Version Chunk ***********************************************************/
/*****************************************************************************/
class CVers : public GinChunk
{
public:
	virtual char const * GetName(void) const		{return("VERS");}
	int	GetVersion(void)							{return(Version);}

protected:
	virtual GinChunk * MakeNew(void)				{return(new CVers);}

	virtual void Load(Gifstream & In)		
		{
		Version=In.Get32();
		}
	
	int Version;
};

/*****************************************************************************/
/*** Material Chunk **********************************************************/
/*****************************************************************************/
struct Material
{
	int TexId;
	int Flags;
};
class CMat4 : public GinChunk
{
public:
	virtual char const * GetName(void) const				{return("MAT4");}
	std::vector<Material> const & GetMaterials(void) const	{return(Mats);}
	int GetTexId(int v);

protected:
	virtual GinChunk * MakeNew(void)						{return(new	CMat4);}
	virtual void Load(Gifstream & In);	
	std::vector<Material> Mats;
};
	
/*****************************************************************************/
/*** Mesh Chunk **************************************************************/
/*****************************************************************************/
struct MeshChunk
{
	int MatId;			
	int NumFace;		
	int MeshNum;		
	int Attrib;		
	int Normals;		
	int Vcol;
	int Tex;
};

class CMesh : public GinChunk
{
public:
	virtual char const * GetName(void) const				{return("MESH");}
	std::vector<MeshChunk> const & GetChunk(void) const		{return(Chunk);}
	int getObj( void ) const								{return Obj;}

protected:
	int Obj;
	std::vector<MeshChunk> Chunk;
	virtual GinChunk * MakeNew(void)						{return(new	CMesh);}
	virtual void Load(Gifstream & In);
};

/*****************************************************************************/
/*** Model Chunk *************************************************************/
/*****************************************************************************/
struct Mod4Chunk
{
	long	nCurObj;			
	char	Name[32];
	float	Radius;
	Vector3	Centre;
	Vector3	Ap;
};

class CMod4: public GinChunk
{
public:
	virtual char const * GetName(void) const				{return("MOD4");}
	std::vector<Mod4Chunk> const & GetChunk(void) const		{return(Chunk);}
	char * GetModelName()									{return(Chunk[0].Name);}
	
protected:
	std::vector<Mod4Chunk> Chunk;
	virtual GinChunk * MakeNew(void)						{return(new	CMod4);}
	virtual void Load(Gifstream & In);
};

/*****************************************************************************/
/*** Texture Name Chunk ******************************************************/
/*****************************************************************************/
class CTexName : public GinChunk
{
public:

	virtual char const * GetName(void) const				{return("TEX4");}
	std::vector<GString> const & GetTexNames(void) const	{return(Names);}
	virtual GinChunk * MakeNew(void)						{return(new CTexName);}
	virtual void Load(Gifstream & In);

protected:
	std::vector<GString> Names;
};

/*****************************************************************************/
/*** Vtx Chunk ***************************************************************/
/*****************************************************************************/
class CPts4: public GinChunk
{
	virtual char const * GetName(void) const				{return("PTS4");}
	virtual GinChunk * MakeNew(void)						{return(new CPts4);}
	virtual void Load(Gifstream & In)
		{
		ModNum = In.Get32();
		int nv = In.Get32();
		Pnts.resize(nv);
		for (int i = 0; i< nv ;i++) 
			{
			In.read((char*)&Pnts[i].x, 4);
			In.read((char*)&Pnts[i].y, 4);
			In.read((char*)&Pnts[i].z, 4);
			}
		}

public:

	std::vector<Vector3> const & GetPts(void) const	{return(Pnts);}
	int GetModNum(void) const					{return ModNum;}

protected:

	int  ModNum;
	std::vector<Vector3> Pnts;
};

/*****************************************************************************/
/*** Poly Chunk **************************************************************/
/*****************************************************************************/
struct sGinTri
{
	int p[3];
	int	vis[3];

BOOL	operator==(sGinTri const &v1)	
		{
			for (int i=0; i<3;i++)
			{
				if (p[i]!=v1.p[i]) return(FALSE);
				if (vis[i]!=v1.vis[i]) return(FALSE);
			}
		return (TRUE);
		}

};

class CPoly: public GinChunk
{
	virtual char const * GetName(void) const	{return("POLY");}
	virtual GinChunk * MakeNew(void)			{return(new CPoly);}

	virtual void Load(Gifstream & In)
		{
		ModNum  = In.Get16();
		MeshNum = In.Get16();
		int nf  = In.Get32();

		Tris.resize(nf);
		for(int i=0; i<nf; i++) 
			{
			for (int j=0; j<3; j++) Tris[i].p[j] = In.Get32();
			for (int h=0; h<3; h++) Tris[i].vis[h] = In.Get32();
			}
		}

public:

	std::vector<sGinTri> const & GetTris(void) const	{return(Tris);}

	int GetModNum(void) const						{return ModNum;}
	int GetMeshNum(void) const						{return MeshNum;}

protected:

	int  ModNum,MeshNum;
	std::vector<sGinTri> Tris;
};

/*****************************************************************************/
/*** Vtx Color Chunk *********************************************************/
/*****************************************************************************/
struct sRGB
{
	float r,g,b;
};

struct sVColTri
{
	sRGB p[3];
};

class CVcol: public GinChunk
{
	virtual char const * GetName(void) const	{return("VCOL");}
	virtual GinChunk * MakeNew(void)			{return(new CVcol);}

	virtual void Load(Gifstream & In)
		{
		ModNum  = In.Get32();
		MeshNum = In.Get32();
		int nf  = In.Get32();

		Tris.resize(nf);
		for(int i=0; i<nf; i++) 
			{
			for (int j=0; j<3; j++) 
				{
				In.read((char*)&Tris[i].p[j].r, 4);
				In.read((char*)&Tris[i].p[j].g, 4);
				In.read((char*)&Tris[i].p[j].b, 4);
				}
			}
		}

public:
	std::vector<sVColTri> const & GetVcolTris(void) const		{return(Tris);}
	int GetModNum(void) const									{return ModNum;}
	int GetMeshNum(void) const									{return MeshNum;}

protected:
	int  ModNum,MeshNum;
	std::vector<sVColTri> Tris;
};

/*****************************************************************************/
/*** Texture UV's Chunk ******************************************************/
/*****************************************************************************/
struct sUV
{
	float u,v;
BOOL	operator==(sUV const &v1)	
		{
		return(u==v1.u && v==v1.v);
		}

inline	operator=(sUV const &Src)	
{
		u=Src.u;
		v=Src.v;
}

};

struct sUVTri
{
	sUV p[3];
BOOL	operator==(sUVTri const &v1)	
		{
		for (int i=0; i<3;i++)
		{
			if (!(p[i]==v1.p[i])) return(FALSE);
		}
		return (TRUE);
		}

inline operator=(sUVTri const &Src)	
{
		for (int i=0; i<3; i++) 
		{
			p[i]=Src.p[i];
		}
}

};

class CUVtri: public GinChunk
{
	virtual char const * GetName(void) const	{return("MAP4");}
	virtual GinChunk * MakeNew(void)			{return(new CUVtri);}

	virtual void Load(Gifstream & In)
		{
		ModNum  = In.Get16();
		MeshNum = In.Get16();
		int nf  = In.Get32();

		Tris.resize(nf);
		for(int i=0; i<nf; i++) 
			{
			for (int j=0; j<3; j++) 
				{
				In.read((char*)&Tris[i].p[j].u, 4);
				In.read((char*)&Tris[i].p[j].v, 4);
				}
			}
		}

public:

	std::vector<sUVTri> const & GetUVTris(void) const	{return(Tris);}
	int GetModNum(void) const							{return ModNum;}
	int GetMeshNum(void) const							{return MeshNum;}

protected:

	int  ModNum,MeshNum;
	std::vector<sUVTri> Tris;
};

/*****************************************************************************/
/*** Scene Tree Chunk ********************************************************/
/*****************************************************************************/
struct sGinWeight
{
	long	VertNo;
	float	Weight;
//	float	X,Y,Z;
	Vector3	Pos;
};
struct sGinAnim
{
	int			Frame;
	Vector3		Pos;
	Quaternion	Ang;
	Vector3		apk;
	Quaternion	apu;
};

class CNode
{
public:

	int		IsNodePrefix(char *InName)	// Checks prefix
			{
			return(IsStrSame(InName,Name,strlen(InName)));
			}
	int		IsNodeName(char *InName)	// Checks full name
			{
			return(IsStrSame(InName,Name));
			}

	void	AddChild(std::vector<CNode> &Tree,CNode &Node,int Parent)			
		{
		Node.ChildList.clear();
		int	TreeSize=Tree.size();
		Node.ParentIdx=Parent;
		Node.Idx=TreeSize;
		Node.Active=true;
		if (Parent!=-1)	Tree[Parent].ChildList.push_back(Node.Idx);
		if (!(Parent==-1 && Node.Idx)) Tree.push_back(Node);
		}

	void	AddPruneChild(std::vector<CNode> &SceneTree,std::vector<int> &PruneTree,int Parent)			
		{
		PruneIdx=PruneTree.size();
		PruneTree.push_back(Idx);
		if (Parent!=-1)	
			{
			PruneParentIdx=SceneTree[Parent].PruneIdx;
			SceneTree[Parent].PruneChildList.push_back(Idx);
			}
		else
			{
			PruneParentIdx=-1;
			}
		}

//	Vector3	GetWorldPos(Vector3	&Pos);
//	Quaternion	GetWorldAng(Quaternion &Q);
	Vector3		GetWorldPos();
	Quaternion	GetWorldAng();


	int							  GetChildCount()			  {return(ChildList.size());}
	int							  GetPruneChildCount()		  {return(PruneChildList.size());}
	std::vector<Vector3>	const &GetPts()				const {return(Pts);}
	std::vector<Vector3>	const &GetRelPts()			const {return(RelPts);}
	std::vector<sGinTri>	const &GetTris()			const {return(Tris);}
	std::vector<sVColTri>	const &GetVColTris()		const {return(VColTris);}
	std::vector<sUVTri>		const &GetUVTris()			const {return(UVTris);}
	std::vector<int>		const &GetTriMaterial()		const {return(TriMaterial);}
	std::vector<sGinAnim>	const &GetAnim()			const {return(Anim);}
	std::vector<sGinAnim>	const &GetKeyAnim()			const {return(KeyAnim);}
	std::vector<sGinAnim>	const &GetShrunkAnim()		const {return(ShrunkAnim);}
	std::vector<sGinAnim>	const &GetShrunkKeyAnim()	const {return(ShrunkKeyAnim);}

	char					*GetName()						  {return(Name);}

	int						Idx,ParentIdx;
	int						PruneIdx,PruneParentIdx;
	char					Name[32];

	Vector3					Pos;
	Quaternion				Ang;
	Vector3					apk;
	Quaternion				apu;
	int						Active;
	Matrix4x4				Mtx,WorldMtx;
//	CIni					UserProp;

	std::vector<int>		ChildList;
	std::vector<int>		PruneChildList;
	std::vector<sGinWeight>	Weights;
	std::vector<Vector3>	Pts;
	std::vector<Vector3>	RelPts;
	std::vector<sGinTri>	Tris;
	std::vector<sVColTri>	VColTris;
	std::vector<sUVTri>		UVTris;
	std::vector<int>		TriMaterial;
	std::vector<sGinAnim>	Anim;
	std::vector<sGinAnim>	KeyAnim;
// Shrunk Anim stuff
	int						StartFrame,EndFrame;
	std::vector<sGinAnim>	ShrunkAnim;
	std::vector<sGinAnim>	ShrunkKeyAnim;

	long	Flags,Pad;
	int		TriggerCount,TriggerIdx;
	int	TriBase;
};

class CGinTree: public GinChunk
{
	virtual char const * GetName(void) const	{return("TREE");}
	virtual GinChunk * MakeNew(void)			{return(new CGinTree);}

	virtual void Load(Gifstream & In);
	void	LoadTree(Gifstream & In,int	Parent);

public:

	std::vector<CNode> const & GetTree(void) const{return(SceneTree);}

protected:
	std::vector<CNode> SceneTree;

};

/*****************************************************************************/
/*** Skin Chunk **************************************************************/
/*****************************************************************************/
class CSkinTree: public GinChunk
{
	virtual char const * GetName(void) const	{return("BONE");}
	virtual GinChunk * MakeNew(void)			{return(new CSkinTree);}
	virtual void Load(Gifstream & In);
	void	LoadTree(Gifstream & In,int	Parent);

public:
	std::vector<CNode> const & GetTree(void) const{return(SkinTree);}

protected:
	std::vector<CNode> SkinTree;
};

/*****************************************************************************/
/*** Tree Anim Chunk *********************************************************/
/*****************************************************************************/

class CAnimTree: public GinChunk
{
	virtual char const * GetName(void) const	{return("ANIM");}
	virtual GinChunk * MakeNew(void)			{return(new CAnimTree);}
	virtual void Load(Gifstream & In);
	void	LoadTree(Gifstream & In,int	Parent,int FrameCount);

public:
	std::vector<CNode> const & GetTree(void) const{return(AnimTree);}

protected:

	std::vector<CNode> AnimTree;

};

/*****************************************************************************/
/*** Tree Key Anim Chunk *****************************************************/
/*****************************************************************************/

class CKeyAnimTree: public GinChunk
{
	virtual char const * GetName(void) const	{return("KEY4");}
	virtual GinChunk * MakeNew(void)			{return(new CKeyAnimTree);}
	virtual void Load(Gifstream & In);
	void	LoadTree(Gifstream & In,int	Parent);

public:
	std::vector<CNode> const & GetTree(void) const{return(AnimTree);}

protected:
	std::vector<CNode> AnimTree;

};

/*****************************************************************************/
/*** User Prop Chunk *********************************************************/
/*****************************************************************************/

class CUserProp: public GinChunk
{
	virtual char const * GetName(void) const	{return("PROP");}
	virtual GinChunk * MakeNew(void)			{return(new CUserProp);}

	virtual void Load(Gifstream & In)
		{
		ModNum  = In.Get32()-1;
		int Size  = In.Get32();
		Prop.resize(Size+1);
		for(int i=0; i<Size; i++) 
			{
			In.read((char*)&Prop[i],1);
			}
		Prop[Size]=0;
		}
public:

	std::vector<char> const & GetUserProp() const	{return(Prop);}
	int GetModNum(void) const						{return ModNum;}

protected:

	int  ModNum;
	std::vector<char> Prop;
};

/*****************************************************************************/
/*** Camera Chunk ************************************************************/
/*****************************************************************************/
struct sCam
{
	std::vector<Vector3>	CamPos;
	std::vector<Vector3>	Target;
} ;

class CCamera: public GinChunk
{
	virtual char const * GetName(void) const				{return("CAM4");}
	virtual GinChunk * MakeNew(void)						{return(new CCamera);}
	virtual void Load(Gifstream & In)
		{
		int Frame,FrameCount = In.Get32();
		ThisCam.CamPos.resize(FrameCount);
		ThisCam.Target.resize(FrameCount);
		for (Frame = 0; Frame< FrameCount;Frame++) 
			{
			In.read((char*)&ThisCam.CamPos[Frame].x, 4); 
			In.read((char*)&ThisCam.CamPos[Frame].y, 4); 
			In.read((char*)&ThisCam.CamPos[Frame].z, 4);
			}
		for (Frame = 0; Frame< FrameCount;Frame++) 
			{
			In.read((char*)&ThisCam.Target[Frame].x, 4); 
			In.read((char*)&ThisCam.Target[Frame].y, 4); 
			In.read((char*)&ThisCam.Target[Frame].z, 4);
			}
		}

public:

	sCam	const & GetCam(void) const	{return(ThisCam);}

protected:

	sCam	ThisCam;
};

/*****************************************************************************/
/*** Gin Loader **************************************************************/
/*****************************************************************************/
class Gin4File : public GObject
{
public:
	Gin4File(void)
		{
		MyFile.AddHandler(new CTexName);
		MyFile.AddHandler(new CVers);
		MyFile.AddHandler(new CMesh);
		MyFile.AddHandler(new CMod4);
		MyFile.AddHandler(new CMat4);
		MyFile.AddHandler(new CPts4);
		MyFile.AddHandler(new CPoly);
		MyFile.AddHandler(new CVcol);
		MyFile.AddHandler(new CUVtri);
		MyFile.AddHandler(new CGinTree);
		MyFile.AddHandler(new CAnimTree);
		MyFile.AddHandler(new CKeyAnimTree);
		MyFile.AddHandler(new CSkinTree);
		MyFile.AddHandler(new CUserProp);
		MyFile.AddHandler(new CCamera);
		}

	void Load(char const * Name);

	std::vector<GinChunk  const *> GetVersionChunk()	{return(VersionChunk);}	
	std::vector<GinChunk  const *> GetTexNamesChunk()	{return(TexNamesChunk);}	
	std::vector<GinChunk  const *> GetMatChunk()		{return(MatChunk);}	
	std::vector<GinChunk  const *> GetMod4Chunk()		{return(Mod4Chunk);}	
	std::vector<GinChunk  const *> GetMshChunk()		{return(MshChunk);}	
	std::vector<GinChunk  const *> GetPointsChunk()		{return(PointsChunk);}	
	std::vector<GinChunk  const *> GetTrisChunk()		{return(TrisChunk);}	
	std::vector<GinChunk  const *> GetVColTrisChunk()	{return(VColTrisChunk);}	
	std::vector<GinChunk  const *> GetUVtrisChunk()		{return(UVtrisChunk);}	
	std::vector<GinChunk  const *> GetSceneTreeChunk()	{return(SceneTreeChunk);}	
	std::vector<GinChunk  const *> GetAnimTreeChunk()	{return(AnimTreeChunk);}	
	std::vector<GinChunk  const *> GetKeyAnimTreeChunk(){return(KeyAnimTreeChunk);}	
	std::vector<GinChunk  const *> GetSkinTreeChunk()	{return(SkinTreeChunk);}	
	std::vector<GinChunk  const *> GetUserPropChunk()	{return(UserPropChunk);}	
	std::vector<GinChunk  const *> GetCameraChunk()		{return(CameraChunk);}	

	char * GetModelName(int ModelNum)					
		{
		CMod4	*Mod=(CMod4*)Mod4Chunk[ModelNum];
		return(Mod->GetModelName());
		};
	

protected:
	GinFile	MyFile;

	std::vector<GinChunk  const *> VersionChunk;
	std::vector<GinChunk  const *> TexNamesChunk;
	std::vector<GinChunk  const *> MatChunk;
	std::vector<GinChunk  const *> Mod4Chunk;
	std::vector<GinChunk  const *> MshChunk;
	std::vector<GinChunk  const *> PointsChunk;
	std::vector<GinChunk  const *> TrisChunk;
	std::vector<GinChunk  const *> VColTrisChunk;
	std::vector<GinChunk  const *> UVtrisChunk;
	std::vector<GinChunk  const *> SceneTreeChunk;
	std::vector<GinChunk  const *> AnimTreeChunk;
	std::vector<GinChunk  const *> KeyAnimTreeChunk;
	std::vector<GinChunk  const *> SkinTreeChunk;
	std::vector<GinChunk  const *> UserPropChunk;
	std::vector<GinChunk  const *> CameraChunk;

};

/*****************************************************************************/
/*** Scene Storage ***********************************************************/
/*****************************************************************************/

class	CScene
{
public:

	void	LoadWithTextures(char const * Name);
	void	Load(char const * Name);

//------------------
//--- Tree Stuff ---
//------------------ 

	CNode	&GetNode(int Idx)								{return(SceneTree[Idx]);}
	CNode	*GetNodePtr(int Idx)							{return(&GetNode(Idx));}
	CNode	*GetNodePtr(char *Name,int Start=0)
		{
		int	TreeSize=GetSceneTreeSize();
		for (int N=Start; N<TreeSize; N++) 	if (GetNode(N).IsNodeName(Name)) return(GetNodePtr(N));
		return(0);
		}
	CNode	*GetNodePtrPrefix(char *Name,int Start=0)
		{
		int	TreeSize=GetSceneTreeSize();
		for (int N=Start; N<TreeSize; N++) 	if (GetNode(N).IsNodePrefix(Name)) return(GetNodePtr(N));
		return(0);
		}

	int		GetSceneNodeCount()								{return(SceneTree.size());}
	int		GetSceneTreeSize()								{return(SceneTree.size());}
	int		GetPruneTreeSize()								{return(PruneTree.size());}

	CNode	&GetSceneNode(int Idx) 							{return(GetNode(Idx));}
	CNode	&GetPruneNode(int Idx) 							{return(GetNode(PruneTree[Idx]));}

	void	PrintSceneTree(int Idx=0)						
		{
#ifdef	_CONSOLE
		printf("SceneTree - %i Nodes\n\n",SceneTree.size());
		PrintTreeSpace=0;
		PrintTreeNode(Idx,0);
#endif
		}
	void	PrintPruneTree(int Idx=0)						
		{
#ifdef	_CONSOLE
		printf("PruneTree - %i Nodes\n\n",PruneTree.size());
		PrintTreeSpace=0;
		PrintTreeNode(Idx,1);
#endif
		}
	void	PrintTreeNode(int Idx,const int);

	void	BuildPruneTree(int Idx, int ParentIdx);
	bool	SetPruneNodes(int Idx);

	void	ResetSceneTree()	{SceneTree.clear();}
	void	ResetPruneTree()	
		{
		int	TreeSize=SceneTree.size();
		for (int n=0;n<TreeSize;n++) SceneTree[n].PruneChildList.clear();
		}
	int		CountAllChildren(int Idx)						{return(_CountAllChildren(Idx)+1);}

//-----------------
//-----------------
//-----------------

	std::vector<GString> const & GetTexNames(void) const		{return(AllTexNames);}
	std::vector<GString> const & GetTexList(void) const			{return(TextureList);}
	std::vector<Material> const & GetMaterials(void) const		{return(AllMaterials);}
	std::vector<int> const & GetUsedMaterialIdx(void) const		{return(UsedMaterials);}
	std::vector<sCam> const & GetCamera(void) const				{return(Camera);}
	void GetNonSharedTextures(std::vector<GString> const & SharedTextures,std::vector<GString> & Dest);

protected:
	int loadPoints(int CurMod, std::vector<GinChunk  const *> &Points,CNode *ThisNode);
	int loadTris(int CurMod, int CurMesh, std::vector<GinChunk  const *> &Tris,CNode *ThisNode);
	int loadUV(int CurMod, int CurMesh, std::vector<GinChunk  const *> &UVTris,CNode *ThisNode);
	int loadVCol(int CurMod, int CurMesh, std::vector<GinChunk  const *> &VColTris,CNode *ThisNode);
	int addMaterialIdx( int idx );

	int		_CountAllChildren(int Idx)
		{
		CNode	&ThisNode=GetNode(Idx);
		int		NodeCount=0;
		int		ChildCount= ThisNode.GetPruneChildCount();
		for (int Loop=0;Loop<ChildCount;Loop++)	NodeCount+=_CountAllChildren(ThisNode.PruneChildList[Loop]);
		return(NodeCount+ChildCount);
		}
	
	GString	FileName;

	std::vector<GString> AllTexNames;
	std::vector<GString> TextureList;
	std::vector<Material> AllMaterials;
	std::vector<int> UsedMaterials;

	std::vector<CNode>	SceneTree;
	std::vector<int>	PruneTree;
	std::vector<sCam> Camera;
	int	CurTree;

	int	PrintTreeSpace;
	
};

/*****************************************************************************/
Matrix4x4	GetWorldMatrix(std::vector<CNode> const &Tree,int Idx);
Vector3		GetWorldPos(std::vector<CNode> const &Tree,int Idx);
Vector3		GetWorldPos(Matrix4x4 &WorldMtx,Vector3 &ThisPos);

#endif

