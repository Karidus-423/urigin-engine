#include "urigin.h"
#include <SDL3/SDL.h>

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event) {
  App_State *state = (App_State *)appstate;

  if (event->type == SDL_EVENT_QUIT) {
    return SDL_APP_SUCCESS;
  }
  return SDL_APP_CONTINUE;
}
