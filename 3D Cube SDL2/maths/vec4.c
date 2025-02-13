#include "vec4.h"

void printVec(struct vec4* vec, const char* name)
{
	printf("%s", name);
	printf("[%f, %f, %f, %f] \n", vec->x, vec->y, vec->z, vec->w);
}
