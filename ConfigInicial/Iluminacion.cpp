/*Práctica 7: Iluminación
Marlene Mariana De la Cruz Padilla
Fecha de entrega:29 de septiembre*/

// Std. Includes
#include <string>

// GLEW
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// GL includes
#include "Shader.h"
#include "Camera.h"
#include "Model.h"

// GLM Mathemtics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Other Libs
#include "SOIL2/SOIL2.h"
#include "stb_image.h"

// Properties
const GLuint WIDTH = 1200, HEIGHT = 1000;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Function prototypes
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow* window, double xPos, double yPos);
void DoMovement();

//Para el control de dia/noche
float moonRotate = 0.0f;
float sunRotate = 0.0f;
bool moveMoon = false;
bool moveSun = true;

//Radio para rotar 180° a la luna y al sol
float radius = 15.0f;

// Camera
Camera camera(glm::vec3(0.0f, 0.0f, 0.0f));
bool keys[1024];
GLfloat lastX = 400, lastY = 300;
bool firstMouse = true;


// Light attributes
glm::vec3 lightPos(radius* cos(glm::radians(sunRotate)), radius* sin(glm::radians(sunRotate)), 0.0f);
glm::vec3 newLightPos(radius* cos(glm::radians(moonRotate)), radius* sin(glm::radians(moonRotate)), 0.0f);


float movelightPos = 0.0f;
GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

float rot = 0.0f;
bool activanim = false;

int main()
{
    // Init GLFW
    glfwInit();
    // Set all the required options for GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    // Create a GLFWwindow object that we can use for GLFW's functions
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Materiales e Iluminacion Marlene", nullptr, nullptr);

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
    //glfwSetInputMode( window, GLFW_CURSOR, GLFW_CURSOR_DISABLED );

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

    // OpenGL options
    glEnable(GL_DEPTH_TEST);

    // Setup and compile our shaders
    Shader shader("Shader/modelLoading.vs", "Shader/modelLoading.frag");
    Shader lampshader("Shader/lamp.vs", "Shader/lamp.frag");
    Shader lightingShader("Shader/lighting.vs", "Shader/lighting.frag");



    // Load models 
    //Replicando modelos de la practica 5
    Model red_dog((char*)"Models/RedDog.obj");
    Model dog2((char*)"Models/13041_Beagle_v1_L1.obj"); //Ruta de perro2 3D
    Model casa((char*)"Models/farmhouse_obj.obj"); //Ruta de la casa 3D
    Model tronco((char*)"Models/trunk wood.obj"); //Ruta del tronco
    Model pasto((char*)"Models/10450_Rectangular_Grass_Patch_v1_iterations-2.obj"); //Ruta del pasto 3D
    Model columpio((char*)"Models/Obj.obj"); //Ruta del columpio 3D
    Model pozo((char*)"Models/well.obj"); //Ruta del pozo 3D

    //Para cargar los modelos de sol y luna
    Model luna((char*)"Models/Moon 2K.obj"); //Ruta de la luna 3D
    Model sol((char*)"Models/objStar.obj"); //Ruta del sol 3D


    glm::mat4 projection = glm::perspective(camera.GetZoom(), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);

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

    // Load textures
    int textureWidth, textureHeight, nrChannels;
    unsigned char* image;

    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    
    stbi_set_flip_vertically_on_load(true);
    image = stbi_load("Models/Texture_albedo.jpg", &textureWidth, &textureHeight, &nrChannels, 0);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);
    if (image)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(image);

    // Configuración de parámetros de la textura
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);

    //Textura para la luna
    GLuint moonTexture;
    glGenTextures(1, &moonTexture);
    glBindTexture(GL_TEXTURE_2D, moonTexture);

    stbi_set_flip_vertically_on_load(true);
    image = stbi_load("Models/moon.jpg", &textureWidth, &textureHeight, &nrChannels, 0);

    if (image)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load moon texture" << std::endl;
    }
    stbi_image_free(image);

    // Configuración de parámetros de la textura
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);

    // Cargar textura para el Sol
    GLuint sunTexture;
    glGenTextures(1, &sunTexture);
    glBindTexture(GL_TEXTURE_2D, sunTexture);

    stbi_set_flip_vertically_on_load(true);
    // Cargar la textura del Sol
    image = stbi_load("Models/sun.jpg", &textureWidth, &textureHeight, &nrChannels, 0);
    if (image)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load sun texture" << std::endl;
    }
    stbi_image_free(image);

    // Configuración de parámetros de la textura del Sol
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


    // Game loop
    while (!glfwWindowShouldClose(window))
    {
        // Set frame time
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Check and call events
        glfwPollEvents();
        DoMovement();

        // Clear the colorbuffer
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //Configuración de las luces
        lightingShader.Use();
        GLint lightPosLoc = glGetUniformLocation(lightingShader.Program, "light.position");
        GLint newLightPosLoc = glGetUniformLocation(lightingShader.Program, "newLight.position");
        GLint viewPosLoc = glGetUniformLocation(lightingShader.Program, "viewPos");

        //glUniform3f(viewPosLoc, camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);

        //Confguración nueva luz
        //GLint newLightPosLoc = glGetUniformLocation(lightingShader.Program, "newLight.position");
        //glUniform3f(newLightPosLoc, newLightPos.x + movelightPos, newLightPos.y + movelightPos, newLightPos.z + movelightPos);

        // Configurar propiedades de la nueva luz
        //glUniform3f(glGetUniformLocation(lightingShader.Program, "newLight.ambient"), 0.2f, 0.2f, 0.2f);
        //glUniform3f(glGetUniformLocation(lightingShader.Program, "newLight.diffuse"), 0.5f, 0.5f, 0.5f);
        //glUniform3f(glGetUniformLocation(lightingShader.Program, "newLight.specular"), 1.0f, 1.0f, 1.0f);

        //// Set lights properties
        //glUniform3f(glGetUniformLocation(lightingShader.Program, "light.ambient"), 0.3f, 0.3f, 0.3f);
        //glUniform3f(glGetUniformLocation(lightingShader.Program, "light.diffuse"), 0.2f, 0.7f, 0.8f);
        //glUniform3f(glGetUniformLocation(lightingShader.Program, "light.specular"), 0.3f, 0.6f, 0.4f);

        // Envía las posiciones de las luces
        glUniform3f(glGetUniformLocation(lightingShader.Program, "light.position"), lightPos.x, lightPos.y, lightPos.z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "newLight.position"), newLightPos.x, newLightPos.y, newLightPos.z);

        // Envía las propiedades de las luces (ambient, diffuse, specular)

        //día
        glUniform3f(glGetUniformLocation(lightingShader.Program, "light.ambient"), 0.5f, 0.5f, 0.3f);   //luz calida
        glUniform3f(glGetUniformLocation(lightingShader.Program, "light.diffuse"), 1.0f, 0.8f, 0.6f);   //luz más intensa
        glUniform3f(glGetUniformLocation(lightingShader.Program, "light.specular"), 0.9f, 0.9f, 0.7f);  //mas reflexión
        //noche
        glUniform3f(glGetUniformLocation(lightingShader.Program, "newLight.ambient"), 0.6f, 0.6f ,0.8f);    //luz tenue y azulada
        glUniform3f(glGetUniformLocation(lightingShader.Program, "newLight.diffuse"), 0.4f, 0.34, 0.6f);    //luz difusa
        glUniform3f(glGetUniformLocation(lightingShader.Program, "newLight.specular"), 0.6f, 0.6f, 0.8f);   //reflexion suave 

        // Envía la posición de la cámara
        glUniform3f(glGetUniformLocation(lightingShader.Program, "viewPos"), camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);


        //Visualizar el día y la noche
        if (moveSun){
            // Materiales que simulan el día
            lightPos = glm::vec3(radius * cos(glm::radians(sunRotate)), radius * sin(glm::radians(sunRotate)), 0.0f);
            glUniform3f(glGetUniformLocation(lightingShader.Program, "material.ambient"), 0.5f, 0.5f, 0.4f);
            glUniform3f(glGetUniformLocation(lightingShader.Program, "material.diffuse"), 0.7f, 0.7f, 0.5f);
            glUniform3f(glGetUniformLocation(lightingShader.Program, "material.specular"), 0.8f, 0.8f, 0.6f);
            glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 32.0f);
        }
        else if (moveMoon){
            // Configuración para el día (Sol)
            newLightPos = glm::vec3(radius * cos(glm::radians(moonRotate)), radius * sin(glm::radians(moonRotate)), 0.0f);
            glUniform3f(glGetUniformLocation(lightingShader.Program, "material.ambient"), 0.1f, 0.1f, 0.2f);
            glUniform3f(glGetUniformLocation(lightingShader.Program, "material.diffuse"), 0.2f, 0.2f, 0.3f);
            glUniform3f(glGetUniformLocation(lightingShader.Program, "material.specular"), 0.3f, 0.3f, 0.4f);
            glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 16.0f);
        }

       glUniform3f(viewPosLoc, camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);

        glm::mat4 view = camera.GetViewMatrix();
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));

        // Set material properties
        /*glUniform3f(glGetUniformLocation(lightingShader.Program, "material.ambient"), 0.5f, 0.5f, 0.5f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.diffuse"), 0.7f, 0.2f, 0.4f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "material.specular"), 0.6f, 0.6f, 0.6f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 0.6f);*/

        // Draw the loaded model
        /*glm::mat4 model(1);
        model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glBindVertexArray(VAO);
        red_dog.Draw(lightingShader);*/
        //glDrawArrays(GL_TRIANGLES, 0, 36);

        //Actividad del previo: Carga de segundo modelo
        //glm::mat4 modelTronco = glm::mat4(1.0f);
        //modelTronco = glm::translate(modelTronco, glm::vec3(3.0f, 0.0f,3.0f));
        //modelTronco = glm::scale(modelTronco, glm::vec3(2.0f)); 
        //glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelTronco));
        //tronco.Draw(lightingShader);

        //Dibujo de modelos 
        
        //Dibujo de modelo de casa
        glm::mat4 modelCasa(1);
        glm::mat4 modelTemp = glm::mat4(1.0f);

        modelCasa = glm::rotate(modelCasa, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f)); //Rotación de casa
        modelCasa = glm::translate(modelCasa, glm::vec3(0.0f, 0.0f, 0.0f));  // Traslada casa a otra posición
        modelTemp = modelCasa = glm::translate(modelCasa, glm::vec3(0.0f, 0.0f, 0.0f));

        modelCasa = glm::scale(modelCasa, glm::vec3(0.2f, 0.2f, 0.2f));       // Escala el modelo 
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelCasa));
        casa.Draw(lightingShader);


        //Dibujo del modelo Perro 1 del previo
        glm::mat4 model(1);
        model = glm::translate(model, glm::vec3(1.3f, 0.65f, 2.5f));
        model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
        glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
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

        glBindVertexArray(0);


        //Sección de luces
        lampshader.Use();
        glUniformMatrix4fv(glGetUniformLocation(lampshader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(lampshader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));


        // Envía la posición de la cámara al shader
        glUniform3f(viewPosLoc, camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);

        //dibujo de luz 1 -LUNA

        if (moveMoon) {
            glm::mat4 modelMoon(1);
            // Calcula la posición en el arco basado en moonRotate
            float xPos = radius * cos(glm::radians(moonRotate));
            float yPos = radius * sin(glm::radians(moonRotate));

            modelMoon = glm::translate(modelMoon, glm::vec3(xPos, -yPos, 0.0f));
            modelMoon = glm::rotate(modelMoon, glm::radians(moonRotate), glm::vec3(0.0f, 1.0f, 0.0f));
            modelMoon = glm::scale(modelMoon, glm::vec3(0.5f)); // Escala el modelo

            glUniformMatrix4fv(glGetUniformLocation(lampshader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelMoon));
            glBindVertexArray(VAO);
            //aplicando textura de luna
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, moonTexture);
            glUniform1i(glGetUniformLocation(lampshader.Program, "material.diffuse"), 0);
            luna.Draw(lampshader);
            glBindVertexArray(0);
        }

        //Dibujo de luz 2 -SOL

        if (moveSun) {
            glm::mat4 modelSun(1);
            // Calcula la posición en el arco basado en sunRotate
            float xPos = radius * cos(glm::radians(sunRotate));
            float yPos = radius * sin(glm::radians(sunRotate));

            // Traslación para mover el sol en un arco
            modelSun = glm::translate(modelSun, glm::vec3(xPos, -yPos, 0.0f));

            // Rotación alrededor del eje Y
            modelSun = glm::rotate(modelSun, glm::radians(sunRotate), glm::vec3(0.0f, 1.0f, 0.0f));

            modelSun = glm::scale(modelSun, glm::vec3(1.0f)); // Escala el modelo
            glUniformMatrix4fv(glGetUniformLocation(lampshader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelSun));
            glBindVertexArray(VAO);
            //aplicando textura
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, sunTexture);
            glUniform1i(glGetUniformLocation(lampshader.Program, "material.diffuse"), 0);
            //Dibujo del modelo de sol
            sol.Draw(lampshader);
            glBindVertexArray(0);
        }
        // Swap the buffers
        glfwSwapBuffers(window);
    }

      
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

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

    if (activanim)
    {
        if (rot > -90.0f)
            rot -= 0.1f;
    }

    //Limitación a 180° de la luna
    if (keys[GLFW_KEY_L] && (moveMoon||moveSun)) {
        moonRotate -= 0.1f;
        if (moonRotate < -180.0f) {
            moonRotate = -180.0f;
        }
        sunRotate -= 0.1;
        if (sunRotate < -180.0f) {
            sunRotate = -180.0f;
        }
    }
    //Limitación a 180° deL sol
    if (keys[GLFW_KEY_O] && (moveSun||moveMoon)) {
        sunRotate += 0.1f;
        if (sunRotate > 0.0f) {
            sunRotate = 0.0f;
        }
        moonRotate += 0.1;
        if (moonRotate > 0.0f) {
            moonRotate = 0.0f;
        }
    }


}

// Is called whenever a key is pressed/released via GLFW
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
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

    //Botón M y N para cambiar entre día y noche
    if (keys[GLFW_KEY_M])
    {
        moveMoon = true;
        moveSun = false;
    }

    if (keys[GLFW_KEY_N])
    {
        moveMoon = false;
        moveSun = true;
    }

}


void MouseCallback(GLFWwindow* window, double xPos, double yPos)
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


