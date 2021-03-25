# Copyright 2021, Kacper Waśniowski
# All rights reserved.
find_package(Catch2 REQUIRED)
find_package(GTest CONFIG)

add_library(catchTestMain STATIC CatchTestsMain.cpp)
target_link_libraries(catchTestMain PUBLIC Catch2::Catch2)

add_subdirectory(ServiceWatchdogResponseHandlersTests)
add_subdirectory(ExampleServicesTests)
add_subdirectory(MessageEventsTests)