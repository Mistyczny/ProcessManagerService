protobuf_generate_cpp(PROTO_WATCHDOG_SERVICE_SRC PROTO_WATCHDOG_SERVICE_HEADERS
    ${WatchdogServiceProtocolDirectory}/WatchdogService.proto
)
protobuf_generate_cpp(PROTO_SERVICE_MODULE_SRC PROTO_SERVICE_MODULE_HEADERS
    ${ServiceModuleProtocolDirectory}/ServiceModule.proto
)

set(SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ServiceBase.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ServiceMain.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ServiceWatchdogConnection.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ServiceContextWorker.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ServiceModulesServer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/EventManager.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Types.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ServiceWatchdogResponseHandlers.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ServiceWatchdogConnectionState.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ServiceGlobals.cpp
    ${PROTO_WATCHDOG_SERVICE_SRC}
    ${PROTO_SERVICE_MODULE_SRC}
)

add_library(Service
        SHARED
    ${SOURCES}
)
add_dependencies(Service
    WatchdogServiceProtocolDownload
)

set_target_properties(Service
    PROPERTIES
        PUBLIC_HEADER
    "${CMAKE_CURRENT_SOURCE_DIR}/include/ServiceTask.hpp"
)

target_link_libraries(Service
        PUBLIC
    pthread
    spdlog
    ${PROTOBUF_LIBRARY}
)

target_include_directories(Service
        PUBLIC
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
    "$<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/Source>"
)

target_include_directories(Service
        PUBLIC
    "$<INSTALL_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
)

target_compile_features(Service PRIVATE cxx_constexpr)
target_compile_features(Service PRIVATE cxx_nullptr)
target_compile_features(Service PRIVATE cxx_override)

add_library(Service::Service ALIAS Service)