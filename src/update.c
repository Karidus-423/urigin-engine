#include "origin.h"
#include <SDL3/SDL_pixels.h>
#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_stdinc.h>
#include <SDL3/SDL_timer.h>
#include <SDL3/SDL_video.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct Pixel {
  unsigned char a;
  unsigned char b;
  unsigned char g;
  unsigned char r;
} Pixel;

typedef struct {
  Pixel *pixels;
  int pitch;
} Buffer;

// Handles all updates. Texture update, Event Updates, etc...

void UpdateApp(AppState *app) {
  Buffer bfr;
  Uint64 tick = SDL_GetTicks() / 300;
  long tick_x = (cos(tick) + 1) * 100;
  long tick_y = (sin(tick) + 1) * 100;

  if (tick > 255) {
    tick = 0;
  }

  bfr.pitch = app->width * sizeof(Pixel);

  bfr.pixels = malloc(sizeof(Pixel) * (app->height) * (app->width));
  for (int y = 0; y < app->height; y++) {
    for (int x = 0; x < app->width; x++) {
      unsigned char r = (x * tick_x) / (app->width - 1);
      unsigned char b = (y * tick_y) / (app->height - 1);
      bfr.pixels[(y * app->width) + x] = (Pixel){
          .a = 255,
          .b = b,
          .g = 0,
          .r = r,
      };
    }
  }

  SDL_UpdateTexture(app->txtr, NULL, bfr.pixels, bfr.pitch);

  free(bfr.pixels);
}
