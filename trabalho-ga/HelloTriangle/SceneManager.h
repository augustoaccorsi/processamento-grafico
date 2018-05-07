#pragma once

#include "Shader.h"

//rand
#include <ctime>    // For time()
#include <cstdlib>  // For srand() and rand()

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>



class SceneManager
{
public:
	SceneManager();
	~SceneManager();
	void initialize(GLuint width, GLuint height);
	void initializeGraphics();

	void addShader(string vFilename, string fFilename);

	//GLFW callbacks
	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
	static void resize(GLFWwindow* window, int width, int height);

	void do_movement();
	void render();
	void run();
	void finish();


	// scene
	void printGameOverScreen();
	void setupScene();
	void setupCamera2D();
	void setupTexture(); //apenas mostra como criar uma textura
private:

	//Screen attributes
	GLfloat xMinScr = -4.0, xMaxScr = 4.0, yMinScr = -4.0, yMaxScr = 4.0, zNearScr = -4.0, zFarScr = 4.0;

	//Current position
	GLchar head_to;

	//Position counters
	GLfloat cont_r, cont_l, cont_t, cont_b, cont;
	
	//Offset texture
	GLfloat x_off = 1.0 / 6, y_off = 1.0 / 4;

	//Position texture
	GLfloat x_tex_r = x_off, x_tex_l = 0.0, y_tex_t = 1.0, y_tex_b = 3 * y_off;

	//Move character
	GLfloat x_axis = 0.0, y_axis = 0.0;

	GLfloat topY, botY, rigX, lefX;

	//enemy
	GLfloat enemyOffset = 0.5, enemySpeed = 0.2f;
	GLfloat xMaxEnemy = 4.0, xMinEnemy = 3.5, yMaxEnemy = 3.5, yMinEnemy = 3.0;

	//GFLW window
	GLFWwindow *window;

	//our shader program
	Shader *shader;

	//scene attributes
	GLuint VAO, VAObg, VAOEnemy, VAOspace, VAOgameover;

	//Transformations - Model Matrix
	glm::mat4 model;

	//2D Camera - Projection matrix
	glm::mat4 projection;

	//Texture index
	unsigned int texture, bgtexture, enemyTexture, spaceTexture, gameovertexture;

	//points
	GLint score = 0;

	//bg offset
	GLfloat x_bgoffset = 0, bgoffset_max = 0, bgoffset_min = 0, x_maxbg = xMaxScr, x_minbg = xMinScr;
	GLfloat xmin_space = -6.0f, xmax_space = 6.0f, space_offset;


};

