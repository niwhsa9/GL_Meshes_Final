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

GLFWwindow* window;
GLuint* sp;
GLuint* shader;
glm::vec3 lightPos = glm::vec3(0.0f, 0.0f, 5.0f);
glm::vec3 camPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 lookPos = glm::vec3(0.0f, 0.0f, 0.0f);
glm::mat4 model;
glm::mat4 nmodel;
float zoom = 45.0f;
glm::vec4 center;

void init() {
	glfwInit();
	window = glfwCreateWindow(1000, 1000, "Display", NULL, window);
	glfwMakeContextCurrent(window);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GLFW_TRUE);
	glewExperimental = true;
	glewInit();
	sp = new GLuint[10];
	shader = new GLuint[20];
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
}

void draw() {
	glClear(GL_COLOR_BUFFER_BIT);
	glClear(GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 1.0f, 1.0f, 1.0f);

	glm::mat4 perspective = glm::perspective(glm::radians(zoom), 1.0f, 0.1f, 100.0f);
	glm::mat4 look = glm::lookAt(camPos, lookPos, glm::vec3(0.0f, 1.0f, 0.0f));

	fillUniforms(sp[0], perspective, look, model, nmodel, lightPos, camPos, glm::vec3(1.0, 1.0, 1.0));
	for (int i = 0; i < osize; i++) {
		glBindTexture(GL_TEXTURE_2D, texture[i]);
		glBindBuffer(GL_ARRAY_BUFFER, buffer[i]);
		glBindVertexArray(vao[i]);
		glUseProgram(sp[0]);
		
		glDrawArrays(GL_TRIANGLES, 0, dsize[i]);
	}
}

void handle() {
	if (glfwGetKey(window, GLFW_KEY_EQUAL) == GLFW_PRESS) zoom -= 1.0f;
	if (glfwGetKey(window, GLFW_KEY_MINUS) == GLFW_PRESS) zoom += 1.0f;
	glm::mat4 origin;
	origin = glm::translate(origin, glm::vec3(-center.x, -center.y, -center.z));
	glm::mat4 relocate;
	relocate = glm::translate(relocate, glm::vec3(center.x, center.y, center.z));
	glm::mat4 transf;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) transf = glm::rotate(transf, glm::radians(2.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) transf = glm::rotate(transf, glm::radians(-2.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) transf = glm::rotate(transf, glm::radians(2.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) transf = glm::rotate(transf, glm::radians(-2.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) transf = glm::rotate(transf, glm::radians(2.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) transf = glm::rotate(transf, glm::radians(-2.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	nmodel = nmodel * transf;
	glm::mat4 finalMat;
	finalMat = relocate * transf * origin;
	model = model*finalMat;

	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) lookPos.y += 0.02;
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) lookPos.y -= 0.02;
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) lookPos.x -= 0.02;
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) lookPos.x += 0.02;

	if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS) camPos = glm::vec3(0.0, 0.0, 3.0);
	if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS) camPos = glm::vec3(0.0, 0.0, -3.0);
	if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) camPos = glm::vec3(-3.0, 0.0, 0.0);
	if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS) camPos = glm::vec3(3.0, 0.0, 3.0);
	if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS) camPos = glm::vec3(1.0, 3.0, 0.0);
	if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) camPos = glm::vec3(0.0, -3.0, 3.0);
}

void loop() {
	int fps = 0;
	int nS = 1;

	while (glfwWindowShouldClose(window) != true && glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS) {
		handle();
		draw();
		glfwSwapBuffers(window);
		glfwPollEvents();
		//glfwSwapInterval(0);
		fps++;

		if (glfwGetTime() > nS) {
			nS++;
			std::cout << "FPS: " << fps << endl;
			fps = 0;
		}
	}
}

int main() {
	init();
	model = glm::scale(model, glm::vec3(0.05, 0.05, 0.05));
	nmodel = model;
	importScene();
	string textures[] = {
		"resources\\tank\\1.jpg",
		"resources\\tank\\2.jpg",
		"resources\\tank\\3.jpg",
		"resources\\tank\\4.jpg",
		"resources\\tank\\5.jpg",
	};
	loadTextures(textures);
	center = model * getCenter();
	createShaderProgram(sp, 0);
	std::cout << sp[0] << std::endl;
	importShader(sp, 0, shader, 0, "shaders//vertexShader.txt", GL_VERTEX_SHADER);
	importShader(sp, 0, shader, 1, "shaders//fragmentShader.txt", GL_FRAGMENT_SHADER);
	glLinkProgram(sp[0]);
	glUseProgram(sp[0]);
	std::cout << endl<< "shader id: " << sp[0] << endl;
	loop();
	return 1;
}