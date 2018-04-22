#include <clock.h>

int
clock_init(GLFWwindow **window) {
	// Initialise GLFW
	if(!glfwInit()) {
		fprintf(stderr, "Failed to initialize GLFW\n");
		getchar();
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Open a window and create its OpenGL context
	*window = glfwCreateWindow(1280, 720, "OpenGL Clock", NULL, NULL);
	if (*window == NULL) {
		fprintf(stderr, "Failed to open GLFW window.\n");
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(*window);

	// Initialize GLEW
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		glfwTerminate();
		return -1;
	}

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(*window, GLFW_STICKY_KEYS, GL_TRUE);

	// Dark grey background
	glClearColor(0.15f, 0.15f, 0.15f, 0.0f);

    return 0;
}

int
clock_run(GLFWwindow *window) {
	GLuint VertexArrayID;
	GLuint programID;


	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	// Create and compile our GLSL program from the shaders
    if (LoadShaders(&programID,
                    "../shaders/SimpleVertexShader.vertexshader",
                    "../shaders/SimpleFragmentShader.fragmentshader")) {
        fprintf(stderr, "Could not load shaders.\n");
        return -1;
    }

	static GLfloat g_vertex_buffer_data[] = { 
		-0.3f, -0.3f, 0.0f, 1.0f,
		 0.3f, -0.3f, 0.0f, 1.0f,
		 0.0f,  0.3f, 0.0f, 1.0f
	};
    Model *model;
    model = model_load("../cube.obj");

	GLuint vertexbuffer;
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, model->bufsize*4*sizeof(GLfloat), model->buffer, GL_STATIC_DRAW);

    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);

    GLfloat **matrices = malloc(2 * sizeof(GLfloat *));

    GLfloat *temp = matrix_new();

    //temp[0] = cos(M_PI*2/60);
    //temp[1] = -sin(M_PI*2/60);
    //temp[4] = sin(M_PI*2/60);
    //temp[5] = cos(M_PI*2/60);

    matrices[0] = temp;
    temp = matrix_new();

    temp[0] = cos(M_PI*2/60/4);
    temp[8] = -sin(M_PI*2/60/4);
    temp[2] = sin(M_PI*2/60/4);
    temp[10] = cos(M_PI*2/60/4);

    matrices[1] = temp;

    GLfloat *mat = matrix_new();

    for (int i = 1; i >= 0; i--) {
        temp = matrix_multip(matrices[i], mat);
        free(mat);
        mat = temp;
    }

    GLfloat *projection = matrix_new();
    GLfloat *buffer;
    projection[14] = -1.0f;
    buffer = matrix_new();
    buffer[0] = (GLfloat)9/16;
    temp = matrix_multip(projection, buffer);
    free(projection);
    projection = buffer;
    temp = malloc(model->bufsize * 4 * sizeof(GLfloat));
    buffer = malloc(model->bufsize * 4 * sizeof(GLfloat));
    memcpy(temp, model->buffer, model->bufsize * 4 * sizeof(GLfloat));

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
        buffer = matrix_transform(temp, model->bufsize, mat);
        free(temp);
        temp = buffer;
        buffer = matrix_transform(buffer, model->bufsize, projection);
        memcpy(model->buffer, buffer, model->bufsize * 4 * sizeof(GLfloat));
        free(buffer);


	    GLuint vertexbuffer;
	    glGenBuffers(1, &vertexbuffer);
	    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	    glBufferData(GL_ARRAY_BUFFER, model->bufsize * 4 * sizeof(GLfloat), model->buffer, GL_STATIC_DRAW);

		// 1rst attribute buffer : vertices
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glVertexAttribPointer(
			0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
			4,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			NULL                // array buffer offset
		);

		// Draw the triangle !
		glDrawArrays(GL_TRIANGLES, 0, model->bufsize); // 3 indices starting at 0 -> 1 triangle

		glDisableVertexAttribArray(0);
		
		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

        // Check if the ESC key was pressed or the window was closed
        usleep(1000000/60);
	} while(glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
		   glfwWindowShouldClose(window) == 0);

	// Close OpenGL window and terminate GLFW
    for (int i = 0; i < 2; i++) {
        free(matrices[i]);
    }
    free(matrices);
	glfwTerminate();

    return 0;

}

