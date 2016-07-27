#include <iostream>
#include <stdlib.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Secondary.h"
#include "Reader.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;

GLuint* buffer;
GLuint* vao;
int* dsize;
int osize;

void storeData(int numObjs, GLfloat** data, int ds[]) {
	vao = new GLuint[numObjs];
	buffer = new GLuint[numObjs];
	glGenVertexArrays(numObjs, vao);
	glGenBuffers(numObjs, buffer);
	dsize = ds;
	osize = numObjs;
	for (int i = 0; i < numObjs; i++) {
		glBindVertexArray(vao[i]);
		glBindBuffer(GL_ARRAY_BUFFER, buffer[i]);
		glBufferData(GL_ARRAY_BUFFER, ds[i] * sizeof(GLfloat), data[i], GL_DYNAMIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 8, (GLvoid*)0); //PERM
	//	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 0, (GLvoid*)0); //TEMP
		glEnableVertexAttribArray(0);
		
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 8, (GLvoid*)(sizeof(GLfloat) * 3));
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 8, (GLvoid*)(sizeof(GLfloat) * 6));
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
	}
}

void fillUniforms(GLuint sp, glm::mat4 proj, glm::mat4 view, glm::mat4 model, glm::mat4 normalModel, glm::vec3 lightPos, glm::vec3 viewPos, glm::vec3 lightCol) {
	glUniformMatrix4fv(glGetUniformLocation(sp, "proj"), 1, GL_FALSE, glm::value_ptr(proj));
	glUniformMatrix4fv(glGetUniformLocation(sp, "view"), 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(glGetUniformLocation(sp, "model"), 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(glGetUniformLocation(sp, "nmodel"), 1, GL_FALSE, glm::value_ptr(normalModel));
	glUniform3f(glGetUniformLocation(sp, "lightPos"), lightPos.x, lightPos.y, lightPos.z);
	glUniform3f(glGetUniformLocation(sp, "viewPos"), viewPos.x, viewPos.y, viewPos.z);
	glUniform3f(glGetUniformLocation(sp, "lightColor"), lightCol.x, lightCol.y, lightCol.z);
}

void createShaderProgram(GLuint* &sp, int index) {
	sp[index] = glCreateProgram();
}

glm::vec4 getCenter() {
	GLfloat x = 0;
	GLfloat y = 0;
	GLfloat z = 0;
	GLfloat ds = 0;

	for (int n = 0; n < osize; n++) {

		for (int i = 0; i < dsize[n]; i += 8) {
			x += odata[n][i];
			y += odata[n][i+1];
			z += odata[n][i+2];
		}
		ds += dsize[n] / 8;
	}

	x = x / ds;
	y = y / ds;
	z = z / ds;

	std::cout << "center: " << x << "," << y << "," << z;
	return glm::vec4(x, y, z, 1.0f);
}