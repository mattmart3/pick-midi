#Cmake file for pick-midi project

project(PickMidi)
cmake_minimum_required(VERSION 2.8)

set(CMAKE_MODULE_PATH "${PickMidi_SOURCE_DIR}/cmake/")

set(PICKMIDI_VERSION_MAJOR 0)
set(PICKMIDI_VERSION_MINOR 0)
set(PICKMIDI_VERSION_SUBMINOR 1)

set(SRCDIR src)

find_package(FFTW REQUIRED)

add_executable(PickMidi ${SRCDIR}/pickmidi.c ${SRCDIR}/fft.c ${SRCDIR}/fft_note_finder.c)

target_link_libraries(PickMidi fftw3 m)
