#pragma once

#include "vec4.h"

struct mat4x4
{
	float values[4][4];
};

// Matrix operations
struct vec4 multiplyVec(struct mat4x4* mat, struct vec4* vec);
struct mat4x4 multiplyMat(struct mat4x4* mat1, struct mat4x4* mat2);

// Matrix utility functions
void makeNull(struct mat4x4* mat);
void makeIdentity(struct mat4x4* mat);
void printElements(struct mat4x4* mat);

// Matrix types
struct mat4x4 createRotationMatrix(float angleInDegrees, struct vec4 axis);
struct mat4x4 createTranslationMatrix(float x, float y, float z);
struct mat4x4 createPerspectiveProjectionMatrix(float left, float right, float top, float bottom, float zNear, float zFar);
struct mat4x4 createOrthographicProjectionMatrix(float left, float right, float top, float bottom, float zNear, float zFar);

// Special matrices
struct mat4x4 createNormalizedToViewportMatrix(float viewportWidth, float viewportHeight);