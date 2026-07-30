cmake_minimum_required(VERSION 3.20)

project(chip-8-emulator VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED ON) # damit C++26 erzwungen wird
# set(CMAKE_BUILD_TYPE Debug)


set(SOURCES # SOURCES ist Name der neuen Variablen
    src/cpu.cpp
    src/memory.cpp
)

# main ist Name des targets. Könnte auch irgendwie anderst benannt werden
add_executable(main src/main.cpp ${SOURCES}) # ${SOURCES} fügt wird durch seinen Wert ersetzt, also durch die cpps

# Ohne das müsste man überall #include "../include/emulator/cpu.hpp" schreiben. 
# Das Standard-Verzeichnis bei #include wird zu /include (wo alle hpps sind) geändert.
target_include_directories(main PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

# von hand builden:
# cmake -B build 
# cmake --build build --target main
# build ist der ordner