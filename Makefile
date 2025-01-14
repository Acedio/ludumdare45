all: main.js

SRC_FILES := $(wildcard *.cc)
HDR_FILES := $(wildcard *.h)
MAP_DIR := assets
TMX_FILES := $(wildcard $(MAP_DIR)/*.tmx)
CSV_FILES := $(patsubst $(MAP_DIR)/%.tmx,$(MAP_DIR)/%.csv,$(TMX_FILES))
CFLAGS := -std=c++17 -s WASM=1 -s USE_SDL=2 -s USE_SDL_MIXER=2 -s USE_SDL_IMAGE=2 -s SDL2_IMAGE_FORMATS='["png"]' --preload-file asset_dir -Wall -Werror

$(MAP_DIR)/%.csv: $(MAP_DIR)/%.tmx
	tiled --export-map $< $@

leveldata.h: $(CSV_FILES) csv_to_map.sh
	./csv_to_map.sh $(sort $(CSV_FILES)) > $@

asset_dir: assets/*.png assets/*.wav
	-mkdir $@
	cp $^ $@

main.js: $(SRC_FILES) $(HDR_FILES) leveldata.h asset_dir
	em++ $(SRC_FILES) $(CFLAGS) -g -s DEMANGLE_SUPPORT=1 -o $@

release: release/main.js

release/main.js: $(SRC_FILES) $(HDR_FILES) leveldata.h asset_dir index.html
	-mkdir release
	em++ $(SRC_FILES) $(CFLAGS) -Os -o $@
	cp index.html release

clean:
	-rm -r asset_dir
	-rm -r release
	-rm index.* leveldata.h assets/*.csv
