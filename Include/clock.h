#ifndef CLOCK_H
#define CLOCK_H

// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <unistd.h>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>

// Include project headers
#include <model.h>
#include <matrix.h>
#include <shader.h>

// Define macros
#define xfree(p) if (p) free((void *)p)

// Declare functions

int clock_init(GLFWwindow **);
int clock_run(GLFWwindow *);

#endif

