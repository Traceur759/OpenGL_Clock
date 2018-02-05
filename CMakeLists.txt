# CMake entry point
cmake_minimum_required (VERSION 3.0)
project (OpenGL_Analog_Clock)

find_package(OpenGL REQUIRED)

set(ALL_LIBS
	${OPENGL_LIBRARY}
	glfw
	GLEW
)

add_definitions(
	-DTW_STATIC
	-DTW_NO_LIB_PRAGMA
	-DTW_NO_DIRECT3D
	-DGLEW_STATIC
	-D_CRT_SECURE_NO_WARNINGS
)

add_executable(clock
    main.c
)

target_link_libraries(clock
    ${ALL_LIBS}
)
