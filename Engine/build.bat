@echo off

set code=%cd%
set workaround=
pushd ..\build
::cl -Zi /Od  -MDd %code%/platform.cpp %code%/glad.c glfw3.lib opengl32.lib user32.lib gdi32.lib shell32.lib /Fe:ClassicGames.exe 
g++ -O0 -ggdb %code%/platform.cpp  %code%/glad.c -march=x86-64 %workaround% -static -static-libgcc -static-libstdc++ -L.  -lglfw3 -lgdi32  -o Game.exe
popd
