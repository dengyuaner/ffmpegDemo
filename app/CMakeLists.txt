cmake_minimum_required(VERSION 3.4.1)
set(lib_src_DIR ${CMAKE_SOURCE_DIR}/src/main/jniLibs/${ANDROID_ABI})
include_directories(
     ${CMAKE_SOURCE_DIR}/src/main/cpp/include
)

add_library(avcodec-57_lib SHARED IMPORTED)
set_target_properties(avcodec-57_lib PROPERTIES IMPORTED_LOCATION
                          ${lib_src_DIR}/libavcodec-57.so)

add_library(avformat-57_lib SHARED IMPORTED)
set_target_properties(avformat-57_lib PROPERTIES IMPORTED_LOCATION
                     ${lib_src_DIR}/libavformat-57.so)

add_library(avutil-55_lib SHARED IMPORTED)
set_target_properties(avutil-55_lib PROPERTIES IMPORTED_LOCATION
                     ${lib_src_DIR}/libavutil-55.so)

add_library(swresample-2_lib SHARED IMPORTED)
set_target_properties(swresample-2_lib PROPERTIES IMPORTED_LOCATION
                     ${lib_src_DIR}/libswresample-2.so)

add_library(swscale-4_lib SHARED IMPORTED)
set_target_properties(swscale-4_lib PROPERTIES IMPORTED_LOCATION
                     ${lib_src_DIR}/libswscale-4.so)


# build application's shared lib
add_library(play_video SHARED
         ${CMAKE_SOURCE_DIR}/src/main/cpp/play_video.cpp)
add_library(decode_video SHARED
         ${CMAKE_SOURCE_DIR}/src/main/cpp/decode_video.cpp)
add_library(play_audio SHARED
         ${CMAKE_SOURCE_DIR}/src/main/cpp/play_audio.cpp
         ${CMAKE_SOURCE_DIR}/src/main/cpp/AudioDevice.c
         ${CMAKE_SOURCE_DIR}/src/main/cpp/FFmpegAudioPlay.c)



#特别注意，你想生成几个so，就要写几个link，写在一个link里面是错误的
target_link_libraries(play_video
log
android
avcodec-57_lib
avformat-57_lib
avutil-55_lib
swresample-2_lib
swscale-4_lib
)
target_link_libraries(decode_video
log
android
avcodec-57_lib
avformat-57_lib
avutil-55_lib
swresample-2_lib
swscale-4_lib
)

#注意加入opensl es
target_link_libraries(play_audio
log
OpenSLES
android
avcodec-57_lib
avformat-57_lib
avutil-55_lib
swresample-2_lib
swscale-4_lib
)