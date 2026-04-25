cmake_minimum_required(VERSION 3.20)
project(myapp C CXX)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

set(SDL3_DIR "C:/SDL3/lib/cmake/SDL3")
find_package(SDL3 CONFIG REQUIRED)

add_executable(VoiceApp
    src/main.cpp
    src/audio/audio.cpp
    src/effects/sine_effect.cpp
    src/ui/ui_manager.cpp
)

target_include_directories(VoiceApp PRIVATE src)
target_link_libraries(VoiceApp PRIVATE SDL3::SDL3)
