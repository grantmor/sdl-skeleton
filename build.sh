rm ./build/native/*.so
clang -std=c99 -fPIC -shared ./src/game_update.c -I./src -o ./build/native/game_update.so
clang -std=c99 ./src/platform_sdl.c -o ./build/native/game -I/usr/include/SDL3 -lSDL3
