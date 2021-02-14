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

add_library(ServiceStatic STATIC ${SOURCES})
target_link_libraries(ServiceStatic ${Boost_LIBRARIES} spdlog WatchdogServiceProto)
target_include_directories(ServiceStatic
        PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/include 
    ${BOOST_ROOT} 
    ${SpdlogInclude} 
    ${CMAKE_BINARY_DIR}/Protocols
)

target_compile_features(ServiceStatic PRIVATE cxx_constexpr)
target_compile_features(ServiceStatic PRIVATE cxx_nullptr)
target_compile_features(ServiceStatic PRIVATE cxx_override)