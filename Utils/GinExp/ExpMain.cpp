/*=========================================================================

	EXPMAIN.CPP

	Author:  Tim Swann @ CLIMAX
	Created:
	Project:
	Purpose:

	Copyright (c) 1998 Climax Development Ltd

===========================================================================*/

/*----------------------------------------------------------------------
	Includes
	-------- */

#include "Asciiexp.h"
#include "ExpFileIO.h"


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

#define NINJAEXP_CLASS_ID	Class_ID(0x689c7996, 0x13531d0d)

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

static HINSTANCE			hInstance;
static AsciiExpClassDesc	AsciiExpDesc;
static bool					controlsInit = FALSE;


/*----------------------------------------------------------------------
	Data
	---- */

/*----------------------------------------------------------------------
	Function:	
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */

BOOL WINAPI DllMain(HINSTANCE hinstDLL,ULONG fdwReason,LPVOID lpvReserved) 
{
	hInstance = hinstDLL;

	// Initialize the custom controls. This should be done only once.
	if (!controlsInit)
	{
		controlsInit = TRUE;
		InitCustomControls(hInstance);
		InitCommonControls();
	}

	return (TRUE);
}


/*----------------------------------------------------------------------
	Function:	
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */

__declspec( dllexport ) const TCHAR* LibDescription() 
{
	return GetString(IDS_LIBDESCRIPTION);
}


__declspec( dllexport ) int LibNumberClasses() 
{
	return 1;
}


__declspec( dllexport ) ClassDesc* LibClassDesc(int i) 
{
	switch(i) {
	case 0: return GetAsciiExpDesc();
	default: return 0;
	}
}


__declspec( dllexport ) ULONG LibVersion() 
{
	return VERSION_3DSMAX;
}


/*----------------------------------------------------------------------
	Function:	
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */

ClassDesc * GetAsciiExpDesc(void)
{
	return &AsciiExpDesc;
}


TCHAR * GetString(int id)
{
	static TCHAR buf[256];

	if (hInstance)
		return LoadString(hInstance, id, buf, sizeof(buf)) ? buf : NULL;

	return NULL;
}


/*----------------------------------------------------------------------
	Function:	
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */

AsciiExp::AsciiExp()
{
}


AsciiExp::~AsciiExp()
{
}


int AsciiExp::ExtCount()
{
	return 1;
}


const TCHAR * AsciiExp::Ext(int n)
{
	switch(n)
	{
	case 0:
		return _T("GIN");
	}
	return _T("");
}


const TCHAR * AsciiExp::LongDesc()
{
	return GetString(IDS_LONGDESC);
}


const TCHAR * AsciiExp::ShortDesc()
{
	return GetString(IDS_SHORTDESC);
}


const TCHAR * AsciiExp::AuthorName() 
{
	return _T("Tim Swann / Mike Armstrong");
}


const TCHAR * AsciiExp::CopyrightMessage() 
{
	return GetString(IDS_COPYRIGHT);
}


const TCHAR * AsciiExp::OtherMessage1() 
{
	return _T("");
}


const TCHAR * AsciiExp::OtherMessage2() 
{
	return _T("");
}


unsigned int AsciiExp::Version()
{
	return 100;
}


static BOOL CALLBACK AboutBoxDlgProc(HWND hWnd, UINT msg, 
	WPARAM wParam, LPARAM lParam)
{
	switch (msg) {
	case WM_INITDIALOG:
		CenterWindow(hWnd, GetParent(hWnd)); 
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDOK:
			EndDialog(hWnd, 1);
			break;
		}
		break;
		default:
			return FALSE;
	}
	return TRUE;
}       


void AsciiExp::ShowAbout(HWND hWnd)
{
//	DialogBoxParam(hInstance, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, AboutBoxDlgProc, 0);
}


static BOOL CALLBACK ExportDlgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	AsciiExp * exp = (AsciiExp*)GetWindowLong(hWnd, GWL_USERDATA); 

	switch (msg)
	{
	case WM_INITDIALOG:
		exp = (AsciiExp*)lParam;
		SetWindowLong(hWnd,GWL_USERDATA,lParam); 
		CenterWindow(hWnd, GetParent(hWnd)); 
//		CheckDlgButton(hWnd, IDC_ANIMCHECK, exp->GetExportAnimFlag());
		break;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDOK:
//			exp->SetExportAnimFlag(IsDlgButtonChecked(hWnd, IDC_ANIMCHECK));
			EndDialog(hWnd, 1);
			break;

		case IDCANCEL:
			EndDialog(hWnd, 0);
			break;
		}
		break;
		default:
			return FALSE;
	}
	return TRUE;
}


// Dummy function for progress bar
DWORD WINAPI fn(LPVOID arg)
{
	return(0);
}


/*----------------------------------------------------------------------
	Function:	
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */

#if MAX_RELEASE == 2500
int AsciiExp::DoExport(const TCHAR * name, ExpInterface * ei, Interface * i, BOOL suppressPrompts)
#else
int AsciiExp::DoExport(const TCHAR * name, ExpInterface * ei, Interface * i, BOOL suppressPrompts,DWORD options)
#endif
{
	int		numChildren;
//	NodeCount = 0;


	// GRAB THE INTERFACE POINTER
	ip = i;
	MAX_hWnd = ip->GetMAXHWnd();
//	SetExportAnimFlag(0);
//	if (!suppressPrompts && !DialogBoxParam(hInstance, MAKEINTRESOURCE(IDD_ASCIIEXPORT_DLG), MAX_hWnd, ExportDlgProc, (LPARAM)this)) return 1;

	tempStream = NULL;
	expStream = NULL;
	nbChunks = 0;


	// GET SCENE FILENAME
	sprintf( SceneName, "%s", FixupName(ip->GetCurFileName()) );
	_strupr( SceneName );


	// OPEN DEBUG TEXT FILE
	tempStream = _tfopen("C:\\ExpDebug.txt",_T("wt"));
	if (!tempStream)	return 0;

	fprintf( tempStream, "TEST\n" );

	// OPEN OUTPUT FILE
	_splitpath(name, drive, dir, fname, ext);
	sprintf(filename, "%s%s%s.GIN", drive, dir, fname);
	_strlwr(filename);
	expStream = _tfopen(filename, _T("wb"));
	if (!expStream)		return 0;

	// WRITE FILE HEADER
	if (WriteChunkHdr( (char*)FILE_ID, 0 ))
	{
		// STARTUP THE PROGRESS BAR
//		if (GetExportAnimFlag())
//			ip->ProgressStart("Exporting with Anims", TRUE, fn, NULL);
//		else
			ip->ProgressStart("Exporting without Vertex Anims", TRUE, fn, NULL);


		Weights = NULL;
		nTotalNodeCount = 0;
		nCurNode = 0;
		nCurObj = 0;
		nbFrames = 0;
		totFrames = 0;
		nbWeights = 0;
		NbEmmitters = 0;

		GetNoFrames();
		fwrite(&totFrames, sizeof(int), 1, expStream);

		WriteChunkHdr( (char*)VERSION_ID, 0);
		int MyVer = VERSION;
		fwrite(&MyVer, sizeof(int), 1, expStream);

		OrigProcess(ip->GetRootNode());
		FfdProcess(ip->GetRootNode(), nTotalNodeCount);

		ExportTree();

		numChildren = ip->GetRootNode()->NumberOfChildren();
		for (int idx=0; idx<numChildren; idx++)
		{
			if (ip->GetCancel())	break;
			nodeEnum( ip->GetRootNode()->GetChildNode(idx));
		}

		ExportMaterialList();
	}

	// WRITE FILE TERMINATOR HEADER
	WriteChunkHdr( (char*)TERMINATOR_ID, 0 );


	// Close the streams
	if (tempStream)	fclose(tempStream);
	if (expStream)	fclose(expStream);
	if (Weights)	free(Weights);

	// FILL IN CHUNK SIZES
	CalcChunkSizes();

	// WE'RE DONE. FINISH THE PROGRESS BAR.
	ip->ProgressEnd();

/*	if (!DialogBoxParam(hInstance, MAKEINTRESOURCE(IDD_ENDDIALOG), MAX_hWnd, ExportDlgProc, (LPARAM)this))
	{
		return 1;
	}*/


	return 1;
}


/*----------------------------------------------------------------------
	Function:	
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */

BOOL AsciiExp::nodeEnum(INode* node) 
{
	nCurNode++;

	ip->ProgressUpdate((int)((float)nCurNode/nTotalNodeCount*100.0f)); 

	if (ip->GetCancel())	return FALSE;

	ExportNode(node);

	for (int c = 0; c < node->NumberOfChildren(); c++)
	{
		if (!nodeEnum(node->GetChildNode(c)))	return FALSE;
	}

	return TRUE;
}


void AsciiExp::BoneProcess(INode* node, int& nodeCount)
{
	nodeCount++;

	// GET WEIGHTS BEFORE BONES ARE EXPORTED
	ExportWeights( node );

	for (int c = 0; c < node->NumberOfChildren(); c++)
	{
		BoneProcess(node->GetChildNode(c), nodeCount);
	}
}


void AsciiExp::FfdProcess(INode* node, int& nodeCount)
{
	nodeCount++;

	// GET WEIGHTS BEFORE BONES ARE EXPORTED
//	ExportFFD( node );
	Control *cont = node->GetTMController();
	if (cont) {

		if ((cont->ClassID() == BIPSLAVE_CONTROL_CLASS_ID) ||
			(cont->ClassID() == BIPBODY_CONTROL_CLASS_ID) ||
			(cont->ClassID() == FOOTPRINT_CLASS_ID))
		{
			IBipedExport *BipIface = (IBipedExport *) cont->GetInterface(I_BIPINTERFACE);
			if (BipIface)
			{
				//BipIface->RemoveNonUniformScale(1);
			}
		}
	}

	for (int c = 0; c < node->NumberOfChildren(); c++)
	{
		FfdProcess(node->GetChildNode(c), nodeCount);
	}
}

void AsciiExp::OrigProcess(INode *node)
{
	char TempName[256];
	sprintf(TempName, "%s", node->GetName());
	_strupr(TempName);
	if (strcmp(TempName, "ORIGIN")==0) {
		Matrix3		tm = node->GetObjTMAfterWSM( ip->GetAnimRange().Start() );
		AffineParts	ap;
		decomp_affine(tm, &ap);
		WriteChunkHdr( (char*)ORIG_ID, 0);
		fwrite(&ap.t.x, sizeof(float), 1, expStream);
		fwrite(&ap.t.z, sizeof(float), 1, expStream);
		fwrite(&ap.t.y, sizeof(float), 1, expStream);
	}
	for (int c = 0; c < node->NumberOfChildren(); c++)
	{
		OrigProcess(node->GetChildNode(c));
	}
}


void AsciiExp::GetNoFrames( void )
{
	TimeValue	start = ip->GetAnimRange().Start();
	TimeValue	end = ip->GetAnimRange().End();
	TimeValue	t;
	int			delta = GetTicksPerFrame();
	Object *	obj = ip->GetRootNode()->EvalWorldState(ip->GetAnimRange().Start()).obj;

	totFrames = 0;
	for (t=start; t<=end; t+=delta)		totFrames++;
}


/*----------------------------------------------------------------------
	Function:	
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */

void AsciiExp::ExportNode(INode* node) 
{
	ObjectState os = node->EvalWorldState(ip->GetAnimRange().Start());

	OutputDebugString(node->GetName());

	if (os.obj)
	{
		switch(os.obj->SuperClassID())
		{
		case GEOMOBJECT_CLASS_ID:
			ip->ProgressStart("Export Model", TRUE, fn, NULL);
			ExportModel( node );
			break;
		case HELPER_CLASS_ID:
			ip->ProgressStart("Export Helper", TRUE, fn, NULL);
			break;
		case CAMERA_CLASS_ID: 
			ip->ProgressStart("Export Camera", TRUE, fn, NULL);
			ExportCamera( node );
			break;
		case LIGHT_CLASS_ID:
			ip->ProgressStart("Export Light", TRUE, fn, NULL);
			ExportLight(node);
			break;
		}
	}
	ExportProp(node);
}

/*----------------------------------------------------------------------
	Function:	Misc Utility functions
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */

/*----------------------------------------------------------------------
	Function:	Misc Utility functions
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */

static Point3 basic_tva[3] =
{
	Point3(0.0,0.0,0.0),Point3(1.0,0.0,0.0),Point3(1.0,1.0,0.0)
};

static Point3 basic_tvb[3] =
{
	Point3(1.0,1.0,0.0),Point3(0.0,1.0,0.0),Point3(0.0,0.0,0.0)
};

static int nextpt[3] = {1,2,0};
static int prevpt[3] = {2,0,1};



BOOL AsciiExp::TMNegParity(Matrix3 &m)
{
	return (DotProd(CrossProd(m.GetRow(0),m.GetRow(1)),m.GetRow(2))<0.0)?1:0;
}


TriObject * AsciiExp::GetTriObjectFromNode(INode *node, TimeValue t, int &deleteIt)
{
	deleteIt = FALSE;

	Object *obj = node->EvalWorldState(t).obj;

	ObjectState	os = obj->Eval(t);
	obj = os.obj;

	if (obj && obj->CanConvertToType(Class_ID(TRIOBJ_CLASS_ID, 0)))
	{
		TriObject *tri = (TriObject *) obj->ConvertToType(t, Class_ID(TRIOBJ_CLASS_ID, 0));
		if (obj != tri) deleteIt = TRUE;

		return tri;
	}
	else
	{
		return NULL;
	}
}

TriObject * AsciiExp::GetTriObjectFromObject(Object *obj, TimeValue t, int &deleteIt)
{
	deleteIt = FALSE;

	if (obj && obj->CanConvertToType(Class_ID(TRIOBJ_CLASS_ID, 0)))
	{
		TriObject *tri = (TriObject *) obj->ConvertToType(t, Class_ID(TRIOBJ_CLASS_ID, 0));
		if (obj != tri) deleteIt = TRUE;

		return tri;
	}
	else
	{
		return NULL;
	}
}

void AsciiExp::make_face_uv(Face *f, Point3 *tv)
{
	int na,nhid,i;
	Point3 *basetv;

	/* make the invisible edge be 2->0 */
	nhid = 2;
	if (!(f->flags&EDGE_A))  nhid=0;
	else if (!(f->flags&EDGE_B)) nhid = 1;
	else if (!(f->flags&EDGE_C)) nhid = 2;
	na = 2-nhid;
	basetv = (f->v[prevpt[nhid]]<f->v[nhid]) ? basic_tva : basic_tvb;
	for (i=0; i<3; i++)
	{
		tv[i] = basetv[na];
		na = nextpt[na];
	}
}

/*----------------------------------------------------------------------
	Function:	
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */

bool AsciiExp::WriteChunkHdr( char* ChunkID, long Size )
{

	SizeTab[nbChunks] = ftell( expStream );
	if (SizeTab[nbChunks] & 0x3)	assert(0);
	//SizeTab[nbChunks] /= 4;

	int sl = strlen(ChunkID) + 1;
	if (sl&3) {
		sl += 4 - (sl & 3);
	}

	fwrite( ChunkID, sl, 1, expStream );
	fwrite( &Size, sizeof(Uint32), 1, expStream );

	nbChunks++;
	if(nbChunks >= MAX_FILE_CHUNKS)
	{
		MessageBox( MAX_hWnd, "INCREASE MAX_FILE_CHUNKS", "ERROR", MB_OK);
		return	false;
	}
	return true;
}


void AsciiExp::CalcChunkSizes( void )
{
	int		hFile;
	long	filelen;
	char *	filebuf;


	hFile = _open(filename, _O_RDONLY | _O_BINARY);

	if(hFile != -1)
	{
		filelen = FIO_FileLen(filename);		// length of file
		filebuf = (char *)malloc(filelen);		// allocate mainmemory

		_read(hFile, filebuf, filelen);			// read in file
		_close(hFile);							// file close
	}
	else
	{
		assert(0);
	}

	for (int i=0;i<(nbChunks-1);i++)
	{
		long	offset;
		long	filepos;

		filepos = SizeTab[i];
		int sl = strlen(&filebuf[filepos]) + 1;
		if (sl&3) {
			sl += 4 - (sl & 3);
		}
		filepos += sl;
		offset = (SizeTab[i+1] - filepos - 4);
		//offset -= 8;							// DON'T INCLUDE CHUNK HDR IN CHUNK SIZE

		*(long*)&filebuf[filepos] = offset;
	}

	FIO_Save( filename, (unsigned char *)filebuf, filelen);

	free( filebuf );

	if(expStream)	fclose( expStream);
}


/*----------------------------------------------------------------------
	Function:	String manipulation functions
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */

#define CTL_CHARS  40
#define SINGLE_QUOTE 39

TCHAR* AsciiExp::FixupName(TCHAR* name)
{
	static char buffer[256];
	TCHAR* cPtr;
	
    _tcscpy(buffer, name);
    cPtr = buffer;
	
    while(*cPtr)
	{
		if (*cPtr <= CTL_CHARS || *cPtr == '-')
			*cPtr = _T('_');
        cPtr++;
    }
	
	return buffer;
}


/*===========================================================================
 end */
