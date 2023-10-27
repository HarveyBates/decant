cmake_minimum_required(VERSION 3.26)

set(CMAKE_CXX_STANDARD 17)

project(decant-visu)

# For GUI
find_package(OpenGL REQUIRED)
find_package(SDL2 REQUIRED)
find_package(spdlog REQUIRED)

# Include LIBUSB
find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBUSB1 REQUIRED libusb-1.0)

# ImGUI files
file(GLOB IMGUI_SRC RELATIVE ${CMAKE_SOURCE_DIR} lib/imgui/*.cpp)
file(GLOB IMGUI_INCLUDE RELATIVE ${CMAKE_SOURCE_DIR} lib/imgui/*.h)

# Project files
file(GLOB_RECURSE SRC RELATIVE ${CMAKE_SOURCE_DIR} src/*.cpp)
file(GLOB_RECURSE INCLUDE RELATIVE ${CMAKE_SOURCE_DIR} include/*.h)

include_directories(${LIBUSB1_INCLUDE_DIRS})
link_directories(${LIBUSB1_LIBRARY_DIRS})

# Required to silence `ZERO_SIZED_ARRAY` in `libusb.h`
add_compile_options(-Wno-zero-length-array)

add_executable(${PROJECT_NAME} ${SRC} ${INCLUDE} ${IMGUI_SRC} ${IMGUI_INCLUDE})

target_compile_options(${PROJECT_NAME} PRIVATE -Wall)
target_compile_options(${PROJECT_NAME} PRIVATE -pedantic)
target_compile_options(${PROJECT_NAME} PRIVATE -Werror)

include_directories(${CMAKE_SOURCE_DIR}/include ${CMAKE_SOURCE_DIR}/lib/imgui)

# Include SDL2 and OpenGL sources
target_include_directories(${PROJECT_NAME} PUBLIC ${SDL2_INCLUDE_DIRS})
target_include_directories(${PROJECT_NAME} PUBLIC ${OPENGL_INCLUDE_DIR})

# Link SDL2 and OpenGL
target_link_libraries(${PROJECT_NAME} spdlog::spdlog $<$<BOOL:${MINGW}>:ws2_32>)
target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES})
target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES})
target_link_libraries(${PROJECT_NAME} ${LIBUSB1_LIBRARIES})
