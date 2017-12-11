project(ChessCMake)
cmake_minimum_required(VERSION 2.8)
set(CMAKE_CXX_STANDARD 14)

set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt5Widgets)
find_package (libmongocxx)
find_package (libbsoncxx)

file(GLOB SOURCES
./Headers/*.h
./Sources/*.cpp
./Forms/*.ui
./*.qrc
)
add_executable(${PROJECT_NAME} ${SOURCES})
qt5_add_resources(Images images.qrc)
qt5_use_modules(ChessCMake Core Gui Widgets)

target_include_directories (${PROJECT_NAME} PRIVATE /usr/local/include/mongocxx/v_noabi;
/usr/local/include/bsoncxx/v_noabi
;/usr/local/include/libmongoc-1.0
;/usr/local/include/libbson-1.0)

TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${QT_LIBRARIES} ${Images}
/usr/local/lib/libmongocxx.so;
/usr/local/lib/libbsoncxx.so)
