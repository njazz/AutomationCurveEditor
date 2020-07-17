cmake_minimum_required(VERSION 3.6)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

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

get_property(SRC_AutomationCurve GLOBAL PROPERTY SRC_AutomationCurve)
get_property(SRC_AutomationCurveJSON GLOBAL PROPERTY SRC_AutomationCurveJSON)
get_property(SRC_AutomationCurveWidget GLOBAL PROPERTY SRC_AutomationCurveWidget)

get_property(HEADERS_AutomationCurve GLOBAL PROPERTY HEADERS_AutomationCurve)
get_property(HEADERS_AutomationCurveJSON GLOBAL PROPERTY SRC_AutomationCurveJSON)
get_property(HEADERS_AutomationCurveWidget GLOBAL PROPERTY SRC_AutomationCurveWidget)

add_library(CurveEditor STATIC 
${SRC_AutomationCurve}
${SRC_AutomationCurveJSON}
${SRC_AutomationCurveWidget}

${HEADERS_AutomationCurve}
${HEADERS_AutomationCurveJSON}
${HEADERS_AutomationCurveWidget}

)


