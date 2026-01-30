cmake_minimum_required(VERSION 3.20)
project(myapp C)

set(CMAKE_C_STANDARD 11)

# Point this to where you installed SDL3
set(SDL3_DIR "C:/SDL3/lib/cmake/SDL3")

find_package(SDL3 CONFIG REQUIRED)

add_executable(VoiceApp main.c some_stuff.c)
target_link_libraries(VoiceApp PRIVATE SDL3::SDL3)
