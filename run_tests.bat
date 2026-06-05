	@echo off
cd /d "%~dp0\out\build\x64-debug"

ctest --output-on-failure

pause