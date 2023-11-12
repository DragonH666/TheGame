@echo off
set code=..\Game
pushd ..\build
gcc %code%\main.c -shared -march=x86-64 -o Game.dll
popd