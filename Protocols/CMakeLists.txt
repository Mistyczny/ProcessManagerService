# Copyright 2021, Kacper Waśniowski
# All rights reserved.

include(ExternalProject)
include_directories(${PROTOBUF_INCLUDE_DIR})

set(ArtifactoryProtocolsPath /process-manager-local/CommunicationProtocols)

#
# Watchdog-Service communication protocol download
#

set(WatchdogServiceProtocolVersion 1.0)
set(WatchdogServiceProtocolPackage WatchdogServiceProtocol_${WatchdogServiceProtocolVersion}.tar)

ExternalProject_Add(WatchdogServiceProtocolDownload
    PREFIX
        ${CMAKE_CURRENT_BINARY_DIR}/download
    BINARY_DIR
        ${WatchdogServiceProtocolDirectory}
    DOWNLOAD_COMMAND
        ${artifactoryDownloader} ${WatchdogServiceProtocolPackage} . ${ArtifactoryProtocolsPath}
    UPDATE_COMMAND ""
    CONFIGURE_COMMAND
        tar xf ${CMAKE_CURRENT_BINARY_DIR}/download/src/${WatchdogServiceProtocolPackage}
    BUILD_COMMAND ""
    INSTALL_COMMAND ""
)

protobuf_generate_cpp(PROTO_WATCHDOG_SERVICE_SRC PROTO_WATCHDOG_SERVICE_HEADER
    ${WatchdogServiceProtocolDirectory}/WatchdogService.proto
)
add_library(ProtoWatchdogServiceLibrary SHARED ${PROTO_WATCHDOG_SERVICE_SRC})
add_dependencies(ProtoWatchdogServiceLibrary WatchdogServiceProtocolDownload)
#
# Service-Module communication protocol download
#

set(ServiceModuleProtocolVersion 1.3)
set(ServiceModuleProtocolPackage ServiceModuleProtocol_${ServiceModuleProtocolVersion}.tar)
ExternalProject_Add(ServiceModuleProtocolDownload
    PREFIX
        ${CMAKE_CURRENT_BINARY_DIR}/download
    BINARY_DIR
        ${ServiceModuleProtocolDirectory}
    DOWNLOAD_COMMAND
        ${artifactoryDownloader} ${ServiceModuleProtocolPackage} . ${ArtifactoryProtocolsPath}
    UPDATE_COMMAND ""
    CONFIGURE_COMMAND
        tar xf ${CMAKE_CURRENT_BINARY_DIR}/download/src/${ServiceModuleProtocolPackage}
    BUILD_COMMAND ""
    INSTALL_COMMAND ""
)

protobuf_generate_cpp(PROTO_SERVICE_MODULE_SRC PROTO_SERVICE_MODULE_HEADER
    ${ServiceModuleProtocolDirectory}/ServiceModule.proto
)
add_library(ProtoServiceModuleLibrary OBJECT ${PROTO_SERVICE_MODULE_SRC})
add_dependencies(ProtoServiceModuleLibrary ServiceModuleProtocolDownload)