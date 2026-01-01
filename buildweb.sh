emcc -std=c99 ./src/platform_sdl.c ./lib/web/libSDL3.a -o ./build/web/index.html -I./inc/web --embed-file ./res@./res -s USE_PTHREADS=1
