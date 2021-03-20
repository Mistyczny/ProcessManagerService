# Copyright 2021, Kacper Waśniowski
# All rights reserved.

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
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ServiceMessageEventsCache.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ServiceSubscriptionEventsCache.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ServiceModulesCache.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/MongoDbEnvironment.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/MongoModulesCollection.cpp
    ${PROTO_WATCHDOG_SERVICE_SRC}
    ${PROTO_SERVICE_MODULE_SRC}
)

list(APPEND SERVICE_PUBLIC_HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/include/Service.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/Event.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/EventManager.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/include/Logging.hpp
)

add_library(Service SHARED ${SOURCES})
add_dependencies(Service
    WatchdogServiceProtocolDownload
    ServiceModuleProtocolDownload
)
set_target_properties(Service PROPERTIES PUBLIC_HEADER "${SERVICE_PUBLIC_HEADERS}")

target_link_libraries(Service
        PUBLIC
    pthread
    mongocxx
    bsoncxx
    spdlog
    ${PROTOBUF_LIBRARY}
    ${Boost_LIBRARIES}
)

target_include_directories(Service
        PUBLIC
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
    "$<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/Source>"
    "$<BUILD_INTERFACE:${MongoCxxIncludes}>"
)

target_include_directories(Service
        PUBLIC
    "$<INSTALL_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
)

target_compile_features(Service PRIVATE cxx_constexpr)
target_compile_features(Service PRIVATE cxx_nullptr)
target_compile_features(Service PRIVATE cxx_override)

add_library(Service::Service ALIAS Service)

include(GNUInstallDirs)

install(TARGETS Service
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
            COMPONENT ${CMAKE_PROJECT_NAME}_RunTime
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
            COMPONENT ${CMAKE_PROJECT_NAME}_RunTime
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
            COMPONENT ${CMAKE_PROJECT_NAME}_Development
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)