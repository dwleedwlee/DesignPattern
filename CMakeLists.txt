cmake_minimum_required(VERSION 3.0)
project("Design pattern in embedded C")
message("project name: ${CMAKE_PROJECT_NAME}")
message("install dir: ${CMAKE_INSTALL_PREFIX}")

set(CMAKE_CXX_COMPILER "C:/MinGW/bin/gcc")
if("$ENV{BLDOPT}" STREQUAL "DEBUG")
	set(CMAKE_BUILD_TYPE Debug)
else() 
	set(CMAKE_BUILD_TYPE Release)
endif()

add_subdirectory(Embedded_C/_DataStructure)
add_subdirectory(Embedded_C/Creational)
add_subdirectory(Embedded_C/Behavioral)
add_subdirectory(Embedded_C/Structural)

#To Do: Link to data structure
add_subdirectory(Embedded_C/_MemoryPool)
