#include "SceneManager.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
struct delta {
	GLfloat y_max;
	GLfloat y_min;
	GLfloat x_max;
	GLfloat x_min;
};

//static controllers for mouse and keyboard
static bool keys[1024];
static bool resized;
static GLuint width, height;
int enemyCount = 0;
int couter = 0;
delta trump, enemy;
bool gameOver = false;
bool paralax = false;

bool checkColision(delta trump, delta enemy) {
	if (enemy.x_max < trump.x_min || enemy.x_min > trump.x_max) return false;
	if (enemy.y_max < trump.y_min || enemy.y_min > trump.y_max) return false;
	return true;
};

void SceneManager::printGameOverScreen() {
	cout << "GAME OVER\n";
	cout << "You have not accomplished the task of save United States from the nuclear attack\n";
	cout << "You have scored " << score << " points";
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

	// GLFW - GLEW - OPENGL general setup -- TODO: config file
	initializeGraphics();
	setupTexture();

}

void SceneManager::initializeGraphics()
{
	// Init GLFW
	glfwInit();

	// Create a GLFWwindow object that we can use for GLFW's functions
	window = glfwCreateWindow(width, height, "Hello Transform", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	// Set the required callback functions
	glfwSetKeyCallback(window, key_callback);

	//Setando a callback de redimensionamento da janela
	glfwSetWindowSizeCallback(window, resize);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;

	}

	// Build and compile our shader program
	addShader("../shaders/transformations.vs", "../shaders/transformations.frag");

	//setup the scene -- LEMBRANDO QUE A DESCRIÇÃO DE UMA CENA PODE VIR DE ARQUIVO(S) DE 
	// CONFIGURAÇÃO
	setupScene();

	resized = true; //para entrar no setup da câmera na 1a vez

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

	// Define the viewport dimensions
	glViewport(0, 0, width, height);
}


void SceneManager::do_movement()
{
	if (keys[GLFW_KEY_ESCAPE])
		glfwSetWindowShouldClose(window, GL_TRUE);

	if (keys[GLFW_KEY_ESCAPE])
		glfwSetWindowShouldClose(window, GL_TRUE);

	if (keys[GLFW_KEY_UP]) {
		if (y_axis < 1.8) {
			y_axis += 0.1f;
			cont += 0.1;
			if (head_to != 't') {
				y_tex_b = y_off;
				y_tex_t = y_off * 2;
				head_to = 't';
				cont = 0.0;
			}
			x_tex_l += x_off;
			x_tex_r += x_off;
			if (x_tex_l >= 1.0) {
				x_tex_l = 0.0;
				x_tex_r = x_off;
			}
		}
	}

	if (keys[GLFW_KEY_DOWN]) {
		if (y_axis > -1.8) {
			y_axis -= 0.1f;
			cont += 0.1;
			if (head_to != 'b') {
				y_tex_b = y_off * 3;
				y_tex_t = y_off * 4;
				head_to = 'b';
				cont = 0.0;
			}
			x_tex_l += x_off;
			x_tex_r += x_off;
			if (x_tex_l >= 1.0) {
				x_tex_l = 0.0;
				x_tex_r = x_off;
			}
		}
	}

	if (keys[GLFW_KEY_LEFT]) {
		if (x_axis > -1.8) {
			x_axis -= 0.1f;
			cont += 0.1;
			if (head_to != 'l') {
				y_tex_b = 0.0f;
				y_tex_t = y_off;
				head_to = 'l';
				cont = 0.0;
			}
			x_tex_l += x_off;
			x_tex_r += x_off;
			if (x_tex_l >= 1.0) {
				x_tex_l = 0.0;
				x_tex_r = x_off;
			}
		}
	}

	if (keys[GLFW_KEY_RIGHT]) {
		if (x_axis < 1.8) {
			x_axis += 0.1f;
			cont += 0.1;
			if (head_to != 'r') {
				y_tex_b = y_off * 2;
				y_tex_t = y_off * 3;
				head_to = 'r';
				cont = 0.0;
			}
			x_tex_l += x_off;
			x_tex_r += x_off;
			if (x_tex_l >= 1.0) {
				x_tex_l = 0.0;
				x_tex_r = x_off;
			}
		}
	}

}

void SceneManager::render()
{
	// Clear the colorbuffer
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Render scene
	shader->Use();
	setupScene();

	if (gameOver)
	{
		//generate space
		model = glm::mat4();
		// Get their uniform location
		GLint modelLoc = glGetUniformLocation(shader->Program, "model");
		// Pass them to the shaders
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		//Background texture
		glBindTexture(GL_TEXTURE_2D, gameovertexture);
		glUniform1i(glGetUniformLocation(shader->Program, "ourTexture1"), 0);

		// render container
		glBindVertexArray(VAOspace);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	}
	else {
		//generate space
		model = glm::mat4();
		// Get their uniform location
		GLint modelLoc = glGetUniformLocation(shader->Program, "model");
		// Pass them to the shaders
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		//Background texture
		glBindTexture(GL_TEXTURE_2D, spaceTexture);
		glUniform1i(glGetUniformLocation(shader->Program, "ourTexture1"), 0);

		// render container
		glBindVertexArray(VAOspace);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		//generate bg_trump
		model = glm::mat4();
		// Get their uniform location
		modelLoc = glGetUniformLocation(shader->Program, "model");
		// Pass them to the shaders
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		//Background texture
		glBindTexture(GL_TEXTURE_2D, bgtexture);
		glUniform1i(glGetUniformLocation(shader->Program, "ourTexture1"), 0);

		// render container
		glBindVertexArray(VAObg);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		//enemy
		// Bind Textures using texture units
		glBindTexture(GL_TEXTURE_2D, enemyTexture);
		glUniform1i(glGetUniformLocation(shader->Program, "ourTexture1"), 0);

		// render container
		glBindVertexArray(VAOEnemy);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		// Create transformations 
		model = glm::mat4();
		model = glm::translate(model, glm::vec3(x_axis, y_axis, 0.0));
		//model = glm::rotate(model, (GLfloat)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));

		// Get their uniform location
		modelLoc = glGetUniformLocation(shader->Program, "model");

		// Pass them to the shaders
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		if (resized) //se houve redimensionamento na janela, redefine a projection matrix
		{
			setupCamera2D();
			resized = false;
		}

		// Bind Textures using texture units
		glBindTexture(GL_TEXTURE_2D, texture);
		glUniform1i(glGetUniformLocation(shader->Program, "ourTexture1"), 0);

		// render container
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	}

}

void SceneManager::run()
{
	//GAME LOOP
	while (!glfwWindowShouldClose(window))
	{
		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();

		//Update method(s)
		do_movement();

		//Render scene	

		//stop animation and bring gameover screen
		if (gameOver) {
			////generate space
			//model = glm::mat4();
			//// Get their uniform location
			//GLint modelLoc = glGetUniformLocation(shader->Program, "model");
			//// Pass them to the shaders
			//glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

			////Background texture
			//glBindTexture(GL_TEXTURE_2D, spaceTexture);
			//glUniform1i(glGetUniformLocation(shader->Program, "ourTexture1"), 0);

			//// render container
			//glBindVertexArray(VAOspace);
			//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		}
		else {
			Sleep(80);
			render();

			// Swap the screen buffers
			glfwSwapBuffers(window);
		}

	}
	printGameOverScreen();

}

void SceneManager::finish()
{
	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();
}


void SceneManager::setupScene()
{
	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	topY = 0.5f + y_axis;
	botY = -0.5f + y_axis;
	rigX = 0.5f + x_axis;
	lefX = -0.5f + x_axis;

	if (topY >= yMaxScr) topY = yMaxScr;
	if (botY <= yMinScr) botY = yMinScr;
	if (rigX >= xMaxScr) rigX = xMaxScr;
	if (lefX <= xMinScr) lefX = xMinScr;

	float vertices[] = {
		// positions		// colors          // texture coords
		rigX, topY, 0.0f,   1.0f, 0.0f, 0.0f,  x_tex_r, y_tex_t, // top right
		rigX, botY, 0.0f,   0.0f, 1.0f, 0.0f,  x_tex_r, y_tex_b, // bottom right
		lefX, botY, 0.0f,   0.0f, 0.0f, 1.0f,  x_tex_l, y_tex_b, // bottom left
		lefX, topY, 0.0f,   1.0f, 1.0f, 0.0f,  x_tex_l, y_tex_t  // top left 
	};
	unsigned int indices[] = {
		0, 1, 3, // first triangle
		1, 2, 3  // second triangle
	};


	x_bgoffset = 0.03f;
	space_offset = 0.01;

	if (couter < 50 && paralax == false) {
		x_maxbg += x_bgoffset;
		x_minbg += x_bgoffset;
		xmax_space += space_offset;
		xmin_space += space_offset;
	}
	else if (couter < 150) {
		paralax = true;
		x_minbg -= x_bgoffset;
		x_maxbg -= x_bgoffset;
		xmax_space -= space_offset;
		xmin_space -= space_offset;
	}
	couter++;
	if (couter == 150) {
		couter = -50;
		paralax = false;
	}

	float spacevertices[] = {
		// positions		// colors          // texture coords
		xmax_space, yMaxScr, 0.0f,   1.0f, 0.0f, 0.0f,  1.0f, 1.0f, // top right
		xmax_space, yMinScr, 0.0f,   0.0f, 1.0f, 0.0f,  1.0f, 0.0f, // bottom right
		xmin_space, yMinScr, 0.0f,   0.0f, 0.0f, 1.0f,  0.0f, 0.0f, // bottom left
		xmin_space, yMaxScr, 0.0f,   1.0f, 1.0f, 0.0f,  0.0f, 1.0f  // top left 
	};

	float bgvertices[] = {
		// positions		// colors          // texture coords
		x_maxbg, yMaxScr, 0.0f,   1.0f, 0.0f, 0.0f,  1.0f, 1.0f, // top right
		x_maxbg, yMinScr, 0.0f,   0.0f, 1.0f, 0.0f,  1.0f, 0.0f, // bottom right
		x_minbg, yMinScr, 0.0f,   0.0f, 0.0f, 1.0f,  0.0f, 0.0f, // bottom left
		x_minbg, yMaxScr, 0.0f,   1.0f, 1.0f, 0.0f,  0.0f, 1.0f  // top left 
	};

	float govertices[] = {
		// positions		// colors          // texture coords
		xMaxScr, yMaxScr, 0.0f,   1.0f, 0.0f, 0.0f,  1.0f, 1.0f, // top right
		xMaxScr, yMinScr, 0.0f,   0.0f, 1.0f, 0.0f,  1.0f, 0.0f, // bottom right
		xMinScr, yMinScr, 0.0f,   0.0f, 0.0f, 1.0f,  0.0f, 0.0f, // bottom left
		xMinScr, yMaxScr, 0.0f,   1.0f, 1.0f, 0.0f,  0.0f, 1.0f  // top left 
	};

	enemyOffset += enemySpeed;


	//float enemyVertices[] = {
	//	// positions		// colors          // texture coords
	//	xMaxEnemy - enemyOffset, yMaxEnemy, 0.0f,   1.0f, 0.0f, 0.0f,  1.0f, 1.0f, // top right
	//	xMinEnemy - enemyOffset, yMaxEnemy, 0.0f,   0.0f, 1.0f, 0.0f,  1.0f, 0.0f, // bottom right
	//	xMinEnemy - enemyOffset, yMinEnemy, 0.0f,   0.0f, 0.0f, 1.0f,  0.0f, 0.0f, // bottom left
	//	xMaxEnemy - enemyOffset, yMinEnemy, 0.0f,   1.0f, 1.0f, 0.0f,  0.0f, 1.0f  // top left
	//};

	float enemyVertices[] = {
		// positions		// colors          // texture coords
		xMaxEnemy - enemyOffset, yMaxEnemy, 0.0f,   1.0f, 0.0f, 0.0f,  0.0f, 1.0f, // top right
		xMinEnemy - enemyOffset, yMaxEnemy, 0.0f,   0.0f, 1.0f, 0.0f,  1.0f, 1.0f, // bottom right
		xMinEnemy - enemyOffset, yMinEnemy, 0.0f,   0.0f, 0.0f, 1.0f,  1.0f, 0.0f, // bottom left
		xMaxEnemy - enemyOffset, yMinEnemy, 0.0f,   1.0f, 1.0f, 0.0f,  0.0f, 0.0f  // top left
	};

	trump.x_max = rigX;
	trump.x_min = lefX;
	trump.y_max = topY;
	trump.y_min = botY;

	enemy.x_max = (xMaxEnemy - enemyOffset);
	enemy.x_min = (xMinEnemy - enemyOffset);
	enemy.y_max = (yMaxEnemy);
	enemy.y_min = (yMinEnemy);

	if (checkColision(trump, enemy)) {
		cout << "game over: " << score << " ";
		gameOver = true;
	}

	if (enemyOffset > 8) {
		GLfloat min = 0.5, max = 8.0;
		yMaxEnemy = (min + rand() * (max - min) / RAND_MAX) - 4.0;
		yMinEnemy = yMaxEnemy - 0.5;
		xMaxEnemy = 4.0;
		xMinEnemy = 3.5;
		enemyOffset = 0.0f;
		enemySpeed += 0.01f;
		if (!gameOver)
			score += 10;
	}

	unsigned int VBO, EBO, VBObg, EBObg, VBOEnemy, EBOEnemy, VBOspace, EBOspace, VBOgo, EBOgo;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// texture coord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	//------------------------------------------------------------------------------------------------
	glGenVertexArrays(1, &VAOspace);
	glGenBuffers(1, &VBOspace);
	glGenBuffers(1, &EBOspace);

	glBindVertexArray(VAOspace);

	glBindBuffer(GL_ARRAY_BUFFER, VBOspace);
	glBufferData(GL_ARRAY_BUFFER, sizeof(spacevertices), spacevertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOspace);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// texture coord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	//------------------------------------------------------------------------------------------------
	glGenVertexArrays(1, &VAObg);
	glGenBuffers(1, &VBObg);
	glGenBuffers(1, &EBObg);

	glBindVertexArray(VAObg);

	glBindBuffer(GL_ARRAY_BUFFER, VBObg);
	glBufferData(GL_ARRAY_BUFFER, sizeof(bgvertices), bgvertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBObg);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// texture coord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	//------------------------------------------------------------------------------------------------
	glGenVertexArrays(1, &VAOEnemy);
	glGenBuffers(1, &VBOEnemy);
	glGenBuffers(1, &EBOEnemy);

	glBindVertexArray(VAOEnemy);

	glBindBuffer(GL_ARRAY_BUFFER, VBOEnemy);
	glBufferData(GL_ARRAY_BUFFER, sizeof(enemyVertices), enemyVertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOEnemy);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// texture coord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glGenVertexArrays(1, &VAOgameover);
	glGenBuffers(1, &VBOgo);
	glGenBuffers(1, &EBOgo);

	glBindVertexArray(VAOgameover);

	glBindBuffer(GL_ARRAY_BUFFER, VBOgo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(govertices), govertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOgo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// texture coord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	//setupTexture();

}

void SceneManager::setupCamera2D()
{
	//corrigindo o aspecto
	float ratio;
	float xMin = -4.0, xMax = 4.0, yMin = -4.0, yMax = 4.0, zNear = -4.0, zFar = 4.0;

	if (width >= height)
	{
		ratio = width / (float)height;
		projection = glm::ortho(xMin, xMax, yMin, yMax, zNear, zFar);
	}
	else
	{
		ratio = height / (float)width;
		projection = glm::ortho(xMin, xMax, yMin, yMax, zNear, zFar);
	}

	xMinScr = xMin, xMaxScr = xMax, yMinScr = yMin, yMaxScr = yMax, zNearScr = zNear, zFarScr = zFar;

	// Get their uniform location
	GLint projLoc = glGetUniformLocation(shader->Program, "projection");
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
}

void SceneManager::setupTexture()
{
	// load and create a texture 
	// -------------------------
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
										   // set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// load image, create texture and generate mipmaps
	int width, height, nrChannels;
	//unsigned char *data = SOIL_load_image("../textures/wall.jpg", &width, &height, 0, SOIL_LOAD_RGB);
	unsigned char *data = stbi_load("../textures/trump_run.png", &width, &height, &nrChannels, 0);

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

	glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture when done, so we won't accidentily mess up our texture.
	glActiveTexture(GL_TEXTURE0);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//--------------------------------------------------------------------------------------------------------
	glGenTextures(1, &spaceTexture);
	glBindTexture(GL_TEXTURE_2D, spaceTexture); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
										   // set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// load image, create texture and generate mipmaps
	int spacewidth, spaceheight, spacenrChannels;
	//unsigned char *data = SOIL_load_image("../textures/wall.jpg", &width, &height, 0, SOIL_LOAD_RGB);
	//unsigned char *spacedata = stbi_load("../textures/background.jpg", &spacewidth, &spaceheight, &spacenrChannels, 0);
	unsigned char *spacedata = stbi_load("../textures/map.png", &spacewidth, &spaceheight, &spacenrChannels, 0);

	if (spacedata)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, spacewidth, spaceheight, 0, GL_RGBA, GL_UNSIGNED_BYTE, spacedata);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(spacedata);

	glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture when done, so we won't accidentily mess up our texture.
	glActiveTexture(GL_TEXTURE0);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//------------------------------------------------------------------------------------------------------------

	//--------------------------------------------------------------------------------------------------------
	glGenTextures(1, &bgtexture);
	glBindTexture(GL_TEXTURE_2D, bgtexture); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
											 // set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// load image, create texture and generate mipmaps
	int bgwidth, bgheight, bgnrChannels;
	//unsigned char *data = SOIL_load_image("../textures/wall.jpg", &width, &height, 0, SOIL_LOAD_RGB);
	//unsigned char *bgdata = stbi_load("../textures/background.jpg", &bgwidth, &bgheight, &bgnrChannels, 0);
	unsigned char *bgdata = stbi_load("../textures/trump.png", &bgwidth, &bgheight, &bgnrChannels, 0);

	if (bgdata)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, bgwidth, bgheight, 0, GL_RGBA, GL_UNSIGNED_BYTE, bgdata);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(bgdata);

	glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture when done, so we won't accidentily mess up our texture.
	glActiveTexture(GL_TEXTURE0);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//------------------------------------------------------------------------------------------------------------

	glGenTextures(1, &enemyTexture);
	glBindTexture(GL_TEXTURE_2D, enemyTexture); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
												// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// load image, create texture and generate mipmaps
	int enemyWidth, enemyHeight, enemyNrChannels;
	//unsigned char *data = SOIL_load_image("../textures/wall.jpg", &width, &height, 0, SOIL_LOAD_RGB);
	//unsigned char *enemyData = stbi_load("../textures/background.jpg", &enemyWidth, &enemyHeight, &enemyNrChannels, 0);
	unsigned char *enemyData = stbi_load("../textures/coreano.png", &enemyWidth, &enemyHeight, &enemyNrChannels, 0);

	if (enemyData)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, enemyWidth, enemyHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, enemyData);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(enemyData);

	glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture when done, so we won't accidentily mess up our texture.
	glActiveTexture(GL_TEXTURE0);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	//------------------------------------------------------------------------------------------------------------

	glGenTextures(1, &gameovertexture);
	glBindTexture(GL_TEXTURE_2D, gameovertexture); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
												// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// load image, create texture and generate mipmaps
	int goWidth, goHeight, goNrChannels;
	//unsigned char *data = SOIL_load_image("../textures/wall.jpg", &width, &height, 0, SOIL_LOAD_RGB);
	//unsigned char *enemyData = stbi_load("../textures/background.jpg", &enemyWidth, &enemyHeight, &enemyNrChannels, 0);
	unsigned char *goData = stbi_load("../textures/game_over.png", &goWidth, &goHeight, &goNrChannels, 0);

	if (goData)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, goWidth, goHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, goData);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(goData);

	glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture when done, so we won't accidentily mess up our texture.
	glActiveTexture(GL_TEXTURE0);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}
