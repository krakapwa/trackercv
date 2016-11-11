cmake_minimum_required(VERSION 3.0.2)
project(eyetracker-cv)

find_package(Qt5Widgets)
find_package(OpenCV REQUIRED)
include_directories( ${OpenCV_INCLUDE_DIRS} )

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

#add_library(mainwindow mainwindow.cpp)
#target_link_libraries (mainwindow Qt5::Widgets)

add_executable(eyetracker-cv main.cpp)
target_link_libraries( eyetracker-cv ${OpenCV_LIBS} )
qt5_use_modules(eyetracker-cv Core Gui Widgets)
