@echo off
echo === Building MyLib Release for Windows ===
cmake -B build -G "Visual Studio 17 2022" -A x64
if errorlevel 1 goto :error
cmake --build build --config Release
if errorlevel 1 goto :error
cmake --install build --config Release
if errorlevel 1 goto :error
echo === Done! Output in .\dist ===
exit /b 0

:error
echo *** Build failed ***
exit /b 1