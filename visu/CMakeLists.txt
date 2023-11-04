cmake_minimum_required(VERSION 3.26)

project(DecantVisualiser)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(MACOSX_BUNDLE_ICON_FILE logo.icns)

file(GLOB_RECURSE WINDOW_SRC_FILES RELATIVE ${CMAKE_SOURCE_DIR} windows/*.cpp)
file(GLOB_RECURSE WINDOW_HEADER_FILES RELATIVE ${CMAKE_SOURCE_DIR} windows/*.h)
file(GLOB_RECURSE WIDGET_SRC_FILES RELATIVE ${CMAKE_SOURCE_DIR} widgets/*.cpp)
file(GLOB_RECURSE WIDGET_HEADER_FILES RELATIVE ${CMAKE_SOURCE_DIR} widgets/*.h)
file(GLOB_RECURSE SRC_FILES RELATIVE ${CMAKE_SOURCE_DIR} src/*.cpp)
file(GLOB_RECURSE HEADER_FILES RELATIVE ${CMAKE_SOURCE_DIR} include/*.h)

find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        REQUIRED)

# Logging (https://github.com/gabime/spdlog)
find_package(spdlog REQUIRED)

# LibUSB
find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBUSB1 REQUIRED libusb-1.0)

# LibUSB includes
include_directories(${LIBUSB1_INCLUDE_DIRS})
link_directories(${LIBUSB1_LIBRARY_DIRS})

# Required to silence `ZERO_SIZED_ARRAY` in `libusb.h`
add_compile_options(-Wno-zero-length-array)

# Executable icon
set(APP_ICON_MACOS "${CMAKE_SOURCE_DIR}/resources/logo.icns")
set_source_files_properties(${APP_ICON_MACOS} PROPERTIES
        MACOSX_PACKAGE_LOCATION "resources")

qt6_add_resources(RESOURCE_ADDED resources.qrc)

add_executable(qt_visu MACOSX_BUNDLE main.cpp ${WINDOW_SRC_FILES} ${WINDOW_HEADER_FILES}
        ${WIDGET_SRC_FILES} ${WIDGET_HEADER_FILES} ${SRC_FILES} ${HEADER_FILES} ${APP_ICON_MACOS} ${RESOURCE_ADDED})

include_directories(${CMAKE_SOURCE_DIR}/windows ${CMAKE_SOURCE_DIR}/widgets
        ${CMAKE_SOURCE_DIR}/include)

target_link_libraries(qt_visu
        Qt::Core
        Qt::Gui
        Qt::Widgets
        spdlog::spdlog $<$<BOOL:${MINGW}>:ws2_32>
        ${LIBUSB1_LIBRARIES}
)

