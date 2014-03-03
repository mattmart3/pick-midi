#Cmake file for pick-midi project

project(PickMidi)
cmake_minimum_required(VERSION 2.8)

set(PICKMIDI_VERSION_MAJOR 0)
set(PICKMIDI_VERSION_MINOR 0)
set(PICKMIDI_VERSION_SUBMINOR 1)

set(CMAKE_C_CFLAGS "${CMAKE_C_FLAGS}" -Wall --pedantic -g)

set(SRCDIR src)

add_executable(PickMidi ${SRCDIR}/freq_finder.c ${SRCDIR}/note_finder.c ${SRCDIR}/pickmidi.c)

target_link_libraries(PickMidi m)
