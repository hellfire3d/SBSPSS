#ifndef FFD_H
#define FFD_H

#include "iparamm.h"
#include "istdplug.h"

// This is the FFD Modifier's class ID.
#define FFD44_CLASS_ID Class_ID(0x21325596, 0x2cd10bd8)
#define FFD33_CLASS_ID Class_ID(0x21325596, 0x2cd10bd9)
#define FFD22_CLASS_ID Class_ID(0x21325596, 0x2cd10bd0)

class FFDMod : public Modifier {	
	public:
		// An addition transformation from "Mod Space".
		// Let's the user move/rotate/scale the source box
		Control *tmControl;
		
		// Parameter block to store parameters
		IParamBlock *pblock;		
		
		// This BitArray will be set to a length of 64. 1 bit for
		// each point indicating its selection state.
		BitArray sel;
		
		// A cache of the input object's bounding box.
		Box3 lbox;

		// Class variables -- these are only used by one instance
		// of this class at a time while it is being edited in
		// the command panel.
		static IObjParam *ip;		
		static IParamMap *pmapParam;
		static MoveModBoxCMode *moveMode;
		static RotateModBoxCMode *rotMode;
		static UScaleModBoxCMode *uscaleMode;
		static NUScaleModBoxCMode *nuscaleMode;
		static SquashModBoxCMode *squashMode;
		static SelectModBoxCMode *selectMode;

		FFDMod();
		~FFDMod();
		
		// From Animatable
		void DeleteThis() {delete this;}		
		void BeginEditParams(IObjParam *ip, ULONG flags,Animatable *prev);
		void EndEditParams(IObjParam *ip,ULONG flags,Animatable *next);				
		CreateMouseCallBack* GetCreateMouseCallBack() {return NULL;} 
		BOOL AssignController(Animatable *control,int subAnim);		
		int SubNumToRefNum(int subNum);
		BOOL SelectSubAnim(int subNum);

		// From BaseObject/Object
		int Display(TimeValue t, INode* inode, ViewExp *vpt, int flags, ModContext *mc);
		void GetWorldBoundBox(TimeValue t,INode* inode, ViewExp *vpt, Box3& box, ModContext *mc);
		int HitTest(TimeValue t, INode* inode, int type, int crossing, int flags, IPoint2 *p, ViewExp *vpt, ModContext* mc);
		void SelectSubComponent(HitRecord *hitRec, BOOL selected, BOOL all, BOOL invert=FALSE);
		void ClearSelection(int selLevel);
		void SelectAll(int selLevel);
		void InvertSelection(int selLevel);
		void ActivateSubobjSel(int level, XFormModes& modes);
		void Move(TimeValue t, Matrix3& partm, Matrix3& tmAxis, Point3& val, BOOL localOrigin);
		void Rotate(TimeValue t, Matrix3& partm, Matrix3& tmAxis, Quat& val, BOOL localOrigin);
		void Scale(TimeValue t, Matrix3& partm, Matrix3& tmAxis, Point3& val, BOOL localOrigin);
		void GetSubObjectCenters(SubObjAxisCallback *cb,TimeValue t,INode *node,ModContext *mc);
		void GetSubObjectTMs(SubObjAxisCallback *cb,TimeValue t,INode *node,ModContext *mc);
		void TransformStart(TimeValue t) {if (ip) ip->LockAxisTripods(TRUE);}
		void TransformFinish(TimeValue t) {if (ip) ip->LockAxisTripods(FALSE);}
		void TransformCancel(TimeValue t) {if (ip) ip->LockAxisTripods(FALSE);}

		// From Modifier
		ChannelMask ChannelsUsed()  {return PART_GEOM|PART_TOPO|PART_SELECT|PART_SUBSEL_TYPE;}
		ChannelMask ChannelsChanged() {return PART_GEOM;}
		Class_ID InputType() {return defObjectClassID;}		
		Interval LocalValidity(TimeValue t);
		void ModifyObject(TimeValue t, ModContext &mc, ObjectState *os, INode *node);

		// From ReferenceTarget/Maker
		int NumRefs() {return 66;}
		RefTargetHandle GetReference(int i);
		void SetReference(int i, RefTargetHandle rtarg);
		int NumSubs() {return 66;}
		Animatable* SubAnim(int i);
		TSTR SubAnimName(int i);

		IOResult Load(ILoad *iload);
		IOResult Save(ISave *isave);
						
		RefResult NotifyRefChanged(Interval changeInt, RefTargetHandle hTarget, PartID& partID, RefMessage message);

		int DrawControlPoints(TimeValue t,ViewExp *vpt,GraphicsWindow *gw,Box3 box,BOOL ht=FALSE,INode *inode=NULL,ModContext *mc=NULL,int flags=0);		
		Matrix3 CompTM(TimeValue t,INode *inode,ModContext *mc);
		void PlugControllers(TimeValue t);

		virtual int NumCtrlPts()=0;
		virtual Control* GetPtCont(int i)=0;
		virtual void SetPtCont(int i,Control *c)=0;
		virtual Point3& GetPt(int i)=0;
		virtual Point3 *GetPtPtr()=0;
		virtual int GridWidth()=0;
		virtual int GridIndex(int i,int j,int k)=0;
		virtual Point3 GetControlPoint(TimeValue t, int i, int src=FALSE)=0;
	};

#endif