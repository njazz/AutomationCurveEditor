#depends on 'data'

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

set_property (GLOBAL PROPERTY SRC_AutomationCurveJSON
${CMAKE_CURRENT_SOURCE_DIR}/AutomationCurveJSON.cpp
${CMAKE_CURRENT_SOURCE_DIR}/FVectorJSON.cpp
${CMAKE_CURRENT_SOURCE_DIR}/SequenceJSON.cpp
)

set_property (GLOBAL PROPERTY HEADERS_AutomationCurveJSON
${CMAKE_CURRENT_SOURCE_DIR}/AutomationCurveJSON.hpp
${CMAKE_CURRENT_SOURCE_DIR}/FVectorJSON.hpp
${CMAKE_CURRENT_SOURCE_DIR}/SequenceJSON.hpp
)