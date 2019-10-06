#include "game.h"

#include <SDL_image.h>

#include "log.h"

void Game::Update(double t, ButtonState buttons) {
  hero->Update(t, buttons, *tilemap);
  boxes->Update(t, *tilemap);
}

void drawBackground(SDL_Renderer* renderer) {
  SDL_SetRenderDrawColor(renderer, 0, 128, 128, 255);
  SDL_RenderClear(renderer);
}

void Game::Draw(SDL_Renderer* renderer) const {
  drawBackground(renderer);
  tilemap->Draw(renderer);
  boxes->Draw(renderer);
  hero->Draw(renderer);
}

std::unique_ptr<Game> Game::Load(SDL_Renderer* renderer) {
  std::unique_ptr<Game> game(new Game());

  game->tileset_texture = IMG_LoadTexture(renderer, "asset_dir/tiles.png");
  if (!game->tileset_texture) {
    return nullptr;
  }
  game->tileset = std::make_unique<TileSet>(game->tileset_texture);
  game->tilemap = TileMap::Load(game->tileset.get());
  game->boxes = std::make_unique<BoxManager>(
      game->tileset.get(), /*TODO: detect number of columns*/ 100);

  Vec start{0, 0};
  for (const TileMapObject& obj : game->tilemap->TileMapObjects()) {
    if (obj.type == ObjectType::START) {
      start.x = obj.location.x;
      start.y = obj.location.y;
    } else if (obj.type == ObjectType::BOX) {
      game->boxes->Add(Vec{obj.location.x, obj.location.y});
    } else if (obj.type == ObjectType::EXIT) {
    } else {
      log("Unknown object type");
    }
  }

  game->hero = std::make_unique<Hero>(game->tileset.get(), start);

  return game;
}

Game::~Game() {
  if (tileset_texture) {
    SDL_DestroyTexture(tileset_texture);
  }
}
