cmake_minimum_required(VERSION 3.27)
project(Constellation)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5 COMPONENTS Core Gui Widgets REQUIRED)

add_executable(Constellation
        main.cpp
        ConstellationWidget.cpp ConstellationWidget.h
        DataReader.cpp DataReader.h
)

target_link_libraries(Constellation Qt5::Core Qt5::Gui Qt5::Widgets)

file(COPY data/A16.xml data/128.xml data/256.xml data/32.xml data/iq_qam32.bin DESTINATION ${CMAKE_BINARY_DIR})
