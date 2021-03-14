# Copyright 2021, Kacper Waśniowski
# All rights reserved.

cmake_minimum_required(VERSION 3.9 FATAL_ERROR)
project(process-manager-service)
enable_testing()

SET(CMAKE_CXX_STANDARD 20)

find_package(Boost REQUIRED)
find_package(Protobuf REQUIRED)
find_package(spdlog REQUIRED)
find_package(Catch2 REQUIRED)

set(artifactoryDownloader ${CMAKE_SOURCE_DIR}/Toolkits/artifactory_downloader.sh)
set(WatchdogServiceProtocolDirectory ${CMAKE_BINARY_DIR}/Protocols/watchdog-service-protocol)
set(ServiceModuleProtocolDirectory ${CMAKE_BINARY_DIR}/Protocols/service-module-protocol)

add_subdirectory(Protocols)
add_subdirectory(Source)
add_subdirectory(Tests)
include(Packaging/Packaging.cmake)