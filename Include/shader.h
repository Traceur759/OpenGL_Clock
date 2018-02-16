#ifndef SHADER_HPP
#define SHADER_HPP

#define xfree(p) if (p) free((void *)p)

int LoadShaders(GLuint *, const char *, const char *);

#endif
