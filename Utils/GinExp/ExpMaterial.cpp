/*=========================================================================

	EXPMATERIAL.CPP

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
#include "string.h"

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

void AsciiExp::ExportMaterialList(void)
{
	int		numMtls = mtlList.Count();
	int		i = 0;


	nbTexs = 0;
	nbMats = 0;

	if (numMtls)
	{
		// EXPORT TEXTURE FILE LIST
		for (i=0; i<numMtls; i++)	DumpMaterial( mtlList.GetMtl(i), i, -1 );
		if (nbTexs)
		{
			WriteChunkHdr( (char*)TEXTURE_ID, 0 );
			fwrite( &nbTexs, sizeof(Uint32), 1, expStream );		
			for (int t=0;t<nbTexs;t++)
			{
				int PathLen = strlen(TexList[t]) + 1;
				if (PathLen&3) {
					PathLen += 4 - (PathLen & 3);
				}
				fwrite( &TexList[t], sizeof(char), PathLen, expStream );
			}
		}

		// EXPORT MATERIAL LIST
		if (nbMats)
		{
			WriteChunkHdr( (char*)MATERIAL_ID, 0 );
			fwrite( &nbMats, sizeof(Uint32), 1, expStream );
			fwrite( &nCurObj, sizeof(Uint32), 1, expStream );
			for (int m=0;m<nbMats;m++)
			{
				fwrite( &MatList[m].material.diffuse.alpha, sizeof(float), 1, expStream);
				fwrite( &MatList[m].material.diffuse.colour.r, sizeof(float), 1, expStream);
				fwrite( &MatList[m].material.diffuse.colour.g, sizeof(float), 1, expStream);
				fwrite( &MatList[m].material.diffuse.colour.b, sizeof(float), 1, expStream);
				fwrite( &MatList[m].material.specular.alpha, sizeof(float), 1, expStream);
				fwrite( &MatList[m].material.specular.colour.r, sizeof(float), 1, expStream);
				fwrite( &MatList[m].material.specular.colour.g, sizeof(float), 1, expStream);
				fwrite( &MatList[m].material.specular.colour.b, sizeof(float), 1, expStream);
				fwrite( &MatList[m].material.tex_flags, sizeof(Uint32), 1, expStream);
				fwrite( &MatList[m].material.attr_flags, sizeof(Uint32), 1, expStream);
			}
		}
	}
}


/*----------------------------------------------------------------------
  ---------------------------------------------------------------------- */
char *makerelative(char const *basepath,char const *newpath, char *outstr)
{
#define FF_PATHNAMEMAX 256
	char temp1[FF_PATHNAMEMAX];
	char temp2[FF_PATHNAMEMAX];
	int i, j, match;

	// Are the filenames the same?
	match = 0;
	for (j = 0; j < FF_PATHNAMEMAX; j++)
		{
		if (basepath[j] != newpath[j])
			{
			match = 0;
			break;
			}
		else if (basepath[j] == '.') // Matching
			{
			match = 1;
			break;
			}
		else if (basepath[j] == 0) // Identical
			{
			match = 0;
			break;
			}
		}
	if (match) // Matching paths and filenames
		{
		strcpy(outstr, "#");
		strcat(outstr, &newpath[j]);
		}
	else if (basepath[0] != newpath[0]) // Drive letters are different
		strcpy(outstr, newpath);
	else
		{
		// Find the paths
		strcpy(temp1, basepath);
		for (i = strlen(temp1) - 1; i >= 0; i--)
			if (temp1[i] == '\\')
				break;
			else
				temp1[i] = 0;

		strcpy(temp2, newpath);
		for (i = strlen(temp2) - 1; i >= 0; i--)
			if (temp2[i] == '\\')
				break;
			else
				temp2[i] = 0;

		// Are the paths the same?
		strcpy(outstr, "");
		if (strcmp(temp1, temp2) == 0) // Paths are the same
			{
			j = strlen(temp1);
			}
		else // Paths are different
			{
			j = 2; // Drives are the same
			// Find different bits
			for (i = 0; i < (int) strlen(temp1); i++)
				{
				if ((temp1[i] == '\\') && (temp2[i] == '\\'))
					j = i + 1;
				else
					if (temp1[i] != temp2[i])
						break;
				}
			if (j > 3)
				{
				for (i = j; i < (int) strlen(temp1); i++)
					if (temp1[i] == '\\')
						strcat(outstr,"..\\");
				}
			else
				j = 2;
			}
		strcat(outstr, &newpath[j]);
		}
	return outstr;
}

bool FileExists(const char* Name)
{
	FILE *f = fopen(Name,"rb");
	if (f) 
	{
		fclose(f);
		return true;
	}
	return false;
}

TSTR AsciiExp::FindTextureFName(TSTR MapName)
{
	char Out[256];
	_splitpath(filename, drive, dir, fname, ext);
	TSTR GinDir   = TSTR(drive)+TSTR(dir);
	_splitpath(MapName, drive, dir, fname, ext);
	TSTR DriveDir = TSTR(drive)+TSTR(dir);
	TSTR FName = TSTR(fname) + TSTR(ext);

	if (!FileExists(GinDir + FName)) 
	{
		MapName = ip->GetCurFilePath();
		_splitpath(MapName, drive, dir, fname, ext);
		DriveDir = TSTR(drive)+TSTR(dir);
		if (!FileExists(GinDir + FName)) 
		{
			int i=0;
			while (i<TheManager->GetMapDirCount())
			{
				DriveDir = TheManager->GetMapDir(i);
				DriveDir = DriveDir + TSTR("\\");
				if (FileExists(DriveDir + FName)) {
					break;
				}
				i++;
			}
			if (i==TheManager->GetMapDirCount())
			{
				DriveDir = TSTR("");
			}
		}
	}
	GinDir.toLower();
	DriveDir.toLower();
	makerelative(GinDir, DriveDir, Out);
	return TSTR(Out);
}

void AsciiExp::DumpTexture( Mtl* mtl, int matno, int subno )
{
	Texmap *	subTex = mtl->GetSubTexmap(ID_DI);

	TSTR N = mtl->GetName();
	if (mtl->ClassID() == Class_ID(DMTL_CLASS_ID, 0))
	{
		if (subTex && subTex->ClassID() == Class_ID(BMTEX_CLASS_ID, 0))
		{
			// If it is a standard material we can see if the map is enabled.
			if (((StdMat*)mtl)->MapEnabled(ID_DI))
			{
				Bool	found = FALSE;
				int		texidx = nbTexs;
				char	TexListTemp[256];


				TSTR mapName = ((BitmapTex *)subTex)->GetMapName();
				TSTR RelativePath = FindTextureFName(mapName);
				_splitpath(mapName, drive, dir, fname, ext);

				if (RelativePath.isNull())
				{
					sprintf( TexListTemp, "%s%s", fname, ext);
				} else
				{
					sprintf( TexListTemp, "%s%s%s", RelativePath, fname, ext);
				}

				for (int t=0;t<nbTexs;t++)
				{
					if (!strcmp(TexList[t], TexListTemp))
					{
						texidx = t;
						found = TRUE;
					}
				}
				if (!found)
				{
					strcpy( TexList[nbTexs], TexListTemp );
					nbTexs++;
					if(nbTexs>= MAXCOUNT) MessageBox( MAX_hWnd, "nbTexs Overflow", "ERROR", MB_OK);
				}
				MatList[matno].SubTex[(subno+1)] = texidx;
			}
		}
	}


	if (mtl->NumSubMtls() > 0) 
	{
		for (int i=0; i<mtl->NumSubMtls(); i++)
		{
			Mtl *	subMtl = mtl->GetSubMtl(i);
			if (subMtl)
			{
				DumpTexture( subMtl, matno, i  );
			}
		}
	}
}

int AsciiExp::getTextureId( Mtl *mtl )
{
	Texmap *	subTex = mtl->GetSubTexmap(ID_DI);

	TSTR N = mtl->GetName();
	if (mtl->ClassID() == Class_ID(DMTL_CLASS_ID, 0))
	{
		if (subTex && subTex->ClassID() == Class_ID(BMTEX_CLASS_ID, 0))
		{
			// If it is a standard material we can see if the map is enabled.
			if (((StdMat*)mtl)->MapEnabled(ID_DI))
			{
				Bool	found = FALSE;
				int		texidx = nbTexs;
				char	TexListTemp[256];


				TSTR mapName = ((BitmapTex *)subTex)->GetMapName();
				TSTR RelativePath = FindTextureFName(mapName);
				_splitpath(mapName, drive, dir, fname, ext);

				fprintf( tempStream, "    texName = %s\n", fname );
				if (RelativePath.isNull())
				{
					sprintf( TexListTemp, "%s%s", fname, ext);
				} else
				{
					sprintf( TexListTemp, "%s%s%s", RelativePath, fname, ext);
				}

				for (int t=0;t<nbTexs;t++)
				{
					if (!strcmp(TexList[t], TexListTemp))
					{
						return t;
					}
				}
				if (!found)
				{
					strcpy( TexList[nbTexs], TexListTemp );
					nbTexs++;
					if(nbTexs>= MAXCOUNT) MessageBox( MAX_hWnd, "nbTexs Overflow", "ERROR", MB_OK);
					
					return nbTexs-1;
				}
			}
		}
	}
	return -1;
}

/*----------------------------------------------------------------------
  ---------------------------------------------------------------------- */

void AsciiExp::DumpMaterial( Mtl* mtl, int matno, int subno , bool recurse)
{
	Color			col;
	float			alpha;
	Uint32			AttrFlags = 0;


	if (!mtl) return;


	sprintf( MatList[nbMats].name, "%s", FixupName( mtl->GetName() ) );

	fprintf( tempStream, "matName = %s %d\n", mtl->GetName(), nbMats );
	// WE KNOW THE STANDARD MATERIAL, SO WE CAN GET SOME EXTRA INFO
	if (mtl->ClassID() == Class_ID(DMTL_CLASS_ID, 0))
	{
		StdMat *	std;

		std = (StdMat*)mtl;
		

		int tmap = getTextureId(mtl);
		if (std->MapEnabled(ID_DI) && tmap != -1)	AttrFlags |= GIN_FLAG_USE_TEXTURE;
		if (std->MapEnabled(ID_RL))			AttrFlags |= GIN_FLAG_USE_ENVTEXTURE;
		if (std->GetTwoSided())				AttrFlags |= GIN_FLAG_DOUBLE_SIDE;
		if (std->GetSelfIllum( 0 ) > 0.2f )	AttrFlags |= GIN_FLAG_IGNORE_LIGHT;
		AttrFlags |= (std->GetTransparencyType()<<3);

		col = std->GetDiffuse( 0 );
		alpha = std->GetOpacity( 0 );
		if (std->MapEnabled(ID_DI)) {
			MatList[nbMats].material.diffuse.alpha = alpha;
			MatList[nbMats].material.diffuse.colour = col;
		} else {
			MatList[nbMats].material.diffuse.alpha = alpha;
			MatList[nbMats].material.diffuse.colour = col;
		}

		col = std->GetSpecular( 0 );
		MatList[nbMats].material.specular.alpha = alpha;
		MatList[nbMats].material.specular.colour = col;

		MatList[nbMats].material.exponent = 6.f;

		fprintf( tempStream, "    tmap = %d\n\n", tmap );
		MatList[nbMats].material.tex_flags = tmap;
		MatList[nbMats].material.attr_flags = AttrFlags;

		nbMats++;

	} else
	{
		if (mtl->ClassID() != Class_ID(MULTI_CLASS_ID, 0))
		{
			memset(&MatList[nbMats], 0, sizeof(MATLIST));
			MatList[nbMats].material.tex_flags = -1;
			nbMats++;
		}
	}

	if (mtl->NumSubMtls() > 0)
	{
		int c = (recurse) ? 1 : mtl->NumSubMtls();
		for (int i=0; i< c ; i++)
		{
			Mtl* subMtl = mtl->GetSubMtl(i);
			if (subMtl)
			{
				DumpMaterial( subMtl, matno, i , true);
			} else
			{
				nbMats++;
			}
		}
	}
	if(nbMats>= MAXCOUNT) MessageBox( MAX_hWnd, "nbMats Overflow", "ERROR", MB_OK);

}


/*----------------------------------------------------------------------
	Function:	
	Purpose:
	Params:
	Returns:
  ---------------------------------------------------------------------- */
int  MtlKeeper::GetSubMtlCount(Mtl* mtl, bool recurse)
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
			} else
			{
				Count ++;
			}
		}
	}
	return Count;
}

BOOL MtlKeeper::AddMtl(Mtl* mtl)
{
	if (!mtl)	return FALSE;

	int numMtls = mtlTab.Count();
	for (int i=0; i<numMtls; i++)
	{
		if (mtlTab[i] == mtl)	return FALSE;
	}

	mtlTab.Append(1, &mtl, 25);

	return TRUE;
}


int MtlKeeper::GetMtlID(Mtl* mtl)
{
	int numMtls = mtlTab.Count();
	int CurIdx = 0;
	int count = 0;
	for (int i=0; i<numMtls; i++)
	{
		if (mtlTab[i] == mtl)	return count;
		count += GetSubMtlCount(mtlTab[i], false);
	}
	return -1;
}


int MtlKeeper::Count()
{
	return mtlTab.Count();
}

Mtl* MtlKeeper::GetMtl(int id)
{
	return mtlTab[id];
}


/*===========================================================================
 end */
