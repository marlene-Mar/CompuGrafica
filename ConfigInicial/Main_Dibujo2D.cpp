/*	Práctica 1: Dibujo de primitivas básicas 2D
	Marlene Mariana De la Cruz Padilla
	Fecha de entrega: 20 de agosto 2024
*/
#include<iostream>

//#define GLEW_STATIC

#include <GL/glew.h>

#include <GLFW/glfw3.h>

// Shaders
#include "Shader.h"

void resize(GLFWwindow* window, int width, int height);

const GLint WIDTH = 800, HEIGHT = 600;


int main() {
	glfwInit();
	//Verificaci�n de compatibilidad 
	/*glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);*/

	GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "Dibujo de Primitivas en 2D Marlene", NULL, NULL);
	glfwSetFramebufferSizeCallback(window, resize);
	
	//Verificaci�n de errores de creacion  ventana
	if (window== NULL) 
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();

		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);
	glewExperimental = GL_TRUE;

	//Verificaci�n de errores de inicializaci�n de glew

	if (GLEW_OK != glewInit()) {
		std::cout << "Failed to initialise GLEW" << std::endl;
		return EXIT_FAILURE;
	}

	// Imprimimos informacin de OpenGL del sistema
	std::cout << "> Version: " << glGetString(GL_VERSION) << std::endl;
	std::cout << "> Vendor: " << glGetString(GL_VENDOR) << std::endl;
	std::cout << "> Renderer: " << glGetString(GL_RENDERER) << std::endl;
	std::cout << "> SL Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;


	// Define las dimensiones del viewport
	//glViewport(0, 0, screenWidth, screenHeight);

    Shader ourShader("Shader/core.vs", "Shader/core.frag");

	// Set up vertex data (and buffer(s)) and attribute pointers
	float vertices[] = {
		// Positions          // Colors
		-0.93f,  0.59f, 0.0f,   0.502f, 0.251f, 0.0f,   // 0
		-0.69f,  0.56f, 0.0f,  0.502f, 0.251f, 0.0f,   // 1
		-0.72f,  0.29f, 0.0f,  1.0f,   0.647f, 0.0f,   // 2
		-0.26f, 0.5f, 0.0f,   1.0f,   0.647f, 0.0f,   // 3
		0.0f, 0.85f, 0.0f,  0.8f, 0.6f, 0.4f,   // 4
		0.22f, 0.85f, 0.0f,  0.8f, 0.6f, 0.4f,   // 5
		0.11f,  0.75f, 0.0f,  0.8f, 0.6f, 0.4f,   // 6
		0.12f,  0.35f, 0.0f,   0.5f, 0.5f, 0.5f,   // 7
		0.61f,  0.53f, 0.0f,   0.5f, 0.5f, 0.5f,   // 8
		 0.35f,  0.25f, 0.0f,   0.5f, 0.5f, 0.5f,   // 9
		 0.13f,  -0.08f, 0.0f,  0.502f, 0.251f, 0.0f,   // 10
		-0.76f,  -0.07f, 0.0f,   1.0f,   0.647f, 0.0f,   // 11
		
	};
	
	//Se dividio la figura en puros triángulos y se fueron uniendo cada una de los vertices
	unsigned int indices[] = {  // note that we start from 0!
		0,1,2,
		1,3,11,
		3,4,6,
		4,5,6,
		5,6,8,
		6,7,8,
		3,6,7,
		11,3,7,
		7,10,11,
		7,8,9,
	};


	GLuint VBO, VAO,EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	// Enlazar  Vertex Array Object
	glBindVertexArray(VAO);

	//2.- Copiamos nuestros arreglo de vertices en un buffer de vertices para que OpenGL lo use
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// 3.Copiamos nuestro arreglo de indices en  un elemento del buffer para que OpenGL lo use
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// 4. Despues colocamos las caracteristicas de los vertices

	//Posicion
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid *)0);
	glEnableVertexAttribArray(0);

	//Color
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid *)(3*sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);


	glBindVertexArray(0); // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs)


	
	while (!glfwWindowShouldClose(window))
	{
		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();

		// Render
		// Clear the colorbuffer
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);


		// Draw our first triangle
		ourShader.Use();
		glBindVertexArray(VAO);

		/*
		Se dibujaron los verticies en un tamaño 7 para que fueran visibles,
		para posteriormente se dibujarán todos los triángulos a la vez
		*/
		glPointSize(7);
		glDrawArrays(GL_POINTS,0,12);	
		glDrawElements(GL_TRIANGLES, 30, GL_UNSIGNED_INT, 0); 

		//glDrawArrays(GL_LINES, 4, 6); 
		
		//glDrawArrays(GL_LINE_LOOP,0,4);
		//glDrawArrays(GL_POLYGON,3,9); //PECHO
		//glDrawArrays(GL_TRIANGLES, 4, 6);
        //glDrawElements(GL_TRIANGLES, 3,GL_UNSIGNED_INT,0);

        //glBindVertexArray(0);
    
		// Swap the screen buffers
		glfwSwapBuffers(window);
	}



	glfwTerminate();
	return EXIT_SUCCESS;
}

void resize(GLFWwindow* window, int width, int height)
{
	// Set the Viewport to the size of the created window
	glViewport(0, 0, width, height);
	//glViewport(0, 0, screenWidth, screenHeight);
}