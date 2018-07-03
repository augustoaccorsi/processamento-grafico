#pragma once

#include <iostream>

//GLAD
#include <glad/glad.h>

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//STB
#include "stb_image.h"

class Character
{
public:
	Character();
	~Character();

	GLfloat posX;
	GLfloat posY;
	GLfloat width;
	GLfloat heigth;

	int sl;
	int sc;
	int snl;
	int snc;

	void initialize(GLfloat x, GLfloat y, GLfloat width, GLfloat heigth, int numLin, int numCol, int spriteLine, int spriteColumn, char* texture);
	void render(GLint attrModel, GLint attrTexture);
	bool isConflicting(Character character);
	

private:
	glm::mat4 model;
	GLuint VAO;

	int sw;
	int sh;
	unsigned int texture;
	char* texture_file;

	void setupScene();
	void setupTexture();
};