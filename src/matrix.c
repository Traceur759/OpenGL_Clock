#include "clock.h"

static GLdouble *
matrix_multip(GLdouble *mat1, GLdouble *mat2) {
    GLdouble *result;
    GLdouble dot_prod;

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

GLdouble *
matrix_new() {
    GLdouble *mat;

    mat = calloc(16, sizeof(GLdouble));
    
    for (int i = 0; i < 4; i++) {
        mat[i*4+i] = 1;
    }

    return mat;
}


int
matrix_transform(GLdouble *vects, int vectcount,
                 GLdouble **mats, int matcount) {
    GLdouble *mat;
    GLdouble dot_prod;
    GLdouble *helper = NULL;
    GLdouble *result;

    mat = matrix_new();
    result = calloc(vectcount*3, sizeof(GLdouble));

    for (int i = matcount-1; i >= 0; i--) {
        xfree(helper);
        helper = matrix_multip(mats[i], mat);
        free(mat);
        mat = helper;
    }

    for (int k = 0; k < vectcount; k++) {
        for (int j = 0; j < 3; j++) {
            dot_prod = 0;
            for (int i = 0; i < 3; i++) {
                dot_prod += vects[k*3+i] * mat[i+j*4];
            }
            result[j+k*3] = dot_prod;
        }
    }
    memcpy(vects, result, vectcount * 3 * sizeof(GLdouble));
    free(result);

    return 0;
}

