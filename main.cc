#include <memory>
#include <SDL.h>
#include <SDL_image.h>
#include <emscripten.h>

#include "tilemap.h"

SDL_Window *global_window;
SDL_Renderer *global_renderer;

std::unique_ptr<TileMap> map;

void drawBackground(SDL_Renderer* renderer) {
  SDL_SetRenderDrawColor(renderer, 0, 128, 128, 255);
  SDL_RenderClear(renderer);
}

void gameLoop() {
  drawBackground(global_renderer);
  drawMap(*map, global_renderer);

  SDL_RenderPresent(global_renderer);
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

  // TODO: Set color key.
  SDL_Texture *tileset_texture =
      IMG_LoadTexture(global_renderer, "asset_dir/tiles.png");
  if (!tileset_texture) {
    return -1;
  }
  map = load(tileset_texture);

  emscripten_set_main_loop(gameLoop, -1, true);

  SDL_DestroyRenderer(global_renderer);
  SDL_DestroyWindow(global_window);
  SDL_Quit();

  return 0;
}
