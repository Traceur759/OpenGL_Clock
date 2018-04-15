#ifndef MATRIX_H
#define MATRIX_H

GLdouble *matrix_new(void);

int matrix_transform(GLdouble *, int,
                     GLdouble **, int);

#endif
