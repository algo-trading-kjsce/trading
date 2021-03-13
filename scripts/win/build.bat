@echo off

md build
cd build

set buildmode=%1

if "%buildmode%"=="" set buildmode=Debug

@echo * Creating CMake files...%\n%%\n%%\n%

call "C:\Program Files (x86)\Microsoft Visual Studio\2019\Professional\VC\Auxiliary\Build\vcvars32.bat"

cmake -DCMAKE_BUILD_TYPE=%buildmode% -G "NMake Makefiles" ..\cpp ..\build

@echo %\n%%\n%* Finished creating CMake files.%\n%%\n%%\n%

@echo * Starting build...%\n%%\n%%\n%

nmake

@echo %\n%%\n%* Finished build.%\n%