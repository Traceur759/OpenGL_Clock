#include "clock.h"


GLfloat *
matrix_new() {
    GLfloat *mat;

    mat = calloc(16, sizeof(GLfloat));
    
    for (int i = 0; i < 4; i++) {
        mat[i*4+i] = 1;
    }

    return mat;
}

GLfloat *
matrix_multip(GLfloat *mat1, GLfloat *mat2) {
    GLfloat *result;
    GLfloat dot_prod;

    result = matrix_new();

    for (int i = 0; i < 4; i++) {

        for (int j = 0; j < 4; j++)  {
            dot_prod = 0;
            for (int k = 0; k < 4; k++) {
                dot_prod += mat1[i*4+k] * mat2[j+k*4];
            }
            result[j+i*4] = dot_prod;
        }
    }
    
    return result;
}


GLfloat *
matrix_transform(GLfloat *vects, int vectcount,
                 GLfloat *mat) {
    GLfloat dot_prod;
    GLfloat *helper = NULL;
    GLfloat *result;

    result = calloc(vectcount*4, sizeof(GLfloat));


    for (int k = 0; k < vectcount; k++) {
        for (int j = 0; j < 4; j++) {
            dot_prod = 0;
            for (int i = 0; i < 4; i++) {
                dot_prod += vects[k*4+i] * mat[i+j*4];
            }
            result[j+k*4] = dot_prod;
        }
        if (result[k*4+3] != 0.0f) {
            GLfloat div = result[k*4+3];
            for (int i = 0; i < 4; i++) {
                result[k*4+i] /= div;
            }
        }
    }
    return result;
}

