#include <memory>
#include <SDL.h>
#include <emscripten.h>

#include "buttons.h"
#include "game.h"

SDL_Window *global_window;
SDL_Renderer *global_renderer;

std::unique_ptr<Game> game;

void gameLoop() {
  // Update game logic.
  ButtonState buttons = GetButtonState();
  game->Update(0 /*TODO: timesteps*/, buttons);

  // Draw.
  game->Draw(global_renderer);

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

  game = Game::Load(global_renderer);
  SDL_assert(game != nullptr);

  emscripten_set_main_loop(gameLoop, -1, true);

  SDL_DestroyRenderer(global_renderer);
  SDL_DestroyWindow(global_window);
  SDL_Quit();

  return 0;
}
