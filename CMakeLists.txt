cmake_minimum_required(VERSION 3.17)

project(OGL)

add_executable(${PROJECT_NAME} src/glad.c src/main.cpp src/cells.cpp src/cells.h src/mainloop.h src/Shader.cpp src/Shader.h)

target_link_libraries(${PROJECT_NAME} GL dl glfw /usr/include/glm/glm.hpp /usr/include/stb_image.h)
