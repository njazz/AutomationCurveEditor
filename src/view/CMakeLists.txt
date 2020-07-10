# depends on 'data'

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

set_property (GLOBAL PROPERTY SRC_AutomationCurveWidget
${CMAKE_CURRENT_SOURCE_DIR}/AutomationCurveWidget.cpp
)

set_property (GLOBAL PROPERTY HEADERS_AutomationCurveWidget
${CMAKE_CURRENT_SOURCE_DIR}/AutomationCurveWidget.hpp
)