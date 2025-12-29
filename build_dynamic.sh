rm *.so
clang -std=c99 -fPIC -shared ./src/game_update.c -I./src -o game_update.so
clang -std=c99 ./src/platform_sdl.c -o game -I/usr/include/SDL3 -lSDL3
