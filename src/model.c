#include <clock.h>

Model *
model_new(size_t size) {
    Model *model = calloc(1, sizeof(Model));
    model->object = calloc((size ? size : 1) *4 , sizeof(GLfloat));
    model->bufsize = size;
    for (int i = 0; i < 5; i++) {
        model->entities[i] = calloc(1, sizeof(Entity));
    }
}

void
model_free(Model *self) {
    free(self->object);
    free(self);
    for (int i = 0; i < 5; i++) {
        free(self->entities[i]);
    }
}

Model *
model_load(const char *path) {
    Model *model;
    GLfloat *vertices;
    int **faces;
    size_t vertcount, facecount[5] = {0};
    char type, check, *buffer;
    FILE *f;
    GLfloat colorset[3];
    size_t entity_read;

    f = fopen(path, "r");
    buffer = malloc(255);
    vertices = malloc(3 * sizeof(GLfloat));
    faces = malloc(5*sizeof(int *));
    for (int i = 0; i < 5; i++) {
        faces[i] = malloc(3 * sizeof(int));
    }
    type = 0;
    vertcount = 0;

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
            faces[entity_read] = realloc(faces[entity_read], (facecount[entity_read]+1)*3*sizeof(GLfloat));
            check = fscanf(f, "%d %d %d\n", &(faces[entity_read][facecount[entity_read]*3]),
                                    &(faces[entity_read][facecount[entity_read]*3+1]),
                                    &(faces[entity_read][facecount[entity_read]*3+2]));
            facecount[entity_read]++;
        }
        else if (type == 'o') {
            check = fscanf(f, "%s\n", buffer);
            if (strstr(buffer, "Sphere")) {
                entity_read = 0;
            }
            else if (strstr(buffer, "Text")) {
                entity_read = 1;
            }
            else if (strstr(buffer, "Sec")) {
                entity_read = 2;
            }
            else if (strstr(buffer, "Min")) {
                entity_read = 3;
            }
            else if (strstr(buffer, "Hrs")) {
                entity_read = 4;
            }
        }
        else {
            check = fscanf(f, "%*[^\n]\n", NULL);
            continue;
        }
    } while(check != EOF);

    int total_facecount = 0;
    for (int i = 0; i < 5; i++) {
        total_facecount += facecount[i];
    }
    model = model_new(total_facecount*3);
    int offset = 0;
    for (int ent = 0; ent < 5; ent++) {
        model->entities[ent]->offset = offset;
        for (int i = 0; i < facecount[ent]; i++) {
            for (int j = 0; j < 3; j++) {
                for (int k = 0; k < 3; k++) {
                    model->object[i*12+j*4+k+offset] = vertices[(faces[ent][i*3+j]-1)*3+k];
                }
                model->object[i*12+j*4+3+offset] = 1;
            }
        }
        offset += facecount[ent]*4*3;
        model->entities[ent]->size = facecount[ent]*3;
    }
    return model;
}

