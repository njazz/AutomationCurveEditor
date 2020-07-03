cmake_minimum_required(VERSION 3.6)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(CurveEditorApp)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../lib)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../third_party/json)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../src "lib")

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../src/data)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../src/json)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../src/view)


###

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../third_party/glfw "GLFW" EXCLUDE_FROM_ALL)


include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../third_party/imgui)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../third_party/imgui/examples)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../third_party/imgui/examples/libs/gl3w/)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../third_party/glfw/include/GLFW)

find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIRS}  ${GLUT_INCLUDE_DIRS})

set(glew_src
${CMAKE_CURRENT_SOURCE_DIR}/../third_party/imgui/examples/libs/gl3w/GL/gl3w.c
)

set(imgui_src

${CMAKE_CURRENT_SOURCE_DIR}/../third_party/imgui/imgui.cpp
${CMAKE_CURRENT_SOURCE_DIR}/../third_party/imgui/imgui_widgets.cpp
${CMAKE_CURRENT_SOURCE_DIR}/../third_party/imgui/imgui_draw.cpp
${CMAKE_CURRENT_SOURCE_DIR}/../third_party/imgui/imgui_demo.cpp

${CMAKE_CURRENT_SOURCE_DIR}/../third_party/imgui/examples/imgui_impl_glfw.cpp
${CMAKE_CURRENT_SOURCE_DIR}/../third_party/imgui/examples/imgui_impl_opengl2.cpp

)

add_executable(CurveEditorTestApp MACOSX_BUNDLE ExampleApp.cpp ${glew_src} ${imgui_src})
target_link_libraries(CurveEditorTestApp CurveEditor glfw ${OPENGL_LIBRARIES})
