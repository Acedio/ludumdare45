#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <emscripten.h>

#include "tilemap.h"

SDL_Window *global_window;
SDL_Renderer *global_renderer;

TileMap map;

void drawBackground(SDL_Renderer* renderer) {
  SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255);
  SDL_RenderClear(renderer);
}

void gameLoop() {
  drawBackground(global_renderer);
}

int main() {
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    return -1;
  }

  // TODO: Set up a context to remove the globals.
  global_window = SDL_CreateWindow("Window!", 0, 0, 400, 400, 0);
  if (!global_window) {
    return -1;
  }
  global_renderer = SDL_CreateRenderer(global_window, -1, 0);
  if (!global_renderer) {
    return -1;
  }

  emscripten_set_main_loop(gameLoop, -1, true);

  SDL_DestroyRenderer(global_renderer);
  SDL_DestroyWindow(global_window);
  SDL_Quit();

  return 0;
}
