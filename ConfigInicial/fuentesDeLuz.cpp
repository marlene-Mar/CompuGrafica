/*Práctica 8: Fuentes de luz 
* Marlene Mariana De la Cruz Padilla
* Fecha de entrega: 6 de octubre de 2024
*/

#include <iostream>
#include <cmath>

// GLEW
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// Other Libs
#include "stb_image.h"

// GLM Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//Load Models
#include "SOIL2/SOIL2.h"


// Other includes
#include "Shader.h"
#include "Camera.h"
#include "Model.h"

// Function prototypes
void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow *window, double xPos, double yPos);
void DoMovement();

// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Camera
Camera  camera(glm::vec3(0.0f, 0.0f, 3.0f));
GLfloat lastX = WIDTH / 2.0;
GLfloat lastY = HEIGHT / 2.0;
bool keys[1024];
bool firstMouse = true;
// Light attributes
glm::vec3 lightPos(0.0f, 0.0f, 0.0f);
bool active;

// Positions of the point lights
glm::vec3 pointLightPositions[] = {
	glm::vec3(0.0f,0.0f, 0.0f),
	glm::vec3(0.0f,0.0f, 0.0f),
	glm::vec3(0.0f,0.0f,  0.0f),
	glm::vec3(0.0f,0.0f, 0.0f)
};

float vertices[] = {
	 -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	   -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	   -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

	   -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
	   -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
	   -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

	   -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

	   -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	   -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	   -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

	   -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	   -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	   -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
};

//Para el cambio de colores en las luces 


//Luz 1
float R0 = 1.0f; float G0 = 0.5f; float B0 = 0.5;
//Luz 2
float R1 = 1.0f; float G1 = 0.5f; float B1 = 0.5;
//Luz 3
float R2 = 1.0f; float G2 = 0.5f; float B2 = 0.5;
//Luz 4
float R3 = 1.0f; float G3 = 0.5f; float B3 = 0.5;

//
float con = 0.2f; float lin = 0.2f; float qua = 0.2f;

// Paso de ajuste de la especular
float specularStep = 0.1f;

glm::vec3 Light1 = glm::vec3(0);


// Deltatime
GLfloat deltaTime = 0.0f;	// Time between current frame and last frame
GLfloat lastFrame = 0.0f;  	// Time of last frame

int main()
{
	// Init GLFW
	glfwInit();
	// Set all the required options for GLFW
	/*glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);*/

	// Create a GLFWwindow object that we can use for GLFW's functions
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Fuentes de luz Marlene", nullptr, nullptr);

	if (nullptr == window)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();

		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);

	glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);

	// Set the required callback functions
	glfwSetKeyCallback(window, KeyCallback);
	glfwSetCursorPosCallback(window, MouseCallback);

	// GLFW Options
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
	glewExperimental = GL_TRUE;
	// Initialize GLEW to setup the OpenGL Function pointers
	if (GLEW_OK != glewInit())
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return EXIT_FAILURE;
	}

	// Define the viewport dimensions
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);



	Shader lightingShader("Shader/lighting.vs", "Shader/lighting.frag");
	Shader lampShader("Shader/lamp.vs", "Shader/lamp.frag");
	

	Model Dog((char*)"Models/ball.obj");
	Model piso((char*)"Models/piso.obj");
	//Replicando modelos de la practica 5
	Model red_dog((char*)"Models/RedDog.obj");
	Model dog2((char*)"Models/13041_Beagle_v1_L1.obj"); //Ruta de perro2 3D
	Model casa((char*)"Models/farmhouse_obj.obj"); //Ruta de la casa 3D
	Model tronco((char*)"Models/trunk wood.obj"); //Ruta del tronco
	Model pasto((char*)"Models/10450_Rectangular_Grass_Patch_v1_iterations-2.obj"); //Ruta del pasto 3D
	Model columpio((char*)"Models/columpio.obj"); //Ruta del columpio 3D
	Model pozo((char*)"Models/well.obj"); //Ruta del pozo 3D
	//Modelos de las luces
	Model faro((char*)"Models/objLamp.obj"); //Ruta del faro 3D
	Model foco((char*)"Models/0064_LED_Bulb_01_OBJ.obj");	//Ruta de foco 3D
	Model xo((char*)"Models/xo.obj");	//Ruta de vela 3D
	Model disco((char*)"Models/webtrcc.obj");	//Ruta de disco ball 3D

	// First, set the container's VAO (and VBO)
	GLuint VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	// normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// Set texture units
	lightingShader.Use();
	glUniform1i(glGetUniformLocation(lightingShader.Program, "Material.diffuse"), 0);
	glUniform1i(glGetUniformLocation(lightingShader.Program, "Naterial.specular"), 1);

	glm::mat4 projection = glm::perspective(camera.GetZoom(), (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.1f, 100.0f);

	// Game loop
	while (!glfwWindowShouldClose(window))
	{

		// Calculate deltatime of current frame
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();
		DoMovement();

		// Clear the colorbuffer
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// OpenGL options
		glEnable(GL_DEPTH_TEST);



		//Load Model


		// Use cooresponding shader when setting uniforms/drawing objects
		lightingShader.Use();

		glUniform1i(glGetUniformLocation(lightingShader.Program, "diffuse"), 0);
		//glUniform1i(glGetUniformLocation(lightingShader.Program, "specular"),1);

		GLint viewPosLoc = glGetUniformLocation(lightingShader.Program, "viewPos");
		glUniform3f(viewPosLoc, camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);


		// Directional light
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.direction"), -0.2f, -1.0f, -0.3f);
		/*glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.ambient"),0.3f,0.3f,0.3f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.diffuse"), 0.3f, 0.3f, 0.3f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.specular"),0.1f, 0.1f, 0.1f);*/

		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.ambient"), 0.3f, 0.3f, 0.3f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.diffuse"), 0.3f, 0.3f, 0.3f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.specular"), 0.1f, 0.1f, 0.1f);



		// Point light 1
		glm::vec3 lightColor;
		lightColor.x = abs(sin(glfwGetTime() * Light1.x));
		lightColor.y = abs(sin(glfwGetTime() * Light1.y));
		lightColor.z = sin(glfwGetTime() * Light1.z);


		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].position"), pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].ambient"), lightColor.x, lightColor.y, lightColor.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].diffuse"), lightColor.x, lightColor.y, lightColor.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].specular"), R0, G0, B0); //Cambio de color en la luz 1
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].constant"), con);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].linear"), lin);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].quadratic"), qua);



		// Point light 2
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].position"), pointLightPositions[1].x, pointLightPositions[1].y, pointLightPositions[1].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].ambient"), 0.05f, 0.05f, 0.05f); //Varia colores
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].diffuse"), 0.0f, 0.0f, 0.0f);	 //varia colores
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].specular"), R1, G1, B1);	//Cambio de color en la luz 2
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].constant"), con);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].linear"), lin);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].quadratic"), qua);

		// Point light 3
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].position"), pointLightPositions[2].x, pointLightPositions[2].y, pointLightPositions[2].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].ambient"), 0.0f, 0.0f, 0.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].diffuse"), 0.0f, 0.0f, 0.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].specular"), R2, G2, B2); //Cambio de color luz 3
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[2].constant"), con);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[2].linear"), lin);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[2].quadratic"), qua);

		// Point light 4
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].position"), pointLightPositions[3].x, pointLightPositions[3].y, pointLightPositions[3].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].ambient"), 0.0f, 0.0f, 0.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].diffuse"), 0.0f, 0.0f, 0.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].specular"), R3, G3, B3); //Cambio de color luz 4
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[3].constant"), con);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[3].linear"), lin);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[3].quadratic"), qua);

		// SpotLight
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.position"), camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.direction"), camera.GetFront().x, camera.GetFront().y, camera.GetFront().z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.ambient"), 0.2f, 0.2f, 0.8f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.diffuse"), 0.2f, 0.2f, 0.8f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.specular"), 0.0f, 0.0f, 0.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.linear"), 0.3f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.quadratic"), 0.7f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.cutOff"), glm::cos(glm::radians(12.0f)));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.outerCutOff"), glm::cos(glm::radians(18.0f)));

		// Set material properties
		glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 5.0f);

		// Create camera transformations
		glm::mat4 view;
		view = camera.GetViewMatrix();

		// Get the uniform locations
		GLint modelLoc = glGetUniformLocation(lightingShader.Program, "model");
		GLint viewLoc = glGetUniformLocation(lightingShader.Program, "view");
		GLint projLoc = glGetUniformLocation(lightingShader.Program, "projection");
	
		// Pass the matrices to the shader
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));


		glm::mat4 model(1);



		//Carga de modelo 
		//view = camera.GetViewMatrix();	
		/*model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		piso.Draw(lightingShader);*/


		//Dibujo de modelos 

	   //Dibujo de modelo de casa
		glm::mat4 modelCasa(1);
		modelCasa = glm::rotate(modelCasa, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f)); //Rotación de casa
		modelCasa = glm::translate(modelCasa, glm::vec3(0.0f, 0.0f, 0.0f));  // Traslada casa a otra posición
		modelCasa = glm::scale(modelCasa, glm::vec3(0.2f, 0.2f, 0.2f));       // Escala el modelo 
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelCasa));
		casa.Draw(lightingShader);


		//Dibujo del modelo Perro 1 del previo
		glm::mat4 modelDog1(1);
		modelDog1 = glm::translate(modelDog1, glm::vec3(1.3f, 0.65f, 2.5f));
		modelDog1 = glm::scale(modelDog1, glm::vec3(1.0f, 1.0f, 1.0f));
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelDog1));
		red_dog.Draw(lightingShader); //Se dibuja el perro

		//Dibujo del modelo perro 2
		glm::mat4 modelDog2(1);
		modelDog2 = glm::rotate(modelDog2, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 1.0f)); //rotación de perro 2
		modelDog2 = glm::translate(modelDog2, glm::vec3(1.0f, 2.5f, 0.28f));  // Traslada dog2 a otra posición
		modelDog2 = glm::scale(modelDog2, glm::vec3(0.01f, 0.01f, 0.01f));       // Escala el modelo
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelDog2));
		dog2.Draw(lightingShader);

		//Dibujo del modelo de pasto 
		glm::mat4 modelPasto(1);
		modelPasto = glm::rotate(modelPasto, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f)); //rotación del pasto
		modelPasto = glm::translate(modelPasto, glm::vec3(0.0f, -1.0f, -0.5f));  // Traslada pasto a otra posición
		modelPasto = glm::scale(modelPasto, glm::vec3(0.06f, 0.06f, 0.06f));       // Escala el modelo
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelPasto));
		pasto.Draw(lightingShader);

		//Dibujo del modelo del tronco 
		glm::mat4 modelTroco(1);
		modelTroco = glm::translate(modelTroco, glm::vec3(2.3f, 0.0f, 0.0f));  // Traslada tronco 1 a otra posición
		modelTroco = glm::scale(modelTroco, glm::vec3(1.5f, 1.5f, 1.5f));       // Escala el modelo
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelTroco));
		tronco.Draw(lightingShader);

		glm::mat4 modelTroco2(1);
		modelTroco2 = glm::translate(modelTroco2, glm::vec3(2.7f, 0.0f, 0.0f));  // Traslada tronco 2 a otra posición
		modelTroco2 = glm::scale(modelTroco2, glm::vec3(1.5f, 1.5f, 1.5f));       // Escala el modelo
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelTroco2));
		tronco.Draw(lightingShader);

		glm::mat4 modelTroco3(1);
		modelTroco3 = glm::translate(modelTroco3, glm::vec3(2.5f, 0.3f, 0.0f));  // Traslada tronco 3 a otra posición
		modelTroco3 = glm::scale(modelTroco3, glm::vec3(1.5f, 1.5f, 1.5f));       // Escala el modelo
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelTroco3));
		tronco.Draw(lightingShader);

		//Dibujo del modelo del columpio
		glm::mat4 modelCol(1);
		modelCol = glm::translate(modelCol, glm::vec3(5.5f, 0.0f, 0.5f));  // Traslada columpio a otra posición
		modelCol = glm::scale(modelCol, glm::vec3(0.00035f, 0.00035f, 0.00035f));       // Escala el modelo
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelCol));
		columpio.Draw(lightingShader);

		//Dibujo del modelo del pozo
		glm::mat4 modelPozo(1);
		modelPozo = glm::rotate(modelPozo, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f)); //rotación del pasto
		modelPozo = glm::translate(modelPozo, glm::vec3(-5.0f, 5.0f, 1.2f));  // Traslada pozo a otra posición
		modelPozo = glm::scale(modelPozo, glm::vec3(0.1f, 0.1f, 0.1f));       // Escala el modelo
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelPozo));
		pozo.Draw(lightingShader);

		//glBindVertexArray(0);


		////glEnable(GL_BLEND);//Avtiva la funcionalidad para trabajar el canal alfa
		//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		//glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		//glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);
		//Dog.Draw(lightingShader);
		////glDisable(GL_BLEND);  //Desactiva el canal alfa 
		//glBindVertexArray(0);



		// Also draw the lamp object, again binding the appropriate shader
		lampShader.Use();
		// Get location objects for the matrices on the lamp shader (these could be different on a different shader)
		modelLoc = glGetUniformLocation(lampShader.Program, "model");
		viewLoc = glGetUniformLocation(lampShader.Program, "view");
		projLoc = glGetUniformLocation(lampShader.Program, "projection");

		// Set matrices
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(5.0f, 6.0f, 1.0f));
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.8f)); // Make it a smaller cube
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));


		// Draw the light object (using light's vertex attributes)
		for (GLuint i = 0; i < 4; i++)
		{
			model = glm::mat4(1);
			//Se translada la luz 1 a otra posición
			model = glm::translate(model, pointLightPositions[0] = glm::vec3(1.2f, 0.3f, 3.0f));
			model = glm::scale(model, glm::vec3(0.5f)); // Make it a smaller cube
			model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f)); //Rotación del foco
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			glBindVertexArray(VAO);
			foco.Draw(lightingShader);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		for (GLuint i = 0; i < 4; i++)
		{
			model = glm::mat4(1);
			//Se translada la luz 2 a otra posición
			model = glm::translate(model, pointLightPositions[1] = glm::vec3(7.5f, 0.0f, -5.0f));
			model = glm::scale(model, glm::vec3(1.0f)); // Make it a smaller cube
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			glBindVertexArray(VAO);
			faro.Draw(lightingShader);
			//glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		for (GLuint i = 0; i < 4; i++)
		{
			model = glm::mat4(1);
			//Se translada la luz 1 a otra posición
			model = glm::translate(model, pointLightPositions[2] = glm::vec3(-5.0f, 3.0f, 3.0f));
			model = glm::scale(model, glm::vec3(0.5f)); // Make it a smaller cube
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			glBindVertexArray(VAO);
			disco.Draw(lightingShader);
			//glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		for (GLuint i = 0; i < 4; i++)
		{
			model = glm::mat4(1);
			//Se translada la luz 2 a otra posición
			model = glm::translate(model, pointLightPositions[3] = glm::vec3(-5.0f, 0.0f, -2.5f));
			model = glm::scale(model, glm::vec3(0.25f)); // Make it a smaller cube
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			glBindVertexArray(VAO);
			xo.Draw(lightingShader);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		glBindVertexArray(0);



		// Swap the screen buffers
		glfwSwapBuffers(window);
	}


	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();



	return 0;
}

// Moves/alters the camera positions based on user input
void DoMovement()
{

	// Camera controls
	if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP])
	{
		camera.ProcessKeyboard(FORWARD, deltaTime);

	}

	if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN])
	{
		camera.ProcessKeyboard(BACKWARD, deltaTime);


	}

	if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT])
	{
		camera.ProcessKeyboard(LEFT, deltaTime);


	}

	if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT])
	{
		camera.ProcessKeyboard(RIGHT, deltaTime);


	}

	/*if (keys[GLFW_KEY_T])
	{
		pointLightPositions[0].x += 0.001f;
	}
	if (keys[GLFW_KEY_G])
	{
		pointLightPositions[0].x -= 0.001f;
	}

	if (keys[GLFW_KEY_Y])
	{
		pointLightPositions[0].y += 0.001f;
	}

	if (keys[GLFW_KEY_H])
	{
		pointLightPositions[0].y -= 0.001f;
	}
	if (keys[GLFW_KEY_U])
	{
		pointLightPositions[0].z -= 0.001f;
	}
	if (keys[GLFW_KEY_J])
	{
		pointLightPositions[0].z += 0.001f;
	}*/

	// Cambios para Point light 1 - Focos
	if (keys[GLFW_KEY_Z]){
		R0 += 0.01f;
		if (R0 > 5.0f) R0 = 5.0f;  
	}
	if (keys[GLFW_KEY_X]){
		R0 -= 0.01f;
		if (R0 < 0.0f) R0 = 0.0f;  
	}
	if (keys[GLFW_KEY_C]){
		G0 += 0.01f;
		if (G0 > 5.0f) G0 = 5.0f;  
	}
	if (keys[GLFW_KEY_V]){
		G0 -= 0.01f;
		if (G0 < 0.0f) G0 = 0.0f;
	}
	if (keys[GLFW_KEY_B]){
		B0 += 0.01f;
		if (B0 > 3.0f) B0 = 3.0f;  
	}
	if (keys[GLFW_KEY_N]){
		B0 -= 0.01f;
		if (B0 < 0.0f) B0 = 0.0f;  
	}


	//Cambios para Point light 2 - Faro
	if (keys[GLFW_KEY_0]){
		R1 += 0.01f;
		if (R1 > 5.0f) R1 = 5.0f;
	}
	if (keys[GLFW_KEY_F]){
		R1 -= 0.01f;
		if (R1 < 0.0f) R1 = 0.0f;
	}
	if (keys[GLFW_KEY_G]){
		G1 += 0.01f;
		if (G1 > 5.0f) G1 = 5.0f;
	}
	if (keys[GLFW_KEY_H]){
		G1 -= 0.01f;
		if (G1 < 0.0f) G1 = 0.0f;
	}
	if (keys[GLFW_KEY_J]){
		B1 += 0.01f;
		if (B1 > 5.0f) B1 = 5.0f;
	}
	if (keys[GLFW_KEY_K]){
		B1 -= 0.01f;
		if (B1 < 0.0f) B1 = 0.0f;
	}
	//Cambios para Point light 3 - Disco ballw

	if (keys[GLFW_KEY_R]){
		R2 += 0.01f;
		if (R2 > 5.0f) R2 = 5.0f;
	}
	if (keys[GLFW_KEY_T]){
		R2 -= 0.01f;
		if (R2 < 0.0f) R2 = 0.0f;
	}
	if (keys[GLFW_KEY_Y]){
		G2 += 0.01f;
		if (G2 > 5.0f) G2 = 5.0f;
	}
	if (keys[GLFW_KEY_U]){
		G2 -= 0.01f;
		if (G2 < 0.0f) G2 = 0.0f;
	}
	if (keys[GLFW_KEY_I]){
		B2 += 0.01f;
		if (B2 > 5.0f) B2 = 5.0f;
	}
	if (keys[GLFW_KEY_O]){
		B2 -= 0.01f;
		if (B2 < 0.0f) B2 = 0.0f;
	}

	// Cambios para Point light 4
	if (keys[GLFW_KEY_1]){
		R3 += 0.01f;
		if (R3 > 5.0f) R3 = 5.0f;
	}
	if (keys[GLFW_KEY_2]){
		R3 -= 0.01f;
		if (R3 < 0.0f) R3 = 0.0f;
	}
	if (keys[GLFW_KEY_3]){
		G3 += 0.01f;
		if (G3 > 5.0f) G3 = 5.0f;
	}
	if (keys[GLFW_KEY_4]){
		G3 -= 0.01f;
		if (G3 < 0.0f) G3 = 0.0f;
	}
	if (keys[GLFW_KEY_5]){
		B3 += 0.01f;
		if (B3 > 5.0f) B3 = 5.0f;
	}
	if (keys[GLFW_KEY_6]){
		B3 -= 0.01f;
		if (B3 < 0.0f) B3 = 0.0f;
	}

	//Cambios para Directional light

	if (keys[GLFW_KEY_M]){
		con += 0.01f;
		if (con > 5.0f) con = 5.0f;
	}
	if (keys[GLFW_KEY_L]){
		con -= 0.01f;
		if (con < 0.0f) con = 0.0f;
	}
	if (keys[GLFW_KEY_P]){
		lin += 0.01f;
		if (lin > 5.0f) lin = 5.0f;
	}
	if (keys[GLFW_KEY_7]){
		lin -= 0.01f;
		if (lin < 0.0f) lin = 0.0f;
	}
	if (keys[GLFW_KEY_8]){
		qua += 0.01f;
		if (qua > 3.0f) qua = 3.0f;
	}
	if (keys[GLFW_KEY_9]){
		qua -= 0.01f;
		if (qua < 0.0f) qua = 0.0f;
	}
	
}

// Is called whenever a key is pressed/released via GLFW
void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mode)
{
	if (GLFW_KEY_ESCAPE == key && GLFW_PRESS == action)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			keys[key] = true;
		}
		else if (action == GLFW_RELEASE)
		{
			keys[key] = false;
		}
	}

	if (keys[GLFW_KEY_SPACE])
	{
		active = !active;
		if (active)
		{
			Light1 = glm::vec3(1.0f, 1.0f, 0.0f);
		}
		else
		{
			Light1 = glm::vec3(0);//Cuado es solo un valor en los 3 vectores pueden dejar solo una componente
		}
	}
}

void MouseCallback(GLFWwindow *window, double xPos, double yPos)
{
	if (firstMouse)
	{
		lastX = xPos;
		lastY = yPos;
		firstMouse = false;
	}

	GLfloat xOffset = xPos - lastX;
	GLfloat yOffset = lastY - yPos;  // Reversed since y-coordinates go from bottom to left

	lastX = xPos;
	lastY = yPos;

	camera.ProcessMouseMovement(xOffset, yOffset);
}