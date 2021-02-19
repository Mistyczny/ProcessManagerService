cmake_minimum_required(VERSION 3.9 FATAL_ERROR)
project(ProcessManagerService)
enable_testing()

set(LIBRARY_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/bin)

SET(CMAKE_CXX_STANDARD 20)

find_package(Protobuf REQUIRED)
find_package(spdlog REQUIRED)
find_package(Catch2 REQUIRED)

set(artifactoryDownloader ${CMAKE_SOURCE_DIR}/Toolkits/artifactory_downloader.sh)

add_subdirectory(Protocols)
add_subdirectory(Source)
add_subdirectory(Tests)