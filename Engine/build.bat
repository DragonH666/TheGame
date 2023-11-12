@echo off

set code=%cd%
set workaround=-Wl,-u,___mingw_vsnprintf -Wl,--defsym,___ms_vsnprintf=___mingw_vsnprintf
pushd ..\build
::cl -Zi /Od  -MDd %code%/platform.cpp %code%/glad.c glfw3.lib opengl32.lib user32.lib gdi32.lib shell32.lib /Fe:ClassicGames.exe 
g++ -O0 %code%/platform.cpp  %code%/glad.o -march=x86-64  %workaround% -L.  -lglfw3 -lgdi32  -o Game.exe
popd
