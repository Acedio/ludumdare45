all: index.html

asset_dir: assets/tiles.png
	mkdir asset_dir && cp assets/tiles.png asset_dir

index.html: main.cc tilemap.cc asset_dir
	em++ main.cc tilemap.cc -std=c++17 -s WASM=1 -s USE_SDL=2 -s USE_SDL_IMAGE=2 -s SDL2_IMAGE_FORMATS='["png"]' --preload-file asset_dir -o index.html

clean:
	rm -r asset_dir
	rm index.*
