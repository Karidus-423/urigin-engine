#include <SDL3/SDL.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_video.h>

typedef enum Window_Flags {
  FULLSCREEN,
  ASPECT_RATIO,
  RESIZABLE,
} Window_Flags;

typedef struct Display {
  int w;
  int h;
} Display;

typedef struct App_State {
  SDL_Window *win;
  SDL_Renderer *renderer;
  SDL_Texture *texture;
  SDL_Event *events;
  Display display;
} App_State;

void UpdateRenderer(App_State *state);
