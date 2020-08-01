include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../data)

set(SRC
${CMAKE_CURRENT_SOURCE_DIR}/AutomationCurveWidget.cpp

${CMAKE_CURRENT_SOURCE_DIR}/FVectorWidget.cpp
${CMAKE_CURRENT_SOURCE_DIR}/SequenceWidget.cpp
${CMAKE_CURRENT_SOURCE_DIR}/TimelineWidget.cpp
${CMAKE_CURRENT_SOURCE_DIR}/ColorSequenceWidget.cpp

${CMAKE_CURRENT_SOURCE_DIR}/WidgetCommon.cpp
)

set(HEADERS

WidgetCommon.hpp

${CMAKE_CURRENT_SOURCE_DIR}/AutomationCurveWidget.hpp
${CMAKE_CURRENT_SOURCE_DIR}/ColorSequenceWidget.hpp

${CMAKE_CURRENT_SOURCE_DIR}/FVectorWidget.hpp
${CMAKE_CURRENT_SOURCE_DIR}/SequenceWidget.hpp
${CMAKE_CURRENT_SOURCE_DIR}/TimelineWidget.hpp

)

if (NOT TARGET CurveEditorViews)
add_library(CurveEditorViews ${SRC} ${HEADERS})
endif()