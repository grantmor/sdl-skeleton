#clang -std=c99 ./src/platform_sdl.c -o game -I/usr/include/SDL3 -lSDL3
emcc -std=c99 ./src/platform_sdl.c ./lib/web/libSDL3.a -o index.html -I./inc/web --embed-file ./res@./res -s USE_PTHREADS=1
