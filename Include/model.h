#ifndef MODEL_LOADER_H
#define MODEL_LOADER_H

typedef struct _entity {
    size_t offset;
    size_t size;
} Entity;

typedef struct _model {
    GLfloat *object;
    size_t bufsize;
    Entity *entities[5];
} Model;

Model * model_load(const char *);

#endif

