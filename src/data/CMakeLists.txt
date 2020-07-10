include_directories(${CMAKE_CURRENT_SOURCE_DIR})

set_property (GLOBAL PROPERTY SRC_AutomationCurve
${CMAKE_CURRENT_SOURCE_DIR}/AutomationCurve.cpp
${CMAKE_CURRENT_SOURCE_DIR}/AutomationCurveEditor.cpp
${CMAKE_CURRENT_SOURCE_DIR}/AutomationCurveConverter.cpp
${CMAKE_CURRENT_SOURCE_DIR}/AutomationCurveCodec.cpp
${CMAKE_CURRENT_SOURCE_DIR}/AutomationCurveEase.cpp
)

set_property (GLOBAL PROPERTY HEADERS_AutomationCurve
${CMAKE_CURRENT_SOURCE_DIR}/AutomationCurve.hpp
)