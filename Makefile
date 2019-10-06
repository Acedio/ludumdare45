all: index.html

asset_dir: assets/tiles.png
	mkdir asset_dir && cp assets/tiles.png asset_dir

# TODO: https://www.gnu.org/software/make/manual/html_node/Pattern-Rules.html
index.html: main.cc tilemap.cc buttons.cc game.cc hero.cc sprite.cc asset_dir
	em++ main.cc tilemap.cc buttons.cc game.cc hero.cc sprite.cc -std=c++17 -s WASM=1 -s USE_SDL=2 -s USE_SDL_IMAGE=2 -s SDL2_IMAGE_FORMATS='["png"]' --preload-file asset_dir -Wall -Werror -g -s DEMANGLE_SUPPORT=1 -o index.html

clean:
	rm -r asset_dir
	rm index.*
