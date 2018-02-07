#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <GL/glew.h>

#include "shader.h"

static int load_code(const char *filepath, char **code) {
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

static int compile_code(const char *filepath, const char *code,
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

GLuint
LoadShaders(const char *vertex_file_path,
            const char *fragment_file_path) {
    char *vertex_code;
    char *fragment_code;

	// Create the shaders
	GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	// Read the Vertex Shader code from the file
    load_code(vertex_file_path, &vertex_code);

	// Read the Fragment Shader code from the file
    load_code(fragment_file_path, &fragment_code);

	GLint Result = GL_FALSE;

    // Compile Vertex Shader
    compile_code(vertex_file_path, vertex_code, VertexShaderID, &Result);

	// Compile Fragment Shader
    compile_code(fragment_file_path, fragment_code, FragmentShaderID, &Result);

	// Link the program
	printf("Linking program\n");
	GLuint ProgramID = glCreateProgram();
	glAttachShader(ProgramID, VertexShaderID);
	glAttachShader(ProgramID, FragmentShaderID);
	glLinkProgram(ProgramID);

    GLuint InfoLogLength;
	// Check the program
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		char *ProgramErrorMessage = malloc(InfoLogLength+1);
		glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, ProgramErrorMessage);
		printf("%s\n", ProgramErrorMessage);
	}

	
	glDetachShader(ProgramID, VertexShaderID);
	glDetachShader(ProgramID, FragmentShaderID);
	
	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);

	return ProgramID;
}


