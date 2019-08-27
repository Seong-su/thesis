cmake_minimum_required(VERSION 3.0.0)
project(thesis VERSION 0.1.0)

set(THESIS_SOURCE_FILES StepReader.cpp Part.cpp AdjacencyGraph.cpp)

add_library(thesis STATIC ${THESIS_SOURCE_FILES})

add_executable(thesis-main main.cpp)
target_link_libraries(thesis-main thesis)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)