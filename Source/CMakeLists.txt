project(Module)

set(SOURCES src/ServiceBase.cpp
            src/ServiceMain.cpp
            src/ServiceConnection.cpp
            src/ServiceContextWorker.cpp
            src/ServiceModulesServer.cpp
            src/EventManager.cpp
            src/Types.cpp)

add_library(ServiceStatic STATIC ${SOURCES})
target_link_libraries(ServiceStatic ${Boost_LIBRARIES} spdlog WatchdogServiceProto)
target_include_directories(ServiceStatic PRIVATE ./include ${BOOST_ROOT} ${SpdlogInclude} ${CMAKE_BINARY_DIR}/Protocols)