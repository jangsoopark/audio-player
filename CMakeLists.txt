cmake_minimum_required(VERSION 3.1.0)
project (audio)


include(cmake/GitUtils.cmake)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY  ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY  ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY  ${CMAKE_BINARY_DIR}/lib)


git_clone(
    PROJECT_NAME    portaudio
    GIT_URL         https://github.com/jangsoopark/portaudio
    GIT_BRANCH      master
    DIRECTORY       ${PROJECT_SOURCE_DIR}/thirdparty
)

subdirs(thirdparty/portaudio)
subdirs(src/audio-player)
