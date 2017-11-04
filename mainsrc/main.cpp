//-------------------------------------------------------------
// CAP5705 University of Florida
// copyright 2017 Corey Toler-Franklin
// main.cpp for environment mapping program
//-------------------------------------------------------------

#define GLEW_STATIC
#include <GL/glew.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <SOIL.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <iostream>
#include <string>
#include <vector>
#include "Shader.h"
#include "Camera.h"
#include "Geometry.h"
#include "MetaballFactory.h"

//----------------------------------------------
// windows
//----------------------------------------------
int windowHeight = 1200;
int windowLength = 1200;

//----------------------------------------------
// Callback functions
//----------------------------------------------
void KeyCallback(GLFWwindow *window, int key, int scan, int act, int mode);
void MouseCallback(GLFWwindow *window, double xPosition, double yPosition);
void ScrollCallback(GLFWwindow *window, double deltaX, double deltaY);
void KeyMovement(void);
bool keys[1024];
GLfloat lastX = windowLength / 2, lastY = windowHeight / 2;
bool firstMouse = true;

//----------------------------------------------
// cube maps
//----------------------------------------------
std::vector<glm::vec3> InitCubeCoords(void);
GLuint GenerateCubeMap(void);
GLuint LoadTextures(char *textPath);

// we modified the input order of the images so the images on sky box
// look right.
std::vector<const char *> cmapFiles = {"../../data/images/negx.jpg",
                                       "../../data/images/posx.jpg",
									   "../../data/images/posy.jpg",
									   "../../data/images/negy.jpg",
									   "../../data/images/negz.jpg",
                                       "../../data/images/posz.jpg"};

//----------------------------------------------
// Camera
//----------------------------------------------
Camera camera(glm::vec3(0.0f, 0.0f, 20.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f);
GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

//----------------------------------------------------
// Main entry point
// TO DO - Manage all OpenGL draw calls here
// Create shader programs
//          e.g. Shader shader("Shaders/shader.vert", "Shaders/shader.frag");
// Load and use shader programs
//          e.g. shader.Use();
// Get and set uniform variables
//          e.g. glGetUniformLocation(shader.Program, "model");
// Set triangles to be drawn
// Bind and draw cubemap textures
//----------------------------------------------------
int main()
{

	//instantiating the window
	//---------------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow *window = glfwCreateWindow(windowLength, windowHeight, "Environment Mapping", nullptr, nullptr);
	if (window == nullptr)
	{
		std::cout << "Could not create the window." << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// associate callbacks
	//--------------------------------------------------------------
	glfwSetKeyCallback(window, KeyCallback);
	glfwSetCursorPosCallback(window, MouseCallback);
	glfwSetScrollCallback(window, ScrollCallback);

	// set the input mode
	//---------------------------------------------------------------
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Init GLEW
	//--------------------------------------------------------------
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		std::cout << "GLEW not initialized." << std::endl;
		return -1;
	}

	// OpenGL Window
	int widthBuff, heightBuff;
	glfwGetFramebufferSize(window, &widthBuff, &heightBuff);
	glViewport(0, 0, widthBuff, heightBuff);

	// intialized a big sky box
	float cubeCoord[] = {
	-100.0f,  100.0f, -100.0f,
	-100.0f, -100.0f, -100.0f,
	100.0f, -100.0f, -100.0f,
	100.0f, -100.0f, -100.0f,
	100.0f,  100.0f, -100.0f,
	-100.0f,  100.0f, -100.0f,

	-100.0f, -100.0f,  100.0f,
	-100.0f, -100.0f, -100.0f,
	-100.0f,  100.0f, -100.0f,
	-100.0f,  100.0f, -100.0f,
	-100.0f,  100.0f,  100.0f,
	-100.0f, -100.0f,  100.0f,

	100.0f, -100.0f, -100.0f,
	100.0f, -100.0f,  100.0f,
	100.0f,  100.0f,  100.0f,
	100.0f,  100.0f,  100.0f,
	100.0f,  100.0f, -100.0f,
	100.0f, -100.0f, -100.0f,

	-100.0f, -100.0f,  100.0f,
	-100.0f,  100.0f,  100.0f,
	100.0f,  100.0f,  100.0f,
	100.0f,  100.0f,  100.0f,
	100.0f, -100.0f,  100.0f,
	-100.0f, -100.0f,  100.0f,

	-100.0f,  100.0f, -100.0f,
	100.0f,  100.0f, -100.0f,
	100.0f,  100.0f,  100.0f,
	100.0f,  100.0f,  100.0f,
	-100.0f,  100.0f,  100.0f,
	-100.0f,  100.0f, -100.0f,

	-100.0f, -100.0f, -100.0f,
	-100.0f, -100.0f,  100.0f,
	100.0f, -100.0f, -100.0f,
	100.0f, -100.0f, -100.0f,
	-100.0f, -100.0f,  100.0f,
	100.0f, -100.0f,  100.0f
	};

	// The teapot
	// Intialize a instance form Geomotry class to read in the obj file of the teapot
	Geometry teapot;
	teapot.Initialize("../../data/models/teapot.obj");

	// teapot VAO prep and manipulation
	GLuint potVAO;
	glGenVertexArrays(1, &potVAO);
	//link shaders
	Shader potShader("Shaders/reflection.vert", "Shaders/reflection.frag");
	// the transform matrix translate from canonical to world view.
	glm::mat4 model(glm::mat3(10.0f));
	model = glm::translate<float>(model, glm::vec3(0.0f, 0.0f, 0.0f));

	// the environment box VAO VBO prep and manipulation
	GLuint VAO, VBO;

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeCoord), cubeCoord, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// link shaders
	Shader cubeShader("Shaders/environment.vert", "Shaders/environment.frag");
	GLuint cubeTex = GenerateCubeMap();


	//game loop, as long as window is open
	while (!glfwWindowShouldClose(window))
	{
		//deltaTime is used to calculate camera movement frame independently
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Render
		glm::mat4 ortho = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, 0.1f, 300.0f);
		// we multiply the field of view by 100 to capture the broadest sight.
		glm::mat4 persp = glm::perspective(100 * glm::radians(camera.zoomfactor), (float)widthBuff / (float)heightBuff, 0.1f, 300.0f);
		glm::mat4 projection = persp;

		// enable depth difference between teapot and envrionment box,
		// so they do not block each other in viewing.
		glEnable(GL_DEPTH_TEST);
		glClear(GL_DEPTH_BUFFER_BIT);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// the tranfrom matrix from world to camera view.
		glm::mat4 view = camera.viewMat();
		glm::mat4 fixView = glm::mat4(glm::mat3(view));

		// set up the teapot shader and draw the tea pot
		potShader.Use();
		glUniformMatrix4fv(glGetUniformLocation(potShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(glGetUniformLocation(potShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(potShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
		glUniform3fv(glGetUniformLocation(potShader.Program, "cameraPos"), 1, glm::value_ptr(camera.pos));
		glBindVertexArray(potVAO);
		// 8 is passed in as vertices and 9 as normals
		teapot.Draw(8, 9);
		// free the binding
		glBindVertexArray(0);

		// set up the environment box and draw it
		glDepthFunc(GL_LEQUAL);
		cubeShader.Use();
		glUniformMatrix4fv(glGetUniformLocation(cubeShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(fixView));
		glUniformMatrix4fv(glGetUniformLocation(cubeShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubeTex);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);

		glfwPollEvents();
		KeyMovement();

		// reset
		glDepthFunc(GL_LESS);

		// swap buffers
		glfwSwapBuffers(window);
	}

	glDeleteVertexArrays(1, &potVAO);
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	// end process
	glfwTerminate();

	// return
	return 0;
}

//=====================================================================
// Cameras and callbacks
//=====================================================================
void KeyCallback(GLFWwindow *window, int key, int scan, int act, int mode)
{
	//escape with escape key
	if (key == GLFW_KEY_ESCAPE && act == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	if (key == GLFW_KEY_UP && act == GLFW_PRESS)
	{
		camera.pitch += 10.0f;
	}

	if (key == GLFW_KEY_DOWN && act == GLFW_PRESS)
	{
		camera.pitch -= 10.0f;
	}

	if (key == GLFW_KEY_LEFT && act == GLFW_PRESS)
	{
		camera.yaw -= 10.0f;
	}

	if (key == GLFW_KEY_RIGHT && act == GLFW_PRESS)
	{
		camera.yaw += 10.0f;
	}
	//updating keys table
	if (act == GLFW_PRESS)
		keys[key] = true;
	else if (act == GLFW_RELEASE)
		keys[key] = false;
}
void MouseCallback(GLFWwindow *window, double xPosition, double yPosition)
{
	//first time being encountered
	if (firstMouse)
	{
		lastX = xPosition;
		lastY = yPosition;
		firstMouse = false;
	}

	//finding change
	GLfloat deltaX = xPosition - lastX;
	GLfloat deltaY = lastY - yPosition;

	//updating last coords
	lastX = xPosition;
	lastY = yPosition;

	camera.mouseIn(deltaX, deltaY);
}
void ScrollCallback(GLFWwindow *window, double deltaX, double deltaY)
{
	camera.mouseScroll(deltaY);
}
void KeyMovement()
{
	//simple forwards, backwards and strafe
	//handled by camera object
	if (keys[GLFW_KEY_W])
		camera.keyIn(FORWARD, deltaTime);
	if (keys[GLFW_KEY_S])
		camera.keyIn(BACKWARD, deltaTime);
	if (keys[GLFW_KEY_A])
		camera.keyIn(LEFT, deltaTime);
	if (keys[GLFW_KEY_D])
		camera.keyIn(RIGHT, deltaTime);
	if (keys[GLFW_KEY_SPACE])
		camera.keyIn(UP, deltaTime);
	if (keys[GLFW_KEY_LEFT_CONTROL])
		camera.keyIn(DOWN, deltaTime);
}

//=====================================================================
// TO DO:
// Set-up Cube Map
// 1. Generate the texture id and bind it
//    Use  glGenTextures and glBindTexture for  GL_TEXTURE_CUBE_MAP
// 2. Load all the cubemap textures (one for each face of the cube - cmapFiles stores these).
//    Use the SOIL API
//    e.g. unsigned char* image = SOIL_load_image(faceLocations[i], &width, &height, 0, SOIL_LOAD_RGB);
//    Remember to free the image memory once you are done with the image.
//    e.g. SOIL_free_image_data(image);
//    Use glTexImage2D to set the texture images
// 3. Use glTexParameteri to set the filter parameters for the GL_TEXTURE_CUBE_MAP
//=====================================================================
GLuint GenerateCubeMap(void)
{
	GLuint texID; // this should be set to the texture id
	glGenTextures(1, &texID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texID);
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

	// read in each image from the data folder and map it to each of the 
	// six faces of the cub map.
	for (int i = 0; i < 6; i++)
	{
		unsigned char *cmapData;
		int width, height, channel;
		GLenum target = GL_TEXTURE_CUBE_MAP_POSITIVE_X + i;
		cmapData = SOIL_load_image(cmapFiles[i], &width, &height, &channel, SOIL_LOAD_AUTO);
		if (cmapData) {
			glTexImage2D(target,
				0,
				GL_RGBA,
				width, height,
				0,
				GL_RGB,
				GL_UNSIGNED_BYTE,
				cmapData);
			SOIL_free_image_data(cmapData);
		}
		else {
			std::cout << "The image is not found " << cmapFiles[i] << "." << std::endl;
			SOIL_free_image_data(cmapData);
		}

		// filtering
		glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	}
	return texID;
}

// we implemented this class for a testing to draw a 2D image on to the screen.
// this is not used to load the envrionment box, because the developer will have
// declare the "targe" that requires too many parameters to be passed in this 
// function. 
GLuint LoadTextures(char * textPath)
{
	GLuint texID; // this should be set to the texture id
	glGenTextures(1, &texID);
	glBindTexture(GL_TEXTURE_2D, texID);
	// glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
	unsigned char *texData;
	int width, height, channel;
	texData = SOIL_load_image(textPath, &width, &height, 0, SOIL_LOAD_RGB);
	if (texData) {
		glTexImage2D(GL_TEXTURE_2D,
			0,
			GL_RGB,
			width, height,
			0,
			GL_RGB,
			GL_UNSIGNED_BYTE,
			texData);
		SOIL_free_image_data(texData);
	}
	else {
		std::cout << "The image is not found " << textPath << "." << std::endl;
		SOIL_free_image_data(texData);
	}
	glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	return texID;
}

//=====================================================================
// TO DO:
// Set-up Cube Coordiates
// Initialize the cubemap coordinates.
// Returns a std::vector<glm::vec3>  with the coordinates
//=====================================================================
std::vector<glm::vec3> InitCubeCoords(void)
{
	std::vector<glm::vec3> input;
	for (int i = -1; i <= 1; i += 2) {
		for (int j = -1; j <= 1; j += 2) {
			for (int k = -1; k <= 1; k += 2) {
				input.push_back(glm::vec3(i, j, k));
			}
		}
	}
	return input;
}
