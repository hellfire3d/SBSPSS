/*********************/
/*** TileSet Stuph ***/
/*********************/

#include	"stdafx.h"
#include	"gl3d.h"
#include	<gl\gl.h>
#include	<gl\glu.h>
#include	<gl\glut.h>
#include	<Vector>

#include	"TileSet.h"
#include	"GinTex.h"
#include	"utils.h"


/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
CTileSet::CTileSet(char *_Filename)
{
	strcpy(Filename,_Filename);
	Load();
}

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
CTileSet::~CTileSet()
{
}

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
int		CTileSet::Load()
{
CScene	Scene;

		Scene.Load(Filename);

int		NodeCount=Scene.GetSceneNodeCount();

		for (int i=1;i<NodeCount;i++)
		{
			AddTileToSet(Scene,i);
		}
/*
std::vector<GString> const &Tex=Scene.GetTexNames();
	
	for (i=0;i<Tex.size();i++)
	{
		TRACE1("%s\n",Tex[i]);
	}
*/

	return(1);
}

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
extern GLint TestTile;
void	CTileSet::AddTileToSet(CScene &ThisScene,int Id)
{
CNode				&ThisNode=ThisScene.GetNode(Id);
std::vector<sTri>	const &TriList=ThisNode.GetTris();
std::vector<TVECTOR>const &VtxList=ThisNode.GetRelPts();
int					TriCount=TriList.size();
GLint				ThisTile;
				
			ThisTile=glGenLists(1);
			glNewList(ThisTile,GL_COMPILE);
			glColor3f(0.5,0.5,0);
			glBegin (GL_TRIANGLES);

			TRACE3("%i %s\t%i Tris\n",Id,ThisNode.GetName(),TriCount);

			for (int T=0; T<TriCount; T++)
			{
				sTri	const &ThisTri=TriList[T];
				TVECTOR Normal=TCrossProduct(VtxList[ThisTri.p[0]],VtxList[ThisTri.p[1]],VtxList[ThisTri.p[2]]);
				glNormal3f( Normal.GetX(), Normal.GetZ(),Normal.GetY());

				for (int P=0; P<3; P++)
				{
					TVECTOR	const &ThisVtx=VtxList[ThisTri.p[P]];
					
					glVertex3f( ThisVtx.GetX(), ThisVtx.GetZ(), ThisVtx.GetY());
				}

			}
	
			glEnd();
			glEndList();
			Tile.push_back(ThisTile);
			TestTile=ThisTile;
}
