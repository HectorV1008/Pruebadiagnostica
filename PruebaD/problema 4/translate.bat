@echo off
REM Script para ejecutar problema4.exe de forma directa (arrastrar y soltar archivo .c sobre este .bat)
if "%~1"=="" (
  echo Uso: translate.bat archivo.c
  echo Arrastra un fichero .c sobre este script para traducirlo en el mismo directorio.
  pause
  exit /b 1
)
set "in=%~1"
set "out=%~dpn1_translated%~x1"
"%~dp0\problema4.exe" "%in%" "%out%"
necho Traducido a %out%
pause
