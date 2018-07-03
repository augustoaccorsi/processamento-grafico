#include "Character.h"

Character::Character()
{
}
Character::~Character()
{
}
void Character::initialize(GLfloat x, GLfloat y, GLfloat w, GLfloat h, int numLin, int numCol, int sl, int sc, char* texture)
{
	texture_file = texture;
	posX = x; posY = y;
	width = w; heigth = h;
	snl = numLin; snc = numCol;
	sl = sl; sc = sc;

	setupScene();
	setupTexture();
}

void Character::render(GLint attrModel, GLint attrTexture)
{
	model = glm::mat4();
	model = glm::translate(model, glm::vec3(posX, posY, 1.0f));
	glUniformMatrix4fv(attrModel, 1, GL_FALSE, glm::value_ptr(model));
	glDrawArrays(GL_TRIANGLES, 0, 2);

	glBindTexture(GL_TEXTURE_2D, texture);
	glUniform2f(attrTexture, sc, sl);

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

bool Character::isConflicting(Character character) {
	if (abs(((posY)-character.posY)) < 0.3f && posX <= (character.posX + character.width) && (posX + width) >= character.posX)
		return true;
	return false;
}

void Character::setupScene()
{
	float xFactor = 1 / (float)snc;
	float yFactor = 1 / (float)snl;

	float textX = sc * xFactor;
	float textY = sl * yFactor;

	float k = 1.0, o = 1.0;

	float vertices[] = {
		// positions             // colors          // texture coords
		width,  heigth, 0.0f,    1.0f, 0.0f, 0.0f,    k,o,
		width, -heigth, 0.0f,    0.0f, 1.0f, 0.0f,    k,0.0f,
		-width, -heigth, 0.0f,   0.0f, 0.0f, 1.0f,    0.0f,0.0f,
		-width,  heigth, 0.0f,   1.0f, 1.0f, 0.0f,    0.0f,o
	};

	unsigned int indices[] = {
		0, 1, 3, // first triangle
		1, 2, 3  // second triangle
	};

	unsigned int VBO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_TRUE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// texture coord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_TRUE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	setupTexture();
}
void Character::setupTexture()
{

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	int nrChannels;
	unsigned char *data = stbi_load(texture_file, &sw, &sh, &nrChannels, 0);

	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, sw, sh, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	glBindTexture(GL_TEXTURE_2D, 0);
	glActiveTexture(GL_TEXTURE0);
}

