# Copyright 2021, Kacper Waśniowski
# All rights reserved.

cmake_minimum_required(VERSION 3.9 FATAL_ERROR)
project(process-manager-service)
enable_testing()

set(CPACK_DEBIAN_PACKAGE_NAME process-manager-service)
set(CPACK_GENERATOR "DEB;RPM")
set(CPACK_PACKAGE_VERSION_MAJOR 1)
set(CPACK_PACKAGE_VERSION_MINOR 0)
set(CPACK_PACKAGE_VERSION_PATCH 0)
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Kacper Waśniowski")
set(CPACK_PACKAGE_CONTACT "Kacper Waśniowski")
set(CPACK_DEBIAN_PACKAGE_DEPENDS "")

SET(CMAKE_CXX_STANDARD 20)

find_package(Boost REQUIRED)
find_package(Protobuf REQUIRED)
find_package(spdlog REQUIRED)
find_package(Catch2 REQUIRED)
find_package(mongocxx REQUIRED)
find_package(bsoncxx REQUIRED)

set(artifactoryDownloader ${CMAKE_SOURCE_DIR}/Toolkits/artifactory_downloader.sh)
set(WatchdogServiceProtocolDirectory ${CMAKE_BINARY_DIR}/Protocols/watchdog-service-protocol)
set(ServiceModuleProtocolDirectory ${CMAKE_BINARY_DIR}/Protocols/service-module-protocol)

set(MongoCxxIncludes    /usr/local/include/mongocxx/v_noabi
                        /usr/local/include/bsoncxx/v_noabi)

add_subdirectory(Protocols)
add_subdirectory(Source)
add_subdirectory(Tests)

configure_file(Doxyfile.in Doxyfile)

include(CPack)