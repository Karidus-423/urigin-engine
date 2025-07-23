#include "urigin.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_surface.h>

void UpdateRenderer(App_State *state) {
  SDL_Renderer *r = state->renderer;
  SDL_Texture *t = state->texture;
  Display d = state->display;
  // Clear renderer------------------------------------------------------------
  SDL_SetRenderDrawColor(r, 0, 0, 0, SDL_ALPHA_OPAQUE);
  SDL_RenderClear(r);
  SDL_SetRenderDrawColor(r, 255, 255, 255, SDL_ALPHA_OPAQUE);
  //---------------------------------------------------------------------------
}
