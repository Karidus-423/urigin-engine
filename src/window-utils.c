#include "urigin.h"

Display GetWindowSize(Window_Flags flag) {
  Display win = {.w = 0, .h = 0};
  SDL_DisplayID id = SDL_GetPrimaryDisplay();
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
