protobuf_generate_cpp(EXAMPLE_PROTO_SRC EXAMPLE_PROTO_HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/Example.proto
)

set(MessageEventsSources
    ${CMAKE_SOURCE_DIR}/Source/src/Types.cpp
    ${CMAKE_SOURCE_DIR}/Source/src/EventManager.cpp
    ${CMAKE_SOURCE_DIR}/Source/src/ServiceMessageEventsCache.cpp
)

add_executable(MessageEventsTest
    MessageEventsTest.cpp
    ${ServiceWatchdogResponseHandlersSources}
    ${EXAMPLE_PROTO_SRC}
    ${EXAMPLE_PROTO_HEADERS}
)

target_link_libraries(MessageEventsTest
        PRIVATE
    pthread
    catchTestMain
    ${PROTOBUF_LIBRARY}
)
target_include_directories(MessageEventsTest
        PRIVATE
    ${CMAKE_SOURCE_DIR}/Source/include
    ${CMAKE_CURRENT_BINARY_DIR}
)