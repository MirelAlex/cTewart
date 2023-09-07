@echo off

:: Setup required Environment
:: --------------------------
set RAYLIB_INCLUDE_DIR=C:\raylib\raylib\src
set RAYLIB_LIB_DIR=C:\raylib\raylib\src
set GLFW_INCLUDE_DIR=C:\glfw-3.3.8.bin.WIN64\include\GLFW
set GLFW_LIB_DIR=C:\glfw-3.3.8.bin.WIN64\lib-mingw-w64
set COMPILER_DIR=C:\raylib\w64devkit\bin
set WINDS="C:\Program Files (x86)\Windows Kits\10\Include\10.0.19041.0\shared"
set WIND="C:\Program Files (x86)\Windows Kits\10\Include\10.0.19041.0\um"
set WINDL="C:\Program Files (x86)\Windows Kits\10\Lib\10.0.19041.0\um\x86"

:: Check if FILENAME argument is provided, otherwise set a default value
:: --------------------------------------------------------------------
if "%1"=="" (
  set FILENAME=cTewart
  set FILENAME_FULL_PATH=%CD%/cTewart
) else (
  set FILENAME=%1
  set FILENAME_FULL_PATH=%~f1
)

:: Compilation variables
:: ----------------------
set CFLAGS=-O3 -Wall -Wextra -I./thirdparty/ -I%RAYLIB_INCLUDE_DIR% -I%GLFW_INCLUDE_DIR% 
set LIBS=-L%RAYLIB_LIB_DIR% -L%GLFW_LIB_DIR% -lraylib -lglfw3 -lopengl32 -lgdi32 -lwinmm
:: Extend path
:: ------------
set PATH=%COMPILER_DIR%;%PATH%
@echo on
:: Cleaning latest build
:: ---------------------------
cmd /c if exist %FILENAME_FULL_PATH%.exe del /F %FILENAME_FULL_PATH%.exe


:: Compilation
:: ------------
%COMPILER_DIR%/gcc.exe %CFLAGS% -o %FILENAME%.exe %FILENAME%.c %LIBS%


:: Executing program
:: -------------------------
cmd /c if exist %FILENAME_FULL_PATH%.exe %FILENAME_FULL_PATH%.exe
