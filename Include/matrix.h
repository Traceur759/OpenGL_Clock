#ifndef MATRIX_H
#define MATRIX_H

GLfloat *matrix_new(void);

GLfloat *matrix_multip(GLfloat *, GLfloat *);

GLfloat *matrix_transform(GLfloat *, int,
                           GLfloat *);

#endif

