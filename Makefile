all: index.html

index.html: main.cc tilemap.cc
	em++ main.cc tilemap.cc -std=c++17 -s WASM=1 -s USE_SDL=2 -s USE_SDL_MIXER=2 -o index.html
