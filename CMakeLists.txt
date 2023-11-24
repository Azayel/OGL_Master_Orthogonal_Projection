cmake_minimum_required(VERSION 3.5.0)
project(VisualGrid VERSION 0.1.0)
#set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")

link_directories(lib/glfw-3.3.8)

include_directories(includes/glad)
include_directories(includes/KHR)
include_directories(includes)
include_directories(includes/GLFW)
file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)

add_executable(VisualGrid ${SOURCES} src/glad.c)
add_subdirectory(lib/glfw-3.3.8)
target_link_libraries(VisualGrid glfw)

find_package(OpenGL REQUIRED)
target_link_libraries(VisualGrid OpenGL::GL)
file(COPY resources DESTINATION /)


