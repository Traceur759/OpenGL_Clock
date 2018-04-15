#include "clock.h"
#include "shader.h"

static int
load_code(const char *filepath, char **code) {
    FILE *file;
    int cursor;
    char c;

    file = fopen(filepath, "r");
    if (file == NULL) {
        fprintf(stderr, "Could not open %s.\n", filepath);
        return 1;
    }

    *code = malloc(256 * sizeof(char));
    if (code == NULL) {
        fprintf(stderr, "Out of memory", filepath);
        return 1;
    }
    cursor = 0;

    while ((c = fgetc(file)) != EOF) {
        (*code)[cursor] = c;
        cursor++;
    }

    return 0;
}

static int
compile_code(const char *filepath, const char *code,
                        GLuint ShaderID, GLuint *Result) {
	int InfoLogLength;
	// Compile Shader
	printf("Compiling shader : %s\n", filepath);
	glShaderSource(ShaderID, 1, (const char **)&code, NULL);
	glCompileShader(ShaderID);

	// Check Shader
	glGetShaderiv(ShaderID, GL_COMPILE_STATUS, Result);
	glGetShaderiv(ShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		char *ShaderErrorMessage = malloc(InfoLogLength+1);
		glGetShaderInfoLog(ShaderID, InfoLogLength, NULL, ShaderErrorMessage);
		printf("%s\n", ShaderErrorMessage);
        free(ShaderErrorMessage);
        return -1;
	}
    return 0;
}

int
LoadShaders(GLuint *programID, const char *vertex_file_path,
            const char *fragment_file_path) {
    int retval = -1;
    const char *vertex_code = NULL;
    const char *fragment_code = NULL;

	// Create the shaders
	GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	// Read the Vertex Shader code from the file
    if (load_code(vertex_file_path, (char **)&vertex_code)) {
        goto end;
    }

	// Read the Fragment Shader code from the file
    if (load_code(fragment_file_path, (char **)&fragment_code)) {
        goto end;
    }

	GLint Result = GL_FALSE;

    // Compile Vertex Shader
    if (compile_code(vertex_file_path, vertex_code, VertexShaderID, &Result)) {
        goto end;
    }

	// Compile Fragment Shader
    if (compile_code(fragment_file_path, fragment_code, FragmentShaderID, &Result)) {
        goto end;
    }

	// Link the program
	printf("Linking program\n");
	*programID = glCreateProgram();
	glAttachShader(*programID, VertexShaderID);
	glAttachShader(*programID, FragmentShaderID);
	glLinkProgram(*programID);

    GLuint InfoLogLength;
	// Check the program
	glGetProgramiv(*programID, GL_LINK_STATUS, &Result);
	glGetProgramiv(*programID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		char *ProgramErrorMessage = malloc(InfoLogLength+1);
		glGetProgramInfoLog(*programID, InfoLogLength, NULL, ProgramErrorMessage);
		printf("%s\n", ProgramErrorMessage);
	}


	glDetachShader(*programID, VertexShaderID);
	glDetachShader(*programID, FragmentShaderID);

	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);

    // If code got here, it means it was successful
	retval = 0;

end:
    xfree(vertex_code);
    xfree(fragment_code);
    return retval;
}


