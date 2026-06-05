@echo off
chcp 65001 > nul
cd /d "%~dp0"

echo Подготовка демонстрационной папки...

if not exist examples\demo_source (
    echo Ошибка: папка examples\demo_source не найдена.
    echo Положи исходные демонстрационные файлы в examples\demo_source.
    pause
    exit /b 1
)

if exist examples\input (
    rmdir /s /q examples\input
)

if exist sorter.log (
    del sorter.log
)

mkdir examples\input

robocopy examples\demo_source examples\input /E > nul

echo.
echo Запуск FileSorter...
echo.

out\build\x64-debug\FileSorter.exe --input examples/input --config examples/config.txt --log sorter.log

echo.
echo Демонстрация завершена.
echo Отсортированные файлы находятся в examples\input
echo Лог-файл: sorter.log
echo.

echo.
echo Демонстрация завершена.
echo Отсортированные файлы находятся в examples\input
echo Лог-файл: sorter.log
echo.

explorer examples\input
notepad sorter.log

pause