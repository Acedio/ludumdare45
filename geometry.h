#ifndef GEOMETRY_H
#define GEOMETRY_H

struct Rect {
  double x;
  double y;
  double w;
  double h;
};

struct Vec {
  double x;
  double y;
};

SDL_Rect ToSDLRect(const Rect& rect);

#endif
