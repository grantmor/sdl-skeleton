memory=$((1024 * 1024 * 1024))

target=native
render=""
defines=""
debug=""

for arg in "$@"; do
  case "$arg" in
    r=*) render="${arg#*=}" ;;
    t=*) target="${arg#*=}" ;;
    d=*) debug="${arg#*=}" ;;
  esac
done

# set default debug based on target if not explicitly passed
if [[ -z "$debug" ]]; then
  if [[ "$target" == web ]]; then
    debug=0
  else
    debug=1
  fi
fi

if [[ "$target" == web ]]; then
  render=gl
  defines=$defines"-DRENDER_GL "
  symbols=""

  if [[ "$debug" == 1 ]]; then
    symbols="-g -sASSERTIONS=2 -sSAFE_HEAP=1"
  else
      symbols=""
  fi

elif [[ "$target" == native ]]; then
  if [[ "$render" == sdl ]]; then
    defines=$defines"-DRENDER_SDL_2D "
  elif [[ "$render" == gl ]]; then
    defines=$defines"-DRENDER_GL "
  else
    render=sdl
    defines=$defines"-DRENDER_SDL_2D "
  fi

  if [[ "$debug" == 1 ]]; then
    defines=$defines"-DDEBUG "
    symbols="-g"
  else
    symbols=""
  fi
fi

echo memory: $memory
echo target: $target
echo render: $render
echo defines: $defines
echo symbols: $symbols

rm ./build/native/*.so
if [[ "$target" == native ]]; then
  clang -std=c99 -Werror=vla $symbols -fPIC -shared ./src/game_update.c -I./src -I./inc -o ./build/native/game_update.so $defines
  clang -std=c99 -Werror=vla $symbols ./src/platform_sdl.c -o ./build/native/game -I/usr/include/SDL3 -I/inc -lSDL3 $defines
elif [[ "$target" == web ]]; then
  emcc -std=c99 ./src/platform_sdl.c ./lib/web/libSDL3.a -o ./build/web/index.html -I./inc/web -I./inc -sINITIAL_MEMORY=$memory --embed-file ./res@./res -sUSE_PTHREADS=1 $symbols
fi
