// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

// Include GLEW
#include "dependente\glew\glew.h"

// Include GLFW
#include "dependente\glfw\glfw3.h"

// Include GLM
#include "dependente\glm\glm.hpp"
#include "dependente\glm\gtc\matrix_transform.hpp"
#include "dependente\glm\gtc\type_ptr.hpp"

#include <cstdlib>
#include <ctime>

#include "shader.hpp"
#include <vector>

//variables
GLFWwindow* window;
const int width = 1024, height = 1024;
float sx = 0.0f, sy = 0.0f, sz = 0;
float sx2 = 0.0f, sy2 = 0.0f, sz2 = 0;
float delta = 1.0;
glm::mat4 rotateTrans(1.0f);
//create matrices for transforms
glm::mat4 trans(1.0f);
glm::mat4 trans2(1.0f);
float i = 0.1;


struct DrawObject {
	GLuint ibo;
	unsigned int* indices;
	float sx = 0.0f, sy = 0.0f;
};


//Handling cursor position
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
	std::cout << "The mouse cursor is: " << xpos << " " << ypos;
}

//Handling left button
void mouse_callback(GLFWwindow* window, int button, int action, int mods)
{
	//std::cout << "The mouse: " << button << " " << action << " " << mods << std::endl;
	delta = 10.0f;
	glm::mat4 scaleObj(1.0f);
	glm::mat4 translate(1.0f);
	if (button == GLFW_MOUSE_BUTTON_LEFT) {
		sx += 0.1;
		sy += 0.1;
		//rotateTrans = glm::rotate(rotateTrans, delta, glm::vec3(0.0, 0.0, 1.0));
		//scaleObj = glm::scale(glm::mat4(1.0f), glm::vec3(sx, sy, sz));
		translate = glm::translate(glm::mat4(1.0f), glm::vec3(sx + 0.3, 0.0, 0.0));
		trans = rotateTrans * translate * scaleObj;
	}
	else if (button == GLFW_MOUSE_BUTTON_RIGHT) {
		sx -= 0.1;
		sy -= 0.1;
		//scaleObj = glm::scale(glm::mat4(1.0f), glm::vec3(sx, sy, sz));
		//rotateTrans = glm::rotate(rotateTrans, -delta, glm::vec3(0.0, 0.0, 1.0));
		translate = glm::translate(glm::mat4(1.0f), glm::vec3(sx + 0.3, 0.0, 0.0));
		trans = rotateTrans * translate * scaleObj;
	}
	
}

//Ex 4: Add callback for mouse button

//Ex 5: Complete callback for adjusting the viewport
void window_callback(GLFWwindow* window, int new_width, int new_height)
{
	//what should we do here?
	//std::cout << new_width << " " << new_height << std::endl;
	glViewport(0, 0, new_width, new_height);
}

int main(void)
{
	// Initialise GLFW
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		getchar();
		return -1;
	}

	// Open a window and create its OpenGL context
	window = glfwCreateWindow(width, height, "Red triangle", NULL, NULL);
	if (window == NULL) {
		fprintf(stderr, "Failed to open GLFW window.");
		getchar();
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
		return -1;
	}

	//specify the size of the rendering window
	glViewport(0, 0, width, height);

	// Dark blue background
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);


	// Create and compile our GLSL program from the shaders
	GLuint programID = LoadShaders("SimpleVertexShader.vertexshader", "SimpleFragmentShader.fragmentshader");

	float g_vertex_buffer_data[] = {
		0.5f,  0.5f, 0.0f,  // top right
		0.5f, -0.5f, 0.0f,  // bottom right
		-0.5f, -0.5f, 0.0f,  // bottom left
		-0.5f,  0.5f, 0.0f   // top left 
	};

	unsigned int indices[] = {  // note that we start from 0!
		0, 1, 3,  // first Triangle
		1, 2, 3,   // second Triangle
	};

	unsigned int indices2[] = {  // note that we start from 0!
		0, 1, 3,  // first Triangle
		1, 2, 3,   // second Triangle
	};

	// A Vertex Array Object (VAO) is an object which contains one or more Vertex Buffer Objects and is designed to store the information for a complete rendered object. 
	GLuint vbo, vao, ibo;
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ibo);

	std::vector<DrawObject> objects;

	DrawObject object1 = DrawObject();
	DrawObject object2 = DrawObject();
	srand(static_cast <unsigned> (time(0)));

	for (int i = 0; i < 5; ++i) {
		DrawObject object1 = DrawObject();
		glGenBuffers(1, &object1.ibo);
		object1.sx += static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		object1.sy += static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		std::cout << sx << "-" << sy << std::endl;
		trans = glm::translate(glm::mat4(1.0f), glm::vec3(object1.sx, object1.sy, sz)) * glm::scale(glm::mat4(1.0f), glm::vec3(0.1, 0.1, sz));
		//bind IBO
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, object1.ibo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices2), indices2, GL_STATIC_DRAW);
		objects.push_back(object1);
	}

	//bind VAO
	glBindVertexArray(vao);

	//bind VBO
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

	//bind IBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	

	//set attribute pointers
	glVertexAttribPointer(
		0,                  // attribute 0, must match the layout in the shader.
		3,                  // size of each attribute
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		3 * sizeof(float),  // stride
		(void*)0            // array buffer offset
	);
	glEnableVertexAttribArray(0);

	
	//glm::mat4 rotateTrans(1.0f);
	//rotateTrans = glm::rotate(rotateTrans, glfwGetTime(), glm::vec3(0.0, 0.0, 1.0));

	// Set a callback for handling mouse cursor position
	// Decomment for ex4
	//glfwSetCursorPosCallback(window, cursor_position_callback);
	glfwSetMouseButtonCallback(window, mouse_callback);

	//Set callback for window resizing
	glfwSetFramebufferSizeCallback(window, window_callback);

	float lastTime = 0.0;
	int count = 0;
	// Check if the window was closed
	while (!glfwWindowShouldClose(window))
	{
		count++;
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
			glfwDestroyWindow(window);
			break;
		}
		// Swap buffers
		glfwSwapBuffers(window);

		// Check for events
		glfwPollEvents();

		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT);

		// Use our shader
		glUseProgram(programID);

		/*float now = glfwGetTime();
		float delta = now - lastTime;
		lastTime = now;*/
		//rotateTrans = glm::rotate(rotateTrans, delta * 5.0f, glm::vec3(0.0, 0.0, 1.0));

		// send variables to shader
		//unsigned int rotateLoc = glGetUniformLocation(programID, "transform");
		//glUniformMatrix4fv(rotateLoc, 1, GL_FALSE, glm::value_ptr(rotateTrans));

		//bind VAO
		glBindVertexArray(vao);

		// send variables to shader

		for (auto& objectC : objects) // access by reference to avoid copying
		{
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, objectC.ibo);

			unsigned int transformLoc = glGetUniformLocation(programID, "transform");
			glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));

			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
			if (count % 10000 == 0) {
				objectC.sx -= 0.01;
				objectC.sy -= 0.01;
				std::cout << objectC.sx << "-" << objectC.sy << std::endl;
			}
			
			trans = glm::translate(glm::mat4(1.0f), glm::vec3(objectC.sx, objectC.sy, sz)) * glm::scale(glm::mat4(1.0f), glm::vec3(0.1, 0.1, sz));
		}
		
	} 
	
	// Cleanup
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ibo);
	glDeleteVertexArrays(1, &vao);
	glDeleteProgram(programID);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}


