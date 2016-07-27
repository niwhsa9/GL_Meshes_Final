#pragma once
#include <glm/mat4x4.hpp>

void storeData(int numObjs, GLfloat** data, int dsize[]);
void fillUniforms(GLuint sp, glm::mat4 proj, glm::mat4 view, glm::mat4 model, glm::mat4 normalModel, glm::vec3 lightPos, glm::vec3 viewPos, glm::vec3 lightCol);
void createShaderProgram(GLuint* &sp, int index);
glm::vec4 getCenter();

extern GLuint* buffer;
extern GLuint* vao;
extern int* dsize;
extern int osize;