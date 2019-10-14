#include <emscripten.h>
#include <memory>
#include <SDL.h>
#include <SDL2/SDL_mixer.h>

#include "buttons.h"
#include "game.h"

SDL_Window *global_window;
SDL_Renderer *global_renderer;

std::unique_ptr<Game> game;

const double kTimestep = 1.0/60.0;

void gameLoop() {
  // Update game logic.
  ButtonState buttons = GetButtonState();
  game->Update(kTimestep, buttons);

  // Draw.
  game->Draw(global_renderer);

  SDL_RenderPresent(global_renderer);
}

int main() {
  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
    return -1;
  }

  // TODO: Set up a context to remove the globals.
  global_window = SDL_CreateWindow("Lift Off!", 0, 0, 640, 480, 0);
  if (!global_window) {
    return -1;
  }
  global_renderer = SDL_CreateRenderer(global_window, -1, 0);
  if (!global_renderer) {
    return -1;
  }
  SDL_RenderSetScale(global_renderer, 2, 2);

  // BUG: https://github.com/emscripten-ports/SDL2/issues/57
  // BUG: https://github.com/emscripten-core/emscripten/issues/6511
  if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
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
