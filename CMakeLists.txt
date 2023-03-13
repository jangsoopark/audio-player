cmake_minimum_required(VERSION 3.1.0)
project (audio)


include(cmake/GitUtils.cmake)
include(ExternalProject)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY  ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY  ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY  ${CMAKE_BINARY_DIR}/lib)


git_clone(
    PROJECT_NAME    portaudio
    GIT_URL         https://github.com/PortAudio/portaudio
    GIT_BRANCH      master
    DIRECTORY       ${PROJECT_SOURCE_DIR}/thirdparty
)

git_clone(
    PROJECT_NAME    asio
    GIT_URL         https://github.com/boostorg/asio
    GIT_BRANCH      master
    DIRECTORY       ${PROJECT_SOURCE_DIR}/thirdparty
)

ExternalProject_Add(ffmpeg
    URL https://github.com/BtbN/FFmpeg-Builds/releases/download/latest/ffmpeg-master-latest-win64-gpl-shared.zip
    CONFIGURE_COMMAND ""  
    BUILD_COMMAND ""  
    INSTALL_COMMAND "" 
    INSTALL_COMMAND "${CMAKE_COMMAND}" -E rename ${CMAKE_CURRENT_BINARY_DIR}/ffmpeg-prefix/src/ffmpeg ${CMAKE_CURRENT_BINARY_DIR}/ffmpeg  
)


configure_file(cmake/CMakeLists.txt.in ${PROJECT_SOURCE_DIR}/thirdparty/ffmpeg/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} . WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/thirdparty/ffmpeg)
execute_process(COMMAND ${CMAKE_COMMAND} --build . WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/thirdparty/ffmpeg)

set(FFMPEG_PATH ${CMAKE_BINARY_DIR}/ffmpeg)
set(FFMPEG_INCLUDE ${FFMPEG_PATH}/include)
set(FFMPEG_LIBRARY ${FFMPEG_PATH}/lib)

include_directories(${FFMPEG_INCLUDE})


if(WIN32)
    file(GLOB_RECURSE FFMPEG_SHARED_LIBRARY CMAKE_CONFIGURE_DEPENDS
        ${FFMPEG_PATH}/bin/*.dll
    )
endif()

subdirs(thirdparty/portaudio)
subdirs(thirdparty/asio)
subdirs(src/audio-player)
