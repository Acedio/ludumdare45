#include "object.h"

std::vector<Event> ObjectManager::Update(double t, const Rect& hero_box) {
  // TODO: check collision
  return {};
}

void ObjectManager::Draw(SDL_Renderer* renderer) const {
  for (const Object& object : objects) {
    SDL_Rect dst = ToSDLRect(object.bounding_box);
    if (object.sprite) {
      object.sprite->Draw(renderer, dst);
    }
  }
}

ObjectType ToObjectType(TileMapObjectType type) {
  switch (type) {
    case TileMapObjectType::EXIT:
      return ObjectType::EXIT;
    default:
      SDL_assert(false);
      return ObjectType::NONE;
  }
}

void ObjectManager::AddTileMapObject(TileMapObjectType type, Vec pos) {
  Object object;
  object.type = ToObjectType(type);
  object.bounding_box = {pos.x, pos.y, 1, 1};
  object.sprite = Sprite(tileset, 6);
  objects.push_back(object);
}
