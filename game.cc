#include "game.h"

#include <SDL_image.h>

void Game::Update(double t, ButtonState buttons) {
}

void drawBackground(SDL_Renderer* renderer) {
  SDL_SetRenderDrawColor(renderer, 0, 128, 128, 255);
  SDL_RenderClear(renderer);
}

void Game::Draw(SDL_Renderer* renderer) const {
  drawBackground(renderer);
  tilemap->Draw(renderer);
}

std::unique_ptr<Game> Game::Load(SDL_Renderer* renderer) {
  std::unique_ptr<Game> game(new Game());

  game->tileset_texture = IMG_LoadTexture(renderer, "asset_dir/tiles.png");
  if (!game->tileset_texture) {
    return nullptr;
  }
  game->tilemap = TileMap::Load(game->tileset_texture);

  return game;
}

Game::~Game() {
  if (tileset_texture) {
    SDL_DestroyTexture(tileset_texture);
  }
}
