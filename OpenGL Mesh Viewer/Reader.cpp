#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <GL/glew.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include "Reader.h"
#include "Secondary.h"
#include <SOIL.h>

using namespace std;

Assimp::Importer importer;
const aiScene* scene;
GLfloat** odata = new GLfloat*[5000000];
GLuint* texture = new GLuint[9];
int ds[9];
char sdata[2000];

void importShader(GLuint* &sp, int spIndex, GLuint* &shader, int shaderIndex, string path, GLenum type) {

	for (int i = 0; i < 2000; i++) sdata[i] = '\0';

	std::ifstream shaderFile;
	shaderFile.open(path);
	if (shaderFile.is_open()) cout << "Shader at: " << path << ", initalized" << endl;

	shaderFile.read(sdata, 2000);

	for (int i = 0; i < 2000; i++) std::cout << sdata[i];
	const char* fdata = sdata;
	shader[shaderIndex] = glCreateShader(type);
	glShaderSource(shader[shaderIndex], 1, &fdata, NULL);
	glCompileShader(shader[shaderIndex]);
	if (type == GL_FRAGMENT_SHADER) {
		glBindFragDataLocation(shader[shaderIndex], 0, "finalColor");
	}
	glAttachShader(sp[spIndex], shader[shaderIndex]);

	char b1[512];
	glGetShaderInfoLog(shader[shaderIndex], 512, NULL, b1);
	cout << endl;
	std::cout << "shader " << path << ":" << std::endl << b1 << std::endl;
	shaderFile.close();
}

void importScene() {
	scene = importer.ReadFile("resources\\tank\\tank.obj", aiProcess_Triangulate | aiProcess_FlipUVs);
	for (int i = 0; i < 9; i++) odata[i] = new GLfloat[5000000];
	for (int i = 0; i < scene->mNumMeshes; i++) {
		const aiMesh* mesh = scene->mMeshes[i];
		ds[i] = mesh->mNumVertices * 8;
		for (int n = 0; n < mesh->mNumVertices; n++) {
			odata[i][n * 8] = mesh->mVertices[n].x;
			odata[i][n * 8 + 1] = mesh->mVertices[n].y;
			odata[i][n * 8 + 2] = mesh->mVertices[n].z;

			
			odata[i][n * 8 + 3] = mesh->mNormals[n].x;
			odata[i][n * 8 + 4] = mesh->mNormals[n].y;
			odata[i][n * 8 + 5] = mesh->mNormals[n].z;
			
			odata[i][n * 8 + 6] = mesh->mTextureCoords[0][n].x;
			odata[i][n * 8 + 7] = mesh->mTextureCoords[0][n].y;
			
		}
	}
	storeData(scene->mNumMeshes, odata, ds);
}

void loadTextures(string files[]) {
	glGenTextures(osize, texture);
	for (int i = 0; i < osize; i++) {
		glBindTexture(GL_TEXTURE_2D, texture[i]);
		int width, height;
		unsigned char* image = SOIL_load_image(files[i].c_str(), &width, &height, 0, SOIL_LOAD_RGB);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(GL_TEXTURE_2D);
		SOIL_free_image_data(image);
	}
}