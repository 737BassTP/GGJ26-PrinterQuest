DEVEL = C:\DEVEL
SDL = $(DEVEL)\SDL\x86_64-w64-mingw32
EMSCRIPTEN = %UserProfile%\Documents\GitHub\emsdk\upstream\emscripten\cache\sysroot\include
FFMPEG = $(DEVEL)\ffmpeg

CFLAGS = -std=c++20
LFLAGS = -lSDL2 -lSDL2_image -lSDL2_mixer
INCLUDE = -I"$(SDL)\include" -I"$(EMSCRIPTEN)" -I"$(FFMPEG)\include"
LIBS = -L"$(SDL)\lib" -L"$(FFMPEG)\lib"

FILES = Source/main.cpp
EXE = GGJ26.exe

CC1 = x86_64-w64-mingw32-g++
CC2 = emcc

LWINPRINT = -lgdi32 -lwinspool -lole32 -loleaut32 -luuid -lwiaguid
LFFMPEG = #-lavcodec -lavfilter -lavutil -lswscale -lswresample

all:
	$(CC1) $(CFLAGS) $(INCLUDE) $(LIBS) $(FILES) $(LFLAGS) $(LWINPRINT) $(LFFMPEG) -o $(EXE)
web:
	cd "%UserProfile%\Documents\GitHub\emsdk"
	emsdk_env.bat
	cd "%UserProfile%\source\repos\GGJ26"
	$(CC2) $(FILES) -s USE_SDL=2 -s USE_SDL_IMAGE=2 -s SDL2_IMAGE_FORMATS=["png"] -s USE_SDL_MIXER=2 -o index.html --preload-file assets
web-run:
	emrun index.html
web-all-run:
	make web
	make web-run
clean:
	rm $(EXE)
	#rm index.*
	rm index.data
	rm index.html
	rm index.js
	rm index.wasm
