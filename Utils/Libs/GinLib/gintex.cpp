#include <algorithm>

#include <gfname.hpp>
#include "gintex.h"

#include "vector3.h"
#include "matrix4x4.h"
#include "quat.h"

using namespace std;

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
// Unconditional string compare
// REturns 1 on equal
int		IsStrSame(char *Str0,char *Str1,int Len)
{
		if (Len==-1)
			{
			Len=strlen(Str0);
			if (strlen(Str1)!=Len) return(0);
			}
		for (int Loop=0;Loop<Len;Loop++) 
			{
			char C0=Str0[Loop];
			char C1=Str1[Loop];
			if (C0>='a' && C0<='z') C0+='A'-'a';
			if (C1>='a' && C1<='z') C1+='A'-'a';
			if (C0!=C1) return(0);
			}
		return(1);
}
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
int CMat4::GetTexId(int v)
{
	int NumOfTextures;

	NumOfTextures=Mats.size();

	if (v>=NumOfTextures)
		{
		GObject::Error(ERR_WARNING,"erroneous material id of %d\n",v);
		v = 0;
		}
	
	if (v>=NumOfTextures)
		GObject::Error(ERR_FATAL,"material asked for when none exists",v);

	return Mats[v].TexId;
}


/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
void CMat4::Load(Gifstream & In)
{
	int m = In.Get32();
	int o = In.Get32();

	Mats.resize(m);
	for (int i=0; i<m; i++)
		{
		for (int j=0; j<8; j++)
			In.Get32();
		
		int tf = (int)In.Get32();
		int temp2 = In.Get32();
		
		if (temp2&1 == 0)
			{
			tf = -1;
			}
		Mats[i].TexId = tf;
		Mats[i].Flags = temp2;

		}
}

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
void CMesh::Load(Gifstream & In)
{
	Obj = In.Get32();
	int NbMesh = In.Get32();

	Chunk.resize(NbMesh);
	
	for (int i=0; i<NbMesh; i++)
		{

		Chunk[i].MatId		= s16(In.Get16());
		Chunk[i].NumFace	= s16(In.Get16());
		Chunk[i].MeshNum	= In.Get32();
		Chunk[i].Attrib		= In.Get32();
		Chunk[i].Normals	= In.Get32();
		Chunk[i].Vcol		= In.Get32();
		Chunk[i].Tex		= In.Get32();
		}
}

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
void CMod4::Load(Gifstream & In)
{
Mod4Chunk	ThisChunk;	

		In.read((char*)&ThisChunk.nCurObj	, sizeof(long));
		In.read(ThisChunk.Name, 32);
		In.read((char*)&ThisChunk.Radius, sizeof(float));
		In.read((char*)&ThisChunk.Centre.x, sizeof(float));
		In.read((char*)&ThisChunk.Centre.y, sizeof(float));
		In.read((char*)&ThisChunk.Centre.z, sizeof(float));
		In.read((char*)&ThisChunk.Ap.x, sizeof(float));
		In.read((char*)&ThisChunk.Ap.y, sizeof(float));
		In.read((char*)&ThisChunk.Ap.z, sizeof(float));
		Chunk.push_back(ThisChunk);
}

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
void CTexName::Load(Gifstream & In)
{
	int	NumOfNames;

	NumOfNames=In.Get32();

	for (int f=0;f<NumOfNames;f++)
		{
		vector<char>	Str;
		bool			Done;
		
		Str.reserve(200);
		Done=false;

		In.Align(4);

		while (!Done)
			{
			char c;
			c=In.get();
			if (!c)
				Done=true;
			Str.push_back(c);
			}

		Names.push_back(&Str[0]);
		}
}

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
void	CGinTree::Load(Gifstream & In)
{
		LoadTree(In,-1);
}

/*****************************************************************************/
void	CGinTree::LoadTree(Gifstream & In,int Parent)
{
CNode	ThisNode;

		In.Align(4);
		
		In.read(ThisNode.Name, 32);
		In.read((char*)&ThisNode.Pos.x, 4);
		In.read((char*)&ThisNode.Pos.y, 4);
		In.read((char*)&ThisNode.Pos.z, 4);

		In.read((char*)&ThisNode.Ang.x, 4);
		In.read((char*)&ThisNode.Ang.y, 4);
		In.read((char*)&ThisNode.Ang.z, 4);
		In.read((char*)&ThisNode.Ang.w, 4);

		In.read((char*)&ThisNode.apk.x, 4);
		In.read((char*)&ThisNode.apk.y, 4);
		In.read((char*)&ThisNode.apk.z, 4);

		In.read((char*)&ThisNode.apu.x, 4);
		In.read((char*)&ThisNode.apu.y, 4);
		In.read((char*)&ThisNode.apu.z, 4);
		In.read((char*)&ThisNode.apu.w, 4);

int		ChildCount= In.Get32();
		SceneTree[0].AddChild( SceneTree,ThisNode,Parent);

int		Idx=ThisNode.Idx;
		for (int Loop=0;Loop<ChildCount;Loop++)	LoadTree(In,Idx);

}

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
void	CSkinTree::Load(Gifstream & In)
{
		In.Get32();	// Skip Obj number
		LoadTree(In,-1);
}

/*****************************************************************************/
void	CSkinTree::LoadTree(Gifstream & In,int Parent)
{
CNode	ThisNode;
int		WeightCount;
		In.Align(4);

		In.read(ThisNode.Name, 32);
		WeightCount=In.Get32();
	 	if (WeightCount)
			{
			ThisNode.Weights.resize(WeightCount);
			for (int Weight=0;Weight<WeightCount;Weight++)
				{
				ThisNode.Weights[Weight].VertNo=In.Get32();
				In.read((char*)&ThisNode.Weights[Weight].Weight, 4);
				In.read((char*)&ThisNode.Weights[Weight].Pos.x, 4);
				In.read((char*)&ThisNode.Weights[Weight].Pos.y, 4);
				In.read((char*)&ThisNode.Weights[Weight].Pos.z, 4);
				}
			}
	
int		ChildCount= In.Get32();
		SkinTree[0].AddChild( SkinTree,ThisNode,Parent);

int		Idx=ThisNode.Idx;
		for (int Loop=0;Loop<ChildCount;Loop++)	LoadTree(In,Idx);

}

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
const   int	CHANNEL_TYPE_TREE  = 7;  // NODE ANIMATION

void	CAnimTree::Load(Gifstream & In)
{
int		AnimType=In.Get32();
		if (AnimType!=CHANNEL_TYPE_TREE) return;

int		FrameCount = In.Get32();
		LoadTree(In,-1,FrameCount);
}

/*****************************************************************************/
void	CAnimTree::LoadTree(Gifstream & In,int Parent,int FrameCount)
{
CNode	ThisNode;

		In.Align(4);
		
		ThisNode.Anim.resize(FrameCount);
		for (int Frame=0;Frame<FrameCount;Frame++)
			{
			In.read((char*)&ThisNode.Anim[Frame].Pos.x, 4);
			In.read((char*)&ThisNode.Anim[Frame].Pos.y, 4);
			In.read((char*)&ThisNode.Anim[Frame].Pos.z, 4);

			In.read((char*)&ThisNode.Anim[Frame].Ang.x, 4);
			In.read((char*)&ThisNode.Anim[Frame].Ang.y, 4);
			In.read((char*)&ThisNode.Anim[Frame].Ang.z, 4);
			In.read((char*)&ThisNode.Anim[Frame].Ang.w, 4);

			In.read((char*)&ThisNode.Anim[Frame].apk.x, 4);
			In.read((char*)&ThisNode.Anim[Frame].apk.y, 4);
			In.read((char*)&ThisNode.Anim[Frame].apk.z, 4);

			In.read((char*)&ThisNode.Anim[Frame].apu.x, 4);
			In.read((char*)&ThisNode.Anim[Frame].apu.y, 4);
			In.read((char*)&ThisNode.Anim[Frame].apu.z, 4); 
			In.read((char*)&ThisNode.Anim[Frame].apu.w, 4);
			}

int		ChildCount= In.Get32();
		AnimTree[0].AddChild( AnimTree,ThisNode,Parent);

int		Idx=ThisNode.Idx;
		for (int Loop=0;Loop<ChildCount;Loop++)	LoadTree(In,Idx,FrameCount);

}

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
void	CKeyAnimTree::Load(Gifstream & In)
{
int		AnimType=In.Get32();
		if (AnimType!=CHANNEL_TYPE_TREE) return;
		AnimType=In.Get32();	// Skip Node type thingy

		LoadTree(In,-1);
}

/*****************************************************************************/
void	CKeyAnimTree::LoadTree(Gifstream & In,int Parent)
{
CNode	ThisNode;
int		KeyCount;
		In.Align(4);

		KeyCount=In.Get32();
		ThisNode.KeyAnim.resize(KeyCount);
		for (int Frame=0;Frame<KeyCount;Frame++)
			{
			ThisNode.KeyAnim[Frame].Frame=In.Get32();
			In.read((char*)&ThisNode.KeyAnim[Frame].Pos.x, 4);
			In.read((char*)&ThisNode.KeyAnim[Frame].Pos.y, 4);
			In.read((char*)&ThisNode.KeyAnim[Frame].Pos.z, 4);

			In.read((char*)&ThisNode.KeyAnim[Frame].Ang.x, 4);
			In.read((char*)&ThisNode.KeyAnim[Frame].Ang.y, 4);
			In.read((char*)&ThisNode.KeyAnim[Frame].Ang.z, 4);
			In.read((char*)&ThisNode.KeyAnim[Frame].Ang.w, 4);

			In.read((char*)&ThisNode.KeyAnim[Frame].apk.x, 4);
			In.read((char*)&ThisNode.KeyAnim[Frame].apk.y, 4);
			In.read((char*)&ThisNode.KeyAnim[Frame].apk.z, 4);

			In.read((char*)&ThisNode.KeyAnim[Frame].apu.x, 4);
			In.read((char*)&ThisNode.KeyAnim[Frame].apu.y, 4);
			In.read((char*)&ThisNode.KeyAnim[Frame].apu.z, 4); 
			In.read((char*)&ThisNode.KeyAnim[Frame].apu.w, 4);
		
			}

int		ChildCount= In.Get32();
		AnimTree[0].AddChild( AnimTree,ThisNode,Parent);

int		Idx=ThisNode.Idx;
		for (int Loop=0;Loop<ChildCount;Loop++)	LoadTree(In,Idx);
}

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
bool	CScene::SetPruneNodes(int Idx)
{
CNode	&ThisNode=GetNode(Idx);
bool	NoPrune=false;
int		ChildCount=ThisNode.GetChildCount();
	
	for (int Loop=0;Loop<ChildCount;Loop++) NoPrune|= SetPruneNodes(ThisNode.ChildList[Loop]);

int		HasData=ThisNode.Weights.size() | ThisNode.Pts.size();

	if (!NoPrune && !HasData)
		{
		ThisNode.Active=false;
		return(false);
		} 
	else
		{
		return(true);
		}
}

/*****************************************************************************/
void	CScene::BuildPruneTree(int Idx, int ParentIdx)
{
CNode	&ThisNode=GetNode(Idx);
		if (!ThisNode.Active) return;

		ThisNode.AddPruneChild(SceneTree,PruneTree,ParentIdx);

int		ChildCount=ThisNode.GetChildCount();
		for (int Loop=0;Loop<ChildCount;Loop++)	BuildPruneTree(ThisNode.ChildList[Loop],ThisNode.Idx);
}

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
int		IsAnimDiff(sGinAnim const &Frm0,sGinAnim const &Frm1)
{
		if (Frm0.Pos.x!=Frm1.Pos.x) return(1);
		if (Frm0.Pos.y!=Frm1.Pos.y) return(1);
		if (Frm0.Pos.z!=Frm1.Pos.z) return(1);

		if (Frm0.Ang.x!=Frm1.Ang.x) return(1);
		if (Frm0.Ang.y!=Frm1.Ang.y) return(1);
		if (Frm0.Ang.z!=Frm1.Ang.z) return(1);
		if (Frm0.Ang.w!=Frm1.Ang.w) return(1);
		return(0);
}

/*****************************************************************************/
void	LoadAndShrinkAnim(CNode const &KeyNode,CNode const &AnimNode,CNode &OutNode)
{
// Calc Extents from Key Frames
int		Key,Frame;
int		StartKey=0,EndKey=KeyNode.KeyAnim.size();

		if (!EndKey) // No keys, create 1 and return
			{
			sGinAnim	ForceKey;
			ForceKey.Frame=0;
			ForceKey.Pos.x=OutNode.Pos.x; ForceKey.Pos.y=OutNode.Pos.y; ForceKey.Pos.z=OutNode.Pos.z;
			ForceKey.Ang.x=OutNode.Ang.x; ForceKey.Ang.y=OutNode.Ang.y; ForceKey.Ang.z=OutNode.Ang.z; ForceKey.Ang.w=OutNode.Ang.w;
			OutNode.KeyAnim.push_back(ForceKey);
			OutNode.ShrunkKeyAnim.push_back(ForceKey);
			EndKey=0;
			OutNode.StartFrame=	0;
			OutNode.EndFrame=	0;
			}
		else
			{
			EndKey--;
			while ((!IsAnimDiff(KeyNode.KeyAnim[StartKey],	KeyNode.KeyAnim[StartKey+1]))	&& StartKey!=EndKey) StartKey++;
			while ((!IsAnimDiff(KeyNode.KeyAnim[EndKey  ],	KeyNode.KeyAnim[EndKey-1]))		&& StartKey!=EndKey) EndKey--;
			OutNode.StartFrame=	KeyNode.KeyAnim[StartKey].Frame; 
			OutNode.EndFrame=	KeyNode.KeyAnim[EndKey].Frame;
			EndKey++;
			}

		OutNode.EndFrame++;

// Transfer Key Frames
int		KeyAnimSize=KeyNode.KeyAnim.size();
			for (Key=0;Key<KeyAnimSize;Key++) OutNode.KeyAnim.push_back(KeyNode.KeyAnim[Key]);				// Transfer All key frames
			for (Key=StartKey; Key<EndKey; Key++) OutNode.ShrunkKeyAnim.push_back(KeyNode.KeyAnim[Key]);	// Transfer Shrunk key frames

// Transfer Normal frames
int		AnimSize=AnimNode.Anim.size();
		if (AnimSize)
			{
			for (Frame=0;Frame<AnimSize;Frame++) OutNode.Anim.push_back(AnimNode.Anim[Frame]);									// Transfer All Normal Anim
			for (Frame=OutNode.StartFrame; Frame<OutNode.EndFrame; Frame++) OutNode.ShrunkAnim.push_back(AnimNode.Anim[Frame]);	// Transfer Shrunk Normal Anim
			}

}

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
void Gin4File::Load(char const * Name)
{
	MyFile.Load(Name);

	MyFile.FindChunks("VERS",VersionChunk);
	MyFile.FindChunks("TEX4",TexNamesChunk);
	MyFile.FindChunks("MAT4",MatChunk);
	MyFile.FindChunks("MOD4",Mod4Chunk);
	MyFile.FindChunks("MESH",MshChunk);
	MyFile.FindChunks("PTS4",PointsChunk);
	MyFile.FindChunks("POLY",TrisChunk);
	MyFile.FindChunks("VCOL",VColTrisChunk);
	MyFile.FindChunks("MAP4",UVtrisChunk);
	MyFile.FindChunks("TREE",SceneTreeChunk);
	MyFile.FindChunks("ANIM",AnimTreeChunk);
	MyFile.FindChunks("KEY4",KeyAnimTreeChunk);
	MyFile.FindChunks("BONE",SkinTreeChunk);
	MyFile.FindChunks("PROP",UserPropChunk);
	MyFile.FindChunks("CAM4",CameraChunk);
}


/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
void	CScene::Load(const char *Name)
{

Gin4File	GinFile;
	FileName=Name;
	GinFile.Load(Name);

std::vector<GinChunk  const *> &VersionChunk=GinFile.GetVersionChunk();
std::vector<GinChunk  const *> &TexNamesChunk=GinFile.GetTexNamesChunk();
std::vector<GinChunk  const *> &MatChunk=GinFile.GetMatChunk();
std::vector<GinChunk  const *> &MshChunk=GinFile.GetMshChunk();
std::vector<GinChunk  const *> &PointsChunk=GinFile.GetPointsChunk();
std::vector<GinChunk  const *> &TrisChunk=GinFile.GetTrisChunk();
std::vector<GinChunk  const *> &VColTrisChunk=GinFile.GetVColTrisChunk();
std::vector<GinChunk  const *> &UVtrisChunk=GinFile.GetUVtrisChunk();
std::vector<GinChunk  const *> &SceneTreeChunk=GinFile.GetSceneTreeChunk();
std::vector<GinChunk  const *> &AnimTreeChunk=GinFile.GetAnimTreeChunk();
std::vector<GinChunk  const *> &KeyAnimTreeChunk=GinFile.GetKeyAnimTreeChunk();
std::vector<GinChunk  const *> &SkinTreeChunk=GinFile.GetSkinTreeChunk();
std::vector<GinChunk  const *> &UserPropChunk=GinFile.GetUserPropChunk();
std::vector<GinChunk  const *> &CameraChunk=GinFile.GetCameraChunk();

int	Node,NodeCount;


// Build Scene Tree
CGinTree	*GT = (CGinTree*)SceneTreeChunk[0];
std::vector<CNode>	const &GinTree=GT->GetTree();

	NodeCount=GinTree.size();
	for (Node=0;Node<NodeCount;Node++)
		{
		CNode	ThisNode=GinTree[Node];
		SceneTree[0].AddChild(SceneTree,ThisNode,ThisNode.ParentIdx);
		}

// Build Node Mtx's
	for (Node=0;Node<NodeCount;Node++)
		{
		CNode	*ThisNode=&SceneTree[Node];
		int		ParentIdx=ThisNode->ParentIdx;
		ThisNode->Mtx.Identity();
// Build Node Mtx's
		if (ParentIdx!=-1)
			{
// LocalMtx
Matrix4x4	ThisMtx;
CNode		*ParentNode=&SceneTree[ParentIdx];		
			ThisMtx.Identity();
			ThisNode->Ang.ToMatrix(ThisMtx);
			ThisMtx.SetTranslation(ThisNode->Pos);
			ThisNode->Mtx=ThisMtx*ParentNode->Mtx;

// WorldMtx
			ThisNode->WorldMtx=GetWorldMatrix(SceneTree,ParentIdx);
			}
		}


//-----------------------------------------------------------------------------
// Load Materials
	int MatN = MatChunk.size();
	if (MatN)
	{
		CMat4 *	Materials = (CMat4*)MatChunk[0];
		AllMaterials = Materials->GetMaterials();

		if (TexNamesChunk.size())
			{
			CTexName * T=(CTexName *)(TexNamesChunk[0]);
			vector<GString> const & Strs=T->GetTexNames();
			TextureList = Strs;
			int		NumOfMeshes=MshChunk.size();
			for (int j=0; j<NumOfMeshes; j++)
				{
				CMesh *M = (CMesh*)MshChunk[j];
				vector<MeshChunk> const & Mat4Id = M->GetChunk();
				for (int i=0; i<Mat4Id.size(); i++)
					{
					if (Mat4Id[i].MatId>=0)
						{
						int tid = Materials->GetTexId(Mat4Id[i].MatId);
						if (tid >= Strs.size() || tid < 0)
							{
							GObject::Error(ERR_WARNING,"Texture index odd (asked for %d, max is %d) adjusting to 0\n",tid,Strs.size());
							tid=0;
							}
	//					else
							{
							GString const & TexFile=Strs[tid];
							/* Only add texture to list if it hasn't been on it before */
							if (find(AllTexNames.begin(),AllTexNames.end(),TexFile) == AllTexNames.end()) AllTexNames.push_back(TexFile);
							}
						}
					}
				}
			}
	}
	else
	{
		GObject::Error(ERR_WARNING,"No Materials\n");
	}

//-----------------------------------------------------------------------------
// Load Models and Meshes
	for (int m=0; m<MshChunk.size(); m++)
	{
		CMesh *M = (CMesh*)MshChunk[m];
		int CurMod = M->getObj();
		CNode	*ThisNode=GetNodePtr(GinFile.GetModelName(CurMod));

		if (loadPoints( CurMod, PointsChunk,ThisNode) != -1)
		{
			vector<MeshChunk> const &MeshChunks = M->GetChunk();
			for (int mn=0; mn<MeshChunks.size(); mn++)
			{
				if (loadTris( CurMod, MeshChunks[mn].MeshNum, TrisChunk,ThisNode)!=-1)
				{
					int MatIdx = addMaterialIdx(MeshChunks[mn].MatId);
					for (int i=0; i<MeshChunks[mn].NumFace; i++)	ThisNode->TriMaterial.push_back(MatIdx);
					loadVCol( CurMod, MeshChunks[mn].MeshNum, VColTrisChunk,ThisNode);
					loadUV(   CurMod, MeshChunks[mn].MeshNum, UVtrisChunk,ThisNode);
					int Size=ThisNode->Tris.size();
					ThisNode->UVTris.resize(Size);
					ThisNode->VColTris.resize(Size);
					ThisNode->TriMaterial.resize(Size);
				}
			}
		}
	}

//-----------------------------------------------------------------------------
// Load Anims
// Sort KeyFrame Anims
int	AnimCount=AnimTreeChunk.size();
		if (AnimCount)
		{
CKeyAnimTree			*KATC= (CKeyAnimTree*)KeyAnimTreeChunk[0];
CAnimTree				*NATC = (CAnimTree*)AnimTreeChunk[0];
std::vector<CNode>const &KeyAnimTree=KATC->GetTree();
std::vector<CNode>const &AnimTree=NATC->GetTree();

		NodeCount=KeyAnimTree.size();
		for (Node=0;Node<NodeCount;Node++)
		{
			LoadAndShrinkAnim(KeyAnimTree[Node],AnimTree[Node],SceneTree[Node]);
		}
		}


//-----------------------------------------------------------------------------
// Load Camera(s)
int	CamCount=CameraChunk.size();

		if (CamCount)
		{
		for (int Cam=0;Cam<CamCount;Cam++)
			{
			CCamera		*CamC= (CCamera	*)CameraChunk[Cam];
			Camera.push_back(CamC->GetCam());
			}
		}
	

//-----------------------------------------------------------------------------
// Load Skin Weight
	if (SkinTreeChunk.size())
	{
CSkinTree	*ST = (CSkinTree*)SkinTreeChunk[0];
std::vector<CNode>	const &SkinTree=ST->GetTree();
	NodeCount=SkinTree.size();
	for (Node=0;Node<NodeCount;Node++)
		{
		int WeightCount=SkinTree[Node].Weights.size();
		if (WeightCount)
			{
			SceneTree[Node].Weights.resize(WeightCount);
			for (int Weight=0;Weight<WeightCount;Weight++) SceneTree[Node].Weights[Weight]=SkinTree[Node].Weights[Weight];
			}
		}
	}

//-----------------------------------------------------------------------------
// Load User Props - Loaded as Ini File
/*
int		PropCount=UserPropChunk.size();
		for (int Loop=0;Loop<PropCount;Loop++)
			{
			CUserProp	*UP=(CUserProp*)UserPropChunk[Loop];
			if (UP)
				{
				CNode	*ThisNode=GetNodePtr(UP->GetModNum()+1);	// Skip SceneRoot);
				if (ThisNode)
					{
					std::vector<char> const & Prop=UP->GetUserProp();
					int Size=Prop.size()-1;
						if (Size>0)
						{

						char *PropPtr=(char*)&Prop[0];
						ThisNode->UserProp.Import(PropPtr);
						}
					}
				}
			}
*/
//-----------------------------------------------------------------------------
// Build Pruned Tree
		ResetPruneTree();
		SetPruneNodes(0);
		BuildPruneTree(0,-1);

//-----------------------------------------------------------------------------
// Print Trees

//	PrintSceneTree();
//	PrintPruneTree();

		
}

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
void	CScene::PrintTreeNode(int Idx,const int Tree)
{
//#ifdef	_CONSOLE
CNode		&Node=SceneTree[Idx];
int			NodeIdx,NodeParentIdx;
std::vector<int>	ChildList;
	if (Tree==0)
		{
		ChildList=Node.ChildList;
		NodeIdx=Node.Idx;
		NodeParentIdx=Node.ParentIdx;
		}
	else
		{
		ChildList=Node.PruneChildList;
		NodeIdx=Node.PruneIdx;
		NodeParentIdx=Node.PruneParentIdx;
		}

int		ChildCount=ChildList.size();

		for (int Sp=0;Sp<PrintTreeSpace;Sp++) printf("| "); 
		printf("(P=%i I=%i C=%i) %s - T=%i, V=%i A=%i\n",NodeParentIdx,NodeIdx,ChildCount,Node.Name,Node.Tris.size(),Node.Pts.size(),Node.Anim.size());
		PrintTreeSpace++;

		for (int Child=0;Child<ChildCount;Child++) PrintTreeNode(ChildList[Child],Tree);
		PrintTreeSpace--;
//#endif
}

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
int CScene::loadPoints( int CurMod, vector<GinChunk  const *> &Points ,CNode *ThisNode)
{
	for (int f=0; f<Points.size(); f++)
		{
		CPts4 * T;

		T=(CPts4 *)(Points[f]);

		if (T->GetModNum() == CurMod)
			{
			std::vector<Vector3> const & ThesePts=T->GetPts();
			int Size= ThesePts.size();
			ThisNode->Pts.resize(Size);
			ThisNode->RelPts.resize(Size);

Matrix4x4	IMtx=ThisNode->Mtx;
			IMtx.Invert();
			
			for (int g=0;g<Size;g++)
				{
/*Abs Pnt*/		ThisNode->Pts[g] = ThesePts[g];
/*Rel Pnt*/		ThisNode->RelPts[g]=IMtx*ThesePts[g];
				}
			return (Size);
			}
		}
	  return -1;
}

/*****************************************************************************/
int CScene::loadTris( int CurMod, int CurMesh, vector<GinChunk  const *> &Tris,CNode *ThisNode)
{
	for (int i=0; i<Tris.size(); i++)
		{
		CPoly * T;

		T=(CPoly *)(Tris[i]);
		if (T->GetModNum() == CurMod && T->GetMeshNum() == CurMesh)
			{
			std::vector<sGinTri> const & TheseTris=T->GetTris();
			int Size = TheseTris.size();

			ThisNode->TriBase=ThisNode->Tris.size();

			for (int j=0; j<Size; j++) ThisNode->Tris.push_back(TheseTris[j]);
			return (Size);
			}
		}
	  return -1;
}

/*****************************************************************************/
int CScene::loadUV( int CurMod, int CurMesh, vector<GinChunk  const *> &UVTris,CNode *ThisNode)
{
	for (int i=0; i<UVTris.size(); i++)
		{
		CUVtri *T;
		T = (CUVtri*)UVTris[i];
		if (T->GetModNum() == CurMod && T->GetMeshNum() == CurMesh)
			{
			std::vector<sUVTri> const & TheseTris=T->GetUVTris();
			int Size = TheseTris.size();

			for (int j=0; j<Size; j++) ThisNode->UVTris.push_back(TheseTris[j]);
			return (Size);
			}
		}
	return -1;
}

/*****************************************************************************/
int CScene::loadVCol( int CurMod, int CurMesh, vector<GinChunk  const *> &VColTris,CNode *ThisNode)
{
	for (int i=0; i<VColTris.size(); i++)
		{
		CVcol *T;
		T = (CVcol*)VColTris[i];
		if (T->GetModNum() == CurMod && T->GetMeshNum() == CurMesh)
			{
			std::vector<sVColTri> const & TheseTris=T->GetVcolTris();
			int Size = TheseTris.size();

			for (int j=0; j<Size; j++) ThisNode->VColTris.push_back(TheseTris[j]);
			return (Size);
			}
		}
	return -1;
}


/*****************************************************************************/
int CScene::addMaterialIdx( int idx )
{
	int c = UsedMaterials.size();
	for (int i=0; i<c; i++)
		{
		if (UsedMaterials[i] == idx) return i;
		}
	UsedMaterials.push_back(idx);
	return c;
}

/*****************************************************************************/
static GString GetNameAndExtOnly(char const * Name)
{
	GString	Rep(Name);

	Rep.Filter("/",'\\');

	GFName		FName(Rep);

	Rep=FName.File();
	Rep+=".";
	Rep+=FName.Ext();

	Rep.Lower();

	return(Rep);
}


/*****************************************************************************/
void CScene::GetNonSharedTextures(vector<GString> const & SharedTextures,vector<GString> & Dest)
{
	for (int f=0;f<AllTexNames.size();f++)
		{
		bool		Found;

		Found=false;

		GString	NameOnly(GetNameAndExtOnly(AllTexNames[f]));

		for (int t=0;t<SharedTextures.size() && !Found;t++)
			{
			GString TexNameOnly(GetNameAndExtOnly(SharedTextures[t]));
			Found=TexNameOnly==NameOnly;
			}

		if (!Found)
			{
			GFName	TexName(AllTexNames[f]);
			GFName	GinName(FileName);
			GinName.File(TexName.File());
			GinName.Ext(TexName.Ext());
			GString FinalName(GinName.FullName());
			
			FinalName.Lower();

			Dest.push_back(FinalName);
			}
		}
}

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/

Matrix4x4	GetWorldMatrix(std::vector<CNode> const &Tree,int Idx)
{
CNode		ThisNode=Tree[Idx];
int			ParentIdx=ThisNode.ParentIdx;
Matrix4x4	ParentMtx, ThisMtx, PosMtx, RotMtx, StrMtx, SclMtx, IStrMtx;

		ParentMtx.Identity();

		if (ParentIdx!=-1) ParentMtx=GetWorldMatrix(Tree,ParentIdx);

// Pos
		PosMtx.Identity(); 
		PosMtx.SetTranslation(ThisNode.Pos);
		
// Rot
		ThisNode.Ang.ToMatrix(RotMtx);

// Stretch
		ThisNode.apu.ToMatrix(StrMtx);
		IStrMtx=StrMtx;
		IStrMtx.Invert();
// Scale
		SclMtx.Identity(); 
		SclMtx.ApplyScale(ThisNode.apk);
		
	ThisMtx= PosMtx*RotMtx*StrMtx*SclMtx*IStrMtx;

	return(ParentMtx*ThisMtx);
}

//----------------------------------------------------------------------------
Vector3	GetWorldPos(std::vector<CNode> const &Tree,int Idx)
{
CNode	ThisNode=Tree[Idx];
Matrix4x4	WorldMtx=GetWorldMatrix(Tree,ThisNode.ParentIdx);

		return(WorldMtx*ThisNode.Pos);
}

//----------------------------------------------------------------------------
Vector3	GetWorldPos(Matrix4x4 &WorldMtx,Vector3 &ThisPos)
{
	return(WorldMtx*ThisPos);
}

Vector3	CNode::GetWorldPos()
{

	return(::GetWorldPos(WorldMtx,Pos));
}
