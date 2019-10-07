#include "game.h"

#include <SDL_image.h>

#include "log.h"

void Game::Update(double t, ButtonState buttons) {
  hero->Update(t, buttons, *tilemap, boxes.get());
  boxes->Update(t, *tilemap);
  // TODO: Pipe in hero location.
  auto events = objects->Update(t, Rect());
  for (const Event& event : events) {
    if (event.type == EventType::WIN) {
      std::cout << "YOU WIN!!!" << std::endl;
    }
  }
}

void drawBackground(SDL_Renderer* renderer) {
  SDL_SetRenderDrawColor(renderer, 0, 128, 128, 255);
  SDL_RenderClear(renderer);
}

void Game::Draw(SDL_Renderer* renderer) const {
  drawBackground(renderer);
  tilemap->Draw(renderer);
  boxes->Draw(renderer);
  // TODO: Maybe have some objects in front and some behind?
  objects->Draw(renderer);
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
  game->objects = std::make_unique<ObjectManager>(game->tileset.get());

  for (const TileMapObject& obj : game->tilemap->TileMapObjects()) {
    if (obj.type == TileMapObjectType::START) {
      Vec pos{obj.location.x, obj.location.y};
      game->hero = std::make_unique<Hero>(game->tileset.get(), pos);
    } else if (obj.type == TileMapObjectType::BOX) {
      game->boxes->Add(Vec{obj.location.x, obj.location.y});
    } else {
      Vec pos{obj.location.x, obj.location.y};
      game->objects->AddTileMapObject(obj.type, pos);
    }
  }

  return game;
}

Game::~Game() {
  if (tileset_texture) {
    SDL_DestroyTexture(tileset_texture);
  }
}
