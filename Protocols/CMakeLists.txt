project(CommunicationProtocols)

include(ExternalProject)
include_directories(${PROTOBUF_INCLUDE_DIR})

set(ArtifactoryProtocolsPath /process-manager-local/CommunicationProtocols)

#
# Watchdog-Service communication protocol download
#

set(WatchdogServiceProtocolVersion 1.0)
set(WatchdogServiceProtocolPackage WatchdogServiceProtocol_${WatchdogServiceProtocolVersion}.tar)

set(WatchdogServiceProtocolDir ${CMAKE_CURRENT_BINARY_DIR}/watchdog-service-protocol)
ExternalProject_Add(WatchdogServiceProtocolDownload
    PREFIX
        ${CMAKE_CURRENT_BINARY_DIR}/download
    BINARY_DIR
        ${WatchdogServiceProtocolDir}
    DOWNLOAD_COMMAND
        ${artifactoryDownloader} ${WatchdogServiceProtocolPackage} . ${ArtifactoryProtocolsPath}
    UPDATE_COMMAND ""
    CONFIGURE_COMMAND
        tar xf ${CMAKE_CURRENT_BINARY_DIR}/download/src/${WatchdogServiceProtocolPackage}
    BUILD_COMMAND ""
    INSTALL_COMMAND ""
)

protobuf_generate_cpp(PROTO_WATCHDOG_SERVICE_SRC PROTO_WATCHDOG_SERVICE_HEADER ${WatchdogServiceProtocolDir}/WatchdogService.proto)
add_library(WatchdogServiceProto ${PROTO_WATCHDOG_SERVICE_HEADER} ${PROTO_WATCHDOG_SERVICE_SRC})
add_dependencies(WatchdogServiceProto WatchdogServiceProtocolDownload)

#
# Service-Module communication protocol download
#

set(ServiceModuleProtocolVersion 1.1)
set(ServiceModuleProtocolPackage ServiceModuleProtocol_${ServiceModuleProtocolVersion}.tar)

set(ServiceModuleProtocolDir ${CMAKE_CURRENT_BINARY_DIR}/service-module-protocol)
ExternalProject_Add(ServiceModuleProtocolDownload
    PREFIX
        ${CMAKE_CURRENT_BINARY_DIR}/download
    BINARY_DIR
        ${ServiceModuleProtocolDir}
    DOWNLOAD_COMMAND
        ${artifactoryDownloader} ${ServiceModuleProtocolPackage} . ${ArtifactoryProtocolsPath}
    UPDATE_COMMAND ""
    CONFIGURE_COMMAND
        tar xf ${CMAKE_CURRENT_BINARY_DIR}/download/src/${ServiceModuleProtocolPackage}
    BUILD_COMMAND ""
    INSTALL_COMMAND ""
)

protobuf_generate_cpp(PROTO_SERVICE_MODULE_SRC PROTO_SERVICE_MODULE_HEADER ${ServiceModuleProtocolDir}/ServiceModule.proto)
add_library(ServiceModuleProto ${PROTO_SERVICE_MODULE_HEADER} ${PROTO_SERVICE_MODULE_SRC})
add_dependencies(ServiceModuleProto ServiceModuleProtocolDownload)