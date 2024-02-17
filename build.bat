@echo off

:: Setup required Environment
:: --------------------------
set RAYLIB_INCLUDE_DIR=C:\raylib\raylib\src
set RAYLIB_LIB_DIR=C:\raylib\raylib\src
:: set GLFW_INCLUDE_DIR=C:\glfw-3.3.8.bin.WIN64\include\GLFW
:: set GLFW_LIB_DIR=C:\glfw-3.3.8.bin.WIN64\lib-mingw-w64
set COMPILER_DIR=C:\raylib\w64devkit\bin
::set WINDS="C:\Program Files (x86)\Windows Kits\10\Include\10.0.19041.0\shared"
::set WIND="C:\Program Files (x86)\Windows Kits\10\Include\10.0.19041.0\um"
::set WINDL="C:\Program Files (x86)\Windows Kits\10\Lib\10.0.19041.0\um\x86"

:: Set build folder and cd to it, if already exists just cd to it
:: ---------------------------------------------------------------
set BUILD_DIR=build
mkdir %BUILD_DIR% 2>nul
cd %BUILD_DIR%

:: Check if FILENAME argument is provided and set it as filename, otherwise set a default value
:: --------------------------------------------------------------------------------------------
if "%1"=="" (
  set FILENAME=cTewart
  set FILENAME_FULL_PATH=%CD%/cTewart
) else (
  set FILENAME=%1
  set FILENAME_FULL_PATH=%~f1
)
echo === Filename set to "%FILENAME%"
echo === Fullpath set to "%FILENAME_FULL_PATH%.exe"

:: Compilation variables
:: ----------------------
set CMD=%SystemRoot%\System32\cmd.exe
set CFLAGS=-O3 -Wall -Wextra -Wunused-parameter -I%RAYLIB_INCLUDE_DIR%
set LIBS=-L%RAYLIB_LIB_DIR% -lraylib -lopengl32 -lgdi32 -lwinmm

:: Extend path
:: ------------
set PATH=%COMPILER_DIR%;%PATH%

:: Cleaning latest build (cTewart / other exec)
:: --------------------------------------------
if /I "%FILENAME%"=="cTewart" (
  if exist "%FILENAME_FULL_PATH%.exe" (
    del /F "%FILENAME_FULL_PATH%.exe"
    echo === "%FILENAME%.exe" already existed -- cleaned successfully.
  ) else (
    echo === "%FILENAME%.exe" not found, no need to clear.
  )
) else (
  if exist "%FILENAME_FULL_PATH%.exe" (
    del /F "%FILENAME_FULL_PATH%.exe"
    echo === "%FILENAME%.exe" already existed -- cleaned successfully.
  ) else (
    echo === "%FILENAME%.exe" not found, no need to clear.
  )
)

:: Compilation
:: ------------
%COMPILER_DIR%/gcc.exe %CFLAGS% -o %FILENAME%.exe ../src/%FILENAME%.c %LIBS%

:: Executing program
:: -----------------
if exist "%FILENAME_FULL_PATH%.exe" (
  echo === Running "%FILENAME%.exe"...
  %CMD% /c %FILENAME_FULL_PATH%.exe
)
