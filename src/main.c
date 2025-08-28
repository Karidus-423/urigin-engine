#include <SDL3/SDL_events.h>
#include <SDL3/SDL_gpu.h>
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

#define NUM_POINTS 500
#define MIN_PIXELS_PER_SECOND 30
#define MAX_PIXELS_PER_SECOND 60

#define APP_NAME "Urigin-Engine"
#define APP_VER "0.0.1"
#define APP_ID "com.urigin.engine"

static SDL_FPoint points[NUM_POINTS];
static float point_speeds[NUM_POINTS];

static Uint64 last_time = 0;

SDL_AppResult SDL_AppInit(void **appstate, int argc, char **argv) {
  SDL_SetAppMetadata(APP_NAME, APP_VER, APP_ID);

  if (!SDL_Init(SDL_INIT_VIDEO)) {
    SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
    return SDL_APP_FAILURE;
  }

  static App_State state = {
      .win = NULL,
      .renderer = NULL,
      .texture = NULL,
      .display = {.w = 0, .h = 0},
  };

  Display display = GetWindowSize(RESIZABLE);
  state.display = display;

  if (!SDL_CreateWindowAndRenderer(APP_NAME, display.w, display.h, 0,
                                   &state.win, &state.renderer)) {
    SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
    return SDL_APP_FAILURE;
  }

  state.texture =
      SDL_CreateTexture(state.renderer, SDL_PIXELFORMAT_RGBA8888,
                        SDL_TEXTUREACCESS_STREAMING, display.h, display.w);
  if (state.texture == NULL) {
    SDL_Log("Couldn't create texture %s", SDL_GetError());
    return SDL_APP_FAILURE;
  }

  state.gpu = SDL_CreateGPUDevice(SDL_GPU_SHADERFORMAT_SPIRV, false, NULL);
  if (state.gpu == NULL) {
    SDL_Log("Couldn't create gpu device: %s", SDL_GetError());
  }

  if (!SDL_ClaimWindowForGPUDevice(state.gpu, state.win)) {
    SDL_Log("Window couldn't claim gpu: %s", SDL_GetError());
  }

  // SDL gives us an addres void** for use to use in their other functions.
  // We are assigning that location to be of an App_State and have the
  // pointer to our &state.
  *(App_State **)appstate = &state;

  return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void *appstate) {
  App_State *state = (App_State *)appstate;
  SDL_Renderer *renderer = state->renderer;
  SDL_Texture *texture = state->texture;

  SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, SDL_ALPHA_OPAQUE_FLOAT);
  SDL_RenderClear(renderer);

  UpdateRenderer((App_State *)appstate);

  SDL_RenderPresent(renderer);

  return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result) {
  /* SDL will clean up the window/renderer for us. */
}
