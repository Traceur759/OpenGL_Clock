#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <GL/glew.h>

#include "shader.h"

GLuint LoadShaders(const char *vertex_file_path, const char *fragment_file_path) {

	// Create the shaders
	GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	// Read the Vertex Shader code from the file
    FILE *vertex_file;
    char *vertex_code = malloc(256 * sizeof(char));
    unsigned int cursor = 0;
    
    vertex_file = fopen(vertex_file_path, "r");
    if (vertex_file == NULL) {
        fprintf(stderr, "Could not open %s.\n", vertex_file_path);
        free(vertex_code);
        return 0;
    }

    while (!feof(vertex_file)) {
        vertex_code[cursor] = getc(vertex_file);
        cursor++;
    }

	// Read the Fragment Shader code from the file
    FILE *fragment_file;
    char *fragment_code = malloc(256 * sizeof(char));
    cursor = 0;
    
    fragment_file = fopen(fragment_file_path, "r");
    if (fragment_file == NULL) {
        fprintf(stderr, "Could not open %s.\n", fragment_file_path);
        free(vertex_code);
        free(fragment_code);
        return 0;
    }

    while (!feof(fragment_file)) {
        fragment_code[cursor] = getc(fragment_file);
        cursor++;
    }

	GLint Result = GL_FALSE;
	int InfoLogLength;


	// Compile Vertex Shader
	printf("Compiling shader : %s\n", vertex_file_path);
	glShaderSource(VertexShaderID, 1, (const char **)&vertex_code, NULL);
	glCompileShader(VertexShaderID);

	// Check Vertex Shader
	glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	//if ( InfoLogLength > 0 ){
	//	std::vector<char> VertexShaderErrorMessage(InfoLogLength+1);
	//	glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
	//	printf("%s\n", &VertexShaderErrorMessage[0]);
	//}



	// Compile Fragment Shader
	printf("Compiling shader : %s\n", fragment_file_path);
	glShaderSource(FragmentShaderID, 1, (const char **)&fragment_code, NULL);
	glCompileShader(FragmentShaderID);

	// Check Fragment Shader
	glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	//if ( InfoLogLength > 0 ){
	//	std::vector<char> FragmentShaderErrorMessage(InfoLogLength+1);
	//	glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
	//	printf("%s\n", &FragmentShaderErrorMessage[0]);
	//}



	// Link the program
	printf("Linking program\n");
	GLuint ProgramID = glCreateProgram();
	glAttachShader(ProgramID, VertexShaderID);
	glAttachShader(ProgramID, FragmentShaderID);
	glLinkProgram(ProgramID);

	// Check the program
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	//if ( InfoLogLength > 0 ){
	//	std::vector<char> ProgramErrorMessage(InfoLogLength+1);
	//	glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
	//	printf("%s\n", &ProgramErrorMessage[0]);
	//}

	
	glDetachShader(ProgramID, VertexShaderID);
	glDetachShader(ProgramID, FragmentShaderID);
	
	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);

	return ProgramID;
}


