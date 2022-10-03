@echo off
if "%1"=="clean" (
	del /F CMakeCache.txt
	del /F Makefile
	del /F cmake_install.cmake
	rd /S /Q CMakeFiles
	rd /S /Q Embedded_C
) else (
	if "%1"=="debug" (
		set BLDOPT=DEBUG
	) else (
		set BLDOPT=RELEASE
	)
	
	cmake -G "MinGW Makefiles" ..
	make
)
