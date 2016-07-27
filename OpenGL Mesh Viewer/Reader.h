#pragma once
#include <GL/glew.h>
#include <stdlib.h>
#include <string>
using namespace std;
void importShader(GLuint* &sp, int spIndex, GLuint* &shader, int shaderIndex, string path, GLenum type);
void importScene();
extern GLfloat** odata;
void loadTextures(string files[]);
extern GLuint* texture;