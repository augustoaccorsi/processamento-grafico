#include <thread>
#include "Character.h"
#include "SceneManager.h"
#include "Constants.h"


static bool keys[Constants::KEY_SIZE];
static bool resized;
static GLuint width, height;

static int tiles[Constants::Constants::ROWS][Constants::COLUMNS];
static glm::vec2 tilesPos[Constants::ROWS][Constants::COLUMNS];

Character *hero = new Character;
Character *bomb = new Character;
bool isConlicted = false;

bool checkColision(double x, double y) {

	cout << "(" << x << "," << y << ")";

	int X = (x / Constants::TILEW) + (y / Constants::TILEH);
	int Y = (y / Constants::TILEH) - (x / Constants::TILEW);

	cout << tiles[Y][X];

	if (tiles[Y][X] == 0)
		return true;
	return false;
}

void ClearCharacteres() {
	hero->sc = 0;
	bomb->sl = 0;
	bomb->sc = 0;
}

void initializeObjects() {
	resized = true;

	int x = 0, y = 0;
	int heroX = 0, heroY = 0;
	int bombX = 0, bombY = 0;

	for (int i = 0; i < Constants::ROWS; i++) {
		for (int j = 0; j < Constants::COLUMNS; j++) {
			x = ((j - i)*Constants::TILEW_HALF);
			y = ((j + i)*Constants::TILEH_HALF);
			if (tiles[i][j] == 2) {
				tiles[i][j] = 0;
				heroX = x + Constants::TILEW_HALF;
				heroY = y + Constants::TILEH_HALF;
			}

			if (tiles[i][j] == 3)
			{
				tiles[i][j] = 0;
				bombX = x + Constants::TILEW_HALF;
				bombY = y + Constants::TILEH_HALF;
			}

			tilesPos[i][j] = glm::vec2(x, y);
		}
	}
	hero->initialize(heroX, heroY, 25, 25, 4, 4, 0, 0, "../textures/hero - Copy.png");
	bomb->initialize(bombX, bombY, 15, 15, 1, 1, 0, 0, "../textures/bomba.png");
}

SceneManager::SceneManager()
{
}

SceneManager::~SceneManager()
{
}

void SceneManager::initialize(GLuint w, GLuint h)
{
	width = w;
	height = h;
	loadMatrix();
	initializeGraphics();
}

void SceneManager::initializeGraphics()
{
	// Init GLFW
	glfwInit();

	// Create a GLFWwindow object that we can use for GLFW's functions
	window = glfwCreateWindow(width, height, "Trabalho GB", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	glfwSetKeyCallback(window, key_callback);
	glfwSetMouseButtonCallback(window, mouseClick);

	glfwSetWindowSizeCallback(window, resize);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;

	}

	addShader("../shaders/transformations.vs", "../shaders/transformations.frag");

	setupScene();
	initializeObjects();
}



void SceneManager::loadMatrix()
{
	int i, j;
	ifstream file("../textures/tilemap.txt");

	if (!file) {
		cout << "Cannot open file.\n";
		return;
	}

	for (i = 0; i < Constants::ROWS; i++) {
		for (j = 0; j < Constants::COLUMNS; j++) {
			file >> tiles[i][j];
		}
	}

	file.close();
}

void SceneManager::addShader(string vFilename, string fFilename)
{
	shader = new Shader(vFilename.c_str(), fFilename.c_str());
}


void SceneManager::key_callback(GLFWwindow * window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
			keys[key] = true;
		else if (action == GLFW_RELEASE)
			keys[key] = false;
	}
}

void SceneManager::resize(GLFWwindow * window, int w, int h)
{
	width = w;
	height = h;
	resized = true;

	glViewport(0, 0, width, height);
}


void SceneManager::do_movement()
{
	if (!keys[GLFW_KEY_UP] && !keys[GLFW_KEY_DOWN] && !keys[GLFW_KEY_RIGHT] && !keys[GLFW_KEY_LEFT]) {
		ClearCharacteres();
	}
	else {
		if (keys[GLFW_KEY_DOWN]) {
			hero->sl = 4;
			hero->sc = ((hero->sc + 1) % 4);
			if (checkColision(hero->posX, hero->posY + 0.5f)) {
				hero->posY += 0.5f;
			}
		}

		if (keys[GLFW_KEY_UP]) {
			hero->sl = 1;
			hero->sc = ((hero->sc + 1) % 4);
			if (checkColision(hero->posX, hero->posY - 0.5f)) {
				hero->posY -= 0.5f;
			}
		}

		if (keys[GLFW_KEY_LEFT]) {
			hero->sl = 2;
			hero->sc = ((hero->sc + 1) % 4);
			if (checkColision(hero->posX - 0.5f, hero->posY)) {
				hero->posX -= 0.5f;
			}
		}

		if (keys[GLFW_KEY_RIGHT]) {
			hero->sl = 3;
			hero->sc = ((hero->sc + 1) % 4);
			if (checkColision(hero->posX + 0.5f, hero->posY)) {
				hero->posX += 0.5f;
			}
		}
	}

	if (keys[GLFW_KEY_ESCAPE])
		glfwSetWindowShouldClose(window, GL_TRUE);
}

void SceneManager::render()
{
	// Clear the colorbuffer
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Render scene
	shader->Use();

	GLint modelLoc = glGetUniformLocation(shader->Program, "model");
	GLint offsetLoc = glGetUniformLocation(shader->Program, "offsetUV");

	for (int i = 0; i < Constants::ROWS; i++)
	{
		for (int j = 0; j < Constants::COLUMNS; j++)
		{
			glm::vec2 offset;
			if (tiles[i][j] == 1) {
				offset = glm::vec2(1, 3);
			}
			else {
				offset = glm::vec2(4, 4);
			}
			glUniform2f(offsetLoc, offset.x, offset.y);

			// Create transformations 
			model = glm::mat4();
			model = glm::translate(model, glm::vec3(tilesPos[i][j].x, tilesPos[i][j].y, 0.0));

			// Pass them to the shaders
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

			if (resized)
			{
				setupCamera2D();
				resized = false;
			}

			glBindTexture(GL_TEXTURE_2D, texture);
			glUniform1i(glGetUniformLocation(shader->Program, "ourTexture1"), 0);
			glBindVertexArray(VAO);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		}
	}

	hero->render(modelLoc, offsetLoc);
	bomb->render(modelLoc, offsetLoc);

	if (!isConlicted)
	{
		if (hero->isConflicting(*bomb))
		{
			isConlicted = true;
			finish();
		}
	}
}

void SceneManager::run()
{
	//GAME LOOP
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		do_movement();

		render();

		glfwSwapBuffers(window);
	}
}

void SceneManager::finish()
{
	glfwTerminate();
}


void SceneManager::setupScene()
{
	float textureX = 1.0, textureY = 1.0;

	float wt = Constants::TILEW, ht = Constants::TILEH;

	float vertices[] = {
		// positions          // colors           // texture coords
		wt,   ht,   0.0f,    1.0f, 0.0f, 0.0f,    textureX,textureY,
		wt,   0.0f, 0.0f,    0.0f, 1.0f, 0.0f,    textureX,0.0f,
		0.0f, 0.0f, 0.0f,    0.0f, 0.0f, 1.0f,    0.0f, 0.0f,
		0.0f, ht,   0.0f,    1.0f, 1.0f, 0.0f,    0.0f, textureY
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

void SceneManager::setupCamera2D()
{
	//corrigindo o aspecto
	float ratio;
	float xMin = -400, xMax = 400, yMin = 600, yMax = 0.0, zNear = -1.0, zFar = 1.0;
	if (width >= height)
	{
		ratio = width / (float)height;
		projection = glm::ortho(xMin*ratio, xMax*ratio, yMin, yMax, zNear, zFar);
	}
	else
	{
		ratio = height / (float)width;
		projection = glm::ortho(xMin, xMax, yMin*ratio, yMax*ratio, zNear, zFar);
	}

	// Get their uniform location
	GLint projLoc = glGetUniformLocation(shader->Program, "projection");
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
}

void SceneManager::setupTexture()
{
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, nrChannels;
	unsigned char *data = stbi_load("../textures/textura.png", &width, &height, &nrChannels, 0);

	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	glBindTexture(GL_TEXTURE_2D, 0);

	glActiveTexture(GL_TEXTURE0);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void SceneManager::mouseClick(GLFWwindow* window, int button, int action, int mods) {
	//TODO
}