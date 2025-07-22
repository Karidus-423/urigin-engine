#include <SDL3/SDL_properties.h>
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

#define WINDOW_HEIGHT SetWindowHeight(float percent);
#define WINDOW_WIDTH SetWindowHeight(float percent);

#define NUM_POINTS 500
#define MIN_PIXELS_PER_SECOND 30
#define MAX_PIXELS_PER_SECOND 60

#define APP_NAME "Urigin-Engine"
#define APP_VER "0.0.0"
#define APP_ID "com.urigin.engine"

static SDL_FPoint points[NUM_POINTS];
static float point_speeds[NUM_POINTS];

static Uint64 last_time = 0;

typedef struct Display {
  int w;
  int h;
} Display;

Display GetWindowSize(Window_Flags flag) {
  Display win = {.w = 0, .h = 0};
  SDL_DisplayID id = SDL_GetPrimaryDisplay();
  // SDL_PropertiesID id_prop = SDL_GetDisplayProperties(id);
  const SDL_DisplayMode *info = SDL_GetDesktopDisplayMode(id);
  float percent = 0.0;

  switch (flag) {
  case FULLSCREEN:
    win.h = info->h;
    win.w = info->w;
    break;
  case ASPECT_RATIO:
    break;
  case RESIZABLE:
    percent = 0.4;
    win.h = info->h * percent;
    win.w = info->w * percent;
    break;
  }
  return win;
}

SDL_AppResult SDL_AppInit(void **appstate, int argc, char **argv) {
  SDL_SetAppMetadata(APP_NAME, APP_VER, APP_ID);

  if (!SDL_Init(SDL_INIT_VIDEO)) {
    SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
    return SDL_APP_FAILURE;
  }

  Display size = GetWindowSize(RESIZABLE);

  if (!SDL_CreateWindowAndRenderer("engine", size.w, size.h, 0, &win,
                                   &renderer)) {
    SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
    return SDL_APP_FAILURE;
  }

  texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888,
                              SDL_TEXTUREACCESS_STREAMING, size.h, size.w);
  if (!texture) {
    SDL_Log("Couldn't create texture %s", SDL_GetError());
    return SDL_APP_FAILURE;
  }

  int i;

  for (i = 0; i < SDL_arraysize(points); i++) {
    points[i].x = SDL_randf() * ((float)size.h);
    points[i].y = SDL_randf() * ((float)size.h);
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

  int width = texture->w;
  int height = texture->h;

  for (i = 0; i < SDL_arraysize(points); i++) {
    const float distance = elapsed * point_speeds[i];
    points[i].x += distance;
    points[i].y += distance;
    if ((points[i].x >= width) || (points[i].y >= height)) {
      if (SDL_rand(2)) {
        points[i].x = SDL_randf() * ((float)height);
        points[i].y = 0.0f;
      } else {
        points[i].x = 0.0f;
        points[i].y = SDL_randf() * ((float)width);
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
