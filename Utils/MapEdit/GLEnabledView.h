#if !defined(AFX_GLENABLEDVIEW_H__59CED13F_E584_11D1_ACB3_E52ED8AC9002__INCLUDED_)
#define AFX_GLENABLEDVIEW_H__59CED13F_E584_11D1_ACB3_E52ED8AC9002__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
/////////////////////////////////////////////////////////////////////////////
// CGLEnabledView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// Global type definitions
	enum ColorsNumber{INDEXED,THOUSANDS,MILLIONS,MILLIONS_WITH_TRANSPARENCY};
	enum ZAccuracy{NORMAL,ACCURATE};
	struct GLInfoStruct
	{
		CString vendor;
		CString renderer;
		CString acceleration;
		CString glextensions;
		CString glversion;
		CString gluextensions;
		CString gluversion;
		int max_lights;
		int max_clip_planes;
		int max_model_stack_depth;
		int max_proj_stack_depth;
		int max_txtr_stack_depth;
		int max_name_stack_depth;
		int max_attrib_stack_depth;
		int max_texture_size;
		int max_list_nesting;
		int max_eval_order;
		CSize max_viewport_dims;
		int auxiliary_buffers;
		int max_smooth_point_size;
		int min_smooth_point_size;
		float smooth_point_granularity;
		int max_smooth_line_size;
		int min_smooth_line_size;
		float smooth_line_granularity;
		int red_bits;
		int blue_bits;
		int green_bits;
		int alpha_bits;
		int depth_bits;
		int stencil_bits;
	};

/////////////////////////////////////////////////////////////////////////////
// CGLEnabledView view

class CGLEnabledView : public CView
{
protected:
	CGLEnabledView();// protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CGLEnabledView)
/** CGLDispList
DESC:-this is an helper class which let you create "display list objects",
       use these objects to define the key elements in your scene (a disp.
	   list is faster than the corresponding GL commands).
      -Through the class members functions you have total control on a
       single display list.
      -An isolated display list save OGL parameters before execution
	   (so it's not affected by preceding transformations or settings).
*******/
	class CGLDispList
	{
	friend class CGLEnabledView;
	private:
		BOOL m_bIsolated;
		int m_glListId;
	public:
		CGLDispList();  // constructor
		~CGLDispList(); // destructor
		void StartDef(BOOL bImmediateExec=FALSE);// enclose a disp.list def.
		void EndDef();
		void Draw();// execute disp list GL commands 
		void SetIsolation(BOOL bValue) {m_bIsolated=bValue;}; // set isolation property
	};

/** CGLTesselator
DESC:-this is a wrapper class which let you create "tesselator objects",
      use these objects to create concave or self intersecting polygons.
     -OGL tessellation objects converts a vertices list describing a convex
	  or self-intersecting polygon in one or more GL primitives.Read the
	  docs to understand the callbacks mechanism.
	 -The callbacks have been defined as a simple and direct mapping to
	  corresponding GL primitive (no per vertex color or texture information).
	 -The callbacks have to be global functions.
	 -A very simple garbage collection list has been implemented to manage new
	  vertices create by combine callback.
	 -Elaboration and drawing occur after EndDef().
*******/
	class CGLTesselator
	{
	public:
		CGLTesselator();  // constructor
		~CGLTesselator(); // destructor
// properties functions
		void SetFilling(BOOL bFill=TRUE);
		BOOL GetFilling();
		void SetWindingRule(GLdouble which);
		GLdouble GetWindingRule();
// definition functions
		void StartDef();
		void AddVertexArray(GLdouble arr[][3],int size);
		void AddVertex(GLdouble vertData[3]);
		void ContourSeparator();
		void EndDef();// here occur drawing

	private:
		GLUtesselator* m_tessObj;
	};

/** CGLQuadric
DESC:-This is a wrapper class which let you create "quadric objects",
      use these objects to create spheres, disks and cylinders.
	 -All GL commands related to quadrics have been wrapped hiding the
	  quadric object parameter.
*******/
	class CGLQuadric
	{
	public:
		CGLQuadric(GLenum drwStyle=GLU_FILL,  // constructor
				   GLenum normals=GLU_FLAT, 
				   GLenum side=GLU_OUTSIDE,
				   BOOL bGenerateTxtrCoords=FALSE);
		~CGLQuadric();  // destructor
// properties functions
		void SetDrawStyle(GLenum style);
		void SetOrientation(GLenum type);
		void SetTextureCoordsGen(BOOL flag);
		void SetNormals(GLenum type);
// drawing functions
		void DrawPartialDisk(GLdouble innerRadius,GLdouble outerRadius,int slices,int loops,GLdouble startAngle,GLdouble sweepAngle);
		void DrawDisk(GLdouble innerRadius,GLdouble outerRadius,int slices,int loops);
		void DrawCylinder(GLdouble baseRadius,GLdouble topRadius,GLdouble height,int slices,int stacks);
		void DrawSphere(GLdouble radius,int longitudeSubdiv,int latitudeSubdiv);

	private:
		GLUquadricObj* m_quadrObj;
	};
// Attributes
public:

// Operations
public:
/* Stock Display lists functions
DESC.: these display lists are internally organized in a vector (20 max),
       you have control on definition and redrawing only. 
       use them for background elements which are to be drawn everytime
       all together.
NOTE: between BeginStockDispList and EndStockDispList should be present OpenGL calls only (see documentation for which are allowed and how are them treated)
*/
	void StartStockDListDef();	// allocates a new stock display list entry and opens a display list definition
	void EndStockListDef();		// closes a stock display list definition
	void DrawStockDispLists();	// executes all the stock display lists
	void ClearStockDispLists(); // deletes all the stock display lists
// Information retrieval function
	GLInfoStruct GetInformation();
// Mouse cursor function
	void SetMouseCursor(HCURSOR mcursor=NULL);
// Attribute retrieval function
	double GetAspectRatio() {return m_dAspectRatio;};
// Text functions
	void PrepareCharset3D(CString fontname,float extrusion,BOOL boldface=FALSE,BOOL italicface=FALSE,BOOL uselines=false,float precision=0.01f);// prepare a 3D charset to be used by successive Text3D calls
	void PrepareCharset3D(const LOGFONT* pLF,float extrusion,BOOL uselines=false,float precision=0.01f);// alternate method to prepare a 3D charset
	void PrepareCharset2D(CString fontname,int height=10,BOOL boldface=FALSE,BOOL italicface=FALSE);// prepare a 2D charset to be used by successive Text2D calls
	void PrepareCharset2D(const LOGFONT* pLF);// alternate method to prepare a 2D charset
	float Text3D(CString text);// draw a 3D text string
	void Text2D(CString text);// draw a bitmapped text string at the current raster position
// Rendering Context switching
	void BeginGLCommands();// use to issue GL commands outside Overridables
	void EndGLCommands();// i.e: in menu event handlers, button events handler etc.
// Overridables
	virtual void OnCreateGL(); // override to set bg color, activate z-buffer, and other global settings
	virtual void OnSizeGL(int cx, int cy); // override to adapt the viewport to the window
	virtual void OnDrawGL(); // override to issue drawing functions
	virtual void VideoMode(ColorsNumber &c,ZAccuracy &z,BOOL &dbuf); // override to specify some video mode parameters

// Overrides
// NOTE: these have been declared private because they shouldn't be
//		 overridden, use the provided virtual functions instead.
private:
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGLEnabledView)
	public:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	//}}AFX_VIRTUAL

// Implementation
protected:
	int m_glErrorCode;
	virtual ~CGLEnabledView();
	CRect m_ClientRect;    // client area size
	double m_dAspectRatio;    // aspect
	GLYPHMETRICSFLOAT* m_gmfvector;
	const int m_numchars;


#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

// Generated message map functions
// NOTE: these have been declared private because they shouldn't be
//		 overridden, use the provided virtual functions instead.
private:
	//{{AFX_MSG(CGLEnabledView)
		afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
		afx_msg void OnDestroy();
		afx_msg BOOL OnEraseBkgnd(CDC* pDC);
		afx_msg void OnSize(UINT nType, int cx, int cy);
		afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

// member variables
private:
	CDC* m_pCDC;	// WinGDI Device Context
	HGLRC m_hRC;	// OpenGL Rendering Context
	HCURSOR m_hMouseCursor;	// mouse cursor handle for the view
	CPalette m_CurrentPalette; // palettes
	CPalette* m_pOldPalette;
	int m_DispListVector[20];	// Internal stock display list vector
	BOOL m_bInsideDispList;	// Disp List definition semaphore
	BOOL m_bExternGLCall;
	BOOL m_bExternDispListCall;
	int m_charsetDListBase;
// initialization helper functions
	unsigned char ComponentFromIndex(int i, UINT nbits, UINT shift);
	void CreateRGBPalette();
	BOOL bSetupPixelFormat();

	CDC* GetCDC() {return m_pCDC;};

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GLENABLEDVIEW_H__59CED13F_E584_11D1_ACB3_E52ED8AC9002__INCLUDED_)
