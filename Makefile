DEVEL = C:\DEVEL
SDL = $(DEVEL)\SDL\x86_64-w64-mingw32
EMSCRIPTEN = %UserProfile%\Documents\GitHub\emsdk\upstream\emscripten\cache\sysroot\include

CFLAGS = -std=c++20
LFLAGS = -lSDL2 -lSDL2_image -lSDL2_mixer
INCLUDE = -I"$(SDL)\include" -I"$(EMSCRIPTEN)"
LIBS = -L"$(SDL)\lib"

FILES = Source/main.cpp
EXE = GGJ26.exe

CC1 = x86_64-w64-mingw32-g++

all:
	$(CC1) $(CFLAGS) $(INCLUDE) $(LIBS) $(FILES) $(LFLAGS) -o $(EXE)
web:
	cd "%UserProfile%\Documents\GitHub\emsdk"
	emsdk_env.bat
	cd "%UserProfile%\source\repos\GGJ26"
	emcc $(FILES)
clean:
	rm $(EXE)

