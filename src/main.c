#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_timer.h>
#include <SDL3/SDL_video.h>
#include <stdio.h>
#define SDL_MAIN_USE_CALLBACKS 1

#include <SDL3/SDL.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_main.h>

#include "urigin.h"

static SDL_Window *win = NULL;
static SDL_Renderer *renderer = NULL;
static SDL_Texture *texture = NULL;

#define WINDOW_HEIGHT 480
#define WINDOW_WIDTH 272

#define NUM_POINTS 500
#define MIN_PIXELS_PER_SECOND 30
#define MAX_PIXELS_PER_SECOND 60

static SDL_FPoint points[NUM_POINTS];
static float point_speeds[NUM_POINTS];

static Uint64 last_time = 0;

SDL_AppResult SDL_AppInit(void **appstate, int argc, char **argv) {
  SDL_SetAppMetadata("Urigin", "0.0", "com.urigin.engine");

  if (!SDL_Init(SDL_INIT_VIDEO)) {
    SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
    return SDL_APP_FAILURE;
  }

  if (!SDL_CreateWindowAndRenderer("engine", 480, 272, 0, &win, &renderer)) {
    SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
    return SDL_APP_FAILURE;
  }

  if (!SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888,
                         SDL_TEXTUREACCESS_STREAMING, WINDOW_HEIGHT,
                         WINDOW_WIDTH)) {
    SDL_Log("Couldn't create texture %s", SDL_GetError());
    return SDL_APP_FAILURE;
  }

  int i;

  for (i = 0; i < SDL_arraysize(points); i++) {
    points[i].x = SDL_randf() * ((float)WINDOW_WIDTH);
    points[i].y = SDL_randf() * ((float)WINDOW_HEIGHT);
    point_speeds[i] =
        MIN_PIXELS_PER_SECOND +
        (SDL_randf() * (MAX_PIXELS_PER_SECOND - MIN_PIXELS_PER_SECOND));
  }

  last_time = SDL_GetTicks();

  return SDL_APP_CONTINUE;
}

void UpdateRenderer(SDL_Renderer *renderer, SDL_Texture *texture) {
  const Uint64 now = SDL_GetTicks();
  const float elapsed = ((float)(now - last_time)) / 800.0f;
  int i;

  for (i = 0; i < SDL_arraysize(points); i++) {
    const float distance = elapsed * point_speeds[i];
    points[i].x += distance;
    points[i].y += distance;
    if ((points[i].x >= WINDOW_WIDTH) || (points[i].y >= WINDOW_HEIGHT)) {
      if (SDL_rand(2)) {
        points[i].x = SDL_randf() * ((float)WINDOW_WIDTH);
        points[i].y = 0.0f;
      } else {
        points[i].x = 0.0f;
        points[i].y = SDL_randf() * ((float)WINDOW_HEIGHT);
      }
      point_speeds[i] =
          MIN_PIXELS_PER_SECOND +
          (SDL_randf() * (MAX_PIXELS_PER_SECOND - MIN_PIXELS_PER_SECOND));
    }
  }

  last_time = now;

  SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
  SDL_RenderClear(renderer);
  SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
  SDL_RenderPoints(renderer, points, SDL_arraysize(points));
}

SDL_AppResult SDL_AppIterate(void *appstate) {
  SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, SDL_ALPHA_OPAQUE_FLOAT);
  SDL_RenderClear(renderer);

  UpdateRenderer(renderer, texture);

  SDL_RenderPresent(renderer);

  return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result) {
  /* SDL will clean up the window/renderer for us. */
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event) {
  if (event->type == SDL_EVENT_QUIT) {
    return SDL_APP_SUCCESS;
  }
  return SDL_APP_CONTINUE;
}
