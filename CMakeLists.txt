cmake_minimum_required(VERSION 3.18)
project(cookieos-welcome VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets Svg)

qt_add_executable(cookieos-welcome
    src/main.cpp
    src/Panels.cpp
    src/WelcomeWindow.cpp
)

target_include_directories(cookieos-welcome PRIVATE src)

target_link_libraries(cookieos-welcome PRIVATE
    Qt6::Core Qt6::Gui Qt6::Widgets Qt6::Svg
)

install(TARGETS cookieos-welcome DESTINATION /usr/local/bin)
