project(ProcessManagerTests)

add_library(catchTestMain STATIC CatchTestsMain.cpp)
target_link_libraries(catchTestMain PUBLIC Catch2::Catch2)