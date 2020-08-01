include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../data)

set (SRC
${CMAKE_CURRENT_SOURCE_DIR}/AutomationCurveJSON.cpp
${CMAKE_CURRENT_SOURCE_DIR}/FVectorJSON.cpp
${CMAKE_CURRENT_SOURCE_DIR}/SequenceJSON.cpp
)

set (HEADERS
${CMAKE_CURRENT_SOURCE_DIR}/AutomationCurveJSON.hpp
${CMAKE_CURRENT_SOURCE_DIR}/FVectorJSON.hpp
${CMAKE_CURRENT_SOURCE_DIR}/SequenceJSON.hpp
)

if (NOT TARGET CurveEditorJSON)
add_library (CurveEditorJSON ${SRC} ${HEADERS})
endif()