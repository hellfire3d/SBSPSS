/*=========================================================================

	EXPBONE.CPP

	Author:  Tim Swann @ CLIMAX
	Created:
	Project:
	Purpose:

	Copyright (c) 1998 Climax Development Ltd

===========================================================================*/

/*----------------------------------------------------------------------
	Includes
	-------- */

#include "AsciiExp.h"


/*	Std Lib
	------- */

/*	Glib
	---- */

/*	Local
	----- */

/*	Graphics
	-------- */

/*----------------------------------------------------------------------
	Tyepdefs && Defines
	------------------- */

/*----------------------------------------------------------------------
	Structure defintions
	-------------------- */

/*----------------------------------------------------------------------
	Positional Vars
	--------------- */

/*----------------------------------------------------------------------
	Function Prototypes
	------------------- */

/*----------------------------------------------------------------------
	Vars
	---- */

/*----------------------------------------------------------------------
	Data
	---- */

/*----------------------------------------------------------------------
	Function:	
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */


void AsciiExp::ExportBones( INode* node )
{

	nbBones = 0;

	WriteChunkHdr( (char*)BONE_ID, 0);
	fwrite( &nCurObj, sizeof( Uint32 ), 1, expStream );

	ExportLimb( node );
}


/*----------------------------------------------------------------------
	Function:	
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */

static bool ThisIsABone( INode * node )
{
	bool	retval = false;
	char *	nodeName;

	nodeName = node->GetName();

//	if (nodeName[0] == 'R' && 
//		nodeName[1] == 'O' &&
//		nodeName[2] == 'O' &&
//		nodeName[3] == 'T')			retval = true;

	retval = true;

	return (retval);
}


/*----------------------------------------------------------------------
	Function:	
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */

int AsciiExp::ValidateBoneKids( INode * node )
{
	int			nbChilds = node->NumberOfChildren();
	int			childCnt = 0;
	
	return nbChilds;
}


bool IsParent(INode *p, INode *c)
{
	while (!c->IsRootNode()) {
		c=c->GetParentNode();
		if (c==p)
			return true;
	}
	return false;
}
/*----------------------------------------------------------------------
	Function:	
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */

void AsciiExp::ExportLimb( INode * node )
{
	INode *		ChildNode;
	ObjectState os = node->EvalWorldState( ip->GetAnimRange().Start() );
	Object *	obj = node->EvalWorldState(ip->GetAnimRange().Start()).obj;
	long		nbChilds;
	Matrix3		tm;
	AffineParts	ap;

	Quat		q;
	char		name[256];
	Point3		fpos;
	Point3		pos, p2;

	// EXPORT BONE
	sprintf( name, "%s", node->GetName() );
	fprintf( tempStream, "OUTPUTTING - %s\n", name );				// DEBUG FILE
	fwrite( &name, sizeof(char), MAX_NAME_LENGTH, expStream );		// WRITE BONE NAME

	// EXPORT WEIGHTS
	long	nbVerts = 0;
	if (nbWeights)
	{
		fprintf( tempStream, "WEIGHTS:\n" );

		for (int i=0;i<nbWeights;i++)
		{
			for (int n=0;n<Weights[i].nodecount;n++)
			{
				if (!strcmp(node->GetName(), Weights[i].names[n]))	nbVerts++;
			}
		}
		fprintf( tempStream, "	nbVerts = %d\n", nbVerts );
		fwrite( &nbVerts, sizeof(long), 1, expStream );		// WRITE BONE WEIGHT COUNT

		if (nbVerts)
		{
			for (int i=0;i<nbWeights;i++)
			{
				for (int n=0;n<Weights[i].nodecount;n++)
				{
					if (!strcmp(node->GetName(), Weights[i].names[n]))
					{
						fwrite( &Weights[i].vertno, sizeof(long), 1, expStream );
						fwrite( &Weights[i].weights[n], sizeof(float), 1, expStream );
						fwrite( &Weights[i].Offsets[n].x, sizeof(float), 1, expStream);
						fwrite( &Weights[i].Offsets[n].z, sizeof(float), 1, expStream);
						fwrite( &Weights[i].Offsets[n].y, sizeof(float), 1, expStream);
					}
				}
			}
		}
	} else {
		fwrite( &nbVerts, sizeof(long), 1, expStream );		// WRITE BONE WEIGHT COUNT
	}

	// EXPORT CHILDREN

	nbBones++;
	nbChilds = node->NumberOfChildren();
	fwrite( &nbChilds, sizeof(int), 1, expStream);
	for (int c=0;c<nbChilds;c++)							// RUN THROUGH ALL OF THE CHILDREN
	{
		ChildNode = node->GetChildNode(c);
		if (ChildNode)	ExportLimb( ChildNode );
	}
}


/*----------------------------------------------------------------------
	Function:	
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */

void AsciiExp::ExportBoneAnim( INode* node )
{	
	TimeValue	start = ip->GetAnimRange().Start();
	TimeValue	end = ip->GetAnimRange().End();
	TimeValue	t;
	int			delta = GetTicksPerFrame();
	Matrix3		tm;
	AffineParts	ap;
	Point3		prevPos;
	Point3		startScale;
	Quat		prevQ;
	Quat		scaleQ;
	Quat		q;
	Quat		InvQ;
	INode *		ChildNode;
	ObjectState os = node->EvalWorldState( ip->GetAnimRange().Start() );
	Object *	obj = node->EvalWorldState(ip->GetAnimRange().Start()).obj;
	long		nbChilds;


	for (t=start; t<=end; t+=delta)
	{
		// TRANSLATION
		if (0)//node->IsRootNode())
		{
			tm = node->GetNodeTM( t );
		} else
		{
			tm = node->GetNodeTM(t) * Inverse(node->GetParentTM(t));
		}
		decomp_affine(tm, &ap);

		fwrite( &ap.t.x, sizeof(float), 1, expStream );				// WRITE BONE X-POS
		fwrite( &ap.t.z, sizeof(float), 1, expStream );				// WRITE BONE Y-POS
		fwrite( &ap.t.y, sizeof(float), 1, expStream );				// WRITE BONE Z-POS

		fwrite( &ap.q.x, sizeof(float), 1, expStream );				// WRITE BONE ROT X-AXIS
		fwrite( &ap.q.z, sizeof(float), 1, expStream );				// WRITE BONE ROT Y-AXIS
		fwrite( &ap.q.y, sizeof(float), 1, expStream );				// WRITE BONE ROT Z-AXIS
		fwrite( &ap.q.w, sizeof(float), 1, expStream );				// WRITE BONE ROT W ANGLE
		
		fwrite( &ap.k.x, sizeof(float), 1, expStream );				// WRITE BONE X-POS
		fwrite( &ap.k.z, sizeof(float), 1, expStream );				// WRITE BONE Y-POS
		fwrite( &ap.k.y, sizeof(float), 1, expStream );				// WRITE BONE Z-POS

		fwrite( &ap.u.x, sizeof(float), 1, expStream );				// WRITE BONE ROT X-AXIS
		fwrite( &ap.u.z, sizeof(float), 1, expStream );				// WRITE BONE ROT Y-AXIS
		fwrite( &ap.u.y, sizeof(float), 1, expStream );				// WRITE BONE ROT Z-AXIS
		fwrite( &ap.u.w, sizeof(float), 1, expStream );				// WRITE BONE ROT W ANGLE
	}

	nbChilds = node->NumberOfChildren();
	for (int c=0;c<nbChilds;c++)							// RUN THROUGH ALL OF THE CHILDREN
	{
		ChildNode = node->GetChildNode(c);
		if (ChildNode)	ExportBoneAnim( ChildNode );
	}
}

void AsciiExp::ExportBoneKeyAnim( INode* node)
{	
	TimeValue	start = ip->GetAnimRange().Start();
	TimeValue	end = ip->GetAnimRange().End();
	int			delta = GetTicksPerFrame();
	Matrix3		tm;
	AffineParts	ap, StartAp;
	Point3		prevPos;
	Point3		startScale;
	Quat		prevQ;
	Quat		scaleQ;
	Quat		q;
	Quat		InvQ;
	INode *		ChildNode;
	ObjectState os = node->EvalWorldState( ip->GetAnimRange().Start() );
	Object *	obj = node->EvalWorldState(ip->GetAnimRange().Start()).obj;
	long		nbChilds;

	int num;
	int i;
	int FrameN;
	Interval ivalid;
	IKey k;
	Control *c;
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
	TimeValue time;


	if (node->EvalWorldState(ip->GetAnimRange().Start()).obj) {

		TimeList tl;
		tl.Clear();
		INode *KeyNode = node;
		c = KeyNode->GetTMController();
		if (c) {
			c = c->GetPositionController();
			if (c) {		
				ikeys = GetKeyControlInterface(c);
				if (ikeys) {

					num = ikeys->GetNumKeys();
					if (num >0)
					{
						for (i = 0; i<num; i++) {
							if (c->ClassID() == Class_ID(TCBINTERP_POSITION_CLASS_ID, 0)) {
								ikeys->GetKey(i, &tcbPosKey);
								time = tcbPosKey.time;
							} else 
							if (c->ClassID() == Class_ID(HYBRIDINTERP_POSITION_CLASS_ID, 0)) {
								ikeys->GetKey(i, &bezPosKey);
								time = bezPosKey.time;
							} else
							if (c->ClassID() == Class_ID(LININTERP_POSITION_CLASS_ID, 0)) {
								ikeys->GetKey(i, &linPosKey);
								time = linPosKey.time;
							}
							tl.Add(time);
						}
					}
				}
			}
		}
		c = KeyNode->GetTMController();
		if (c) {
			c = c->GetRotationController();
			if (c) {		
				ikeys = GetKeyControlInterface(c);
				if (ikeys) {

					num = ikeys->GetNumKeys();
					if (num >0)
					{

						for (i = 0; i<num; i++) {
							if (c->ClassID() == Class_ID(TCBINTERP_ROTATION_CLASS_ID, 0)) {
								ikeys->GetKey(i, &tcbRotKey);
								int rots;
								if (i)
								{
									ITCBRotKey oldtcbRotKey;
									ikeys->GetKey(i-1, &oldtcbRotKey);
									rots = fabs(tcbRotKey.val.angle - oldtcbRotKey.val.angle) / (0.6667 * PI) + 1;
									for (int k=1; k<rots; k++)
									{
										tl.Add(oldtcbRotKey.time+(int)((float)k*(tcbRotKey.time-oldtcbRotKey.time)/rots));
									}
								}
								time = tcbRotKey.time;
							} else 
							if (c->ClassID() == Class_ID(HYBRIDINTERP_ROTATION_CLASS_ID, 0)) {
								ikeys->GetKey(i, &bezRotKey);
								time = bezRotKey.time;
							} else
							if (c->ClassID() == Class_ID(LININTERP_ROTATION_CLASS_ID, 0)) {
								ikeys->GetKey(i, &linRotKey);
								time = linRotKey.time;
							}
							tl.Add(time);
						}
					}
				}
			}
		}
		c = KeyNode->GetTMController();
		if (c) {
			c = c->GetScaleController();
			if (c) {		
				ikeys = GetKeyControlInterface(c);
				if (ikeys) {

					num = ikeys->GetNumKeys();
					if (num >0)
					{
						for (i = 0; i<num; i++) {
							if (c->ClassID() == Class_ID(TCBINTERP_SCALE_CLASS_ID, 0)) {
								ikeys->GetKey(i, &tcbSclKey);
								time = tcbSclKey.time;
							} else 
							if (c->ClassID() == Class_ID(HYBRIDINTERP_SCALE_CLASS_ID, 0)) {
								ikeys->GetKey(i, &bezSclKey);
								time = bezSclKey.time;
							} else
							if (c->ClassID() == Class_ID(LININTERP_SCALE_CLASS_ID, 0)) {
								ikeys->GetKey(i, &linSclKey);
								time = linSclKey.time;
							}

							tl.Add(time);
						}
					}
				}
			}
		}
		if (tl.Count()) {
			num = tl.Count();
			WriteChunkHdr( (char*)KEY_ID, 0);
			fwrite( &CHANNEL_TYPE_BONE, sizeof( Uint32 ), 1, expStream );
			fwrite( &CHANNEL_INFO_ALL, sizeof( Uint32) , 1, expStream);
			fwrite( &nCurObj, sizeof( Uint32 ), 1, expStream );
			fwrite( &CurBone, sizeof( Uint32 ), 1, expStream );
			fwrite( &num, sizeof( Uint32 ), 1, expStream );

			for (num = 0; num <tl.Count(); num++) {
				time = tl.Get(num);
				tm = node->GetNodeTM(time) * Inverse(node->GetParentTM(time));

				FrameN = (int)((float)(time - start) / delta);

				decomp_affine(tm, &ap);
				if (num == 0)
					StartAp = ap;

				fwrite( &FrameN, sizeof(Uint32), 1, expStream);
				fwrite( &ap.t.x, sizeof(float), 1, expStream );				// WRITE BONE X-POS
				fwrite( &ap.t.z, sizeof(float), 1, expStream );				// WRITE BONE Y-POS
				fwrite( &ap.t.y, sizeof(float), 1, expStream );				// WRITE BONE Z-POS

				fwrite( &ap.q.x, sizeof(float), 1, expStream );				// WRITE BONE X-POS
				fwrite( &ap.q.z, sizeof(float), 1, expStream );				// WRITE BONE Y-POS
				fwrite( &ap.q.y, sizeof(float), 1, expStream );				// WRITE BONE Z-POS
				fwrite( &ap.q.w, sizeof(float), 1, expStream );				// WRITE BONE Z-POS

				fwrite( &ap.k.x, sizeof(float), 1, expStream );				// WRITE BONE X-POS
				fwrite( &ap.k.z, sizeof(float), 1, expStream );				// WRITE BONE Y-POS
				fwrite( &ap.k.y, sizeof(float), 1, expStream );				// WRITE BONE Z-POS

				fwrite( &ap.u.x, sizeof(float), 1, expStream );				// WRITE BONE X-POS
				fwrite( &ap.u.z, sizeof(float), 1, expStream );				// WRITE BONE Y-POS
				fwrite( &ap.u.y, sizeof(float), 1, expStream );				// WRITE BONE Z-POS
				fwrite( &ap.u.w, sizeof(float), 1, expStream );				// WRITE BONE Z-POS
			}
		}

	}
	CurBone++;	
	nbChilds = node->NumberOfChildren();
	for (int cn=0;cn<nbChilds;cn++)							// RUN THROUGH ALL OF THE CHILDREN
	{
		ChildNode = node->GetChildNode(cn);
		if (ChildNode)	ExportBoneKeyAnim( ChildNode);
	}
}


void AsciiExp::ExportBoneKeyAnimPos( INode* node)
{	
	TimeValue	start = ip->GetAnimRange().Start();
	TimeValue	end = ip->GetAnimRange().End();
	int			delta = GetTicksPerFrame();
	Matrix3		tm;
	AffineParts	ap;
	Point3		prevPos;
	Point3		startScale;
	Quat		prevQ;
	Quat		scaleQ;
	Quat		q;
	Quat		InvQ;
	INode *		ChildNode;
	ObjectState os = node->EvalWorldState( ip->GetAnimRange().Start() );
	Object *	obj = node->EvalWorldState(ip->GetAnimRange().Start()).obj;
	long		nbChilds;

	int num;
	int i;
	int FrameN;
	Interval ivalid;
	IKey k;
	Control *c;
	IKeyControl *ikeys;
	ITCBPoint3Key tcbPosKey;
	ITCBRotKey tcbRotKey;
	ITCBScaleKey tcbScaleKey;
	IBezPoint3Key bezPosKey;
	IBezQuatKey bezRotKey;
	IBezScaleKey bezScaleKey;
	ILinPoint3Key linPosKey;
	ILinRotKey linRotKey;
	ILinScaleKey linScaleKey;
	TimeValue time;

	if (node->EvalWorldState(ip->GetAnimRange().Start()).obj) {
		TimeList tl;
		tl.Clear();
		INode *KeyNode = node;
		while (KeyNode) {
			c = KeyNode->GetTMController();
			if (c) {
				c = c->GetPositionController();
				if (c) {		
					ikeys = GetKeyControlInterface(c);
					if (ikeys) {

						num = ikeys->GetNumKeys();
						if (num >0)
						{
							for (i = 0; i<num; i++) {
								if (c->ClassID() == Class_ID(TCBINTERP_POSITION_CLASS_ID, 0)) {
									ikeys->GetKey(i, &tcbPosKey);
									time = tcbPosKey.time;
								} else 
								if (c->ClassID() == Class_ID(HYBRIDINTERP_POSITION_CLASS_ID, 0)) {
									ikeys->GetKey(i, &bezPosKey);
									time = bezPosKey.time;
								} else
								if (c->ClassID() == Class_ID(LININTERP_POSITION_CLASS_ID, 0)) {
									ikeys->GetKey(i, &linPosKey);
									time = linPosKey.time;
								}
								tl.Add(time);
							}
						}
					}
				}
			}
			if (KeyNode->IsRootNode())
				break;
			KeyNode = KeyNode->GetParentNode();
		}
		if (tl.Count()) {
			num = tl.Count();
			WriteChunkHdr( (char*)KEY_ID, 0);
			fwrite( &CHANNEL_TYPE_BONE, sizeof( Uint32 ), 1, expStream );
			fwrite( &CHANNEL_INFO_POSITION, sizeof( Uint32) , 1, expStream);
			fwrite( &nCurObj, sizeof( Uint32 ), 1, expStream );
			fwrite( &CurBone, sizeof( Uint32 ), 1, expStream );
			fwrite( &num, sizeof( Uint32 ), 1, expStream );

			for (num = 0; num <tl.Count(); num++) {
				time = tl.Get(num);
				tm = node->GetNodeTM(time) * Inverse(node->GetParentTM(time));

				FrameN = (int)((float)(time - start) / delta);

				decomp_affine(tm, &ap);

				fwrite( &FrameN, sizeof(Uint32), 1, expStream);
				fwrite( &ap.t.x, sizeof(float), 1, expStream );				// WRITE BONE X-POS
				fwrite( &ap.t.z, sizeof(float), 1, expStream );				// WRITE BONE Y-POS
				fwrite( &ap.t.y, sizeof(float), 1, expStream );				// WRITE BONE Z-POS
			}
		}

	}
	CurBone++;
	nbChilds = node->NumberOfChildren();
	for (int cn=0;cn<nbChilds;cn++)							// RUN THROUGH ALL OF THE CHILDREN
	{
		ChildNode = node->GetChildNode(cn);
		if (ChildNode)	ExportBoneKeyAnimPos( ChildNode);
	}
}

void AsciiExp::ExportBoneKeyAnimRot( INode* node)
{	
	TimeValue	start = ip->GetAnimRange().Start();
	TimeValue	end = ip->GetAnimRange().End();
	int			delta = GetTicksPerFrame();
	Matrix3		tm;
	AffineParts	ap;
	Point3		prevPos;
	Point3		startScale;
	Quat		prevQ;
	Quat		scaleQ;
	Quat		q;
	Quat		InvQ;
	INode *		ChildNode;
	ObjectState os = node->EvalWorldState( ip->GetAnimRange().Start() );
	Object *	obj = node->EvalWorldState(ip->GetAnimRange().Start()).obj;
	long		nbChilds;


	int num;
	int i;
	int FrameN;
	Interval ivalid;
	IKey k;
	Control *c;
	IKeyControl *ikeys;
	ITCBPoint3Key tcbPosKey;
	ITCBRotKey tcbRotKey;
	ITCBScaleKey tcbScaleKey;
	IBezPoint3Key bezPosKey;
	IBezQuatKey bezRotKey;
	IBezScaleKey bezScaleKey;
	ILinPoint3Key linPosKey;
	ILinRotKey linRotKey;
	ILinScaleKey linScaleKey;
	TimeValue time;

	if (node->EvalWorldState(ip->GetAnimRange().Start()).obj) {
		TimeList tl;
		tl.Clear();
		INode *KeyNode = node;
		while (KeyNode) {
			c = KeyNode->GetTMController();
			if (c) {
				c = c->GetRotationController();
				if (c) {		
					ikeys = GetKeyControlInterface(c);
					if (ikeys) {

						num = ikeys->GetNumKeys();
						if (num >0)
						{

							for (i = 0; i<num; i++) {
								if (c->ClassID() == Class_ID(TCBINTERP_ROTATION_CLASS_ID, 0)) {
									ikeys->GetKey(i, &tcbRotKey);
									time = tcbRotKey.time;
								} else 
								if (c->ClassID() == Class_ID(HYBRIDINTERP_ROTATION_CLASS_ID, 0)) {
									ikeys->GetKey(i, &bezRotKey);
									time = bezRotKey.time;
								} else
								if (c->ClassID() == Class_ID(LININTERP_ROTATION_CLASS_ID, 0)) {
									ikeys->GetKey(i, &linRotKey);
									time = linRotKey.time;
								}
								tl.Add(time);
							}
						}
					}
				}
			}
			if (KeyNode->IsRootNode())
				break;
			KeyNode = KeyNode->GetParentNode();
		}

		if (tl.Count()) {
			num = tl.Count();
			WriteChunkHdr( (char*)KEY_ID, 0);
			fwrite( &CHANNEL_TYPE_BONE, sizeof( Uint32 ), 1, expStream );
			fwrite( &CHANNEL_INFO_ROTATION, sizeof( Uint32) , 1, expStream);
			fwrite( &nCurObj, sizeof( Uint32 ), 1, expStream );
			fwrite( &CurBone, sizeof( Uint32 ), 1, expStream );
			fwrite( &num, sizeof( Uint32 ), 1, expStream );
			for (num = 0; num <tl.Count(); num++) {
				time = tl.Get(num);
				tm = node->GetNodeTM(time) * Inverse(node->GetParentTM(time));

				FrameN = (int)((float)(time - start) / delta);

				decomp_affine(tm, &ap);

				fwrite( &FrameN, sizeof(Uint32), 1, expStream);
				fwrite( &ap.q.x, sizeof(float), 1, expStream );				// WRITE BONE X-POS
				fwrite( &ap.q.z, sizeof(float), 1, expStream );				// WRITE BONE Y-POS
				fwrite( &ap.q.y, sizeof(float), 1, expStream );				// WRITE BONE Z-POS
				fwrite( &ap.q.w, sizeof(float), 1, expStream );				// WRITE BONE Z-POS
			}
		}
	}

	CurBone++;

	nbChilds = node->NumberOfChildren();
	for (int cn=0;cn<nbChilds;cn++)							// RUN THROUGH ALL OF THE CHILDREN
	{
		ChildNode = node->GetChildNode(cn);
		if (ChildNode)	ExportBoneKeyAnimRot( ChildNode);
	}
}

void AsciiExp::ExportBoneKeyAnimScl( INode* node)
{	
	TimeValue	start = ip->GetAnimRange().Start();
	TimeValue	end = ip->GetAnimRange().End();
	int			delta = GetTicksPerFrame();
	Matrix3		tm;
	AffineParts	ap;
	Point3		prevPos;
	Point3		startScale;
	Quat		prevQ;
	Quat		scaleQ;
	Quat		q;
	Quat		InvQ;
	INode *		ChildNode;
	ObjectState os = node->EvalWorldState( ip->GetAnimRange().Start() );
	Object *	obj = node->EvalWorldState(ip->GetAnimRange().Start()).obj;
	long		nbChilds;

	int num;
	int i;
	int FrameN;
	Interval ivalid;
	IKey k;
	Control *c;
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
	TimeValue time;

	char text[200];
	sprintf(text, "%s", node->GetName());

	if (node->EvalWorldState(ip->GetAnimRange().Start()).obj) {
		TimeList tl;
		tl.Clear();
		INode *KeyNode = node;
		while (KeyNode) {
			c = KeyNode->GetTMController();
			if (c) {
				c = c->GetScaleController();
				if (c) {		
					ikeys = GetKeyControlInterface(c);
					if (ikeys) {

						num = ikeys->GetNumKeys();
						if (num >0)
						{
							for (i = 0; i<num; i++) {
								if (c->ClassID() == Class_ID(TCBINTERP_SCALE_CLASS_ID, 0)) {
									ikeys->GetKey(i, &tcbSclKey);
									time = tcbSclKey.time;
								} else 
								if (c->ClassID() == Class_ID(HYBRIDINTERP_SCALE_CLASS_ID, 0)) {
									ikeys->GetKey(i, &bezSclKey);
									time = bezSclKey.time;
								} else
								if (c->ClassID() == Class_ID(LININTERP_SCALE_CLASS_ID, 0)) {
									ikeys->GetKey(i, &linSclKey);
									time = linSclKey.time;
								}

								tl.Add(time);
							}
						}
					}
				}
			}
			if (KeyNode->IsRootNode())
				break;
			KeyNode = KeyNode->GetParentNode();
		}

		if (tl.Count()) {
			num = tl.Count();
			WriteChunkHdr( (char*)KEY_ID, 0);
			fwrite( &CHANNEL_TYPE_BONE, sizeof( Uint32 ), 1, expStream );
			fwrite( &CHANNEL_INFO_SCALE, sizeof( Uint32) , 1, expStream);
			fwrite( &nCurObj, sizeof( Uint32 ), 1, expStream );
			fwrite( &CurBone, sizeof( Uint32 ), 1, expStream );
			fwrite( &num, sizeof( Uint32 ), 1, expStream );

			for (num = 0; num <tl.Count(); num++) {
				time = tl.Get(num);					
				tm = node->GetNodeTM(time) * Inverse(node->GetParentTM(time));

				FrameN = (int)((float)(time - start) / delta);

				decomp_affine(tm, &ap);

				fwrite( &FrameN, sizeof(Uint32), 1, expStream);
				fwrite( &ap.k.x, sizeof(float), 1, expStream );				// WRITE BONE X-POS
				fwrite( &ap.k.z, sizeof(float), 1, expStream );				// WRITE BONE Y-POS
				fwrite( &ap.k.y, sizeof(float), 1, expStream );				// WRITE BONE Z-POS

				fwrite( &ap.u.x, sizeof(float), 1, expStream );				// WRITE BONE X-POS
				fwrite( &ap.u.z, sizeof(float), 1, expStream );				// WRITE BONE Y-POS
				fwrite( &ap.u.y, sizeof(float), 1, expStream );				// WRITE BONE Z-POS
				fwrite( &ap.u.w, sizeof(float), 1, expStream );				// WRITE BONE Z-POS
			}
		}
	}
	CurBone++;
	nbChilds = node->NumberOfChildren();
	for (int cn=0;cn<nbChilds;cn++)							// RUN THROUGH ALL OF THE CHILDREN
	{
		ChildNode = node->GetChildNode(cn);
		if (ChildNode)	ExportBoneKeyAnimScl( ChildNode);
	}
}

/*----------------------------------------------------------------------
	Function:	
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */

void AsciiExp::ExportWeights( INode* node )
{
	Object *					objPtr = NULL;
	Modifier *					phyMod = NULL;
	IPhysiqueExport *			phyExport = NULL;
	IPhyContextExport *			contextExport = NULL;
	IPhyVertexExport *			vtxExport = NULL;
	IPhyRigidVertex *			rvtExport = NULL;
	IPhyBlendedRigidVertex *	brvExport = NULL;
	Point3						offsetvector;
	INode *						bonenode = NULL;
	float						weight = 0.f;
	int							nbNodes = 0;
	int							vtype = 0;
	INode *						RootNode = NULL;
	ModContext *mc;


	Weights = NULL;

	phyMod = FindPhysiqueModifier (node, &mc);
	if (phyMod)
	{
		PhyMc = mc;
		phyExport = (IPhysiqueExport *)phyMod->GetInterface( I_PHYINTERFACE );
		if (phyExport)
		{
			contextExport = (IPhyContextExport *)phyExport->GetContextInterface( node );
			if (contextExport)
			{
				contextExport->ConvertToRigid( TRUE );
				contextExport->AllowBlending( TRUE );

				nbWeights = contextExport->GetNumberVertices();

				Weights = (WEIGHTS *)malloc( nbWeights * sizeof(WEIGHTS) );

				for (int i=0;i<nbWeights;i++)
				{
					vtxExport = (IPhyVertexExport *)contextExport->GetVertexInterface( i );

					if (vtxExport)
					{
						vtype = vtxExport->GetVertexType();

						if (vtype == RIGID_TYPE)
						{
							rvtExport = (IPhyRigidVertex *)vtxExport;
							bonenode = rvtExport->GetNode();

							Weights[i].vertno = i;
							Weights[i].nodecount = 1;
							Weights[i].weights[0] = 1.f;
							Weights[i].Offsets[0] = rvtExport->GetOffsetVector();
							sprintf( Weights[i].names[0], "%s", bonenode->GetName() );
						}
						else
						{
							brvExport = (IPhyBlendedRigidVertex *)vtxExport;
							nbNodes = brvExport->GetNumberNodes();

							Weights[i].vertno = i;
							Weights[i].nodecount = nbNodes;
							if (nbNodes > MAX_BONE_NODES)
							{
								MessageBox( MAX_hWnd, "INCREASE MAX_BONE_NODES", "ERROR", MB_OK );
								return;
							}
							for (int n=0;n<nbNodes;n++)
							{
								bonenode = brvExport->GetNode( n );

								weight = brvExport->GetWeight( n );
								offsetvector = brvExport->GetOffsetVector( n );

								Weights[i].weights[n] = weight;
								Weights[i].Offsets[n] = brvExport->GetOffsetVector(n);
								sprintf( Weights[i].names[n], "%s", bonenode->GetName() );
							}
						}

						contextExport->ReleaseVertexInterface( vtxExport );
					}
				}
			}
		}
		ExportBones(ip->GetRootNode());
		if (Weights) {
			free (Weights);
			Weights = NULL;
		}
	}
}

bool AsciiExp::IsBoned(INode *node)
{
	if (FindPhysiqueModifier(node))
		return true;
	return false;
}

/*----------------------------------------------------------------------
	Function:	
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */


Modifier * AsciiExp::FindPhysiqueModifier (INode* node, ModContext**mc)
{
	// Get object from node. Abort if no object.
	Object* ObjectPtr = node->GetObjectRef();

	if (!ObjectPtr) return NULL;

	// Is derived object ?
	if (ObjectPtr->SuperClassID() == GEN_DERIVOB_CLASS_ID)
	{
		// Yes -> Cast.
		IDerivedObject* DerivedObjectPtr = static_cast<IDerivedObject*>(ObjectPtr);

		// Iterate over all entries of the modifier stack.
		int ModStackIndex = 0;
		while (ModStackIndex < DerivedObjectPtr->NumModifiers())
		{
			// Get current modifier.
			Modifier* ModifierPtr = DerivedObjectPtr->GetModifier(ModStackIndex);

			// Is this Physique ?
			if (ModifierPtr->ClassID() == Class_ID(PHYSIQUE_CLASS_ID_A, PHYSIQUE_CLASS_ID_B))
			{
				*mc = DerivedObjectPtr->GetModContext(ModStackIndex);
				// Yes -> Exit.
				return ModifierPtr;
			}

			// Next modifier stack entry.
			ModStackIndex++;
		}
	}

	// Not found.
	return NULL;
}

Modifier * AsciiExp::FindPhysiqueModifier (INode* node)
{
	// Get object from node. Abort if no object.
	Object* ObjectPtr = node->GetObjectRef();

	if (!ObjectPtr) return NULL;

	// Is derived object ?
	if (ObjectPtr->SuperClassID() == GEN_DERIVOB_CLASS_ID)
	{
		// Yes -> Cast.
		IDerivedObject* DerivedObjectPtr = static_cast<IDerivedObject*>(ObjectPtr);

		// Iterate over all entries of the modifier stack.
		int ModStackIndex = 0;
		while (ModStackIndex < DerivedObjectPtr->NumModifiers())
		{
			// Get current modifier.
			Modifier* ModifierPtr = DerivedObjectPtr->GetModifier(ModStackIndex);

			// Is this Physique ?
			if (ModifierPtr->ClassID() == Class_ID(PHYSIQUE_CLASS_ID_A, PHYSIQUE_CLASS_ID_B))
			{
				// Yes -> Exit.
				return ModifierPtr;
			}

			// Next modifier stack entry.
			ModStackIndex++;
		}
	}

	// Not found.
	return NULL;
}



/*===========================================================================
 end */
