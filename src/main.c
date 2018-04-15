// Include APIs
#include "clock.h"

int main(int argc, char *argv[]) {

    GLFWwindow *window;
	GLuint VertexArrayID;
	GLuint programID;

    if (clock_init(&window)) {
        return -1;
    }

	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	// Create and compile our GLSL program from the shaders
    if (LoadShaders(&programID,
                    "../shaders/SimpleVertexShader.vertexshader",
                    "../shaders/SimpleFragmentShader.fragmentshader")) {
        fprintf(stderr, "Could not load shaders.\n");
        return -1;
    }

	static GLdouble g_vertex_buffer_data[] = { 
		-0.3f, -0.3f, 0.3f,
		 0.3f, -0.3f, -0.3f,
		 0.0f,  0.3f, -0.0f
	};

	GLuint vertexbuffer;
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);

    GLdouble **matrices = calloc(1, sizeof(GLdouble *));

    GLdouble *rot_mat = matrix_new();

    rot_mat[0] = cos(M_PI*2/60);
    rot_mat[1] = -sin(M_PI*2/60);
    rot_mat[4] = sin(M_PI*2/60);
    rot_mat[5] = cos(M_PI*2/60);

    *matrices = rot_mat;


	do {
		// Clear the screen. It's not mentioned before Tutorial 02,
        // but it can cause flickering, so it's there nonetheless.
		glClear(GL_COLOR_BUFFER_BIT);

		// Use our shader
		glUseProgram(programID);
        //glfwGetCursorPos(window, &xpos, &ypos);
        //g_vertex_buffer_data[0] = (float)xpos / 1280 * 2 - 1 - 0.15;
        //g_vertex_buffer_data[1] = 2 - (float)ypos / 720 * 2 - 1 - 0.15;

        //g_vertex_buffer_data[3] = (float)xpos / 1280 * 2 - 1 + 0.15;
        //g_vertex_buffer_data[4] = 2 - (float)ypos / 720 * 2 - 1 - 0.15;

        //g_vertex_buffer_data[6] = (float)xpos / 1280 * 2 - 1;
        //g_vertex_buffer_data[7] = 2 - (float)ypos / 720 * 2 - 1 + 0.15;
        matrix_transform(g_vertex_buffer_data, 3,
                         matrices, 1);

	    GLuint vertexbuffer;
	    glGenBuffers(1, &vertexbuffer);
	    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

		// 1rst attribute buffer : vertices
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glVertexAttribPointer(
			0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_DOUBLE,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			NULL                // array buffer offset
		);

		// Draw the triangle !
		glDrawArrays(GL_TRIANGLES, 0, 3); // 3 indices starting at 0 -> 1 triangle

		glDisableVertexAttribArray(0);
		
		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

        // Check if the ESC key was pressed or the window was closed
        usleep(1000000/60);
	} while(glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
		   glfwWindowShouldClose(window) == 0);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}

