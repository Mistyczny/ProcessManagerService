# Copyright 2021, Kacper Waśniowski
# All rights reserved.

protobuf_generate_cpp(PROTO_GPS_SRC PROTO_GPS_HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/GPS.proto
)

set(SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/ServiceOneTestMain.cpp
${CMAKE_CURRENT_SOURCE_DIR}/GpsCoordinates.cpp
    ${PROTO_GPS_SRC}
)

add_executable(ServiceOneTest ${SOURCES})
target_link_libraries(ServiceOneTest
        PUBLIC
    pthread
    Service
)

target_include_directories(ServiceOneTest
        PUBLIC
    ${CMAKE_SOURCE_DIR}/Source/include
    ${CMAKE_CURRENT_BINARY_DIR}
)