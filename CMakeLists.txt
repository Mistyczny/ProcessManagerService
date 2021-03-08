cmake_minimum_required(VERSION 3.9 FATAL_ERROR)
project(ProcessManagerService)
enable_testing()

SET(CMAKE_CXX_STANDARD 20)
set(CMAKE_VERBOSE_MAKEFILE ON)
find_package(Boost REQUIRED)
find_package(Protobuf REQUIRED)
find_package(spdlog REQUIRED)
find_package(Catch2 REQUIRED)

set(artifactoryDownloader ${CMAKE_SOURCE_DIR}/Toolkits/artifactory_downloader.sh)

add_subdirectory(Protocols)
add_subdirectory(Source)
add_subdirectory(Tests)
include(Packaging/Packaging.cmake)