#include "mat4x4.h"
#include <math.h>

#define PI 3.1415f

struct vec4 multiplyVec(struct mat4x4* mat, struct vec4* vec)
{
	struct vec4 transformedVec;
	transformedVec.x = (mat->values[0][0] * vec->x + mat->values[0][1] * vec->y + mat->values[0][2] * vec->z + mat->values[0][3] * vec->w);
	transformedVec.y = (mat->values[1][0] * vec->x + mat->values[1][1] * vec->y + mat->values[1][2] * vec->z + mat->values[1][3] * vec->w);
	transformedVec.z = (mat->values[2][0] * vec->x + mat->values[2][1] * vec->y + mat->values[2][2] * vec->z + mat->values[2][3] * vec->w);
	transformedVec.w = (mat->values[3][0] * vec->x + mat->values[3][1] * vec->y + mat->values[3][2] * vec->z + mat->values[3][3] * vec->w);

	return transformedVec;
}

struct mat4x4 multiplyMat(struct mat4x4* mat1, struct mat4x4* mat2)
{
	struct mat4x4 newMat;
	makeIdentity(&newMat);

	int i;
	for (i = 0; i < 4; i++)
	{
		newMat.values[i][0] = mat1->values[i][0] * mat2->values[0][0] + mat1->values[i][1] * mat2->values[1][0] + mat1->values[i][2] * mat2->values[2][0] + mat1->values[i][3] * mat2->values[3][0];
		newMat.values[i][1] = mat1->values[i][0] * mat2->values[0][1] + mat1->values[i][1] * mat2->values[1][1] + mat1->values[i][2] * mat2->values[2][1] + mat1->values[i][3] * mat2->values[3][1];
		newMat.values[i][2] = mat1->values[i][0] * mat2->values[0][2] + mat1->values[i][1] * mat2->values[1][2] + mat1->values[i][2] * mat2->values[2][2] + mat1->values[i][3] * mat2->values[3][2];
		newMat.values[i][3] = mat1->values[i][0] * mat2->values[0][3] + mat1->values[i][1] * mat2->values[1][3] + mat1->values[i][2] * mat2->values[2][3] + mat1->values[i][3] * mat2->values[3][3];
	}
	return newMat;
}

struct mat4x4 createRotationMatrix(float angleInDegrees, struct vec4 axis)
{
	struct mat4x4 rotationMatrix;
	makeIdentity(&rotationMatrix);

	float angleInRadians = angleInDegrees * (PI / 180.0f);

	if (axis.x)
	{
		rotationMatrix.values[1][1] = cos(angleInRadians);
		rotationMatrix.values[1][2] = -sin(angleInRadians);
		rotationMatrix.values[2][1] = sin(angleInRadians);
		rotationMatrix.values[2][2] = cos(angleInRadians);
	}
	else if (axis.y)
	{
		rotationMatrix.values[0][0] = cos(angleInRadians);
		rotationMatrix.values[0][2] = sin(angleInRadians);
		rotationMatrix.values[2][0] = -sin(angleInRadians);
		rotationMatrix.values[2][2] = cos(angleInRadians);
	}
	else if (axis.z)
	{
		rotationMatrix.values[0][0] = cos(angleInRadians);
		rotationMatrix.values[0][1] = -sin(angleInRadians);
		rotationMatrix.values[1][0] = sin(angleInRadians);
		rotationMatrix.values[1][1] = cos(angleInRadians);
	}

	return rotationMatrix;
}

struct mat4x4 createTranslationMatrix(float x, float y, float z)
{
	struct mat4x4 translationMatrix;
	makeIdentity(&translationMatrix);

	translationMatrix.values[0][3] = x;
	translationMatrix.values[1][3] = y;
	translationMatrix.values[2][3] = z;
	
	return translationMatrix;
}


struct mat4x4 createPerspectiveProjectionMatrix(float left, float right, float top, float bottom, float zNear, float zFar)
{
	struct mat4x4 perspective, orthographic;
	makeNull(&perspective);
	makeIdentity(&orthographic);

	perspective.values[0][0] = zNear;
	perspective.values[1][1] = zNear;
	perspective.values[2][2] = zNear + zFar;
	perspective.values[2][3] = -(zNear * zFar);
	perspective.values[3][2] = 1;

	orthographic.values[0][0] = 2.0f / (right - left);
	orthographic.values[1][1] = 2.0f / (top - bottom);
	orthographic.values[2][2] = 2.0f / (zFar - zNear);
	orthographic.values[2][3] = -((zFar + zNear) / (zFar - zNear));

	struct mat4x4 perspectiveProjectionMatrix = multiplyMat(&orthographic, &perspective);
	return perspectiveProjectionMatrix;
}

struct mat4x4 createOrthographicProjectionMatrix(float left, float right, float top, float bottom, float zNear, float zFar)
{
	struct mat4x4 orthographic;
	makeIdentity(&orthographic);

	orthographic.values[0][0] = 2.0f / (right - left);
	orthographic.values[1][1] = 2.0f / (top - bottom);
	orthographic.values[2][2] = 2.0f / (zFar - zNear);
	orthographic.values[2][3] = -((zFar + zNear) / (zFar - zNear));

	return orthographic;
}

struct mat4x4 createNormalizedToViewportMatrix(float viewportWidth, float viewportHeight)
{
	// TODO 1st
	struct mat4x4 viewportTransform;
	makeIdentity(&viewportTransform);

	viewportTransform.values[0][0] = viewportWidth / 2.0f;
	viewportTransform.values[0][3] = viewportWidth / 2.0f;
	viewportTransform.values[1][1] = viewportHeight / 2.0f;
	viewportTransform.values[1][3] = viewportHeight / 2.0f;

	return viewportTransform;
}

void makeNull(struct mat4x4* mat)
{
	int row, col;
	for (row = 0; row < 4; row++)
	{
		for (col = 0; col < 4; col++)
		{
			mat->values[row][col] = 0.0f; // Set all the values to be 0
		}
	}
}

void makeIdentity(struct mat4x4* mat)
{
	int row, col;
	for (row = 0; row < 4; row++)
	{
		for (col = 0; col < 4; col++)
		{
			if (row == col)
			{
				mat->values[row][col] = 1.0f;
				
			} else {
				mat->values[row][col] = 0.0f; // Set all the values to be 0
			}
		}
	}
}

void printElements(struct mat4x4* mat)
{
	int row, col;
	for (row = 0; row < 4; row++)
	{
		printf("[");
		for (col = 0; col < 4; col++)
		{
			printf(col == 3 ? "%f] \n" : "%f ", mat->values[row][col]);
		}
	}
}


