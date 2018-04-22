# CMake entry point
cmake_minimum_required (VERSION 3.0)
project(OpenGL_Analog_Clock C)
cmake_policy(SET CMP0072 NEW)

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(glfw3 REQUIRED)

include_directories(
    Include/
    .
)

set(ALL_LIBS
	${OPENGL_LIBRARY}
	glfw
	GLEW
)

set(CMAKE_C_FLAGS "-O0 -ggdb")

add_definitions(
	-DTW_STATIC
	-DTW_NO_LIB_PRAGMA
	-DTW_NO_DIRECT3D
	-DGLEW_STATIC
	-D_CRT_SECURE_NO_WARNINGS
)

add_executable(
    clock
    src/main.c
    src/clock.c
    src/shader.c
    src/matrix.c
    src/model.c
)

target_link_libraries(
    clock
    ${ALL_LIBS}
)
