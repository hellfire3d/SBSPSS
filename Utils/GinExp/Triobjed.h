/**********************************************************************
 *<
	FILE: triobjed.h

	DESCRIPTION:   Editable Triangle Mesh Object

	CREATED BY: Rolf Berteig

	HISTORY: created 4 March 1996

 *>	Copyright (c) 1996, All Rights Reserved.
 **********************************************************************/

#ifndef __TRIOBJED__
#define __TRIOBJED__

#include "meshadj.h"
typedef Tab<Point3> Point3Tab;

#ifdef _SUBMTLASSIGNMENT
	#include "sbmtlapi.h"
#endif

// Selection levels
#define SL_OBJECT	0
#define SL_VERTEX	1
#define SL_FACE		2
#define SL_EDGE		3

#define ALIGN_CONST	0
#define ALIGN_VIEW	1

#define DEF_PICKBOX_SIZE	4

class WeldVertCMode;
class CreateVertCMode;
class BuildFaceCMode;
class DivideEdgeCMode;
class TurnEdgeCMode;
class AttachPickMode;
class ExtrudeCMode;
class AlignFaceCMode;
class FlipNormCMode;
class CutEdgeCMode;	// SCA

#define CID_CREATEVERT	CID_USER + 973
#define CID_BUILDFACE	CID_USER + 975
#define CID_WELDVERT	CID_USER + 978
#define CID_DIVIDEEDGE	CID_USER + 979
#define CID_TURNEDGE	CID_USER + 980
#define CID_EXTRUDE		CID_USER + 990
#define CID_ALIGNFACE	CID_USER + 1000
#define CID_FLIPNORM	CID_USER + 1010
#define CID_CUTEDGE    CID_USER + 1020	// SCA

#define SELTYPE_SINGLE	1
#define SELTYPE_POLY	2
#define SELTYPE_ELEMENT	3

#define IDC_SELSINGLE	0x4010
#define IDC_SELPOLY		0x4011
#define IDC_SELELEMENT	0x4012

#define MAX_MATID	0xffff

class NamedSelSetList {
	public:
		Tab<BitArray*> sets;
		Tab<TSTR*>	   names;

		~NamedSelSetList();
		BitArray &operator[](int i) {return *sets[i];}
		int Count() {return sets.Count();}
		void AppendSet(BitArray &nset,TSTR &name);
		void DeleteSet(int i);
		IOResult Load(ILoad *iload);
		IOResult Save(ISave *isave);
		void SetSize(int size);
		NamedSelSetList& operator=(NamedSelSetList& from);
		void DeleteSetElements(BitArray &set,int m=1);
		int FindSet(TSTR &name);
	};

#ifdef _SUBMTLASSIGNMENT
class EditTriObject : public TriObject, public EventUser, public ISubMtlAPI {
#else
class EditTriObject : public TriObject, public EventUser {
#endif
	public:
		
		// Class vars
		static HWND hParams1;
		static HWND hParams2;
		static HACCEL hAccel;
		static Interface *ip;
		static MoveModBoxCMode *moveMode;
		static RotateModBoxCMode *rotMode;
		static UScaleModBoxCMode *uscaleMode;
		static NUScaleModBoxCMode *nuscaleMode;
		static SquashModBoxCMode *squashMode;
		static SelectModBoxCMode *selectMode;
		static WeldVertCMode *weldVertMode;
		static CreateVertCMode *createVertMode;
		static BuildFaceCMode* buildFaceMode;
		static TurnEdgeCMode* turnEdgeMode;
		static DivideEdgeCMode* divideEdgeMode;
		static AttachPickMode* attachPickMode;
		static ExtrudeCMode* extrudeMode;
		static AlignFaceCMode* alignFaceMode;
		static FlipNormCMode* flipMode;
		static CutEdgeCMode * cutEdgeMode;	// SCA
		static float falloff, pinch, bubble;
		static int selType;
		static float normScale;
		static BOOL selByVert;
		static BOOL inBuildFace;
		static BOOL faceUIValid;
		static BOOL inNormalMove;
		static BOOL ignoreBackfaces, ignoreBackedges;
		static BOOL ignoreVisEdge;
		static int pickBoxSize;
		static int weldBoxSize;
		static int attachMat;
		static BOOL condenseMat;
		static bool sliceSplit, cutQuadSafe, cutRefine;
		static Quat sliceRot;
		static Point3 sliceCenter;
		static float sliceSize;
		static bool sliceMode;

		// Cache for computing coord. systems
		static FaceClusterList *clust;
		static Point3Tab *centers;
		static Point3Tab *normals;
		static DWORDTab *vclust;
		static Tab<float> *distTab;
		Point3Tab &CoordCenters();
		Point3Tab &CoordNormals();
		DWORDTab &VertClusters();
		Tab<float> *EdgeDistTab(int iterations);
		void FreeDistTab();
		void FreeCoordCaches();
		void MakeClusterList();
		void InvalidateCoordCaches(BOOL selChange=FALSE);		

		NamedSelSetList vselSet;
		NamedSelSetList fselSet;
		NamedSelSetList eselSet;		
		Tab<Control*> cont;

		EditTriObject();

		// Animatable methods
		void DeleteThis() {delete this;}
		Class_ID ClassID() {return Class_ID(EDITTRIOBJ_CLASS_ID,0);}
		void GetClassName(TSTR& s) ;//{s = GetString(IDS_RB_EDITABLEMESH);}
		void BeginEditParams(IObjParam  *ip, ULONG flags,Animatable *prev);
		void EndEditParams(IObjParam *ip, ULONG flags,Animatable *next);
		int NumSubs()  {return cont.Count();}
		Animatable* SubAnim(int i);
		TSTR SubAnimName(int i);
		BOOL AssignController(Animatable *control,int subAnim);
		int SubNumToRefNum(int subNum) {return subNum;}
		BOOL SelectSubAnim(int subNum);

		// Reference methods
		int NumRefs() {return cont.Count();}
		RefTargetHandle GetReference(int i);
		void SetReference(int i, RefTargetHandle rtarg);
		void CreateContArray();
		void SynchContArray();
		//BOOL BypassTreeView();
		void DeletePointConts(BitArray &set);
		void PlugControllersSel(TimeValue t,BitArray &set);
		BOOL PlugControl(TimeValue t,int i);
		void SetPointAnim(TimeValue t, int i, Point3 pt);
		BOOL CloneVertCont(int from, int to);
		RefResult NotifyRefChanged(Interval changeInt, RefTargetHandle hTarget, PartID& partID, RefMessage message);

		// BaseObject methods
		ObjectState Eval(TimeValue time);
		void Transform(TimeValue t, Matrix3& partm, Matrix3 tmAxis, BOOL localOrigin, Matrix3 xfrm, BOOL ar, int type);
		void Move(TimeValue t, Matrix3& partm, Matrix3& tmAxis, Point3& val, BOOL localOrigin=FALSE);
		void Rotate(TimeValue t, Matrix3& partm, Matrix3& tmAxis, Quat& val, BOOL localOrigin=FALSE);
		void Scale(TimeValue t, Matrix3& partm, Matrix3& tmAxis, Point3& val, BOOL localOrigin=FALSE);
		void TransformStart(TimeValue t) {if (ip) ip->LockAxisTripods(TRUE);}
		void TransformFinish(TimeValue t) {if (ip) ip->LockAxisTripods(FALSE); InvalidateCoordCaches(); FreeDistTab();}
		void TransformCancel(TimeValue t) {if (ip) ip->LockAxisTripods(FALSE); FreeDistTab();}
		int Display(TimeValue t, INode* inode, ViewExp *vpt, int flags);
		void GetWorldBoundBox (TimeValue t, INode * inode, ViewExp* vp, Box3& box);
		void GetLocalBoundBox (TimeValue t, INode* inode, ViewExp* vp, Box3& box);
		int HitTest(TimeValue t, INode* inode, int type, int crossing, int flags, IPoint2 *p, ViewExp *vpt, ModContext* mc);
		void CloneSelSubComponents(TimeValue t);
		void AcceptCloneSelSubComponents(TimeValue t);
		void SelectSubComponent(
			HitRecord *hitRec, BOOL selected, BOOL all, BOOL invert=FALSE);
		void ClearSelection(int selLevel);
		void SelectAll(int selLevel);
		void InvertSelection(int selLevel);
		void ActivateSubobjSel(int level, XFormModes& modes );		
		void GetSubObjectCenters(SubObjAxisCallback *cb,TimeValue t,INode *node,ModContext *mc);
		void GetSubObjectTMs(SubObjAxisCallback *cb,TimeValue t,INode *node,ModContext *mc);		

		// Object methods		
		TCHAR *GetObjectName() ;//{ return GetString(IDS_RB_EDITABLEMESH);}
		BOOL IsSubClassOf(Class_ID classID);

		BOOL SupportsNamedSubSels() {return TRUE;}
		void ActivateSubSelSet(TSTR &setName);
		void NewSetFromCurSel(TSTR &setName);
		void RemoveSubSelSet(TSTR &setName);
		void SetupNamedSelDropDown();
		int NumNamedSelSets();
		TSTR GetNamedSelSetName(int i);
		void SetNamedSelSetName(int i,TSTR &newName);
		void NewSetByOperator(TSTR &newName,Tab<int> &sets,int op);

		// Reference methods
		RefTargetHandle Clone(RemapDir& remap = NoRemap());
		IOResult Load(ILoad *iload);
		IOResult Save(ISave *isave);

		// From EventUser
		void Notify() {DeleteSelected();/*delete key was pressed*/}

		// Local methods
		void VertsHaveBeenDeleted(BitArray &set);
		NamedSelSetList &GetSelSet();
		void KeyboardAccelerator(int id);
		void RemoveRollupPages();
		void SetRollupPages();
		int GetSubobjectLevel();
		void SetSubobjectLevel(int level);
		void SetSelType(int type);
		void HoldVerts();
		void HoldTopology();
		void DeleteSelected();
		Point3 CalcRegionAffect(Point3 v,Point3 center,Point3 delta);
		Point3 CalcRegionAffect(float dist,Point3 delta);		
		void DeleteVertSet(BitArray set);
		void DeleteFaceSet(BitArray set, BitArray *isoVert);		
		void HideSelectedVerts();
		void UnhideAllVerts();
		void HideSelectedFaces();
		void UnhideAllFaces();
		BOOL WeldSelectedVerts(float thresh);
		void WeldSelectedVerts(Point3 pt);
		void CollapseSelectedVerts();
		void CollapseSelFaces();
		void CollapseSelEdges();
		void WeldVertSet(BitArray set,Point3 *weldPoint=NULL);
		void AddNewVertex(Point3 pt);
		void BuildNewFace(int *v);
		void MakePlanar();
		float GetPolyFaceThesh();
		HitRecord *VertHitsToFaceHits(HitRecord *hitRec,BOOL all);
		DWORD GetSelSmoothBits(DWORD &invalid);
		void SetSelSmoothBits(DWORD bits,DWORD which);		
		void SelectBySmoothGroup(DWORD bits,BOOL clear);
		void AutoSmooth(float thresh);
		DWORD GetUsedSmoothBits();
		int GetSelMatIndex();
		void SetSelMatIndex(int index);
		void SelectByMat(int index,BOOL clear);
		void InvalidateSurfaceUI();
		void FlipSelNormals();
		void FlipANormal(int ix);
		void UnifySelNormals();
		void DetachVertFaces(TSTR &name,BOOL doFaces,BOOL del=TRUE,BOOL elem=FALSE);
		void ShowNormals();
		void SetSelEdgeVis(BOOL onOff);
		void AutoEdge(float angle);
		void AttachObject(INode *node);
		void MultiAttachObject(INodeTab &nodeTab);
		void TesselateFaces(float tens,BOOL edge);
		void ExplodeFaces(float thresh, BOOL objs, TSTR &name);
		void DoExtrude();
		void StartExtrudeMode();
		void BeginNormalMove(TimeValue t);
		void EndNormalMove(TimeValue t,BOOL accept);
		void NormalMove(TimeValue t, float amount);
		void CloneSelVerts();
		void CloneSelFaces(BitArray &set);
		void BreakVerts();
		void RemoveIsoVerts();
		void AlignVertsTo(int which);
		void AlignFacesTo(int which,DWORD f);
		Matrix3 ComputeAlignTM(int which, float &zcent);
		void SetEdgeVisCheckState();
		void NSCopy();
		void NSPaste();
		BOOL GetUniqueSetName(TSTR &name);
		int SelectNamedSet();
		void SelectByNormal(BOOL front);
		void SetNumSelLabel();
		Color GetSelVertColor();
		void SetSelVertColor(Color c);
		void SelectVertByColor(Point3 selcol,int selDeltaR,int selDeltaG,int selDeltaB,BOOL add,BOOL sub);
		BOOL AllocateVertColor();
		BOOL SplitSharedVertCol();
		void CreateCurveFromEdge(TSTR name,int curveType,BOOL ignoreHiddenEdges);
		void SelectOpenEdges();
		BOOL DoAttachMatOptionDialog();
		int DoCut (Point3 & viewNorm, DWORD e1, float prop1, DWORD e2, float prop2);	// SCA
		void Slice ();
		void EnterSliceMode (HWND hwnd);
		void ExitSliceMode (HWND hwnd);

#ifdef _SUBMTLASSIGNMENT
		void*	GetInterface(ULONG id);
		MtlID	GetNextAvailMtlID();
		BOOL	HasFaceSelection();
		void	SetSelFaceMtlID(MtlID id, BOOL bResetUnsel = FALSE);
		int		GetSelFaceUniqueMtlID();
		int		GetSelFaceAnyMtlID();
		int		GetMaxMtlID();
#endif

	};

// --- Command Modes -----------------------------------------------

class ExtrudeMouseProc : public MouseCallBack {
	private:
		MoveTransformer moveTrans;
		EditTriObject *eo;
		Interface *ip;
		IPoint2 om;

	public:
		ExtrudeMouseProc(EditTriObject* o, IObjParam *i)
			: moveTrans(i) {eo=o;ip=i;}
		int proc( 
			HWND hwnd, 
			int msg, 
			int point, 
			int flags, 
			IPoint2 m );
	};

class ExtrudeSelectionProcessor : public GenModSelectionProcessor {
	protected:
		HCURSOR GetTransformCursor();
		
	public:
		ExtrudeSelectionProcessor(ExtrudeMouseProc *mc, EditTriObject *o, IObjParam *i) 
			: GenModSelectionProcessor(mc,o,i) {}
	};

class ExtrudeCMode : public CommandMode {
	private:
		ChangeFGObject fgProc;
		ExtrudeSelectionProcessor mouseProc;
		ExtrudeMouseProc eproc;
		EditTriObject* eo;

	public:
		ExtrudeCMode(EditTriObject* o, IObjParam *i) :
			fgProc(o), mouseProc(&eproc,o,i), eproc(o,i) {eo=o;}

		int Class() { return MODIFY_COMMAND; }
		int ID() { return CID_EXTRUDE; }
		MouseCallBack *MouseProc(int *numPoints) { *numPoints=2; return &mouseProc; }
		ChangeForegroundCallback *ChangeFGProc() { return &fgProc; }
		BOOL ChangeFG( CommandMode *oldMode ) { return oldMode->ChangeFGProc() != &fgProc; }
		void EnterMode();
		void ExitMode();
	};



class AttachPickMode : 
		public PickModeCallback,
		public PickNodeCallback {
	public:		
		EditTriObject* eo;
		IObjParam *ip;

		AttachPickMode(EditTriObject* o, IObjParam *i)
			{eo=o;ip=i;}
		BOOL HitTest(IObjParam *ip,HWND hWnd,ViewExp *vpt,IPoint2 m,int flags);
		BOOL Pick(IObjParam *ip,ViewExp *vpt);
		void EnterMode(IObjParam *ip);
		void ExitMode(IObjParam *ip);		
		
		BOOL Filter(INode *node);
		BOOL RightClick(IObjParam *ip,ViewExp *vpt) {return TRUE;}
		PickNodeCallback *GetFilter() {return this;}
	};


class WeldVertMouseProc : public MoveModBox {
	private:		
		EditTriObject *et;
		IObjParam *ip;		

	public:
		WeldVertMouseProc(EditTriObject* e, IObjParam *i)
			: MoveModBox(e,i) {et=e;ip=i;}		
		BOOL HitTestVerts(IPoint2 &m, ViewExp *vpt,int &v);
		int proc( 
			HWND hwnd, 
			int msg, 
			int point, 
			int flags, 
			IPoint2 m );
	};

class WeldVertSelectionProcessor : public GenModSelectionProcessor {
	protected:
		HCURSOR GetTransformCursor();
		
	public:
		WeldVertSelectionProcessor(WeldVertMouseProc *mc, Object *o, IObjParam *i) 
			: GenModSelectionProcessor(mc,o,i) {}
	};

class WeldVertCMode : public CommandMode {
	private:
		ChangeFGObject fgProc;
		WeldVertSelectionProcessor mouseProc;
		WeldVertMouseProc eproc;
		EditTriObject* et;

	public:
		WeldVertCMode(EditTriObject* mod, IObjParam *i) :
			fgProc(mod), mouseProc(&eproc,mod,i), eproc(mod,i) {et=mod;}

		int Class() { return MODIFY_COMMAND; }
		int ID() { return CID_WELDVERT; }
		MouseCallBack *MouseProc(int *numPoints) {*numPoints=2; return &mouseProc;}
		ChangeForegroundCallback *ChangeFGProc() {return &fgProc; }
		BOOL ChangeFG( CommandMode *oldMode ) {return oldMode->ChangeFGProc() != &fgProc;}
		void EnterMode();
		void ExitMode();
	};


class CreateVertMouseProc : public MouseCallBack {
	private:		
		EditTriObject *et;
		IObjParam *ip;		

	public:
		CreateVertMouseProc(EditTriObject* mod, IObjParam *i)
			{et=mod;ip=i;}
		int proc( 
			HWND hwnd, 
			int msg, 
			int point, 
			int flags, 
			IPoint2 m );
	};

class CreateVertCMode : public CommandMode {
	private:
		ChangeFGObject fgProc;		
		CreateVertMouseProc proc;
		EditTriObject* et;

	public:
		CreateVertCMode(EditTriObject* mod, IObjParam *i) 
			: fgProc(mod), proc(mod,i) {et=mod;}

		int Class() { return MODIFY_COMMAND; }
		int ID() { return CID_CREATEVERT; }
		MouseCallBack *MouseProc(int *numPoints) {*numPoints=1; return &proc;}
		ChangeForegroundCallback *ChangeFGProc() {return &fgProc;}
		BOOL ChangeFG(CommandMode *oldMode) {return oldMode->ChangeFGProc()!= &fgProc;}
		void EnterMode();
		void ExitMode();
	};


class BuildFaceMouseProc : public MouseCallBack {
	private:		
		EditTriObject *et;
		IObjParam *ip;		
		int vts[3];
		IPoint3 mpts[3];
		int pt;
		IPoint2 lm;

	public:
		BuildFaceMouseProc(EditTriObject* e, IObjParam *i);
		void DrawFace(HWND hWnd,IPoint2 &m);
		BOOL HitTestVerts(IPoint2 &m, ViewExp *vpt,int &v);
		int proc( 
			HWND hwnd, 
			int msg, 
			int point, 
			int flags, 
			IPoint2 m );
	};

class BuildFaceCMode : public CommandMode {
	private:
		ChangeFGObject fgProc;		
		BuildFaceMouseProc proc;
		EditTriObject *et;

	public:
		BuildFaceCMode(EditTriObject* e, IObjParam *i) 
			: fgProc(e), proc(e,i) {et=e;}

		int Class() { return MODIFY_COMMAND; }
		int ID() { return CID_BUILDFACE; }
		MouseCallBack *MouseProc(int *numPoints) {*numPoints=999999; return &proc;}
		ChangeForegroundCallback *ChangeFGProc() {return &fgProc;}
		BOOL ChangeFG(CommandMode *oldMode) {return oldMode->ChangeFGProc()!= &fgProc;}
		void EnterMode();
		void ExitMode();
	};


class AlignFaceMouseProc : public MouseCallBack {
	public:
		EditTriObject *et;
		IObjParam *ip;
		int which;

		AlignFaceMouseProc(EditTriObject* e, IObjParam *i)
			{et=e;ip=i;which=ALIGN_CONST;}
		HitRecord *HitTestFaces(IPoint2 &m, ViewExp *vpt);
		int proc( 
			HWND hwnd, 
			int msg, 
			int point, 
			int flags, 
			IPoint2 m );
	};

class AlignFaceCMode : public CommandMode {
	public:
		ChangeFGObject fgProc;		
		AlignFaceMouseProc proc;
		EditTriObject* et;
	
		AlignFaceCMode(EditTriObject* e, IObjParam *i) 
			: fgProc(e), proc(e,i	) {et=e;}

		int Class() { return MODIFY_COMMAND; }
		int ID() { return CID_ALIGNFACE; }
		MouseCallBack *MouseProc(int *numPoints) {*numPoints=1; return &proc;}
		ChangeForegroundCallback *ChangeFGProc() {return &fgProc;}
		BOOL ChangeFG(CommandMode *oldMode) {return oldMode->ChangeFGProc()!= &fgProc;}
		void EnterMode();
		void ExitMode();
	};


class FlipNormMouseProc : public MouseCallBack {
	public:
		EditTriObject *et;
		IObjParam *ip;		

		FlipNormMouseProc(EditTriObject* e, IObjParam *i)
			{et=e;ip=i;}
		HitRecord *HitTestFaces(IPoint2 &m, ViewExp *vpt);
		int proc( 
			HWND hwnd, 
			int msg, 
			int point, 
			int flags, 
			IPoint2 m );
	};

class FlipNormCMode : public CommandMode {
	public:
		ChangeFGObject fgProc;		
		FlipNormMouseProc proc;
		EditTriObject* et;
	
		FlipNormCMode(EditTriObject* e, IObjParam *i) 
			: fgProc(e), proc(e,i	) {et=e;}

		int Class() {return MODIFY_COMMAND;}
		int ID() {return CID_FLIPNORM;}
		MouseCallBack *MouseProc(int *numPoints) {*numPoints=1; return &proc;}
		ChangeForegroundCallback *ChangeFGProc() {return &fgProc;}
		BOOL ChangeFG(CommandMode *oldMode) {return oldMode->ChangeFGProc()!= &fgProc;}
		void EnterMode();
		void ExitMode();
	};



class PickEdgeMouseProc : public MouseCallBack {
	public:
		EditTriObject *et;
		IObjParam *ip;

		PickEdgeMouseProc(EditTriObject* e, IObjParam *i)
			{et=e;ip=i;}
		HitRecord *HitTestEdges(IPoint2 &m, ViewExp *vpt);
		int proc( 
			HWND hwnd, 
			int msg, 
			int point, 
			int flags, 
			IPoint2 m );
		virtual void EdgePick(DWORD edge)=0;
	};

class DivideEdgeProc : public PickEdgeMouseProc {
	public:
		DivideEdgeProc(EditTriObject* e, IObjParam *i) : PickEdgeMouseProc(e,i) {}
		void EdgePick(DWORD edge);
	};

class TurnEdgeProc : public PickEdgeMouseProc {
	public:
		TurnEdgeProc(EditTriObject* e, IObjParam *i) : PickEdgeMouseProc(e,i) {}
		void EdgePick(DWORD edge);
	};

class DivideEdgeCMode : public CommandMode {
	private:
		ChangeFGObject fgProc;		
		DivideEdgeProc proc;
		EditTriObject* et;

	public:
		DivideEdgeCMode(EditTriObject* e, IObjParam *i) 
			: fgProc(e), proc(e,i) {et=e;}

		int Class() { return MODIFY_COMMAND; }
		int ID() { return CID_DIVIDEEDGE; }
		MouseCallBack *MouseProc(int *numPoints) {*numPoints=1; return &proc;}
		ChangeForegroundCallback *ChangeFGProc() {return &fgProc;}
		BOOL ChangeFG(CommandMode *oldMode) {return oldMode->ChangeFGProc()!= &fgProc;}
		void EnterMode();
		void ExitMode();
	};

class TurnEdgeCMode : public CommandMode {
	private:
		ChangeFGObject fgProc;		
		TurnEdgeProc proc;
		EditTriObject* et;

	public:
		TurnEdgeCMode(EditTriObject* e, IObjParam *i) 
			: fgProc(e), proc(e,i) {et=e;}

		int Class() { return MODIFY_COMMAND; }
		int ID() { return CID_TURNEDGE; }
		MouseCallBack *MouseProc(int *numPoints) {*numPoints=1; return &proc;}
		ChangeForegroundCallback *ChangeFGProc() {return &fgProc;}
		BOOL ChangeFG(CommandMode *oldMode) {return oldMode->ChangeFGProc()!= &fgProc;}
		void EnterMode();
		void ExitMode();
	};

class CutEdgeProc : public MouseCallBack {	// SCA -- derived from DivideEdgeProc.
public:
	EditTriObject *et;
	IObjParam *ip;
	DWORD e1;
	bool e1set;
	float prop1;
	IPoint2 m1, oldm2;

	CutEdgeProc(EditTriObject* e, IObjParam *i) { et=e; ip=i; e1set = FALSE;}
	HitRecord *HitTestEdges(IPoint2 &m, ViewExp *vpt);
	int proc(HWND hwnd, int msg, int point, int flags, IPoint2 m );
	void DrawCutter (HWND hWnd,IPoint2 &m);
};

class CutEdgeCMode : public CommandMode {	// SCA
private:
	ChangeFGObject fgProc;	
	CutEdgeProc proc;
	EditTriObject* et;

public:
	CutEdgeCMode(EditTriObject* e, IObjParam *i) 
		: fgProc(e), proc(e,i) {et=e;}

	int Class() { return MODIFY_COMMAND; }
	int ID() { return CID_CUTEDGE; }
	MouseCallBack *MouseProc(int *numPoints) {*numPoints=20; return &proc;}
	ChangeForegroundCallback *ChangeFGProc() {return &fgProc;}
	BOOL ChangeFG(CommandMode *oldMode) {return oldMode->ChangeFGProc()!= &fgProc;}
	void EnterMode();
	void ExitMode();
};

// --- Restore objects ---------------------------------------------

class MeshSelRestore : public RestoreObj {	
	public:		   	
		BitArray undo, redo;
		EditTriObject *et;
		int level;

		MeshSelRestore(EditTriObject *et);
		void Restore(int isUndo);
		void Redo();
		TSTR Description() {return TSTR(_T("Mesh Sel"));}
	};

class MeshVertRestore : public RestoreObj {	
	public:		   	
		Tab<Point3> undo, redo;
		EditTriObject *et;		

		MeshVertRestore(EditTriObject *et);
		void Restore(int isUndo);
		void Redo();
		void EndHold() {et->ClearAFlag(A_HELD);}
		TSTR Description() {return TSTR(_T("Mesh Vert"));}
	};

class MeshVertColorRestore : public RestoreObj {	
	public:		   	
		Tab<VertColor> undo, redo;
		EditTriObject *et;		

		MeshVertColorRestore(EditTriObject *et);
		void Restore(int isUndo);
		void Redo();
		void EndHold() {et->ClearAFlag(A_HELD);}
		TSTR Description() {return TSTR(_T("Mesh Vert Color"));}
	};

class MeshTopoRestore : public RestoreObj {
	public:
		EditTriObject *et;
		Tab<Point3> uverts, rverts;
		Tab<Point3> uTverts, rTverts;
		Tab<Face> ufaces, rfaces;
		Tab<TVFace> utvFaces, rtvFaces;
		Tab<VertColor> uvertCol, rvertCol;
		Tab<TVFace> uvcFaces, rvcFaces;
		BitArray uvertSel, ufaceSel, uedgeSel, uvertHide;
		BitArray rvertSel, rfaceSel, redgeSel, rvertHide;
		BOOL undone;
		Tab<Control*> ucont, rcont;

		MeshTopoRestore(EditTriObject *et);
		void Restore(int isUndo);
		void Redo();
		void EndHold() {et->ClearAFlag(A_HELD);}
		TSTR Description() {return TSTR(_T("Mesh Topo"));}
	};

class MeshVertHideRestore : public RestoreObj {	
	public:		   	
		BitArray undo, redo;
		EditTriObject *et;		

		MeshVertHideRestore(EditTriObject *et);
		void Restore(int isUndo);
		void Redo();
		TSTR Description() {return TSTR(_T("Mesh Vert Hide"));}
	};

class MeshFaceHideRestore : public RestoreObj {	
	public:		   	
		BitArray undo, redo;
		EditTriObject *et;		

		MeshFaceHideRestore(EditTriObject *et);
		void Restore(int isUndo);
		void Redo();		
		TSTR Description() {return TSTR(_T("Mesh Face Hide"));}
	};

class MeshFaceMatRestore : public RestoreObj {	
	public:		   	
		Tab<MtlID> undo, redo;
		EditTriObject *et;		

		MeshFaceMatRestore(EditTriObject *et);
		void Restore(int isUndo);
		void Redo();
		void EndHold() {et->ClearAFlag(A_HELD);}
		TSTR Description() {return TSTR(_T("Mesh Face Mat"));}
	};

class MeshFaceSmoothRestore : public RestoreObj {	
	public:		   	
		Tab<DWORD> undo, redo;
		EditTriObject *et;		

		MeshFaceSmoothRestore(EditTriObject *et);
		void Restore(int isUndo);
		void Redo();
		void EndHold() {et->ClearAFlag(A_HELD);}
		TSTR Description() {return TSTR(_T("Mesh Face Smooth"));}
	};

class MeshEdgeVisRestore : public RestoreObj {	
	public:		   	
		Tab<DWORD> undo, redo;
		EditTriObject *et;		

		MeshEdgeVisRestore(EditTriObject *et);
		void Restore(int isUndo);
		void Redo();
		void EndHold() {et->ClearAFlag(A_HELD);}
		TSTR Description() {return TSTR(_T("Mesh Edge Vis"));}
	};

class FaceIndexRec {
	public:
		DWORD v[3], flags;
	};
class UVFaceIndexRec {
	public:
		DWORD v[3];
	};


class MeshFaceIndexRestore : public RestoreObj {	
	public:		   	
		Tab<FaceIndexRec> undo, redo;
		Tab<UVFaceIndexRec> tundo, tredo;
		EditTriObject *et;		

		MeshFaceIndexRestore(EditTriObject *et);
		void Restore(int isUndo);
		void Redo();		
		TSTR Description() {return TSTR(_T("Mesh Face Index"));}
	};

class AppendSetRestore : public RestoreObj {
	public:
		BitArray set;
		TSTR name;
		NamedSelSetList *setList;
		EditTriObject *et;

		AppendSetRestore(NamedSelSetList *sl,EditTriObject *e) {
			setList = sl; et = e;
			}   		
		void Restore(int isUndo) {
			set  = *setList->sets[setList->Count()-1];
			name = *setList->names[setList->Count()-1];
			setList->DeleteSet(setList->Count()-1);
			if (et->ip) et->ip->NamedSelSetListChanged();
			}
		void Redo() {
			setList->AppendSet(set,name);
			if (et->ip) et->ip->NamedSelSetListChanged();
			}
				
		TSTR Description() {return TSTR(_T("Append Set"));}
	};

class DeleteSetRestore : public RestoreObj {
	public:
		BitArray set;
		TSTR name;
		int index;
		NamedSelSetList *setList;
		EditTriObject *et;

		DeleteSetRestore(int i,NamedSelSetList *sl,EditTriObject *e) {
			setList = sl; et = e; index = i;
			set  = *setList->sets[index];
			name = *setList->names[index];
			}   		
		void Restore(int isUndo) {
			BitArray *n = new BitArray(set);
			TSTR *nm = new TSTR(name);
			setList->sets.Insert(index,1,&n);
			setList->names.Insert(index,1,&nm);
			if (et->ip) et->ip->NamedSelSetListChanged();
			}
		void Redo() {
			setList->DeleteSet(index);
			if (et->ip) et->ip->NamedSelSetListChanged();
			}
				
		TSTR Description() {return TSTR(_T("Delete Set"));}
	};

class SetNameRestore : public RestoreObj {
	public:
		TSTR undo, redo;
		int index;
		NamedSelSetList *setList;
		EditTriObject *et;
		SetNameRestore(int i,NamedSelSetList *sl,EditTriObject *e) {
			index = i; setList = sl; et = e;
			undo = *setList->names[index];
			}

		void Restore(int isUndo) {			
			redo = *setList->names[index];
			*setList->names[index] = undo;
			if (et->ip) et->ip->NamedSelSetListChanged();
			}
		void Redo() {
			*setList->names[index] = redo;
			if (et->ip) et->ip->NamedSelSetListChanged();
			}
				
		TSTR Description() {return TSTR(_T("Set Name"));}
	};




class AttachHitByName : public HitByNameDlgCallback {
public:
	EditTriObject *eo;

	AttachHitByName(EditTriObject *e) {eo=e;}
	TCHAR *dialogTitle();//	{return GetString(IDS_RB_MULTIATTACH);}
	TCHAR *buttonText();// 	{return GetString(IDS_RB_ATTACH);}
	int filter(INode *node);	
	void proc(INodeTab &nodeTab);	
};


// --- tridata.cpp functions ---------------------------------------

float AffectRegionFunction(float dist,float falloff,float pinch,float bubble);
Point3 FaceNormal(Mesh &mesh, DWORD fi);
Point3 FaceCenter(Mesh &mesh, DWORD fi);
BOOL AnyFaceVertSelected(Mesh &mesh, DWORD fi);
Point3 AverageSelVertNormal(Mesh& mesh);
Point3 AverageSelVertCenter(Mesh& mesh);
void BuildAverageNormals(Mesh& mesh, Point3Tab& normals);
void MatrixFromNormal(Point3& normal, Matrix3& mat);
void BitArrayDeleteSet(BitArray &ba,BitArray &set,int m=1);
void MeshDeleteVertices(Mesh &mesh,BitArray& set);
void MeshDeleteFaces(Mesh &mesh,BitArray& set);
void PolyFromFace(Mesh& mesh,AdjFaceList &al,DWORD f,BitArray &set,float thresh,BOOL ignoreVisEdge);
void ElementFromFace(Mesh &mesh, AdjFaceList &al,DWORD f, BitArray &set);
AdjFaceList *BuildAdjFaceList(Mesh &mesh);
int GetEdgeIndex(Face &f,DWORD v0,DWORD v1);
DWORD GetOtherIndex(Face *f,DWORD v0,DWORD v1);
float AngleBetweenFaces(Mesh *mesh,DWORD f0, DWORD f1);
void FaceCenterTessellate(Mesh *mesh);
void EdgeTessellate(Mesh *mesh, AdjEdgeList *ae, AdjFaceList *af, float tens);
BOOL IsEdgeSelected(MEdge edge,Face *faces,BitArray &esel);
BOOL IsFaceSelected(MEdge edge,BitArray &fsel);
BOOL IsEdgeVisible(MEdge edge,Face *faces);
Point3 ButterFlySubdivide(MEdge edg,Mesh *mesh,AdjFaceList *af,float tens);
Point3 EdgeCenter(MEdge edge,Point3 *verts);
void ElemExplodeFaces(Mesh *mesh, float thresh, AdjFaceList *af);
void ObjExplodeFaces(EditTriObject *eo,
		INode *node, TSTR &name, Interface *ip,
		Mesh *mesh, float thresh, AdjFaceList *af);
void SwapEdgeVerts(Face *f,DWORD& v1,DWORD& v2);
DWORD GetFaceVertIndex(Face &f,DWORD v);
void EdgeTessellate(Mesh *mesh, AdjEdgeList *ae, AdjFaceList *af, float tens);
void ExtrudeFaces(EditTriObject *eo, Mesh& mesh,
		BitArray& sel, AdjEdgeList& el, BOOL doFace);
BOOL GetDetachObjectName(HWND hDlg,TSTR &name, BOOL &elem);
void DeselectHiddenFaces(Mesh &mesh);
void SyncSelectionSets(Mesh &mesh);

#endif //__TRIOBJED__
