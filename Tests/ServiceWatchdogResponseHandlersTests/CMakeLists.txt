project(ServiceWatchdogResponseHandlersTests)

set(ServiceWatchdogResponseHandlersSources
    ${CMAKE_SOURCE_DIR}/Source/src/ServiceWatchdogResponseHandlers.cpp
    ${CMAKE_SOURCE_DIR}/Source/src/ServiceWatchdogConnectionState.cpp
    ${CMAKE_SOURCE_DIR}/Source/src/Types.cpp
)

add_executable(ServiceWatchdogConnectResponseTest ServiceWatchdogConnectResponseTest.cpp ${ServiceWatchdogResponseHandlersSources})
target_link_libraries(ServiceWatchdogConnectResponseTest 
        PRIVATE 
    pthread 
    catchTestMain
    ProtoWatchdogServiceLibrary
    ${PROTOBUF_LIBRARY}
)
target_include_directories(ServiceWatchdogConnectResponseTest
        PRIVATE
    ${CMAKE_SOURCE_DIR}/Source/include
    ${CMAKE_BINARY_DIR}/Protocols
)

add_executable(ServiceWatchdogPingResponseTest ServiceWatchdogPingResponseTest.cpp ${ServiceWatchdogResponseHandlersSources})
target_link_libraries(ServiceWatchdogPingResponseTest 
        PRIVATE 
    pthread 
    catchTestMain
    ProtoWatchdogServiceLibrary
    ${PROTOBUF_LIBRARY}
)
target_include_directories(ServiceWatchdogPingResponseTest
        PRIVATE
    ${CMAKE_SOURCE_DIR}/Source/include
    ${CMAKE_BINARY_DIR}/Protocols
)

add_executable(ServiceWatchdogReconnectResponseTest ServiceWatchdogReconnectResponseTest.cpp ${ServiceWatchdogResponseHandlersSources})
target_link_libraries(ServiceWatchdogReconnectResponseTest 
        PRIVATE 
    pthread 
    catchTestMain
    ProtoWatchdogServiceLibrary
    ${PROTOBUF_LIBRARY}
)
target_include_directories(ServiceWatchdogReconnectResponseTest
        PRIVATE
    ${CMAKE_SOURCE_DIR}/Source/include
    ${CMAKE_BINARY_DIR}/Protocols
)

add_test(NAME ServiceWatchdogConnectResponseTest COMMAND ServiceWatchdogConnectResponseTest)
add_test(NAME ServiceWatchdogPingResponseTest COMMAND ServiceWatchdogPingResponseTest)
add_test(NAME ServiceWatchdogReconnectResponseTest COMMAND ServiceWatchdogReconnectResponseTest)