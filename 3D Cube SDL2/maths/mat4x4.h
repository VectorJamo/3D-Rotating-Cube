#pragma once

#include "vec4.h"

struct mat4x4
{
	float mat[4][4];
};

// Matrix operations
struct mat4x4 multiplyMat(struct mat4x4 mat1, struct mat4x4 mat2);

// Matrix types
struct mat4x4 createRotationMatrix(float angleInDegrees);
struct mat4x4 createPerspectiveProjectionMatrix(float fovX, float fovY, float zNear, float zFar);
struct mat4x4 createOrthographicProjectionMatrix(float width, float height, float zNear, float zFar);