# depends on 'data'

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

set_property (GLOBAL PROPERTY SRC_AutomationCurveWidget
${CMAKE_CURRENT_SOURCE_DIR}/AutomationCurveWidget.cpp

${CMAKE_CURRENT_SOURCE_DIR}/FVectorWidget.cpp
${CMAKE_CURRENT_SOURCE_DIR}/SequenceWidget.cpp
${CMAKE_CURRENT_SOURCE_DIR}/TimelineWidget.cpp
${CMAKE_CURRENT_SOURCE_DIR}/ColorSequenceWidget.cpp


)

set_property (GLOBAL PROPERTY HEADERS_AutomationCurveWidget
${CMAKE_CURRENT_SOURCE_DIR}/AutomationCurveWidget.hpp
${CMAKE_CURRENT_SOURCE_DIR}/ColorSequenceWidget.hpp

${CMAKE_CURRENT_SOURCE_DIR}/FVectorWidget.hpp
${CMAKE_CURRENT_SOURCE_DIR}/SequenceWidget.hpp
${CMAKE_CURRENT_SOURCE_DIR}/TimelineWidget.hpp
)