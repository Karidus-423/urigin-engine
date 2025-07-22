#include <SDL3/SDL.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_video.h>

typedef enum Window_Flags {
  FULLSCREEN,
  ASPECT_RATIO,
  RESIZABLE,
} Window_Flags;

void UpdateRenderer(SDL_Renderer *renderer, SDL_Texture *texture);
