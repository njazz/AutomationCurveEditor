#depends on 'data'

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

set_property (GLOBAL PROPERTY SRC_AutomationCurveJSON
${CMAKE_CURRENT_SOURCE_DIR}/AutomationCurveJSON.cpp
)

set_property (GLOBAL PROPERTY HEADERS_AutomationCurveJSON
${CMAKE_CURRENT_SOURCE_DIR}/AutomationCurveJSON.hpp
)