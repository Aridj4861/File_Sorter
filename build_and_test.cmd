@echo off
chcp 65001 > nul
cd /d "%~dp0"

echo Удаление старой папки build...
if exist build (
    rmdir /s /q build
)

echo.
echo Настройка CMake...
cmake -S . -B build

echo.
echo Сборка проекта...
cmake --build build

echo.
echo Запуск тестов...
cd build
ctest --output-on-failure

echo.
pause