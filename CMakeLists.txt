# WICHTIG !!! Findet CMAKE die packages nicht? Wurde vscode in nix shell geöffnet? siehe README!
cmake_minimum_required(VERSION 3.20)

project(chip-8-emulator VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED ON) # damit C++26 erzwungen wird
# set(CMAKE_BUILD_TYPE Debug)


set(SOURCES # SOURCES ist Name der neuen Variablen
    src/cpu.cpp
    src/memory.cpp
    src/emulator.cpp
    src/screen.cpp
)

find_package(PkgConfig REQUIRED)

# pkg_check_modules ist eine Funktion von PkgConfig
# das IMPORTED_TARGET sorgt dafür, dass ein target namens PkgConfig::SDL2 verfügbar ist
pkg_check_modules(SDL2 REQUIRED IMPORTED_TARGET sdl2)

# main ist Name des targets. Könnte auch irgendwie anderst benannt werden
add_executable(main src/main.cpp ${SOURCES}) # ${SOURCES} wird durch seinen Wert ersetzt, also durch die cpps
# ${...} ist syntax, dass er nicht den String SOURCES nimmt sondern Inhalt der Variablen. (wie escaping)

# Ohne das müsste man überall #include "../include/emulator/cpu.hpp" schreiben. 
# Das Standard-Verzeichnis bei #include wird zum Ornder /include/emulator (wo alle hpps sind) geändert.
target_include_directories(main PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include/emulator)

target_link_libraries(main PRIVATE PkgConfig::SDL2)
# von hand builden:
# cmake -B build 
# cmake --build build --target main
# build ist der ordner