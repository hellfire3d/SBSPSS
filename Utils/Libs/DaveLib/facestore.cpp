/****************************************/
/*** Generic Face Compilation Storage ***/
/****************************************/
// Contains tri data with texture data


#include <ginio.h>
#include <gintex.h>
#include <math.h>

#include "FaceStore.h"
#include "TexGrab.h"
#include "DaveLib.h"
#include "PsxTypes.h"
#include <algorithm>

//***************************************************************************
Vector3 normalise(Vector3 &v)
{
	float sqmag = v.x * v.x + v.y * v.y + v.z * v.z;
	if (sqmag > 0.001f)
	{
		float mag = (float)sqrt( sqmag );
		Vector3 out;

		out.x = v.x / mag;
		out.y = v.y / mag;
		out.z = v.z / mag;

		return out;
	} else
	{
		return v;
	}
}

//***************************************************************************
Vector3 crossProduct( Vector3 &v0, Vector3 &v1, Vector3 &v2 )
{
	Vector3 dv1, dv2;
	Vector3 out;

	dv1.x = v1.x - v0.x;
	dv1.y = v1.y - v0.y;
	dv1.z = v1.z - v0.z;

	dv2.x = v2.x - v0.x;
	dv2.y = v2.y - v0.y;
	dv2.z = v2.z - v0.z;

	out.x = (dv1.z * dv2.y) - (dv1.y * dv2.z);
	out.y = (dv1.x * dv2.z) - (dv1.z * dv2.x);
	out.z = (dv1.y * dv2.x) - (dv1.x * dv2.y);

	out = normalise(out);
	return out;
}

//***************************************************************************
inline float dotProduct( Vector3 &v0, Vector3 &v1 )
{
	float dp = v0.x * v1.x + v0.y * v1.y + v0.z * v1.z;
	return dp;
}

//***************************************************************************
inline bool aprox( float x, float y )
{
	if (fabs(x-y)<0.03)
	{
		return true;
	} else
	{
		return false;
	}
}

//***************************************************************************
inline bool uvaprox( sUV &uv0, sUV &uv1 )
{
	return (aprox(uv0.u, uv1.u) && aprox(uv0.v, uv1.v));
}

//***************************************************************************
void	CFaceStore::SetTPageFlag(CFace &F,int MatFlag)
{
		
		switch (MatFlag>>3)
		{
//			case 0:		F.TPageFlag=2; break;	/* Subtractive */
			case 1:		F.TPageFlag=1; break;	/* Additive */
			default:	F.TPageFlag=0; break;
		}
}

//***************************************************************************
CFace	&CFaceStore::AddFace(vector<Vector3> const &P, const sGinTri &T, const sUVTri &uv,GString const &Tex,int MatFlag,bool ProcessTexFlag )
{
CFace	F;

		for (int i=0; i<3; i++)
		{
			F.uvs[i] = uv.p[i];
			F.vtx[i] = P[T.p[i]];
		}
		F.TPageFlag=0;
		F.OtOfs=0;
		F.TexName=Tex;
		F.Mat = -1;
		SetTPageFlag(F,MatFlag);
CFace	&NF=AddFace(F,ProcessTexFlag);
		return(NF);
}

//***************************************************************************
// All AddFace's lead to here!
CFace	&CFaceStore::AddFace(CFace &F,bool ProcessTexFlag)
{
int		ListSize=FaceList.size();
		FaceList.resize(ListSize+1);

// Process Vtx's (for Quad)
		for (int i=0; i<3; i++)
		{
			F.pts[i]=AddVtx(VtxList,F.vtx[i]);
// Limit UV's
			if (F.uvs[i].u < 0.f) F.uvs[i].u=0.f;
			if (F.uvs[i].u > 1.f) F.uvs[i].u=1.f;
			if (F.uvs[i].v < 0.f) F.uvs[i].v=0.f;
			if (F.uvs[i].v > 1.f) F.uvs[i].v=1.f;
		}

		if (ProcessTexFlag && F.Mat==-1)
		{
			F.Mat=AddTex(F.TexName);
		}
		F.Normal = crossProduct( F.vtx[0], F.vtx[1], F.vtx[2] );
		F.Avail = true;
		
		FaceList[ListSize]=F;
		return(F);
}

//***************************************************************************
void	CFaceStore::AddFaces(vector<CFace> &Faces,bool ProcessTexFlag)
{
int		ListSize=Faces.size();

		for (int i=0 ;i<ListSize ;i++)
		{
			AddFace(Faces[i],ProcessTexFlag);
		}
}

//***************************************************************************
void	CFaceStore::AddFaces(CFaceStore &Faces,bool ProcessTexFlag)
{
int		ListSize=Faces.GetFaceCount();

		for (int i=0 ;i<ListSize ;i++)
		{
			AddFace(Faces[i],ProcessTexFlag);
		}
}

//***************************************************************************
//*** Texture Stuff *********************************************************
//***************************************************************************
int		CFaceStore::AddTex(GString const &TexName)
{
		if (!TexGrab)
		{
			GObject::Error(ERR_FATAL,"TexGrab Not Defined!!\n");
		}
static	GString	LastTex;
static	int		LastIdx=-1;
vector<FileInfo> const	&TexList=TexGrab->GetTexInfoList();
int		ListSize=TexList.size();

GString	Filename=TexName;
		Filename.Lower();

		LastTex=Filename;

		for (LastIdx=0; LastIdx<ListSize; LastIdx++)
		{
			GString	const &ListName=TexList[LastIdx].GetActualFileName();
			if (ListName==Filename) return(LastIdx);
		}
// Does file exist
		if (!FindFile(TexName))
			GObject::Error(ERR_FATAL,"Texture not found %s!!\n",TexName);

		TexGrab->AddFile(Filename);
		LastIdx=ListSize;
		return(ListSize);
}

//***************************************************************************
void	CFaceStore::Process(vector<sTri> &OutTriList,vector<sQuad> &OutQuadList,vector<sVtx> &OutVtxList)
{
		BBox.XMin=+30000;
		BBox.YMin=+30000;
		BBox.XMax=-30000;
		BBox.YMax=-30000;

		Quad();
		BuildOutTriList(OutTriList,OutVtxList);
		BuildOutQuadList(OutQuadList,OutVtxList);
}

//***************************************************************************
//***************************************************************************
//***************************************************************************
void	CFaceStore::SetupUV(CFace const &In, sTri &Out)
{
vector<sTexOutInfo>	&TexInfo=TexGrab->GetTexInfo();
sTexOutInfo			&ThisTex=TexInfo[In.Mat];
ASSERT(In.Mat<TexInfo.size());

int		uv0[2],uv1[2],uv2[2];
int		XOfs=0,YOfs=0;
			
int		W = ThisTex.w - 1;
int		H = ThisTex.h - 1;

		if (ThisTex.Rotated)
		{
			Out.uv0[0] = (ThisTex.u + H) - round(In.uvs[0].v * H);
			Out.uv0[1] = (ThisTex.v + W) - round(In.uvs[0].u * W);
			Out.uv1[0] = (ThisTex.u + H) - round(In.uvs[1].v * H);
			Out.uv1[1] = (ThisTex.v + W) - round(In.uvs[1].u * W);
			Out.uv2[0] = (ThisTex.u + H) - round(In.uvs[2].v * H);
			Out.uv2[1] = (ThisTex.v + W) - round(In.uvs[2].u * W);
			ASSERT(0==1);
		} 
		else
		{
			if (ThisTex.w!=ThisTex.OrigW) printf("WW");
			if (ThisTex.h!=ThisTex.OrigH) printf("HH");
int			U=ThisTex.u;
int			V=ThisTex.v+H;

			uv0[0] = U +	round(In.uvs[0].u * W);
			uv0[1] = V -	round(In.uvs[0].v * H);
			uv1[0] = U +	round(In.uvs[1].u * W);
			uv1[1] = V -	round(In.uvs[1].v * H);
			uv2[0] = U +	round(In.uvs[2].u * W);
			uv2[1] = V -	round(In.uvs[2].v * H);

		}
		
		Out.uv0[0]=(uv0[0]-XOfs); Out.uv0[1]=(uv0[1]-YOfs);
		Out.uv1[0]=(uv1[0]-XOfs); Out.uv1[1]=(uv1[1]-YOfs);
		Out.uv2[0]=(uv2[0]-XOfs); Out.uv2[1]=(uv2[1]-YOfs);

		Out.TPage=ThisTex.Tpage;
		Out.Clut=ThisTex.Clut;
//		Out.PolyCode=GPU_PolyFT3Code;
		Out.PolyCode=GPU_PolyGT3Code;

		if (In.TPageFlag)
		{
			Out.PolyCode|=GPUCode_SemiTrans;
			if (In.TPageFlag!=1)
			{
				Out.TPage|=In.TPageFlag<<5;
			}
		}

}



//***************************************************************************
void	CFaceStore::SetupUV(CFace const &In, sQuad &Out)
{
vector<sTexOutInfo>	&TexInfo=TexGrab->GetTexInfo();
sTexOutInfo			&ThisTex=TexInfo[In.Mat];
ASSERT(In.Mat<TexInfo.size());

int		uv0[2],uv1[2],uv2[2],uv3[2];
int		XOfs=0,YOfs=0;
			
int		W = ThisTex.w - 1;
int		H = ThisTex.h - 1;

		if (ThisTex.Rotated)
		{
			Out.uv0[0] = (ThisTex.u + H) - round(In.uvs[0].v * H);
			Out.uv0[1] = (ThisTex.v + W) - round(In.uvs[0].u * W);
			Out.uv1[0] = (ThisTex.u + H) - round(In.uvs[1].v * H);
			Out.uv1[1] = (ThisTex.v + W) - round(In.uvs[1].u * W);
			Out.uv2[0] = (ThisTex.u + H) - round(In.uvs[2].v * H);
			Out.uv2[1] = (ThisTex.v + W) - round(In.uvs[2].u * W);
			ASSERT(0==1);
		} 
		else
		{
			if (ThisTex.w!=ThisTex.OrigW) printf("WW");
			if (ThisTex.h!=ThisTex.OrigH) printf("HH");
int			U=ThisTex.u;
int			V=ThisTex.v+H;

			uv0[0] = U +	round(In.uvs[0].u * W);
			uv0[1] = V -	round(In.uvs[0].v * H);
			uv1[0] = U +	round(In.uvs[1].u * W);
			uv1[1] = V -	round(In.uvs[1].v * H);
			uv2[0] = U +	round(In.uvs[2].u * W);
			uv2[1] = V -	round(In.uvs[2].v * H);
			uv3[0] = U +	round(In.uvs[3].u * W);
			uv3[1] = V -	round(In.uvs[3].v * H);

		}
		
		Out.uv0[0]=(uv0[0]-XOfs); Out.uv0[1]=(uv0[1]-YOfs);
		Out.uv1[0]=(uv1[0]-XOfs); Out.uv1[1]=(uv1[1]-YOfs);
		Out.uv2[0]=(uv2[0]-XOfs); Out.uv2[1]=(uv2[1]-YOfs);
		Out.uv3[0]=(uv3[0]-XOfs); Out.uv3[1]=(uv3[1]-YOfs);

		Out.TPage=ThisTex.Tpage;
		Out.Clut=ThisTex.Clut;
//		Out.PolyCode=GPU_PolyFT4Code;
		Out.PolyCode=GPU_PolyGT4Code;

		if (In.TPageFlag)
		{
			Out.PolyCode|=GPUCode_SemiTrans;
			if (In.TPageFlag!=1)
			{
				Out.TPage|=In.TPageFlag<<5;
			}
		}
}



//***************************************************************************
int		CFaceStore::AddVtx(vector<sVtx> &OutVtxList,Vector3 &InVtx)
{
int		ListSize=OutVtxList.size();
sVtx	ThisVtx;

		ThisVtx.vx=round(InVtx.x*Scale);
		ThisVtx.vy=round(InVtx.y*Scale);
		ThisVtx.vz=round(InVtx.z*Scale);

		for (int i=0; i<ListSize; i++)
		{
			if (OutVtxList[i]==ThisVtx) return(i);
		}

		OutVtxList.push_back(ThisVtx);
		return(ListSize);
}

//***************************************************************************
int		CFaceStore::AddVtx(vector<sVtx> &OutVtxList,sVtx &ThisVtx)
{
int		ListSize=OutVtxList.size();

		for (int i=0; i<ListSize; i++)
		{
			if (OutVtxList[i]==ThisVtx) return(i);
		}

		OutVtxList.push_back(ThisVtx);
		return(ListSize);
}

//***************************************************************************
void	CFaceStore::ParseVtx4BBox(sVtx &ThisVtx)
{
		if (BBox.XMin>+ThisVtx.vx) BBox.XMin=+ThisVtx.vx;
		if (BBox.XMax<+ThisVtx.vx) BBox.XMax=+ThisVtx.vx;
		if (BBox.YMin>+ThisVtx.vy) BBox.YMin=+ThisVtx.vy;
		if (BBox.YMax<+ThisVtx.vy) BBox.YMax=+ThisVtx.vy;
}
//***************************************************************************
void	CFaceStore::BuildOutTriList(vector<sTri> &OutTriList,vector<sVtx> &OutVtxList)
{
int		FaceCount=TriFaceList.size();
int		ListSize=OutTriList.size();
		OutTriList.resize(ListSize+FaceCount);

		for (int i=0; i<FaceCount; i++)
		{
			CFace	&InFace=TriFaceList[i];
			sTri	&OutFace=OutTriList[ListSize+i];

			OutFace.P0=AddVtx(OutVtxList,InFace.vtx[0]);
			OutFace.P1=AddVtx(OutVtxList,InFace.vtx[1]);
			OutFace.P2=AddVtx(OutVtxList,InFace.vtx[2]);

			ParseVtx4BBox(OutVtxList[OutFace.P0]);
			ParseVtx4BBox(OutVtxList[OutFace.P1]);
			ParseVtx4BBox(OutVtxList[OutFace.P2]);

// Materials and other shit
			SetupUV(InFace,OutFace);
			OutFace.OTOfs=InFace.OtOfs;

// RGB Shite
int			ZOfs=+4*Scale;
			int		Z[3];

			Z[0]=(OutVtxList[OutFace.P0].vz+ZOfs)/8;
			Z[1]=(OutVtxList[OutFace.P1].vz+ZOfs)/8;
			Z[2]=(OutVtxList[OutFace.P2].vz+ZOfs)/8;

			for (int p=0; p<3; p++)
			{
				if (Z[p]<0 ) Z[p]=0;
				if (Z[p]>15) Z[p]=15;

			}
			
			OutFace.C0=Z[0]*4;
			OutFace.C1=Z[1]*4;
			OutFace.C2=Z[2]*4;
		}

}

//***************************************************************************
void	CFaceStore::BuildOutQuadList(vector<sQuad> &OutQuadList,vector<sVtx> &OutVtxList)
{
int		FaceCount=QuadFaceList.size();
int		ListSize=OutQuadList.size();
		OutQuadList.resize(ListSize+FaceCount);

		for (int i=0; i<FaceCount; i++)
		{
			CFace	&InFace=QuadFaceList[i];
			sQuad	&OutFace=OutQuadList[ListSize+i];

			OutFace.P0=AddVtx(OutVtxList,InFace.vtx[0]);
			OutFace.P1=AddVtx(OutVtxList,InFace.vtx[1]);
			OutFace.P2=AddVtx(OutVtxList,InFace.vtx[2]);
			OutFace.P3=AddVtx(OutVtxList,InFace.vtx[3]);

			ParseVtx4BBox(OutVtxList[OutFace.P0]);
			ParseVtx4BBox(OutVtxList[OutFace.P1]);
			ParseVtx4BBox(OutVtxList[OutFace.P2]);
			ParseVtx4BBox(OutVtxList[OutFace.P3]);

// Materials and other shit
			SetupUV(InFace,OutFace);
			OutFace.OTOfs=InFace.OtOfs;
// RGB Shite
int			ZOfs=+4*Scale;
			int		Z[4];

			Z[0]=(OutVtxList[OutFace.P0].vz+ZOfs)/8;
			Z[1]=(OutVtxList[OutFace.P1].vz+ZOfs)/8;
			Z[2]=(OutVtxList[OutFace.P2].vz+ZOfs)/8;
			Z[3]=(OutVtxList[OutFace.P3].vz+ZOfs)/8;

			for (int p=0; p<4; p++)
			{
				if (Z[p]<0 ) Z[p]=0;
				if (Z[p]>15) Z[p]=15;

			}
			
			OutFace.C0=Z[0]*4;
			OutFace.C1=Z[1]*4;
			OutFace.C2=Z[2]*4;
			OutFace.C3=Z[3]*4;
		}
}

//***************************************************************************
//***************************************************************************
//***************************************************************************
int		CFaceStore::WriteTriList(FILE *File,vector<sTri> &List)
{
int		ListSize=List.size();
int		Pos=ftell(File);

		for (int i=0; i<ListSize; i++)
		{
			sTri	ThisTri=List[i];
			fwrite(&ThisTri,1,sizeof(sTri),File);
		}
		return(Pos);
}

//***************************************************************************
int		CFaceStore::WriteQuadList(FILE *File,vector<sQuad> &List)
{
int		ListSize=List.size();
int		Pos=ftell(File);

		for (int i=0; i<ListSize; i++)
		{
			sQuad	&ThisQuad=List[i];
			fwrite(&ThisQuad,1,sizeof(sQuad),File);
		}
		return(Pos);
}

//***************************************************************************
int		CFaceStore::WriteVtxList(FILE *File,vector<sVtx> &List)
{
int		ListSize=List.size();
int		Pos=ftell(File);

		for (int i=0; i<ListSize; i++)
		{
			sVtx	&ThisVtx=List[i];
			fwrite(&ThisVtx,1,sizeof(sVtx),File);
		}
		return(Pos);
}

//***************************************************************************
//***************************************************************************
//***************************************************************************
//***************************************************************************
//***************************************************************************
//***************************************************************************
//***************************************************************************
bool CFaceStore::CanConnect( int f0, int f1 )
{
	CFace &F0 = FaceList[f0];
	CFace &F1 = FaceList[f1];

	// check materials match
	if (F0.Mat != F1.Mat) return false;

	// check normals
	float dp = dotProduct( F0.Normal, F1.Normal );
	if (dp < 0.98f)	return false;

	// check pnt connections
	bool found = false;
	int  chkp00 ;
	int  chkp01 ;
	int  chkp10 ;
	int  chkp11 ;
	for (int i=0; i<3 && !found; i++)
	{
		int npi = (i + 1)%3;
		for (int j=0; j<3 && !found; j++)
		{
			int npj = (j + 1)%3;
			if (F0.pts[i] == F1.pts[npj] && F0.pts[npi] == F1.pts[j])
			{
				chkp00 = i;
				chkp01 = npi;
				chkp10 = j;
				chkp11 = npj;
				found = true;
			}
		}
	}

	if (found)
	{
		// check uv
		return (uvaprox(F0.uvs[chkp00], F1.uvs[chkp11]) && uvaprox(F0.uvs[chkp01], F1.uvs[chkp10]));
	}

	return false;
}

//***************************************************************************
int CFaceStore::GetUnconnectedPoint(int f0, int f1, int &f0p0, int &f0p1 )
{
	CFace &F0 = FaceList[f0];
	CFace &F1 = FaceList[f1];

	for (int i=0; i<3; i++)
	{
		int npi = (i + 1)%3;
		for (int j=0; j<3; j++)
		{
			int npj = (j + 1)%3;
			if (F0.pts[i] == F1.pts[npj] && F0.pts[npi] == F1.pts[j])
			{
				int cp = (j+2) % 3;
				f0p0 = F0.pts[i];
				f0p1 = F0.pts[npi];
				return cp;
			}
		}
	}

	return -1;
}

//***************************************************************************
int CFaceStore::CountFacesAttached ( int f )
{
	int count = 0;
	int c = FaceList.size();

	for (int i=0; i<c; i++)
	{
		if (FaceList[i].Avail && i!=f)
		{
			if (CanConnect(f, i))
			{
				count ++;
			}
		}
	}
	
	return count;
}

//***************************************************************************
void CFaceStore::FollowFace( int id, CFace &F )
{
	int minF = -1;
	int minC = 9999;
	int c = FaceList.size();
	for (int i=0; i<c; i++)
	{
		if (FaceList[i].Avail && id != i && CanConnect(id, i))
		{
			int fec = CountFacesAttached( i );
			if (fec < minC)
			{
				minF = i;
				minC = fec;
			}
		}
	}

	CFace &nf = FaceList[minF];

	int ptc = F.pts.size();
	int v0, v1;
	int unc = GetUnconnectedPoint( id, minF, v0, v1);

	for (int j=0; j<ptc; j++) if (F.pts[j] == v1) break;

	
	if (j != (ptc-1))
	{
		int idx = (j+1)%ptc;
		vector<int>::iterator pb, pe, pm;
		vector<sUV>::iterator ub, ue, um;
		vector<Vector3>::iterator vb, ve, vm;
		pb = F.pts.begin();	pe = F.pts.end();
		ub = F.uvs.begin();	ue = F.uvs.end();
		vb = F.vtx.begin();	ve = F.vtx.end();

		pm = pb + idx;
		um = ub + idx;
		vm = vb + idx;

		rotate(pb, pm, pe);
		rotate(ub, um, ue);
		rotate(vb, vm, ve);
	}

	for (j=0; j<ptc; j++) if (F.pts[j] == v0)	break;

	if (j != (ptc-2))
	{
		int idx = (j+1)%ptc;
		vector<int>::iterator pb, pe, pm;
		vector<sUV>::iterator ub, ue, um;
		vector<Vector3>::iterator vb, ve, vm;
		pb = F.pts.begin();	pe = F.pts.end()-1;
		ub = F.uvs.begin();	ue = F.uvs.end()-1;
		vb = F.vtx.begin();	ve = F.vtx.end()-1;

		pm = pb + idx;
		um = ub + idx;
		vm = vb + idx;

		rotate(pb, pm, pe);
		rotate(ub, um, ue);
		rotate(vb, vm, ve);
	}

	F.pts.resize(ptc+1);
	F.vtx.resize(ptc+1);
	F.uvs.resize(ptc+1);

	F.pts[ptc] = nf.pts[unc];
	F.vtx[ptc] = nf.vtx[unc];
	F.uvs[ptc] = nf.uvs[unc];
	nf.Avail = false;

	if (minC && MaxStrip>F.pts.size()) FollowFace( minF, F );
}

//***************************************************************************
bool	CFaceStore::GetFace( CFace &F )
{
int		minF = -1;
int		minC = 9999;
int		c = FaceList.size();

	for (int i=0; i<c; i++)
		{
		if (FaceList[i].Avail)
			{
			int fec = CountFacesAttached( i );
			if (fec < minC)
				{
				minF = i;
				minC = fec;
				}
			}
		}

	if (minF == -1)
		return false;

	F.Mat = FaceList[minF].Mat;
	F.pts = FaceList[minF].pts;
	F.uvs = FaceList[minF].uvs;
	F.vtx = FaceList[minF].vtx;
	FaceList[minF].Avail = false;
	F.Normal= FaceList[minF].Normal;
	F.OtOfs=FaceList[minF].OtOfs;
	F.TPageFlag=FaceList[minF].TPageFlag;

	if (minC && MaxStrip>F.pts.size()) FollowFace( minF, F );

	return true;
}

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
void	CFaceStore::Quad()
{
	if (MaxStrip==4)
	{
		bool strips = true;
		while (strips)
		{
			CFace f;
			strips = GetFace( f );
			if (strips)
			{
				if (f.pts.size() == 3)
					TriFaceList.push_back(f); 
				else
				QuadFaceList.push_back(f); 
			}
		}
	}
	else
	{	// No quadding, copy direct
		int	FaceCount=FaceList.size();
		for (int Loop=0;Loop<FaceCount;Loop++) TriFaceList.push_back(FaceList[Loop]);
	}
}

