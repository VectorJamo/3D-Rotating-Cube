#include <stdio.h>
#include <SDL3/SDL.h>

#include "maths/vec4.h"
#include "maths/mat4x4.h"

static SDL_Window* window = NULL;
static SDL_Renderer* renderer = NULL;

void createWindowSystem()
{
	int status = SDL_Init(SDL_INIT_VIDEO);

	if (!status)
		printf(":(");
	else
		printf("SDL Init successful!\n");

	window = SDL_CreateWindow("3D Cube", 800, 800, 0);
	renderer = SDL_CreateRenderer(window, NULL);
}

void renderLineFromPoints(struct vec4* p1, struct vec4* p2)
{
	SDL_RenderLine(renderer, p1->x/p1->w, p1->y / p1->w, p2->x / p2->w, p2->y / p2->w);
}

void renderPoints(struct vec4* points, int numPoints, int isShape)
{
	int i;
	for (i = 0; i < numPoints-1; i++)
	{
		renderLineFromPoints(&points[i], &points[i + 1]);
	}
	if (isShape)
	{
		renderLineFromPoints(&points[0], &points[numPoints - 1]);
	}
}

struct vec4* transformPoints(struct vec4* points, int numPoints, struct mat4x4* projectionMatrix, struct mat4x4* viewportMatrix) 
{
	struct vec4* transformedPoints = (struct vec4*)(malloc(sizeof(struct vec4)*numPoints));
	if (transformedPoints == NULL)
	{
		printf("Failed to allocate memory. \n");
		return;
	}
	int i;
	for (i = 0; i < numPoints; i++)
	{
		struct vec4 worldToNDC = multiplyVec(projectionMatrix, &points[i]);
		struct vec4 ndcToViewport = multiplyVec(viewportMatrix, &worldToNDC);

		transformedPoints[i] = ndcToViewport;
	}
	return transformedPoints;
}
int main(int argc, char** argv)
{
	createWindowSystem();

	// Init other stuff
	
	// Matrices
	// Create the perspective and orthographic matrix
	printf("Perspective Matrix: \n");
	struct mat4x4 perspective = createPerspectiveProjectionMatrix(-200.0f, 200.0f, 200.0f, -200.0f, 50.0f, 200.0f);
	printElements(&perspective);

	printf("Orthographic Matrix:\n");
	struct mat4x4 orthographic = createOrthographicProjectionMatrix(-200.0f, 200.0f, 200.0f, -200.0f, 50.0f, 200.0f);
	printElements(&orthographic);
	
	printf("Viewport Matrix:\n");
	struct mat4x4 viewportMatrix = createNormalizedToViewportMatrix(800, 800);
	printElements(&viewportMatrix);

	// Rectangle
	struct vec4 points[] = {
		{-50.0f,  50.0f, 10.0f, 1.0f},
		{-50.0f, -50.0f, 10.0f, 1.0f},
		{50.0f,  -50.0f, 10.0f, 1.0f},
		{50.0f,   50.0f, 10.0f, 1.0f},
	};

	// Cube position coordinates (local space)
	struct vec4 cube[] = {
		{-50.0f,  50.0f, -50.0f, 1.0f}, // Front
		{-50.0f, -50.0f, -50.0f, 1.0f}, // Front
		{50.0f,  -50.0f, -50.0f, 1.0f}, // Front
		{50.0f,   50.0f, -50.0f, 1.0f}, // Front

		{-50.0f,  50.0f, 50.0f, 1.0f}, // Back
		{-50.0f, -50.0f, 50.0f, 1.0f}, // Back
		{50.0f,  -50.0f, 50.0f, 1.0f}, // Back
		{50.0f,   50.0f, 50.0f, 1.0f}, // Back

		{-50.0f,   50.0f, -50.0f, 1.0f}, // Left
		{-50.0f,  -50.0f, -50.0f, 1.0f}, // Left
		{-50.0f,  -50.0f, 50.0f, 1.0f}, // Left
		{-50.0f,   50.0f, 50.0f, 1.0f}, // Left

		{50.0f,   50.0f, -50.0f, 1.0f}, // Right
		{50.0f,  -50.0f, -50.0f, 1.0f}, // Right
		{50.0f,  -50.0f, 50.0f, 1.0f}, // Right
		{50.0f,   50.0f, 50.0f, 1.0f}, // Right

		{-50.0f, 50.0f, -50.0f, 1.0f}, // Top
		{ 50.0f, 50.0f, -50.0f, 1.0f}, // Top
		{ 50.0f, 50.0f, 50.0f, 1.0f}, // Top
		{-50.0f, 50.0f, 50.0f, 1.0f}, // Top

		{-50.0f, -50.0f, -50.0f, 1.0f}, // Bottom
		{ 50.0f, -50.0f, -50.0f, 1.0f}, // Bottom
		{ 50.0f, -50.0f, 50.0f, 1.0f}, // Bottom
		{-50.0f,  50.0f, 50.0f, 1.0f}, // Bottom
	};

	// Multiply the vectors with the matrices
	struct vec4 worldToNDC = multiplyVec(&orthographic, &points[0]);
	struct vec4 ndcToViewport = multiplyVec(&viewportMatrix, &worldToNDC);

	printVec(&points[0], "World Vector:");
	printVec(&worldToNDC, "NDC Vector:");
	printVec(&ndcToViewport, "Final Viewport Vector:");

	float angle = 0.0f;
	float speed = 0.01f;

	printf("Perspective projection matrix: \n");
	struct mat4x4 perspectiveProjectionMatrix = createPerspectiveProjectionMatrix(-200.0f, 200.0f, 200.0f, -200.0f, 50.0f, 200.0f);
	printElements(&perspectiveProjectionMatrix);

	int running = 1;
	while (running)
	{
		SDL_Event ev;
		while (SDL_PollEvent(&ev))
		{
			if (ev.type == SDL_EVENT_QUIT)
			{
				running = 0;
			}
		}
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
		SDL_RenderClear(renderer);

		// Model matrices (Local space to world space)
		// Translation Matrix
		// Rotation Matrix
		struct vec4 rotationAxis = { 0.0f, 1.0f, 0.0f, 0.0f };
		struct mat4x4 rotationMatrix = createRotationMatrix(angle, rotationAxis);

		struct mat4x4 translationMatrix = createTranslationMatrix(0.0f, 0.0f, 100.0f);

		struct vec4* newCubeVerts = (struct vec4*)malloc(sizeof(struct vec4) * 4 * 6);

		if (newCubeVerts == NULL)
		{
			printf("Failed to allocate memeory.");
			return -1;
		}
		// First, apply rotation.
		int j;
		for (j = 0; j < 4 * 6; j++)
		{
			newCubeVerts[j] = multiplyVec(&rotationMatrix, &cube[j]);
		}
		// Then, translation.
		for (j = 0; j < 4 * 6; j++)
		{
			newCubeVerts[j] = multiplyVec(&translationMatrix, &newCubeVerts[j]);
		}

		// Transform all the cube's points
		struct vec4* transformedCube = transformPoints(newCubeVerts, 4 * 6, &perspectiveProjectionMatrix, &viewportMatrix);

		// Render
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

		renderPoints(transformedCube, 4 * 6, 1);

		SDL_RenderPresent(renderer);

		free(newCubeVerts);
		free(transformedCube);
		angle += speed;
	}

	return 0;
}