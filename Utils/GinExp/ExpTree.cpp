#include "AsciiExp.h"

Matrix3 Uniform_Matrix(Matrix3 orig_cur_mat)
{          
	AffineParts   parts;  
	Matrix3       mat;   

	///Remove  scaling  from orig_cur_mat
	//1) Decompose original and get decomposition info
	decomp_affine(orig_cur_mat, &parts); 

	//2) construct 3x3 rotation from quaternion parts.q
	parts.q.MakeMatrix(mat);

	//3) construct position row from translation parts.t  
	mat.SetRow(3,  parts.t);

	return(mat);
}
 
 //GET_RELATIVE_MATRIX: RETURNS RELATIVE
 //MATRIX WITHOUT NON_UNIFORM SCALING
Matrix3 Get_Relative_Matrix(INode *node, int t)
{          
	/* Note: This function removes the non-uniform scaling 
	from MAX node transformations. before multiplying the 
	current node by  the inverse of its parent. The 
	removal  must be done on both nodes before the 
	multiplication and Inverse are applied. This is especially 
	useful for Biped export (which uses non-uniform scaling on 
	its body parts.) */

	INode *p_node  =    node->GetParentNode();

	Matrix3      orig_cur_mat;  // for current and parent 
	Matrix3     orig_par_mat;  // original matrices 

	Matrix3      cur_mat;       // for current and parent 
	Matrix3     par_mat;       // decomposed matrices 
                                                                 
	//Get transformation matrices
	orig_cur_mat   =      node->GetNodeTM(t);
	if (p_node)
	{
		orig_par_mat   =    p_node->GetNodeTM(t); 
	} else
	{
		orig_par_mat.IdentityMatrix();
	}

	char Text[256];
	sprintf(Text, "%s", node->GetName());
	cur_mat = orig_cur_mat;
	par_mat = orig_par_mat;
/*	Control *c = node->GetTMController();
	if (c) 
	{
		if ((c->ClassID() == BIPSLAVE_CONTROL_CLASS_ID) ||
			(c->ClassID() == BIPBODY_CONTROL_CLASS_ID) ||
			(c->ClassID() == FOOTPRINT_CLASS_ID))
		{
			//Decompose each matrix
			cur_mat =  Uniform_Matrix(orig_cur_mat);
		}
	}
	if (p_node)
	{
		c = p_node->GetTMController();
		if (c) 
		{
			if ((c->ClassID() == BIPSLAVE_CONTROL_CLASS_ID) ||
				(c->ClassID() == BIPBODY_CONTROL_CLASS_ID) ||
				(c->ClassID() == FOOTPRINT_CLASS_ID))
			{
				par_mat =  Uniform_Matrix(orig_par_mat);
			}
		}
	}*/

	//then return relative matrix in coordinate space of parent
	return(cur_mat * Inverse( par_mat)); 
}


void AsciiExp::ExportTree()
{
	WriteChunkHdr( (char*)TREE_ID, 0);
	ExportTreeNodes(ip->GetRootNode());

	WriteChunkHdr( (char*)ANIM_ID, 0);
	fwrite( &CHANNEL_TYPE_TREE, sizeof( Uint32 ), 1, expStream );
// Change by Dave - Need frame count!!
	fwrite( &totFrames, sizeof( Uint32 ), 1, expStream );
	ExportTreeAnim(ip->GetRootNode());
/*
	WriteChunkHdr( (char*)MOTION_ID, 0);
	fwrite( &totFrames, sizeof( Uint32 ), 1, expStream );
	ExportMotionAnim(ip->GetRootNode());
*/
	WriteChunkHdr( (char*)KEY_ID, 0);
	fwrite( &CHANNEL_TYPE_TREE, sizeof( Uint32 ), 1, expStream );
	fwrite( &CHANNEL_INFO_ALL, sizeof( Uint32) , 1, expStream);
	ExportTreeKeyAnim(ip->GetRootNode());

	ExportParticles();
}

void AsciiExp::ExportParticles()
{
	WriteChunkHdr( (char*)PART_ID, 0);
	fwrite(&NbEmmitters, sizeof(int), 1, expStream);
	NbEmmitters = 0;
	ExportParticleNodes(ip->GetRootNode());
}

void AsciiExp::ExportParticleNodes( INode *node)
{
	ObjectState os = node->EvalWorldState( ip->GetAnimRange().Start() );
	Object *	obj = node->EvalWorldState(ip->GetAnimRange().Start()).obj;
	if (obj && obj->GetInterface(I_PARTICLEOBJ))
	{
		fwrite(&NbEmmitters, sizeof(int), 1, expStream);
		Control *VisControl = node->GetVisController();
		if (VisControl) {
			float VisF;
			TimeValue	start = ip->GetAnimRange().Start();
			TimeValue	end = ip->GetAnimRange().End();
			int			delta = GetTicksPerFrame();
			TimeValue	t;
			fwrite( &totFrames, sizeof( Uint32 ), 1, expStream );
			for (t=start; t<=end; t+=delta) {
				Interval I = Interval(t, t);
				VisControl->GetValue(t, &VisF, I, CTRL_ABSOLUTE);
				fwrite( &VisF, sizeof(float), 1, expStream);
			}
		} else
		{
			float VisF = 1.f;
			TimeValue	start = ip->GetAnimRange().Start();
			TimeValue	end = ip->GetAnimRange().End();
			int			delta = GetTicksPerFrame();
			TimeValue	t;
			fwrite( &totFrames, sizeof( Uint32 ), 1, expStream );
			for (t=start; t<=end; t+=delta) {
				fwrite( &VisF, sizeof(float), 1, expStream);
			}		
		}
	}
	NbEmmitters ++;
	int nbChilds = node->NumberOfChildren();
	fwrite( &nbChilds, sizeof(int), 1, expStream);
	for (int c=0;c<nbChilds;c++)							// RUN THROUGH ALL OF THE CHILDREN
	{
		INode *ChildNode = node->GetChildNode(c);
		if (ChildNode)	ExportParticleNodes( ChildNode );
	}
}

void AsciiExp::ExportTreeNodes( INode * node )
{
	INode *		ChildNode;
	long		nbChilds;
	Matrix3		tm;
	AffineParts	ap;

	char		name[256];
	ObjectState os = node->EvalWorldState( ip->GetAnimRange().Start() );
	Object *	obj = node->EvalWorldState(ip->GetAnimRange().Start()).obj;
	
	if (obj && obj->GetInterface(I_PARTICLEOBJ))
	{
		NbEmmitters ++;
	}
	// EXPORT BONE
	sprintf( name, "%s", node->GetName() );
	fprintf( tempStream, "OUTPUTTING - %s\n", name );				// DEBUG FILE
	fwrite( &name, sizeof(char), MAX_NAME_LENGTH, expStream );		// WRITE BONE NAME

	// BONE TRANSLATION
/*Dave!!	tm = node->GetNodeTM( ip->GetAnimRange().Start() ) * Inverse(node->GetParentTM(ip->GetAnimRange().Start()));*/
	tm = Get_Relative_Matrix(node , ip->GetAnimRange().Start());
	decomp_affine( tm, &ap );

	fwrite( &ap.t.x, sizeof(float), 1, expStream );				// WRITE BONE X-POS
	fwrite( &ap.t.z, sizeof(float), 1, expStream );				// WRITE BONE Y-POS
	fwrite( &ap.t.y, sizeof(float), 1, expStream );				// WRITE BONE Z-POS

	fwrite( &ap.q.x, sizeof(float), 1, expStream );				// WRITE BONE ROT X-AXIS
	fwrite( &ap.q.z, sizeof(float), 1, expStream );				// WRITE BONE ROT Y-AXIS
	fwrite( &ap.q.y, sizeof(float), 1, expStream );				// WRITE BONE ROT Z-AXIS
	fwrite( &ap.q.w, sizeof(float), 1, expStream );				// WRITE BONE ROT W ANGLE

	fwrite( &ap.k.x, sizeof(float), 1, expStream);
	fwrite( &ap.k.z, sizeof(float), 1, expStream);
	fwrite( &ap.k.y, sizeof(float), 1, expStream);

	fwrite( &ap.u.x, sizeof(float), 1, expStream);
	fwrite( &ap.u.z, sizeof(float), 1, expStream);
	fwrite( &ap.u.y, sizeof(float), 1, expStream);
	fwrite( &ap.u.w, sizeof(float), 1, expStream);

	Matrix3 m0(TRUE);
	Matrix3 m1(TRUE);
	Matrix3 m2(TRUE);
	Matrix3 m3(TRUE);
	Matrix3 m4(TRUE);

	m0.SetTrans(ap.t);
	ap.q.MakeMatrix(m1);
	ap.u.MakeMatrix(m2);
	m3.Scale(ap.k);
	m4 = Inverse(m2);

	Matrix3 j = m0 * m1 * m4 * m3 * m2;
	Matrix3 y = m2 * m3 * m4 * m1 * m0;

	// EXPORT CHILDREN
	nbChilds = ValidateBoneKids( node );
	fwrite( &nbChilds, sizeof(long), 1, expStream );		// WRITE BONE CHILD COUNT

	nbBones++;
	nbChilds = node->NumberOfChildren();
	for (int c=0;c<nbChilds;c++)							// RUN THROUGH ALL OF THE CHILDREN
	{
		ChildNode = node->GetChildNode(c);
		if (ChildNode)	ExportTreeNodes( ChildNode );
	}
}
/*
void	AsciiExp::ExportMotionAnim( INode* node)
{
	TimeValue	start = ip->GetAnimRange().Start();
	TimeValue	end = ip->GetAnimRange().End();
	TimeValue	t;
	int			delta = GetTicksPerFrame();
	Matrix3		tm;
	AffineParts	ap;
	INode *		ChildNode;
	long		nbChilds;
	Control *Cnt= node->GetTMController();

	if (Cnt) 
		if (Cnt->ClassID() == BIPBODY_CONTROL_CLASS_ID)
			for (t=start; t<=end; t+=delta)
				{
//				tm = node->GetNodeTM(t) * Inverse(node->GetParentTM(t));
				tm = node->GetNodeTM(t) ;
				decomp_affine(tm, &ap);

				fwrite( &ap.t.x, sizeof(float), 1, expStream );				// WRITE BONE X-POS
				fwrite( &ap.t.z, sizeof(float), 1, expStream );				// WRITE BONE Y-POS
				fwrite( &ap.t.y, sizeof(float), 1, expStream );				// WRITE BONE Z-POS
				}

	nbChilds = node->NumberOfChildren();
	for (int c=0;c<nbChilds;c++)							// RUN THROUGH ALL OF THE CHILDREN (Just cos Im lazy)
	{
		ChildNode = node->GetChildNode(c);
		if (ChildNode)	ExportMotionAnim( ChildNode );
	}
}
*/
void AsciiExp::ExportTreeAnim( INode* node )
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

	char name[256];
	sprintf( name, "%s", node->GetName() );

	for (t=start; t<=end; t+=delta)
	{
//		tm = node->GetNodeTM(t) * Inverse(node->GetParentTM(t));
		tm = Get_Relative_Matrix(node , t);
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
	fwrite( &nbChilds, sizeof(int), 1, expStream);
	for (int c=0;c<nbChilds;c++)							// RUN THROUGH ALL OF THE CHILDREN
	{
		ChildNode = node->GetChildNode(c);
		if (ChildNode)	ExportTreeAnim( ChildNode );
	}
}

void AsciiExp::ExportTreeKeyAnim( INode* node)
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


	TSTR Name = node->GetName();
	TimeList tl;
	tl.Clear();
	INode *KeyNode = node;
	c = KeyNode->GetTMController();
	if (c) {

		if ((c->ClassID() == BIPSLAVE_CONTROL_CLASS_ID) ||
			(c->ClassID() == BIPBODY_CONTROL_CLASS_ID) ||
			(c->ClassID() == FOOTPRINT_CLASS_ID))
		{
			for (i=0; i<c->NumSubs(); i++)
			{
				Animatable *a = c->SubAnim(i);
				int k = a->NumKeys();
				for (int j=0; j<k; j++)
				{
					time = a->GetKeyTime(j);
					if (time > start && time < end)
						tl.Add( time );
				}
			}
			int k = c->NumKeys();
			for (int j=0; j<k; j++)
			{
				time = c->GetKeyTime(j);
				if (time > start && time < end)
					tl.Add( time );
			}
			
		}

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
						if (time > start && time < end)
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
						if (time > start && time < end)
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

						if (time > start && time < end)
							tl.Add(time);
					}
				}
			}
		}
	}
	if (tl.Count())
	{
		tl.Add(start);
		tl.Add(end);
	}
	num = tl.Count();
	fwrite( &num, sizeof( Uint32 ), 1, expStream );

	for (num = 0; num <tl.Count(); num++) {
		time = tl.Get(num);
//		tm = node->GetNodeTM(time) * Inverse(node->GetParentTM(time));
		tm = Get_Relative_Matrix(node , time);

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

	CurBone++;	
	nbChilds = node->NumberOfChildren();
	fwrite( &nbChilds, sizeof(int), 1, expStream);
	for (int cn=0;cn<nbChilds;cn++)							// RUN THROUGH ALL OF THE CHILDREN
	{
		ChildNode = node->GetChildNode(cn);
		if (ChildNode)	ExportTreeKeyAnim( ChildNode);
	}
}

