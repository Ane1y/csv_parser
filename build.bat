@echo off

IF EXIST .\build rmdir /S /Q .\build
if exist .\CMakeFiles rmdir /S /Q .\CMakeFiles
if exist .\CMakeCache.txt rmdir /S /Q .\CMakeCache.txt
mkdir build && cd build
cmake ..
cmake --build .\ --target ALL_BUILD INSTALL --config Release
move .\Release\csvreader.exe  ..\