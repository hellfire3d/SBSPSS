#include "max.h"
#include "asciiexp.h"

#define EDITMESH_CLASS_ID			0x00050

typedef Tab<Point3> Point3Tab;
//typedef Tab<AnimPoint3> AnimPoint3Tab;
typedef Tab<Face> FaceTab;

class XFormProc {
	public:
		virtual Point3 proc(Point3& p, Matrix3 &mat, Matrix3 &imat)=0;
		virtual void SetMat( Matrix3& mat ) {}
	};

class MoveXForm : public XFormProc {
	private:
		Point3 delta, tdelta;		
	public:
		Point3 proc(Point3& p, Matrix3 &mat, Matrix3 &imat) 
			{ return p + tdelta; }
		void SetMat( Matrix3& mat ) 
			{ tdelta = VectorTransform(Inverse(mat),delta); }
		MoveXForm(Point3 d) { delta = d; }
	};

class RotateXForm : public XFormProc {
	private:
		Matrix3 rot, trot;
	public:
		Point3 proc(Point3& p, Matrix3 &mat, Matrix3 &imat) 
			{ return (trot*p)*imat; }
		void SetMat( Matrix3& mat ) 
			{ trot = mat * rot; }
		RotateXForm(Quat q) { q.MakeMatrix(rot); }
	};

class ScaleXForm : public XFormProc {
	private:
		Matrix3 scale, tscale;
	public:
		Point3 proc(Point3& p, Matrix3 &mat, Matrix3 &imat) 
			{ return (p*tscale)*imat; }
		void SetMat( Matrix3& mat ) 
			{ tscale = mat*scale; }
		ScaleXForm(Point3 s) { scale = ScaleMatrix(s); }
	};


class VertexDelta {
	public:
		Point3Tab	deltas;  //DS-- I needed this public		
		BitArray	hide;

		void SetSize(int size, BOOL clear=TRUE);
		Point3 operator[](int i);
		void Set(TimeValue t,int i, Point3& p);
		void Move(TimeValue t,int i, const Point3& p);
		void Apply(TimeValue t,Mesh& mesh);
		void Scale(float f);
		VertexDelta& operator=(VertexDelta& from);

		~VertexDelta();
		VertexDelta();
	};


class TopoOp {
	public:
		virtual void Apply(Mesh& mesh)=0;
		virtual TopoOp *Clone()=0;
		virtual ~TopoOp() {}
	};

typedef Tab<TopoOp*> TopoOpTab;

class FaceMap {
	public:
		DWORD v[3];
	};
typedef Tab<FaceMap> FaceMapTab;


// Bits for attribs:
// First 3 bits are edge visibility
// 4th bit is face visibility
// 29,30 and 31 indicate which if any should be applied
#define ATTRIB_APPLY_EDGE		(1<<31)
#define ATTRIB_APPLY_FACE		(1<<30)
#define ATTRIB_APPLY_MATID		(1<<29)
#define ATTRIB_APPLY_SMGROUP	(1<<28)

// Mat ID takes bit 5-21
#define ATTRIB_MATID_SHIFT	5
#define ATTRIB_MATID_MASK	0xffff


class TopoDelta {
	public:				
		DWORDTab cverts;			// Clone verts
		DWORDTab dverts;			// Delete verts		
		Point3Tab nverts;			// Add verts
		Point3Tab nTverts;			// Add texture verts
		DWORDTab dfaces;			// Delete faces
		Tab<Face> nfaces;			// New faces
		Tab<TVFace> nTVfaces;		// New texture vert faces
		FaceMapTab map;				// Remap faces
		FaceMapTab tmap;			// TVFace remap
		DWORDTab attribs;			// Changes attributes of a face.
		DWORDTab smgroups;			// Changes smooth groups for a face.
		DWORD inputFaces; 			// Number of faces on input mesh
		DWORD inputVerts;			// Number of vertices input mesh has
		
	};

class EditMeshData : public LocalModData {
	public:
		DWORD flags;
		
		// These record the changes to the incomming object.
		VertexDelta	vdelta;
		TopoDelta	tdelta;

		// The selection sets.
		BitArray vsel;
		BitArray fsel;		
		BitArray esel;

		// Lists of named selection sets
		NamedSelSetList vselSet;
		NamedSelSetList fselSet;
		NamedSelSetList eselSet;

		// While an object is being edited, this exists.
//		EMTempData *tempData;
};

inline float BPoly4(int i, float u)
	{
	float s = 1.0f-u;
	switch (i) {
		case 0: return s*s*s;
		case 1: return 3.0f*u*s*s;
		case 2: return 3.0f*u*u*s;
		case 3: return u*u*u;
		default: return 0.0f;
		}
	}
inline float BPoly3(int i, float u)
	{
	float s = 1.0f-u;
	switch (i) {
		case 0: return s*s;
		case 1: return 2.0f*u*s;
		case 2: return u*u;
		default: return 0.0f;
		}
	}
inline float BPoly2(int i, float u)
	{	
	switch (i) {
		case 0: return 1.0f-u;
		case 1: return u;
		default: return 0.0f;
		}
	}

#define EPSILON	0.001f


Point3 FfdLatticeSize(Box3 box)
	{
	Point3 size = box.Max()-box.Min();
	if (size.x==0.0f) size.x = 0.001f;
	if (size.y==0.0f) size.y = 0.001f;
	if (size.z==0.0f) size.z = 0.001f;
	return size;
	}

Matrix3 FFDMod::CompTM(TimeValue t,INode *inode,ModContext *mc)
	{
	// Compute a the transformation out of lattice space into world space	
	Matrix3 ntm = inode->GetObjTMBeforeWSM(t);
	Matrix3 ctm(1);
	if (mc && mc->tm) {
		ntm = Inverse(*(mc->tm)) * ntm;
		}
	tmControl->GetValue(t,&ctm,FOREVER,CTRL_RELATIVE);
	return ctm * ntm;
	}

Box3 ComputeBBox(TimeValue t, INode *node) {
	Box3 box; // The computed box
	Matrix3 mat; // The Object TM
	Matrix3 sclMat(1); // This will be used to apply the scaling
	// Get the result of the pipeline at the current time
	Object *obj = node->EvalWorldState(t).obj;
	// Determine if the object is in world space or object space
	// so we can get the correct TM.  We can check this by getting

	if (node->GetObjTMAfterWSM(t).IsIdentity()) {
		mat = Inverse(node->GetObjTMBeforeWSM(t));

	} else {
		// It's in object space, get the Object TM.
		mat = node->GetObjectTM(t);
	}
	// Extract just the scaling part from the TM
	AffineParts parts;
	decomp_affine(mat, &parts);
	//ApplyScaling(sclMat, ScaleValue(parts.k*parts.f, parts.u));
	// Get the bound box, and affect it by just
	// the scaling portion
	obj->GetDeformBBox(t, box, &sclMat);
	return box;
}

void AsciiExp::ExportFFD( INode* node , FFDMod *FfdMod, ModContext *mc, Mesh *mesh)
{
	TimeValue	start = ip->GetAnimRange().Start();
	TimeValue	end = ip->GetAnimRange().End();
	int			delta = GetTicksPerFrame();
	int			nbCtrlPnts;
	TimeValue	t;
	int			i;
	int			Depth;
	int			Width;
	int			Height;

	if (node->IsNodeHidden())	
		return;

	if (FfdMod) {

		ObjectState	os;
		Object *	obj;

		os = node->EvalWorldState( 0 );
		obj = node->GetObjectRef();
		while (obj && obj->SuperClassID() == GEN_DERIVOB_CLASS_ID) {
			obj = ((IDerivedObject*)obj)->GetObjRef();
		}

		if (!obj)				return;

		if (FfdMod->ClassID() == FFD44_CLASS_ID) {
			Depth = Width = Height = 4;
		} else
		if (FfdMod->ClassID() == FFD33_CLASS_ID) {
			Depth = Width = Height = 3;
		} else
		if (FfdMod->ClassID() == FFD22_CLASS_ID) {
			Depth = Width = Height = 2;
		}

		// Evaluate the TM controller
		Matrix3 ctm(1);
		FfdMod->tmControl->GetValue(start,&ctm,FOREVER,CTRL_RELATIVE);
		
		// Get the ModContext TM (if there is one)
		Matrix3 tm  = mc->tm ? *mc->tm : Matrix3(1);
		
		// Apply our TM to the MC TM
		tm *= Inverse(ctm);
		
		// The origin of the TM is the lower left corner of the
		// box, not the center.
		tm.SetTrans(tm.GetTrans()-mc->box->Min());
		
		// Compute scale factors to normalize lattice space
		// to the extents of the box.
		Point3 s = FfdLatticeSize(*mc->box);	

		for (i=0; i<3; i++) {
			if (s[i]==0.0f) s[i] = 1.0f;
			else s[i] = 1.0f/s[i];
			}
		tm.Scale(s,TRUE);

		Matrix3 itm = Inverse(tm);

		AffineParts	ap;
		decomp_affine(tm, &ap);

		nbCtrlPnts = FfdMod->NumCtrlPts();
		WriteChunkHdr( (char*)FFD_ID, 0);
		fwrite( &nCurObj, sizeof( Uint32 ), 1, expStream );
		fwrite( &FfdNum, sizeof(Uint32), 1, expStream);
		fwrite( &Width, sizeof( Uint32 ), 1, expStream );
		fwrite( &Height, sizeof( Uint32 ), 1, expStream );
		fwrite( &Depth, sizeof( Uint32 ), 1, expStream );
		
		MRow *mr = tm.GetAddr();

		fwrite( &mr[0][0], sizeof(MRow),1,expStream);
		fwrite( &mr[1][0], sizeof(MRow),1,expStream);
		fwrite( &mr[2][0], sizeof(MRow),1,expStream);
		fwrite( &mr[3][0], sizeof(MRow),1,expStream);

		int dd,dw,dh;
		int nv = obj->NumPoints();

/*		for (i=0; i<nv; i++) {
			float tw = 0.f;
			for (dw = 0; dw<Width; dw++) {
				for (dd = 0; dd<Depth; dd++) {
					for (dh = 0; dh<Height; dh++) {
						float w;
						Point3 pp = tm*obj->GetPoint(i);
						if (pp.x>=-0.001f && pp.x<=1.001f && pp.y>=-0.001f && pp.y<=1.001f && pp.z>=-0.001f && pp.z<=1.001f) {
							if (FfdMod->ClassID() == FFD44_CLASS_ID) {
								w = BPoly4(dw,pp.x) * BPoly4(dd,pp.y) * BPoly4(dh,pp.z);
							} else
							if (FfdMod->ClassID() == FFD33_CLASS_ID) {
								w = BPoly3(dw,pp.x) * BPoly3(dd,pp.y) * BPoly3(dh,pp.z);
							} else
							if (FfdMod->ClassID() == FFD22_CLASS_ID) {
								w = BPoly2(dw,pp.x) * BPoly2(dd,pp.y) * BPoly2(dh,pp.z);
							}
						} else {
							w = 0;
						}
						tw+=w;
					}
				}
			}
		}*/
		fprintf( tempStream, "node = %s\n\n", node->GetName() );
		fprintf( tempStream, "selected = ");
		BitArray Selection = mesh->VertexTempSel();
		for (dw = 0; dw<Width; dw++) {
			for (dd = 0; dd<Depth; dd++) {
				for (dh = 0; dh<Height; dh++) {
					for (i=0; i<nv; i++) {
						float w;
						Point3 pp = tm*obj->GetPoint(i);
						//if (pp.x>=-0.0001f && pp.x<=1.0001f && pp.y>=-0.0001f && pp.y<=1.0001f && pp.z>=-0.0001f && pp.z<=1.0001f) {
						if (Selection[i])
						{
							if (FfdMod->ClassID() == FFD44_CLASS_ID) {
								w = BPoly4(dw,pp.x) * BPoly4(dd,pp.y) * BPoly4(dh,pp.z);
							} else
							if (FfdMod->ClassID() == FFD33_CLASS_ID) {
								w = BPoly3(dw,pp.x) * BPoly3(dd,pp.y) * BPoly3(dh,pp.z);
							} else
							if (FfdMod->ClassID() == FFD22_CLASS_ID) {
								w = BPoly2(dw,pp.x) * BPoly2(dd,pp.y) * BPoly2(dh,pp.z);
							}
							fprintf( tempStream, "%d %f, ", i, w);
						} else {
							w = 0;
						}
						fwrite(&w, sizeof(float), 1, expStream);
					}
					fprintf( tempStream, "\nendselected\n");

				}
			}
		}


		WriteChunkHdr( (char*)ANIM_ID, 0);
		fwrite( &CHANNEL_TYPE_FFD, sizeof( Uint32 ), 1, expStream );
		fwrite( &nCurObj, sizeof( Uint32 ), 1, expStream );
		fwrite( &totFrames, sizeof( Uint32 ), 1, expStream );
		fwrite( &FfdNum, sizeof(Uint32), 1, expStream);
		fwrite( &Width, sizeof( Uint32 ), 1, expStream );
		fwrite( &Height, sizeof( Uint32 ), 1, expStream );
		fwrite( &Depth, sizeof( Uint32 ), 1, expStream );
		for (i=0; i<nbCtrlPnts; i++) {
			for (t=start; t<=end; t+=delta) {

				Matrix3 ntm = node->GetObjTMBeforeWSM(t);
				Matrix3 ctm(1);
				if (mc && mc->tm) {
					ntm = Inverse(*(mc->tm)) * ntm;
					}
				FfdMod->tmControl->GetValue(t,&ctm,FOREVER,CTRL_RELATIVE);
				Matrix3 mm = ctm * ntm;
				Box3 lbox = *mc->box;
				Matrix3 stm = ScaleMatrix(FfdLatticeSize(lbox));
				stm.SetTrans(lbox.Min());

				Point3 np = FfdMod->GetControlPoint(t,i);
				Point3 p = stm*FfdMod->GetControlPoint(t, i);
				Point3 p2 = mm * p;
				fwrite(&p2.x, sizeof(float), 1, expStream);
				fwrite(&p2.z, sizeof(float), 1, expStream);
				fwrite(&p2.y, sizeof(float), 1, expStream);
			}
		}

		TimeValue	start = ip->GetAnimRange().Start();
		TimeValue	end = ip->GetAnimRange().End();
		int			delta = GetTicksPerFrame();
		Control *c;
		WriteChunkHdr( (char*)KEY_ID, 0);
		fwrite( &CHANNEL_TYPE_FFD, sizeof( Uint32 ), 1, expStream );
		fwrite( &CHANNEL_INFO_ALL, sizeof( Uint32) , 1, expStream);
		fwrite( &nCurObj, sizeof( Uint32), 1, expStream );
		fwrite( &FfdNum, sizeof(Uint32), 1, expStream);
		fwrite( &nbCtrlPnts, sizeof( Uint32), 1, expStream );
		for (i=0; i<nbCtrlPnts; i++) {
			TimeList tl;
			IKeyControl *ikeys;
			ITCBPoint3Key tcbPosKey;
			ITCBRotKey tcbRotKey;
			ITCBScaleKey tcbSclKey;
			IBezPoint3Key bezPosKey;
			IBezQuatKey bezRotKey;
			IBezScaleKey bezSclKey;
			ILinPoint3Key linPosKey;
			ILinRotKey linRotKey;
			ILinScaleKey linSclKey;
			int num;
			TimeValue time;
			tl.Clear();

			c = FfdMod->GetPtCont(i);
			if (c) {
				ikeys = GetKeyControlInterface(c);
				int blah = c->IsKeyable();
				if (ikeys) {
					num = ikeys->GetNumKeys();
					if (num >0)
					{
						for (int j = 0; j<num; j++) {
							if (c->ClassID() == Class_ID(TCBINTERP_POINT3_CLASS_ID, 0)) {
								ikeys->GetKey(j, &tcbPosKey);
								time = tcbPosKey.time;
							} else 
							if (c->ClassID() == Class_ID(HYBRIDINTERP_POINT3_CLASS_ID, 0)) {
								ikeys->GetKey(j, &bezPosKey);
								time = bezPosKey.time;
							}
							tl.Add(time);
						}
					}
				}
			}
			if (!tl.Count()) {
				tl.Add(ip->GetAnimRange().Start());
			}
			num = tl.Count();
			
			fwrite(&num, sizeof(int), 1, expStream);
			for ( int j = 0; j<num; j++) {
				time = tl.Get(j);

				Matrix3 ntm = node->GetObjTMBeforeWSM(time);
				Matrix3 ctm(1);
				if (mc && mc->tm) {
					ntm = Inverse(*(mc->tm)) * ntm;
					}
				FfdMod->tmControl->GetValue(time,&ctm,FOREVER,CTRL_RELATIVE);
				Matrix3 mm = ctm * ntm;
				Box3 lbox = *mc->box;
				Matrix3 stm = ScaleMatrix(FfdLatticeSize(lbox));
				stm.SetTrans(lbox.Min());

				Point3 np = FfdMod->GetControlPoint(time,i);
				Point3 p = stm*FfdMod->GetControlPoint(time, i);
				Point3 p2 = mm * p;

				time = (time - start)/delta;
				fwrite(&time, sizeof(int), 1, expStream);
				fwrite(&p2.x, sizeof(float), 1, expStream);
				fwrite(&p2.z, sizeof(float), 1, expStream);
				fwrite(&p2.y, sizeof(float), 1, expStream);
			}
		}
	}
}

bool AsciiExp::FindFFDModifier (INode* node)
{
	// Get object from node. Abort if no object.
	Object* ObjectPtr = node->GetObjectRef();
	FfdNum = 0;
	bool Found= false;

	if (!ObjectPtr) 
		return false;

	// Is derived object ?

	int needDel;

	while (ObjectPtr->SuperClassID() == GEN_DERIVOB_CLASS_ID)
	{
		IDerivedObject* DerivedObjectPtr = static_cast<IDerivedObject*>(ObjectPtr);
		ObjectPtr = DerivedObjectPtr->GetObjRef();
	}
	TriObject * tri = GetTriObjectFromObject(ObjectPtr, ip->GetAnimRange().Start(), needDel);
	if (!tri)	return false;

	Mesh* mesh = &tri->mesh;

	BitArray SaveVtx = mesh->vertSel;
	BitArray SaveFce = mesh->faceSel;
	BitArray SaveEdg = mesh->edgeSel;

	ObjectPtr = node->GetObjectRef();
	while (ObjectPtr->SuperClassID() == GEN_DERIVOB_CLASS_ID)
	{
		// Yes -> Cast.
		IDerivedObject* DerivedObjectPtr = static_cast<IDerivedObject*>(ObjectPtr);

		// Iterate over all entries of the modifier stack.
		int ModStackIndex = DerivedObjectPtr->NumModifiers() - 1;
		while (ModStackIndex >= 0 )
		{
			// Get current modifier.
			Modifier* ModifierPtr = DerivedObjectPtr->GetModifier(ModStackIndex);
			ModContext *modContext = DerivedObjectPtr->GetModContext(ModStackIndex);
			// Is this FFD ?
			if (ModifierPtr->ClassID() == FFD44_CLASS_ID || 
				ModifierPtr->ClassID() == FFD33_CLASS_ID ||
				ModifierPtr->ClassID() == FFD22_CLASS_ID)
			{
				// Yes -> Exit.
				ExportFFD(node, (FFDMod*)ModifierPtr, modContext, mesh);
				FfdNum++;
				Found = true;
			} else
			if (ModifierPtr->ClassID() == Class_ID(EDITMESH_CLASS_ID,0))
			{
				EditMeshData *meshData = (EditMeshData*)modContext->localData;
				mesh->vertSel = meshData->vsel;
				mesh->faceSel = meshData->fsel;
				mesh->edgeSel = meshData->esel;
			}

			// Next modifier stack entry.
			ModStackIndex--;

			
		}
		ObjectPtr = DerivedObjectPtr->GetObjRef();
	}

	mesh->vertSel = SaveVtx;
	mesh->faceSel = SaveFce;
	mesh->edgeSel = SaveEdg;

	if (needDel)	delete tri;

	return Found;
}

