/*Práctica 5 Carga de modelos
De la Cruz Padilla Marlene Mariana 
Fecha de entrega: 15 de septiembre de 2024*/

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
const GLuint WIDTH = 1200, HEIGHT = 800;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Function prototypes
void KeyCallback( GLFWwindow *window, int key, int scancode, int action, int mode );
void MouseCallback( GLFWwindow *window, double xPos, double yPos );
void DoMovement( );


// Camera
Camera camera( glm::vec3( 0.0f, 0.0f, 3.0f ) );
bool keys[1024];
GLfloat lastX = 400, lastY = 300;
bool firstMouse = true;

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;



int main( )
{
    // Init GLFW
    glfwInit( );
    // Set all the required options for GLFW
    glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
    glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );
    glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );
    glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE );
    glfwWindowHint( GLFW_RESIZABLE, GL_FALSE );
    
    // Create a GLFWwindow object that we can use for GLFW's functions
    GLFWwindow *window = glfwCreateWindow( WIDTH, HEIGHT, "Carga de modelos y camara sintetica Marlene", nullptr, nullptr );
    
    if ( nullptr == window )
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate( );
        
        return EXIT_FAILURE;
    }
    
    glfwMakeContextCurrent( window );
    
    glfwGetFramebufferSize( window, &SCREEN_WIDTH, &SCREEN_HEIGHT );
    
    // Set the required callback functions
    glfwSetKeyCallback( window, KeyCallback );
    glfwSetCursorPosCallback( window, MouseCallback );
    
    // GLFW Options
    //glfwSetInputMode( window, GLFW_CURSOR, GLFW_CURSOR_DISABLED );
    
    // Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
    glewExperimental = GL_TRUE;
    // Initialize GLEW to setup the OpenGL Function pointers
    if ( GLEW_OK != glewInit( ) )
    {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return EXIT_FAILURE;
    }
    
    // Define the viewport dimensions
    glViewport( 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT );
    
    // OpenGL options
    glEnable( GL_DEPTH_TEST );
    
    // Setup and compile our shaders
    Shader shader( "Shader/modelLoading.vs", "Shader/modelLoading.frag" );
    
    // Load models
    Model dog((char*)"Models/RedDog.obj"); //Ruta de donde se encuentra el archivo 3D perro del previo
    Model dog2((char*)"Models/13041_Beagle_v1_L1.obj"); //Ruta de perro2 3D
    Model casa((char*)"Models/farmhouse_obj.obj"); //Ruta de la casa 3D
    Model tronco((char*)"Models/trunk wood.obj"); //Ruta del tronco
    Model pasto((char*)"Models/10450_Rectangular_Grass_Patch_v1_iterations-2.obj"); //Ruta del pasto 3D
    Model columpio((char*)"Models/Obj.obj"); //Ruta del columpio 3D
    Model pozo((char*)"Models/well.obj"); //Ruta del pozo 3D

    glm::mat4 projection = glm::perspective( camera.GetZoom( ), ( float )SCREEN_WIDTH/( float )SCREEN_HEIGHT, 0.1f, 100.0f );
    
  

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
        glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.Use();

        glm::mat4 view = camera.GetViewMatrix();
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));

        // Draw the loaded model

        //Dibujo de modelo de casa
        glm::mat4 modelCasa(1);
        modelCasa = glm::rotate(modelCasa, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f)); //Rotación de casa
        modelCasa = glm::translate(modelCasa, glm::vec3(0.0f, 0.0f, 0.0f));  // Traslada casa a otra posición
        modelCasa = glm::scale(modelCasa, glm::vec3(0.2f, 0.2f, 0.2f));       // Escala el modelo 
         glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelCasa));
        casa.Draw(shader);
        
        //Dibujo del modelo Perro 1 del previo
        glm::mat4 model(1);
        model = glm::translate(model, glm::vec3(1.3f, 0.65f, 2.5f));
        model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
        dog.Draw(shader); //Se dibuja el perro

        //Dibujo del modelo perro 2
        glm::mat4 modelDog2(1);
        modelDog2 = glm::rotate(modelDog2, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 1.0f)); //rotación de perro 2
        modelDog2 = glm::translate(modelDog2, glm::vec3(1.0f, 2.5f, 0.28f));  // Traslada dog2 a otra posición
        modelDog2 = glm::scale(modelDog2, glm::vec3(0.01f, 0.01f, 0.01f));       // Escala el modelo
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelDog2));
        dog2.Draw(shader);

        //Dibujo del modelo de pasto 
        glm::mat4 modelPasto(1);
        modelPasto = glm::rotate(modelPasto, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f)); //rotación del pasto
        modelPasto = glm::translate(modelPasto, glm::vec3(0.0f, -1.0f, -0.5f));  // Traslada pasto a otra posición
        modelPasto = glm::scale(modelPasto, glm::vec3(0.06f, 0.06f, 0.06f));       // Escala el modelo
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelPasto));
        pasto.Draw(shader);

        //Dibujo del modelo del tronco 
        glm::mat4 modelTroco(1);
        modelTroco = glm::translate(modelTroco, glm::vec3(2.3f, 0.0f, 0.0f));  // Traslada tronco 1 a otra posición
        modelTroco = glm::scale(modelTroco, glm::vec3(1.5f, 1.5f, 1.5f));       // Escala el modelo
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelTroco));
        tronco.Draw(shader);

        glm::mat4 modelTroco2(1);
        modelTroco2 = glm::translate(modelTroco2, glm::vec3(2.7f, 0.0f, 0.0f));  // Traslada tronco 2 a otra posición
        modelTroco2 = glm::scale(modelTroco2, glm::vec3(1.5f, 1.5f, 1.5f));       // Escala el modelo
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelTroco2));
        tronco.Draw(shader);

        glm::mat4 modelTroco3(1);
        modelTroco3 = glm::translate(modelTroco3, glm::vec3(2.5f, 0.3f, 0.0f));  // Traslada tronco 3 a otra posición
        modelTroco3 = glm::scale(modelTroco3, glm::vec3(1.5f, 1.5f, 1.5f));       // Escala el modelo
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelTroco3));
        tronco.Draw(shader);

        //Dibujo del modelo del columpio
        glm::mat4 modelCol(1);
        modelCol = glm::translate(modelCol, glm::vec3(5.5f, 0.0f, 0.5f));  // Traslada columpio a otra posición
        modelCol = glm::scale(modelCol, glm::vec3(0.00035f, 0.00035f, 0.00035f));       // Escala el modelo
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelCol));
        columpio.Draw(shader);

        //Dibujo del modelo del pozo
        glm::mat4 modelPozo(1);
        modelPozo = glm::rotate(modelPozo, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f)); //rotación del pasto
        modelPozo = glm::translate(modelPozo, glm::vec3(-5.0f, 5.0f, 1.2f));  // Traslada pozo a otra posición
        modelPozo = glm::scale(modelPozo, glm::vec3(0.1f, 0.1f, 0.1f));       // Escala el modelo
        glUniformMatrix4fv(glGetUniformLocation(shader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelPozo));
        pozo.Draw(shader);
        

        // Swap the buffers
        glfwSwapBuffers( window );
    }
    
    glfwTerminate( );
    return 0;
}


// Moves/alters the camera positions based on user input
void DoMovement( )
{
    // Camera controls
    if ( keys[GLFW_KEY_W] || keys[GLFW_KEY_UP] )
    {
        camera.ProcessKeyboard( FORWARD, deltaTime );
    }
    
    if ( keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN] )
    {
        camera.ProcessKeyboard( BACKWARD, deltaTime );
    }
    
    if ( keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT] )
    {
        camera.ProcessKeyboard( LEFT, deltaTime );
    }
    
    if ( keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT] )
    {
        camera.ProcessKeyboard( RIGHT, deltaTime );
    }

   
}

// Is called whenever a key is pressed/released via GLFW
void KeyCallback( GLFWwindow *window, int key, int scancode, int action, int mode )
{
    if ( GLFW_KEY_ESCAPE == key && GLFW_PRESS == action )
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
    
    if ( key >= 0 && key < 1024 )
    {
        if ( action == GLFW_PRESS )
        {
            keys[key] = true;
        }
        else if ( action == GLFW_RELEASE )
        {
            keys[key] = false;
        }
    }

 

 
}

void MouseCallback( GLFWwindow *window, double xPos, double yPos )
{
    if ( firstMouse )
    {
        lastX = xPos;
        lastY = yPos;
        firstMouse = false;
    }
    
    GLfloat xOffset = xPos - lastX;
    GLfloat yOffset = lastY - yPos;  // Reversed since y-coordinates go from bottom to left
    
    lastX = xPos;
    lastY = yPos;
    
    camera.ProcessMouseMovement( xOffset, yOffset );
}

