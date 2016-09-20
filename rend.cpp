/* CS580 Homework 3 */

#include	"stdafx.h"
#include	"stdio.h"
#include	"math.h"
#include	"Gz.h"
#include	"rend.h"

#define PI (3.1415926535f)

int GzRotXMat(float degree, GzMatrix mat)
{
// Create rotate matrix : rotate along x axis
// Pass back the matrix using mat value
	if (mat == NULL)
	{
		return GZ_FAILURE;
	}
	float rDegree = degree * (PI / 180);
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			mat[i][j] = 0;
		}
	}
	mat[0][0] = 1;
	mat[1][1] = cos(rDegree);
	mat[1][2] = -sin(rDegree);
	mat[2][1] = sin(rDegree);
	mat[2][2] = cos(rDegree);
	mat[3][3] = 1;
	return GZ_SUCCESS;
}


int GzRotYMat(float degree, GzMatrix mat)
{
// Create rotate matrix : rotate along y axis
// Pass back the matrix using mat value
	if (mat == NULL)
	{
		return GZ_FAILURE;
	}
	float rDegree = degree * (PI / 180);
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			mat[i][j] = 0;
		}
	}
	mat[0][0] = cos(rDegree);
	mat[0][2] = sin(rDegree);
	mat[1][1] = 1;
	mat[2][0] = -sin(rDegree);
	mat[2][2] = cos(rDegree);
	mat[3][3] = 1;
	return GZ_SUCCESS;
}


int GzRotZMat(float degree, GzMatrix mat)
{
// Create rotate matrix : rotate along z axis
// Pass back the matrix using mat value
	if (mat == NULL)
	{
		return GZ_FAILURE;
	}
	float rDegree = degree * (PI / 180);
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			mat[i][j] = 0;
		}
	}
	mat[0][0] = cos(rDegree);
	mat[0][1] = -sin(rDegree);
	mat[1][0] = sin(rDegree);
	mat[1][1] = cos(rDegree);
	mat[2][2] = 1;
	mat[3][3] = 1;
	return GZ_SUCCESS;
}


int GzTrxMat(GzCoord translate, GzMatrix mat)
{
// Create translation matrix
// Pass back the matrix using mat value
	if (translate == NULL || mat == NULL)
	{
		return GZ_FAILURE;
	}
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			mat[i][j] = 0;
		}
	}
	mat[0][3] = translate[X];
	mat[1][3] = translate[Y];
	mat[2][3] = translate[Z];
	mat[3][3] = 1;
	return GZ_SUCCESS;
}


int GzScaleMat(GzCoord scale, GzMatrix mat)
{
// Create scaling matrix
// Pass back the matrix using mat value
	if (scale == NULL || mat == NULL)
	{
		return GZ_FAILURE;
	}
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			mat[i][j] = 0;
		}
	}
	mat[0][0] = scale[X];
	mat[1][1] = scale[Y];
	mat[2][2] = scale[Z];
	mat[3][3] = 1;
	return GZ_SUCCESS;
}


//----------------------------------------------------------
// Begin main functions

int GzNewRender(GzRender **render, GzDisplay	*display)
{
/*  
- malloc a renderer struct 
- setup Xsp and anything only done once 
- save the pointer to display 
- init default camera 
*/ 
	if (render != NULL && display != NULL)
	{
		// malloc a renderer struct
		*render = (GzRender *)malloc(sizeof(GzRender));

		// TODO: setup Xsp and anything only done once
		int xs = display->xres;
		int ys = display->yres;
		(*render)->Xsp[0][0] = xs / 2.0f;
		(*render)->Xsp[0][1] = 0;
		(*render)->Xsp[0][2] = 0;
		(*render)->Xsp[0][3] = xs / 2.0f;
		(*render)->Xsp[1][0] = 0;
		(*render)->Xsp[1][1] = -ys / 2.0f;
		(*render)->Xsp[1][2] = 0;
		(*render)->Xsp[1][3] = ys / 2.0f;
		(*render)->Xsp[2][0] = 0;
		(*render)->Xsp[2][1] = 0;
		(*render)->Xsp[2][2] = MAXINT;
		(*render)->Xsp[2][3] = 0;
		(*render)->Xsp[3][0] = 0;
		(*render)->Xsp[3][1] = 0;
		(*render)->Xsp[3][2] = 0;
		(*render)->Xsp[3][3] = 1;
		(*render)->matlevel = -1;

		// save the pointer to display
		(*render)->display = display;

		// init default camera
		(*render)->camera.FOV = DEFAULT_FOV;
		(*render)->camera.lookat[X] = 0;
		(*render)->camera.lookat[Y] = 0;
		(*render)->camera.lookat[Z] = 0;
		(*render)->camera.position[X] = DEFAULT_IM_X;
		(*render)->camera.position[Y] = DEFAULT_IM_Y;
		(*render)->camera.position[Z] = DEFAULT_IM_Z;
		(*render)->camera.worldup[X] = 0;
		(*render)->camera.worldup[Y] = 1;
		(*render)->camera.worldup[Z] = 0;

		return GZ_SUCCESS;
	}
	return GZ_FAILURE;
}


int GzFreeRender(GzRender *render)
{
/* 
-free all renderer resources
*/
	if (render != NULL) {
		free(render);
	}
	return GZ_SUCCESS;
}


int GzBeginRender(GzRender *render)
{
/*  
- setup for start of each frame - init frame buffer color,alpha,z
- compute Xiw and projection xform Xpi from camera definition 
- init Ximage - put Xsp at base of stack, push on Xpi and Xiw 
- now stack contains Xsw and app can push model Xforms when needed 
*/ 
	int status = GZ_SUCCESS;
	if (render != NULL && render->display != NULL) {
		status |= GzInitDisplay(render->display);
		render->flatcolor[RED] = 0;
		render->flatcolor[GREEN] = 0;
		render->flatcolor[BLUE] = 0;

		// TODO: do I need to do this now?
		(render)->camera.Xpi[0][0] = 1;
		(render)->camera.Xpi[0][1] = 0;
		(render)->camera.Xpi[0][2] = 0;
		(render)->camera.Xpi[0][3] = 0;
		(render)->camera.Xpi[1][0] = 0;
		(render)->camera.Xpi[1][1] = 1;
		(render)->camera.Xpi[1][2] = 0;
		(render)->camera.Xpi[1][3] = 0;
		(render)->camera.Xpi[2][0] = 0;
		(render)->camera.Xpi[2][1] = 0;
		(render)->camera.Xpi[2][2] = 1 / tan(((render)->camera.FOV / 2) * (PI / 180));
		(render)->camera.Xpi[2][3] = 0;
		(render)->camera.Xpi[3][0] = 0;
		(render)->camera.Xpi[3][1] = 0;
		(render)->camera.Xpi[3][2] = 0;
		(render)->camera.Xpi[3][3] = 1;
	}
	return status;
}

int GzPutCamera(GzRender *render, GzCamera *camera)
{
/*
- overwrite renderer camera structure with new camera definition
*/
	return GZ_SUCCESS;	
}

int GzPushMatrix(GzRender *render, GzMatrix	matrix)
{
/*
- push a matrix onto the Ximage stack
- check for stack overflow
*/
	// check arguments
	if (render == NULL || matrix == NULL)
	{
		return GZ_FAILURE;
	}
	// check for stack overflow
	if (render->matlevel == MATLEVELS)
	{
		return GZ_FAILURE;
	}
	// If the stack is empty, push matrix
	if (render->matlevel == -1) {
		render->matlevel++;
		for (int i = 0; i < 4; ++i)
		{
			for (int j = 0; j < 4; ++j)
			{
				render->Ximage[render->matlevel][i][j] = matrix[i][j];
			}
		}
	}
	// otherwise push top matrix * matrix
	else
	{
		// calculate matrix multiplication
		GzMatrix newMatrix;
		for (int i = 0; i < 4; ++i)
		{
			for (int j = 0; j < 4; ++j)
			{
				newMatrix[i][j] = 0;
				for (int k = 0; j < 4; ++k)
				{
					newMatrix[i][j] += render->Ximage[render->matlevel][i][k] * matrix[k][j];
				}
			}
		}
		// push new matrix
		render->matlevel++;
		for (int i = 0; i < 4; ++i)
		{
			for (int j = 0; j < 4; ++j)
			{
				render->Ximage[render->matlevel][i][j] = newMatrix[i][j];
			}
		}
	}
	return GZ_SUCCESS;
}

int GzPopMatrix(GzRender *render)
{
/*
- pop a matrix off the Ximage stack
- check for stack underflow
*/
	// check arguments
	if (render == NULL)
	{
		return GZ_FAILURE;
	}
	// check for stack underflow
	if (render->matlevel < 0)
	{
		return GZ_FAILURE;
	}
	// pop a matrix off the Ximage stack (just decrease top pointer)
	render->matlevel--;
	return GZ_SUCCESS;
}


int GzPutAttribute(GzRender	*render, int numAttributes, GzToken	*nameList, 
	GzPointer	*valueList) /* void** valuelist */
{
/*
- set renderer attribute states (e.g.: GZ_RGB_COLOR default color)
- later set shaders, interpolaters, texture maps, and lights
*/
	
	if (render != NULL) {
		for (int i = 0; i < numAttributes; ++i) {
			switch (nameList[i]) {
			case GZ_RGB_COLOR:
				// TODO: Do I have to increment through tokens (ints) and use (sizeof) token type
				// to increment the ponter through the value list
				GzColor * color = (GzColor *)valueList[i];
				// clamp color values
				(*color)[RED] = fmaxf(0, fminf(4095, (*color)[RED]));
				(*color)[GREEN] = fmaxf(0, fminf(4095, (*color)[GREEN]));
				(*color)[BLUE] = fmaxf(0, fminf(4095, (*color)[BLUE]));
				render->flatcolor[RED] = (*color)[RED];
				render->flatcolor[GREEN] = (*color)[GREEN];
				render->flatcolor[BLUE] = (*color)[BLUE];
				break;
				// later set shaders, interpolaters, texture maps, and lights
			}
		}
		return GZ_SUCCESS;
	}
	return GZ_FAILURE;
}

int GzPutTriangle(GzRender	*render, int numParts, GzToken *nameList, GzPointer	*valueList)
/* numParts : how many names and values */
{
/*  
- pass in a triangle description with tokens and values corresponding to 
      GZ_POSITION:3 vert positions in model space 
- Xform positions of verts using matrix on top of stack 
- Clip - just discard any triangle with any vert(s) behind view plane 
       - optional: test for triangles with all three verts off-screen (trivial frustum cull)
- invoke triangle rasterizer  
*/ 
	return GZ_SUCCESS;
}

/* NOT part of API - just for general assistance */

void sortTriangleVertices(float * values, int ** sortedIndices)
{
	// Default order
	*sortedIndices = (int *)malloc(sizeof(int) * 3);
	(*sortedIndices)[0] = 0;
	(*sortedIndices)[1] = 1;
	(*sortedIndices)[2] = 2;

	float v0 = values[0];
	float v1 = values[1];
	float v2 = values[2];

	int tempIndex;
	float tempFloat;

	if (v0 > v1) {
		tempIndex = (*sortedIndices)[0];
		(*sortedIndices)[0] = (*sortedIndices)[1];
		(*sortedIndices)[1] = tempIndex;
		tempFloat = v0;
		v0 = v1;
		v1 = tempFloat;
	}
	if (v1 > v2) {
		tempIndex = (*sortedIndices)[1];
		(*sortedIndices)[1] = (*sortedIndices)[2];
		(*sortedIndices)[2] = tempIndex;
		tempFloat = v1;
		v1 = v2;
		v2 = tempFloat;
	}
	if (v0 > v1) {
		tempIndex = (*sortedIndices)[0];
		(*sortedIndices)[0] = (*sortedIndices)[1];
		(*sortedIndices)[1] = tempIndex;
		tempFloat = v0;
		v0 = v1;
		v1 = tempFloat;
	}
}

int sign(float value) {
	if (value == 0) {
		return 0;
	}
	return fabsf(value) / value;
}

void getPlane(GzCoord * triangleVertices, float * A, float * B, float * C, float * D) {
	float X1 = triangleVertices[1][X] - triangleVertices[0][X];
	float Y1 = triangleVertices[1][Y] - triangleVertices[0][Y];
	float Z1 = triangleVertices[1][Z] - triangleVertices[0][Z];
	float X2 = triangleVertices[2][X] - triangleVertices[0][X];
	float Y2 = triangleVertices[2][Y] - triangleVertices[0][Y];
	float Z2 = triangleVertices[2][Z] - triangleVertices[0][Z];
	*A = Y1*Z2 - Z1*Y2;
	*B = Z1*X2 - X1*Z2;
	*C = X1*Y2 - Y1*X2;
	float x = triangleVertices[0][X];
	float y = triangleVertices[0][Y];
	float z = triangleVertices[0][Z];
	*D = -1 * ((*A)*x + (*B)*y + (*C)*z);
}

float interpolateZ(float A, float B, float C, float D, float x, float y) {
	return -1 * (A*x + B*y + D) / C;
}

short	ctoi(float color)		/* convert float color to GzIntensity short */
{
  return(short)((int)(color * ((1 << 12) - 1)));
}

