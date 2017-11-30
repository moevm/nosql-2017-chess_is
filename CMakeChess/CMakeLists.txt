project(ChessCMake)
cmake_minimum_required(VERSION 2.8)

set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt5Widgets)


aux_source_directory(${PROJECT_NAME} SRC_LIST)
file(GLOB SOURCES
    ./Headers/*.h
    ./Sources/*.cpp
    ./Forms/*.ui
    ./*.qrc
)
add_executable(${PROJECT_NAME} ${SOURCES})
qt5_add_resources(Images images.qrc)
qt5_use_modules(ChessCMake Core Gui Widgets)
TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${QT_LIBRARIES} ${Images})
