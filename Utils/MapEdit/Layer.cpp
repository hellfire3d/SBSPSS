/******************/
/*** Layer Core ***/
/******************/


#include	"stdafx.h"
#include	"gl3d.h"
#include	<gl\gl.h>
#include	<gl\glu.h>
#include	<gl\glut.h>
//#include	"GLEnabledView.h"

//#include	"MapEditDoc.h"
//#include	"MapEditView.h"

#include	"Core.h"
#include	"Layer.h"
#include	"Utils.h"

/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
char	*CLayer::GetName()
{
	return(Name);
}

/*****************************************************************************/
void	CLayer::SetName(char *_Name)
{
	sprintf(Name,"%s",_Name);
}

/*****************************************************************************/
