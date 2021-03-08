project(ServiceLibrary)

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
)

add_library(Service STATIC ${SOURCES})
set_target_properties(Service PROPERTIES PUBLIC_HEADER ${CMAKE_CURRENT_SOURCE_DIR}/include/ServiceTask.hpp)

target_link_libraries(Service
        PUBLIC
    pthread
    spdlog
    WatchdogServiceProto
    ${PROTOBUF_LIBRARY}
)

target_include_directories(Service
        PUBLIC
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
    "$<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/Protocols>"
)

target_include_directories(Service
        PUBLIC
    "$<INSTALL_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
)

target_compile_features(Service PRIVATE cxx_constexpr)
target_compile_features(Service PRIVATE cxx_nullptr)
target_compile_features(Service PRIVATE cxx_override)

add_library(Service::Service ALIAS Service)