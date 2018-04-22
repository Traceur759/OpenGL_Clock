#include <clock.h>

Model *
model_new(size_t size) {
    Model *model = calloc(1, sizeof(Model));
    model->buffer = calloc((size ? size : 1) *4 , sizeof(GLfloat));
    model->bufsize = size;
}

void
model_free(Model *self) {
    free(self->buffer);
    free(self);
}

Model *
model_load(const char *path) {
    Model *model;
    GLfloat *vertices;
    int *faces;
    size_t vertcount, facecount;
    char type, check, *line;
    FILE *f;

    f = fopen(path, "r");
    line = malloc(255);
    vertices = malloc(3 * sizeof(GLfloat));
    faces = malloc(3 * sizeof(int));
    type = 0;
    vertcount = 0;
    facecount = 0;

    do {
        check = fscanf(f, "%c", &type);
        if (check == EOF) {
            break;
        }
        else if (type == 'v') {
            vertices = realloc(vertices, (vertcount+1)*3*sizeof(GLfloat));
            check = fscanf(f, "%f %f %f\n", &(vertices[vertcount*3]),
                                    &(vertices[vertcount*3+1]),
                                    &(vertices[vertcount*3+2]));
            vertcount++;
        }
        else if (type == 'f') {
            faces = realloc(faces, (facecount+1)*3*sizeof(GLfloat));
            check = fscanf(f, "%d %d %d\n", &(faces[facecount*3]),
                                    &(faces[facecount*3+1]),
                                    &(faces[facecount*3+2]));
            facecount++;
        }
        else {
            check = fscanf(f, "%*[^\n]\n", NULL);
            continue;
        }
    } while(check != EOF);

    model = model_new(facecount*3);
    for (int i = 0; i < facecount; i++) {
        for (int j = 0; j < 3; j++) {
            for (int k = 0; k < 3; k++) {
                model->buffer[i*12+j*4+k] = vertices[(faces[i*3+j]-1)*3+k];
            }
            model->buffer[i*12+j*4+3] = 1;
        }
    }

    return model;
}

