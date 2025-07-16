cmake_minimum_required(VERSION 3.16)
project(Monitor LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set(Qt6_DIR    "D:/IF NASU/Addons/Qt/6.9.1/msvc2022_64/lib/cmake/Qt6")

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)

add_executable(Calculus
    main.cpp
    MainWindow.cpp
    MainWindow.h
)

set_source_files_properties(main.cpp PROPERTIES SKIP_AUTOMOC ON)

target_link_libraries(Calculus
    PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
)
