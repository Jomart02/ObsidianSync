
set(TARGET ObsidianSync)

cmake_minimum_required(VERSION 3.22)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 COMPONENTS Widgets REQUIRED)

set(MOC_HDRS
        obsidiansync.h

)

set(SRCS ${MOC_HDRS}
        main.cpp
        obsidiansync.cpp

)

set(UIS
        obsidiansync.ui

)

add_executable(${TARGET} ${SRCS} ${UIS})

target_link_libraries(${TARGET} Qt6::Widgets setting)
# add_dependencies(${TARGET} )
