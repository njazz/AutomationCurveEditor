cmake_minimum_required(VERSION 3.6)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_OSX_DEPLOYMENT_TARGET 10.11)

project(CurveEditor)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/.)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../third_party/json)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../third_party/imgui)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/data)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/json)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/view)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/data)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/json)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/view)

add_library(CurveEditor STATIC 
CurveEditorLib.cpp

)
target_link_libraries(CurveEditor CurveEditorData CurveEditorJSON CurveEditorViews )
