/*=========================================================================

	EXPMODEL.CPP

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

void AsciiExp::ExportModel(INode* node)
{
	ObjectState os = node->EvalWorldState( ip->GetAnimRange().Start() );
	char nodeName[256];

	// Targets are actually geomobjects, but we will export them
	// from the camera and light objects, so we skip them here.
	if (os.obj->ClassID() == Class_ID(TARGET_CLASS_ID, 0))		return;
	ip->ProgressUpdate(0,0,"Export Model0");

	sprintf(nodeName, "%s", node->GetName());
	strupr(nodeName);
	if (nodeName[0] != 'R' ||
		nodeName[1] != 'O' ||
		nodeName[2] != 'O' ||
		nodeName[3] != 'T')
	{
	ip->ProgressUpdate(0,0,"Export ModelRoot");
		WriteChunkHdr( (char*)OBJ_ID, 0 );
		fwrite(&totFrames, sizeof(int), 1, expStream);
		fwrite(nodeName, MAX_NAME_LENGTH, 1, expStream);

	ip->ProgressUpdate(0,0,"Export Model-Mesh");
		ExportMesh( node );
	}
	
}


/*----------------------------------------------------------------------
	Function:	
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */

void AsciiExp::ExportMesh( INode* node )
{

	Matrix3		tm = node->GetObjTMAfterWSM( ip->GetAnimRange().Start() );
	Uint32		EvalFlags = 0;
	Point3		ObjPos;
	Point3		ObjRot;
	Point3		ObjSca;
	Point3      center = Point3(0.f, 0.f, 0.f);
	AffineParts	ap;
	Mesh *		mesh;
	ObjectState	os;
	Object *	obj;
	BOOL		needDel;
	TriObject *	tri;

	os = node->EvalWorldState( ip->GetAnimRange().Start() );
	obj = node->EvalWorldState( ip->GetAnimRange().Start() ).obj;

	Mtl *		nodeMtl = node->GetMtl();
	if (!nodeMtl) return;

	if (!os.obj)				return;
	if (!obj->IsRenderable())	return;
	if (node->IsNodeHidden())	return;
	if (!obj || os.obj->SuperClassID() != GEOMOBJECT_CLASS_ID)	return; // Safety net. This shouldn't happen.

	tri = GetTriObjectFromNode(node, ip->GetAnimRange().Start(), needDel);
	if (!tri)	return;

	mesh = &tri->mesh;

	mtlList.AddMtl(nodeMtl);

	ip->ProgressUpdate(0,0,"ExportMeshList");
	ExportMeshList( node, mesh );
	ip->ProgressUpdate(0,0,"ExportPoints");
	ExportPoints( node, mesh );
	ip->ProgressUpdate(0,0,"ExportVectors");
	ExportVectors( node, mesh);
	ip->ProgressUpdate(0,0,"ExportVectorsDone");

	if (needDel)	delete tri;

	tri = GetTriObjectFromNode(node, ip->GetAnimRange().End(), needDel);
	if (!tri)	return;

	mesh = &tri->mesh;
	ip->ProgressUpdate(0,0,"ExportNormals");
	ExportNormals( node, mesh );
	if (needDel)	delete tri;
	ip->ProgressUpdate(0,0,"ExportNormalsDone");

	tm = node->GetObjTMAfterWSM(ip->GetAnimRange().Start());// * Inverse(node->GetParentTM(t));
	decomp_affine(tm, &ap);

	ip->ProgressUpdate(0,0,"WriteHeader0");
	WriteChunkHdr( (char*)MODEL_ID, 0 );
	fwrite( &nCurObj, sizeof(long), 1, expStream );
char	NodeName[MAX_NAME_LENGTH];
	sprintf(NodeName,"%s",node->GetName());
//	fwrite( node->GetName(), MAX_NAME_LENGTH, 1, expStream);
	fwrite( NodeName, MAX_NAME_LENGTH, 1, expStream);
	fwrite( &radius, sizeof(float), 1, expStream );
	fwrite( &center.x, sizeof(float), 1, expStream );
	fwrite( &center.y, sizeof(float), 1, expStream );
	fwrite( &center.z, sizeof(float), 1, expStream );
	fwrite( &ap.t.x, sizeof(float), 1, expStream );
	fwrite( &ap.t.z, sizeof(float), 1, expStream );
	fwrite( &ap.t.y, sizeof(float), 1, expStream );
//ProgressUpdate(int pct, BOOL showPct = TRUE, TCHAR *title = NULL)=0

	ip->ProgressUpdate(0,0,"WriteHeaderDone");
	
	ip->ProgressUpdate(0,0,"FindPhysiqueModifier");
	if (FindPhysiqueModifier(node)) 
		{
		ip->ProgressUpdate(0,0,"ExportWeights");
		ExportWeights(node);
		} 
	else 
	ip->ProgressUpdate(0,0,"FindFFDModifier");
	if (FindFFDModifier(node)) 
		{
		} 
	else
		{
		ip->ProgressUpdate(0,0,"ExportNodeAnimMesh");
		ExportNodeAnimMesh( node );
		}
	
//	ExportAnimMesh( node );
	ip->ProgressUpdate(0,0,"FindEditableMesh");
	FindEditableMesh(node);
	ip->ProgressUpdate(0,0,"ExportVisTrack");
	ExportVisTrack( node );
	nCurObj++;
}

void AsciiExp::FindEditableMesh(INode *node) 
{
	Object *o = node->GetObjectRef();
	while (o && o->SuperClassID() == GEN_DERIVOB_CLASS_ID) 
		{
		o = ((IDerivedObject*)o)->GetObjRef();
		}
	if (o && o->ClassID() == Class_ID(EDITTRIOBJ_CLASS_ID,0)) 
		{
		ExportMeshVertAnim(node, (EditTriObject*)o);
		}
}

void AsciiExp::ExportMeshVertAnim(INode *node, EditTriObject *e)
{
	TimeValue	start = ip->GetAnimRange().Start();
	TimeValue	end = ip->GetAnimRange().End();
	int			delta = GetTicksPerFrame();

	int Controls = e->cont.Count();

	
	Matrix3		tm = node->GetObjTMAfterWSM( ip->GetAnimRange().Start() );
	Point3      p;

	Controls = 0;
	for (int i=0; i<e->cont.Count(); i++) {
		if (e->cont[i]) {
			Controls++;
		}
	}

	ITCBPoint3Key tcbPosKey;
	IBezPoint3Key bezPosKey;

	WriteChunkHdr( (char*)KEY_ID, 0);
	fwrite( &CHANNEL_TYPE_VERTEX, sizeof( Uint32 ), 1, expStream );
	fwrite( &CHANNEL_INFO_POSITION, sizeof( Uint32) , 1, expStream);
	fwrite( &nCurObj, sizeof( Uint32 ), 1, expStream );
	fwrite( &Controls, sizeof(int), 1, expStream);

	for (i=0; i<e->cont.Count(); i++) {
		if (e->cont[i]) {
			Control *c = GetControlInterface(e->cont[i]);
			IKeyControl *ikeys;
			TimeList tl;
			TimeValue time;
			tl.Clear();
			if (c) {
				ikeys = GetKeyControlInterface(c);
				int num = ikeys->GetNumKeys();
				for (int j = 0; j<num; j++) 
					{
					if (c->ClassID() == Class_ID(TCBINTERP_POINT3_CLASS_ID, 0)) {
						ikeys->GetKey(j, &tcbPosKey);
						time = tcbPosKey.time;
					} 
					else 
					if (c->ClassID() == Class_ID(HYBRIDINTERP_POINT3_CLASS_ID, 0)) 
					{
						ikeys->GetKey(j, &bezPosKey);
						time = bezPosKey.time;
					}
					tl.Add(time);
				}
				num = tl.Count();
				fwrite(&i, sizeof(int), 1, expStream);
				fwrite(&num, sizeof(int), 1, expStream);
				for (j=0; j<num; j++) {
					time = tl.Get(j);
					tm = node->GetObjTMAfterWSM(time);
					e->cont[i]->GetValue(time,&p,FOREVER);
					p  = tm * p;
					time = (time-start) / delta;
					fwrite(&time, sizeof(int), 1, expStream);
					fwrite(&p.x, sizeof(float), 1, expStream);
					fwrite(&p.z, sizeof(float), 1, expStream);
					fwrite(&p.y, sizeof(float), 1, expStream);
				}
			}
		}
	}
}

/*----------------------------------------------------------------------
  ---------------------------------------------------------------------- */
int  GetSubMtlCount(Mtl* mtl, bool recurse = false)
{
	int Count = 0;
	if (!mtl)
		return 0;
	if (mtl->ClassID() == Class_ID(DMTL_CLASS_ID, 0))
		Count ++;

	if (mtl->NumSubMtls() > 0)
	{
		int c = (recurse) ? 1 : mtl->NumSubMtls();
		for (int i=0; i<c; i++)
		{
			Mtl* subMtl = mtl->GetSubMtl(i);
			if (subMtl)
			{
				Count += GetSubMtlCount( subMtl, true);
			}
		}
	}
	return Count;
}

void AsciiExp::ExportMeshList(INode * node, Mesh * mesh)
{
	MtlID		matid;
	MtlID		matref;
	Mtl *		nodeMtl = node->GetMtl();
	int			m;
	ObjectState	os;
	Object *	obj;
	Matrix3		tm = node->GetObjTMAfterWSM( ip->GetAnimRange().Start() );
	BOOL		negScale = TMNegParity(tm);
	int			vx1, vx2, vx3;
	const char * name = node->GetName();


//	fprintf( tempStream, "MESHLIST = %s\n\n", name );

	obj = node->EvalWorldState( ip->GetAnimRange().Start() ).obj;
	mesh->buildNormals();

	// Order of the vertices. Get 'em counter clockwise if the objects is
	// negatively scaled.
	if (negScale)
	{
		vx1 = 2;
		vx2 = 1;
		vx3 = 0;
	}
	else
	{
		vx1 = 0;
		vx2 = 1;
		vx3 = 2;
	}

	int MtlCount = GetSubMtlCount(nodeMtl);
	if (MtlCount == 0) return;
	// Find all mesh materials used
	nbMeshs = 0;
	for (int i=0; i<mesh->getNumFaces(); i++)
	{
		Bool found = FALSE;

		matid = ((unsigned long)mesh->getFaceMtlIndex(i)) % MtlCount;

//		fprintf( tempStream, "MATID = %d - FACEID = %d\n", mesh->faces[i].getMatID(), mesh->getFaceMtlIndex(i) );

		for (int f=0;f<nbMeshs;f++)
		{
			if (matid == MeshSet[f].MatId)
			{
				MeshSet[f].nbFaces++;
				found = TRUE;
				break;
			}
		}
		if (!found)
		{
			MeshSet[nbMeshs].MatId = matid;
			MeshSet[nbMeshs].nbFaces = 1;
			nbMeshs++;
			if(nbMeshs>= MAXCOUNT) MessageBox( MAX_hWnd, "nbMeshs Overflow", "ERROR", MB_OK);

		}
	}


	// Sort material id's for ease of access

	for (m=0;m<nbMeshs;m++)
	{
		for (int m2=0;m2<(nbMeshs-1);m2++)
		{
			if (MeshSet[m2].MatId > MeshSet[m2+1].MatId)
			{
				MESH_SET	meshset = MeshSet[m2];
				MeshSet[m2] = MeshSet[m2+1];
				MeshSet[m2+1] = meshset;
			}
		}
	}


	for (m=0;m<nbMeshs;m++)
	{
		int		nbFaces;

		// EXPORT POLY CONNECT LIST - PER MESH
		WriteChunkHdr( (char*)POLY_ID, 0 );
		fwrite( &nCurObj, sizeof(Uint16), 1, expStream );
		fwrite( &m, sizeof(Uint16), 1, expStream );
		nbFaces = MeshSet[m].nbFaces;
		fwrite( &nbFaces, sizeof(Uint32), 1, expStream );

		for (i=0; i<mesh->getNumFaces(); i++)
		{
			matid = ((unsigned long)mesh->getFaceMtlIndex(i)) % MtlCount;
			if (matid == MeshSet[m].MatId)
			{
				fwrite( &mesh->faces[i].v[vx1], sizeof(long), 1, expStream );
				fwrite( &mesh->faces[i].v[vx2], sizeof(long), 1, expStream );
				fwrite( &mesh->faces[i].v[vx3], sizeof(long), 1, expStream );
long		HF0=mesh->faces[i].getEdgeVis(vx1);
long		HF1=mesh->faces[i].getEdgeVis(vx2);
long		HF2=mesh->faces[i].getEdgeVis(vx3);
				fwrite( &HF0, sizeof(long), 1, expStream );
				fwrite( &HF1, sizeof(long), 1, expStream );
				fwrite( &HF2, sizeof(long), 1, expStream );
//		fprintf( tempStream, "Face = %d - HF= %d %d %d\n", i,HF0,HF1,HF2);
			}
		}


		// EXPORT FACE MAP TEXCOORDS IF WE HAVE THEM...
		if (obj->HasUVW())
		{
			if (!CheckForAndExportFaceMap(nodeMtl, mesh, m))
			{
				// If not, export standard tverts
				int numTVx = mesh->getNumTVerts();

				if (numTVx)
				{
					int		nbFaces;

					WriteChunkHdr( (char*)UV_ID, 0 );
					fwrite( &nCurObj, sizeof(Uint16), 1, expStream );
					fwrite( &m, sizeof(Uint16), 1, expStream );
					nbFaces = MeshSet[m].nbFaces;
					fwrite( &nbFaces, sizeof(Uint32), 1, expStream );

					for (i=0; i<mesh->getNumFaces(); i++)
					{
						matid = ((unsigned long)mesh->getFaceMtlIndex(i)) % MtlCount;

						if (matid == MeshSet[m].MatId)
						{
							UVVert		tv;
							int			idx;

							idx = mesh->tvFace[i].t[vx1];
							tv = mesh->tVerts[idx];
							fwrite(&tv.x, sizeof(float), 1, expStream);
							fwrite(&tv.y, sizeof(float), 1, expStream);

							idx = mesh->tvFace[i].t[vx2];
							tv = mesh->tVerts[idx];
							fwrite(&tv.x, sizeof(float), 1, expStream);
							fwrite(&tv.y, sizeof(float), 1, expStream);

							idx = mesh->tvFace[i].t[vx3];
							tv = mesh->tVerts[idx];
							fwrite(&tv.x, sizeof(float), 1, expStream);
							fwrite(&tv.y, sizeof(float), 1, expStream);
						}
					}
				}
			}
		}
		{
			int		nbFaces;

			WriteChunkHdr( (char*)FACENORMAL_ID, 0 );
			fwrite( &nCurObj, sizeof(Uint16), 1, expStream );
			fwrite( &m, sizeof(Uint16), 1, expStream );
			nbFaces = MeshSet[m].nbFaces;
			fwrite( &nbFaces, sizeof(Uint32), 1, expStream );

			for (i=0; i<mesh->getNumFaces(); i++)
			{
				matid = ((unsigned long)mesh->getFaceMtlIndex(i)) % MtlCount;
				if (matid == MeshSet[m].MatId)
				{
					Point3 n=GetVertexNormal(mesh, i,mesh->getRVertPtr(mesh->faces[i].v[vx1]));
					fwrite(&n.x, sizeof(float), 1, expStream);
					fwrite(&n.y, sizeof(float), 1, expStream);
					fwrite(&n.z, sizeof(float), 1, expStream);
					
					n=GetVertexNormal(mesh, i,mesh->getRVertPtr(mesh->faces[i].v[vx2]));
					fwrite(&n.x, sizeof(float), 1, expStream);
					fwrite(&n.y, sizeof(float), 1, expStream);
					fwrite(&n.z, sizeof(float), 1, expStream);

					n=GetVertexNormal(mesh, i,mesh->getRVertPtr(mesh->faces[i].v[vx3]));
					fwrite(&n.x, sizeof(float), 1, expStream);
					fwrite(&n.y, sizeof(float), 1, expStream);
					fwrite(&n.z, sizeof(float), 1, expStream);
				}
			}
		}
//		tris[result].VertNormals[s] = Normalize(transpose * (m * get_vertex_normal(&triobj->mesh, j, triobj->mesh.getRVertPtr(fces[j].v[s]))));
		if (mesh->getNumVertCol())
		{
			Point3 vc;
			WriteChunkHdr( (char*)VCOL_ID, 0 );
			fwrite( &nCurObj, sizeof(Uint32), 1, expStream );
			fwrite( &m      , sizeof(Uint32), 1, expStream );
			int nbFaces = MeshSet[m].nbFaces;
			fwrite( &nbFaces, sizeof(Uint32), 1, expStream );
			for (i=0; i<mesh->getNumFaces(); i++)
			{
				matid = ((unsigned long)mesh->getFaceMtlIndex(i)) % MtlCount;

				if (matid == MeshSet[m].MatId)
				{
					TVFace *f = &mesh->vcFace[i];
					int vert = f->t[vx1];
					vc = mesh->vertCol[vert];
					fwrite(&vc.x, sizeof(float), 1, expStream);
					fwrite(&vc.y, sizeof(float), 1, expStream);
					fwrite(&vc.z, sizeof(float), 1, expStream);

					vert = f->t[vx2];
					vc = mesh->vertCol[vert];
					fwrite(&vc.x, sizeof(float), 1, expStream);
					fwrite(&vc.y, sizeof(float), 1, expStream);
					fwrite(&vc.z, sizeof(float), 1, expStream);

					vert = f->t[vx3];
					vc = mesh->vertCol[vert];
					fwrite(&vc.x, sizeof(float), 1, expStream);
					fwrite(&vc.y, sizeof(float), 1, expStream);
					fwrite(&vc.z, sizeof(float), 1, expStream);
				}
			}
		}
		
	}


	// EXPORT MESHSET INFO
	WriteChunkHdr( (char*)MESH_ID, 0 );
	fwrite( &nCurObj, sizeof(long), 1, expStream );
	fwrite( &nbMeshs, sizeof(long), 1, expStream );

	int NodeMatRef = mtlList.GetMtlID(nodeMtl);
	for (m=0;m<nbMeshs;m++)
	{
		matref = MeshSet[m].MatId + NodeMatRef;
//		fprintf( tempStream, "matIdx = %d %d\n", MeshSet[m].MatId, matref );

		fwrite(&matref, sizeof(Uint16), 1, expStream);
		fwrite(&MeshSet[m].nbFaces, sizeof(Uint16), 1, expStream);

		Uint32 TempStore;
		TempStore = m;
		fwrite(&TempStore, sizeof(Uint32), 1, expStream);	 // polt list
		TempStore = -1;
		fwrite(&TempStore, sizeof(Uint32), 1, expStream);    // attr   list
		fwrite(&TempStore, sizeof(Uint32), 1, expStream);	 // normal list
		TempStore = -1;
		if (!mesh->vertCol)
			TempStore = m;
		fwrite(&TempStore, sizeof(Uint32), 1, expStream);	 // vertex col list
		TempStore = -1;
		if (obj->HasUVW())
			TempStore = m;
		fwrite(&TempStore, sizeof(Uint32), 1, expStream);	 // texture list
	}
}


/*----------------------------------------------------------------------
  ---------------------------------------------------------------------- */

BOOL AsciiExp::CheckForAndExportFaceMap(Mtl* mtl, Mesh* mesh, int meshno)
{
	MtlID	matid;
	ULONG	matreq;

	return FALSE;

	if (!mtl || !mesh)		return FALSE;

	matreq = mtl->Requirements(-1);

	// ARE WE USING FACE MAPPING?
	if (!(matreq & MTLREQ_FACEMAP))		return FALSE;


	// OK, WE HAVE A FACEMAP SITUATION HERE...

	WriteChunkHdr( (char*)UV_ID, 0 );
	fwrite( &nCurObj, sizeof(Uint16), 1, expStream );
	fwrite( &meshno, sizeof(Uint16), 1, expStream );
	Uint32 mnf = MeshSet[meshno].nbFaces;
	fwrite( &mnf, sizeof(Uint32), 1, expStream );

	for (int i=0; i<mesh->getNumFaces(); i++)
	{
		matid = mesh->getFaceMtlIndex(i);//faces[i].getMatID();

		if (matid == MeshSet[meshno].MatId)
		{
			Point3		tv[3];
			Face *		f;
//			NJS_COLOR	uv;
			
			f = &mesh->faces[i];
			make_face_uv( f, tv );

			fwrite(&tv[0].x, sizeof(float), 1, expStream);
			fwrite(&tv[0].y, sizeof(float), 1, expStream);
			fwrite(&tv[1].x, sizeof(float), 1, expStream);
			fwrite(&tv[1].y, sizeof(float), 1, expStream);
			fwrite(&tv[2].x, sizeof(float), 1, expStream);
			fwrite(&tv[2].y, sizeof(float), 1, expStream);
		}
	}

	return TRUE;
}


/*----------------------------------------------------------------------
  ---------------------------------------------------------------------- */

void AsciiExp::ExportPoints(INode * node, Mesh * mesh)
{
	Matrix3		tm = node->GetObjTMAfterWSM( ip->GetAnimRange().Start() );
	Point3		Trans;
	int			nbVerts;
	
	radius = 0.f;
	nbVerts = mesh->getNumVerts();

	// EXPORT MODEL POINTS
	WriteChunkHdr( (char*)POINTS_ID, 0 );
	fwrite( &nCurObj, sizeof(Uint32), 1, expStream );
	fwrite( &nbVerts, sizeof(Uint32), 1, expStream );

	for (int i=0; i<nbVerts; i++)
	{
		Point3 v1;
		Point3 v2;

		tm = node->GetObjTMAfterWSM( ip->GetAnimRange().Start() );
		v1 = mesh->verts[i];
		//v2 = VectorTransform( tm, v1 );
		//v2 += tm.GetTrans();
		v2 = tm * v1;
		Trans = v2;

		//Trans.y = Trans.y;
		fwrite( &Trans.x, sizeof(float), 1, expStream );
		fwrite( &Trans.z, sizeof(float), 1, expStream );
		fwrite( &Trans.y, sizeof(float), 1, expStream );

		if (radius < fabs(Trans.x))	radius = (float)fabs(Trans.x);
		if (radius < fabs(Trans.y))	radius = (float)fabs(Trans.y);
		if (radius < fabs(Trans.z))	radius = (float)fabs(Trans.z);
	}
}

void AsciiExp::ExportVectors(INode * node, Mesh * mesh)
{
	Matrix3		tm = node->GetObjTMAfterWSM( ip->GetAnimRange().Start() );
	Point3		Trans;
	int			nbVerts;
	
	radius = 0.f;
	nbVerts = mesh->getNumVerts();

	// EXPORT MODEL POINTS
	WriteChunkHdr( (char*)VECT_ID, 0 );
	fwrite( &nCurObj, sizeof(Uint32), 1, expStream );
	fwrite( &nbVerts, sizeof(Uint32), 1, expStream );

	for (int i=0; i<nbVerts; i++)
	{
		Point3 v1;

		v1 = mesh->verts[i];

		fwrite( &v1.x, sizeof(float), 1, expStream );
		fwrite( &v1.z, sizeof(float), 1, expStream );
		fwrite( &v1.y, sizeof(float), 1, expStream );
	}
}

/*----------------------------------------------------------------------
  ---------------------------------------------------------------------- */

void AsciiExp::ExportNormals(INode * node, Mesh * mesh)
{
	int			nbVerts;
	Point3 *	vnorms;
	Point3		fn;  // Face normal
	Point3		vn;  // Vertex normal

	int			i;
	Matrix3		tm = node->GetObjTMAfterWSM( ip->GetAnimRange().End() );
	BOOL		negScale = TMNegParity(tm);
	int			vx1, vx2, vx3;


	if (negScale)
	{
		vx1 = 2;
		vx2 = 1;
		vx3 = 0;
	}
	else
	{
		vx1 = 0;
		vx2 = 1;
		vx3 = 2;
	}

	
	mesh->buildNormals();

	nbVerts = mesh->getNumVerts();
	vnorms = new Point3[nbVerts];

	for (i=0; i<mesh->getNumVerts(); i++)
	{
		vnorms[i] = GetAverageVertNormal(node, mesh, i);
	}


	// EXPORT MODEL NORMALS
	WriteChunkHdr( (char*)NORMAL_ID, 0 );
	fwrite( &nCurObj, sizeof(Uint32), 1, expStream );
	fwrite( &nbVerts, sizeof(Uint32), 1, expStream );

	for (i=0; i<nbVerts; i++)
	{
		vn = vnorms[i];
		fwrite( &vn.x, sizeof(float), 1, expStream );
		fwrite( &vn.z, sizeof(float), 1, expStream );
		fwrite( &vn.y, sizeof(float), 1, expStream );
	}

	delete vnorms;
}

/*----------------------------------------------------------------------
	Function:	
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */

Point3 AsciiExp::GetVertexNormal(Mesh* mesh, int faceNo, RVertex* rv)
{
	Face* f = &mesh->faces[faceNo];
	DWORD smGroup = f->smGroup;
	int numNormals;
	Point3 vertexNormal;

	// Is normal specified
	// SPCIFIED is not currently used, but may be used in future versions.
	if (rv->rFlags & SPECIFIED_NORMAL)
	{
//		fprintf( pStream, "SPECIFIED_NORMAL\n");
		vertexNormal = rv->rn.getNormal();
	}
	// If normal is not specified it's only available if the face belongs
	// to a smoothing group
	else if ((numNormals = rv->rFlags & NORCT_MASK) && smGroup)
	{
		// If there is only one vertex is found in the rn member.
		if (numNormals == 1)
		{
//			fprintf( pStream, "SINGLE SMOOTHING GROUP\n");
			vertexNormal = rv->rn.getNormal();
		}
		else
		{
//			fprintf( pStream, "SMOOTHING GROUP %d\n", numNormals);

			// If two or more vertices are there you need to step through them
			// and find the vertex with the same smoothing group as the current face.
			// You will find multiple normals in the ern member.
			for (int i = 0; i < numNormals; i++)
			{
				vertexNormal = rv->ern[i].getNormal();
			}
		}
	}
	else
	{
//		fprintf( pStream, "NO SMOOTHING GROUP\n");
		// Get the normal from the Face if no smoothing groups are there
		vertexNormal = mesh->getFaceNormal(faceNo);
	}

	return vertexNormal;
}

Point3 AsciiExp::GetAverageVertNormal(INode *node, Mesh* mesh, int VertNo)
{
	int i;
	int c=0;
	Matrix3 tm = node->GetNodeTM(ip->GetAnimRange().Start());
	tm.NoTrans();
	Point3 n= Point3(0.f,0.f,0.f);
	for (i=0; i<mesh->getNumFaces(); i++) {
		if (mesh->faces[i].v[0] == VertNo || mesh->faces[i].v[1] == VertNo || mesh->faces[i].v[2] == VertNo)
		{
			Point3 p0 = tm * mesh->verts[mesh->faces[i].v[0]];
			Point3 p1 = tm * mesh->verts[mesh->faces[i].v[1]];
			Point3 p2 = tm * mesh->verts[mesh->faces[i].v[2]];
			Point3 Nrm = Normalize(CrossProd(p0 - p1, p0 - p2));
			n += Nrm;
			c ++;
		}
	}
	n /= (float)c;
	return Normalize(n);
}

void AsciiExp::ExportNodeAnimMesh( INode* node )
{
	INode *parent;
	Matrix3 parentTM, nodeTM, localTM;
	parent = node->GetParentNode();

	TimeValue	start = ip->GetAnimRange().Start();
	TimeValue	end = ip->GetAnimRange().End();
	int			delta = GetTicksPerFrame();
	TimeValue	t;
	Matrix3		tm;
	Matrix3		StartM;
	AffineParts	ap;
	int			i;
	Quat		q;
	Quat		StartQ, StartU;
	Point3		StartP, StartS;

	nodeTM = node->GetNodeTM(start);
	parentTM = parent->GetNodeTM(start);
	StartM = nodeTM*Inverse(parentTM);

	for (t=start; t<=end; t+=delta) 
		{
		nodeTM = node->GetNodeTM(t);
		parentTM = parent->GetNodeTM(t);
		tm = nodeTM*Inverse(parentTM);
		for (i=0;i<4;i++) {
			Point3 m0 = StartM.GetRow(i);
			Point3 m1 = tm.GetRow(i);
			if (fabs(m0.x-m1.x)>0.001 ||
				fabs(m0.y-m1.y)>0.001 ||
				fabs(m0.z-m1.z)>0.001)
				break;

		}
		if (i!=4)
			break;
	}
	if (t==end)
		return;

	char text[250];
	sprintf(text,"%s", node->GetName());

	WriteChunkHdr( (char*)ANIM_ID, 0);
	fwrite( &CHANNEL_TYPE_NODE, sizeof( Uint32 ), 1, expStream );
	fwrite( &nCurObj, sizeof( Uint32 ), 1, expStream );
	fwrite( &totFrames, sizeof( Uint32 ), 1, expStream );
	for (t=start; t<=end; t+=delta) {
		// TRANSLATION
		tm = node->GetNodeTM(t) * Inverse(node->GetParentTM(t));
		decomp_affine(tm, &ap);

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

void AsciiExp::ExportVisTrack( INode* node )
{
	Control *VisControl = node->GetVisController();
	if (VisControl) {
		float VisF;
		TimeValue	start = ip->GetAnimRange().Start();
		TimeValue	end = ip->GetAnimRange().End();
		int			delta = GetTicksPerFrame();
		TimeValue	t;
		WriteChunkHdr( (char*)VIS_ID, 0);
		fwrite( &nCurObj, sizeof( Uint32 ), 1, expStream );
		fwrite( &totFrames, sizeof( Uint32 ), 1, expStream );
		for (t=start; t<=end; t+=delta) {
			Interval I = Interval(t, t);
			VisControl->GetValue(t, &VisF, I, CTRL_ABSOLUTE);
			fwrite( &VisF, sizeof(float), 1, expStream);
		}
	}

}

void AsciiExp::ExportAnimMesh( INode* node )
{
	TimeValue	start = ip->GetAnimRange().Start();
	TimeValue	end = ip->GetAnimRange().End();
	int			delta = GetTicksPerFrame();
	BOOL		needDel;
	Mesh		*mesh;
	TriObject	*tri;
	TimeValue	t;

	WriteChunkHdr( (char*)ANIM_ID, 0);
	fwrite( &CHANNEL_TYPE_VERTEX, sizeof( Uint32 ), 1, expStream );
	fwrite( &nCurObj, sizeof( Uint32 ), 1, expStream );
	fwrite( &totFrames, sizeof( Uint32 ), 1, expStream );
	for (t=start; t<=end; t+=delta) 
		{
		tri = GetTriObjectFromNode(node, t, needDel);
		if (!tri)	return;
		mesh = &tri->mesh;
		Matrix3 tm = node->GetObjTMAfterWSM( t );
		for (int i=0; i<mesh->getNumVerts(); i++)
			{
			Point3 v1;
			Point3 v2;

			v1 = mesh->verts[i];
			v2 = tm * v1;

			fwrite( &v2.x, sizeof(float), 1, expStream );
			fwrite( &v2.z, sizeof(float), 1, expStream );
			fwrite( &v2.y, sizeof(float), 1, expStream );
			}
		if (needDel) tri->DeleteThis();
	}
}

/*===========================================================================
 end */
