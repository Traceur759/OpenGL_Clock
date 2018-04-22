#ifndef MODEL_LOADER_H
#define MODEL_LOADER_H

typedef struct _model {
    GLfloat *buffer;
    size_t bufsize;
} Model;

Model * model_load(const char *path);

#endif

