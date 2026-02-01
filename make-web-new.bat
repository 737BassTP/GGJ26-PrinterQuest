cd D:\GitHub\emsdk
call emsdk_env.bat
cd D:\GitHub\737BassTP-GGJ26
emcc Source/test-web.cpp -s USE_SDL=2 -s USE_SDL_IMAGE=2 -s SDL2_IMAGE_FORMATS=["png"] -s USE_SDL_MIXER=2 -o index.html --preload-file assets
pause
emrun index.html
