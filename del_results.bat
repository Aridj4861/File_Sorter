@echo off
chcp 65001 > nul
cd /d "%~dp0"

echo Очистка демонстрационных результатов...

if exist examples\input (
    rmdir /s /q examples\input
    echo Удалена папка examples\input
) else (
    echo Папка examples\input не найдена
)

if exist sorter.log (
    del sorter.log
    echo Удалён файл sorter.log
) else (
    echo Файл sorter.log не найден
)

echo.
echo Очистка завершена.
pause