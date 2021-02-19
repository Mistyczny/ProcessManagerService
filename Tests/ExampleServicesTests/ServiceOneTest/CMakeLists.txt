project(ServiceOneTest)

set(SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/ServiceOneTestMain.cpp)

add_executable(ServiceOneTest ${SOURCES})
target_link_libraries(ServiceOneTest
        PUBLIC
        pthread
    ServiceStatic
)

target_include_directories(ServiceOneTest
        PUBLIC
    ${CMAKE_SOURCE_DIR}/Source/include
)