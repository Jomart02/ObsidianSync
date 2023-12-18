cmake_minimum_required(VERSION 3.22)
set(TARGET setting)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)


set(SRCS 
    settingupdate.h settingupdate.cpp
)


find_package(Qt6 REQUIRED COMPONENTS Widgets)

add_library(${TARGET} STATIC ${SRCS} )
target_link_libraries(${TARGET} Qt6::Widgets )
target_include_directories(${TARGET} PUBLIC ./)