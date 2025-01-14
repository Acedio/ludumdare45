#include "game.h"

#include <SDL_image.h>

#include "leveldata.h"
#include "log.h"

void Game::Update(double t, ButtonState buttons) {
  std::vector<Event> events = hero->Update(t, buttons, *tilemap, boxes.get());
  boxes->Update(t, *tilemap);
  auto o_events = objects->Update(t, hero->BoundingBox());
  events.insert(events.end(), o_events.begin(), o_events.end());
  for (const Event& event : events) {
    if (event.type == EventType::WIN) {
      ++level;
      LoadLevel(level, tileset.get());
      std::cout << "YOU WIN!!!" << std::endl;
    } else if (event.type == EventType::DIE) {
      InitializeSound();
      if (!audio_error && hurt_sound) {
        Mix_PlayChannel(-1, hurt_sound, 0);
      }
      particles.Add(Particle{
          .rect = hero->BoundingBox(),
          .sprite = hero->CurrentSprite(),
          .vel = {5.0, -10.0},
          .angle = 0,
          .rot_vel = 4,
          .remove = false,
      });
      LoadLevel(level, tileset.get());
      std::cout << "YOU DIE!!!" << std::endl;
    } else if (event.type == EventType::JUMP) {
      // TODO: Move this or make it happen on first sound.
      InitializeSound();
      if (!audio_error && jump_sound) {
        Mix_PlayChannel(-1, jump_sound, 0);
      }
    }
  }
  particles.Update(t);
}

void drawBackground(SDL_Renderer* renderer) {
  SDL_SetRenderDrawColor(renderer, 131, 233, 235, 255);
  SDL_RenderClear(renderer);
}

void Game::Draw(SDL_Renderer* renderer) const {
  drawBackground(renderer);
  tilemap->Draw(renderer);
  if (level == 0) {
    SDL_RenderCopy(renderer, overlay_texture, NULL, NULL);
  }

  boxes->Draw(renderer);
  // TODO: Maybe have some objects in front and some behind?
  objects->Draw(renderer);
  hero->Draw(renderer);
  particles.Draw(renderer);
}

void Game::LoadLevel(int level, const TileSet* tileset) {
  SDL_assert(level >= 0 && level < leveldata.size());
  tilemap = TileMap::Load(leveldata[level], tileset);
  boxes = std::make_unique<BoxManager>(tileset,
                                       /*TODO: detect number of columns*/ 100);
  objects = std::make_unique<ObjectManager>(tileset);

  for (const TileMapObject& obj : tilemap->TileMapObjects()) {
    if (obj.type == TileMapObjectType::START) {
      Vec pos{obj.location.x, obj.location.y};
      hero = std::make_unique<Hero>(tileset, pos);
    } else if (obj.type == TileMapObjectType::BOX) {
      boxes->Add(Vec{obj.location.x, obj.location.y});
    } else {
      Vec pos{obj.location.x, obj.location.y};
      objects->AddTileMapObject(obj.type, pos);
    }
  }
}

void Game::InitializeSound() {
  if (!audio_initialized) {
    // BUG: https://github.com/emscripten-ports/SDL2/issues/57
    // BUG: https://github.com/emscripten-core/emscripten/issues/6511
    audio_error = Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0;
    audio_initialized = true;

    if (!audio_error) {
      jump_sound = Mix_LoadWAV("asset_dir/jump.wav");
      hurt_sound = Mix_LoadWAV("asset_dir/hurt.wav");
    }
  }
}

std::unique_ptr<Game> Game::Load(SDL_Renderer* renderer) {
  std::unique_ptr<Game> game(new Game());

  game->tileset_texture = IMG_LoadTexture(renderer, "asset_dir/tiles.png");
  if (!game->tileset_texture) {
    return nullptr;
  }
  game->tileset = std::make_unique<TileSet>(game->tileset_texture);

  game->overlay_texture = IMG_LoadTexture(renderer, "asset_dir/overlay.png");
  if (!game->overlay_texture) {
    return nullptr;
  }

  game->level = 0;
  game->LoadLevel(game->level, game->tileset.get());

  return game;
}

Game::~Game() {
  if (tileset_texture) {
    SDL_DestroyTexture(tileset_texture);
  }
  if (overlay_texture) {
    SDL_DestroyTexture(overlay_texture);
  }
  if (jump_sound) {
    Mix_FreeChunk(jump_sound);
  }
  if (hurt_sound) {
    Mix_FreeChunk(hurt_sound);
  }
}
