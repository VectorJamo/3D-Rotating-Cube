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
	SDL_RenderLine(renderer, p1->x, p1->y, p2->x, p2->y);
}

void renderPoints(struct vec4* points, int numPoints)
{
	int i;
	for (i = 0; i < numPoints-1; i++)
	{
		renderLineFromPoints(&points[i], &points[i + 1]);
	}
}
int main(int argc, char** argv)
{
	createWindowSystem();

	// Init other stuff
	struct vec4 points[] = {
		{10.0f, 20.0f, 1.0f, 1.0f},
		{100.0f, 200.0f, 1.0f, 1.0f},
		{200.0, 300.0f, 1.0f, 1.0f},
		{500.0f, 400.0f, 1.0f, 1.0f},
		{500.0f, 500.0f, 1.0f, 1.0f},
		{600.0f, 600.0f, 1.0f, 1.0f},
	};

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

			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
			SDL_RenderClear(renderer);


			// Render
			SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

			int allPoints = sizeof(points) / sizeof(struct vec4);
			renderPoints(points, allPoints);
			
			SDL_RenderPresent(renderer);
		}
	}

	return 0;
}