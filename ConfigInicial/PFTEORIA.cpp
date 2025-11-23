//Proyecto Final
//422132844
//CGEIH Grupo 5
//Palafox Jimenez Raúl 
#include <iostream>
#include <cmath>
#include <string>

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
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow* window, double xPos, double yPos);
void DoMovement();
void UpdateDoorAnimation();
void UpdateWindowsAnimation();
void UpdateBellAnimation();
void UpdateSignAnimation();
void UpdateCortinasAnimation();

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

// Variables para animación de puerta
bool doorAnimationActive = false;
float doorRotationAngle = 0.0f;
float doorRotationSpeed = 1.0f;
bool doorOpening = true;
const glm::vec3 DOOR_PIVOT = glm::vec3(-4.350004f, -1.229998f, -5.860039f);
const float MAX_DOOR_ANGLE = 90.0f; // 90 grados de apertura

// Variables para animación de ventanas
bool windowsAnimationActive = false;
float windowsRotationAngle = 0.0f;
float windowsRotationSpeed = 1.0f;
bool windowsAreOpen = false;

// Variables para animación de campana
float bellRotationAngle = 0.0f;
float bellRotationSpeed = 3.0f;
bool bellSwinging = false;
bool bellSwingDirection = true; // true = derecha, false = izquierda
const glm::vec3 BELL_PIVOT = glm::vec3(-1.13727f, 1.61114f, -6.40422f);
const float MAX_BELL_ANGLE = 15.0f; // 15 grados de oscilación
float bellTimer = 0.0f; // Temporizador para la campana
const float BELL_DURATION = 5.0f; // Duración de 5 segundos

// Variables para animación de letrero
bool signAnimationActive = false;
float signRotationAngleX = 0.0f;
float signRotationAngleY = 0.0f;
float signRotationSpeed = 2.0f;
int signAnimationPhase = 0;
bool showSign1 = true; // true = mostrar letrero "OPEN", false = mostrar letrero "CLOSED"

// Variables para animación de cortinas
bool cortinasAnimationActive = false;
float cortinasScaleX = 1.0f;
float cortinasTargetScale = 3.5f;
float cortinasScaleSpeed = 2.0f;
bool cortinasAreScaled = false;

// Pivotes de las ventanas
const glm::vec3 V1_PIVOT = glm::vec3(-9.390120f, 10.340143f, -3.639997f);
const glm::vec3 V2_PIVOT = glm::vec3(-3.039997f, 10.340143f, -3.639997f);
const glm::vec3 V3_PIVOT = glm::vec3(-1.729999f, 10.340143f, -3.639997f);
const glm::vec3 V4_PIVOT = glm::vec3(4.180001f, 10.340143f, -3.639997f);

// Pivotes de las cortinas
const glm::vec3 CORTINA1_PIVOT = glm::vec3(4.89717f, 11.3683f, -5.17732f);
const glm::vec3 CORTINA2_PIVOT = glm::vec3(-10.2291f, 11.4903f, -5.18742f);

// posiciones luces
glm::vec3 pointLightPositions[] = {
	// LUCES AMARILLAS PRINCIPALES (1-4)
	glm::vec3(-3.51363f, 1.96959f, -15.7404f),    // Posición 1
	glm::vec3(-3.51363f, 1.96959f, -20.5627f),    // Posición 2 
	glm::vec3(-3.51363f, 1.96959f, -25.3914f),    // Posición 3
	glm::vec3(-3.51363f, 1.96959f, -30.2137f),    // Posición 4

	// LUCES TENUES (5-7)
	glm::vec3(-9.77577f, 2.51689f, -9.34232f),    // Posición 5
	glm::vec3(-9.77577f, 2.51689f, -18.3423f),    // Posición 6
	glm::vec3(-9.77577f, 2.51689f, -27.3427f),    // Posición 7

	// NUEVAS LUCES BLANCAS BRILLANTES (8-10)
	glm::vec3(1.39525f, 12.0955f, -16.4711f),     // Posición 8 - LUZ BLANCA
	glm::vec3(1.39525f, 12.0955f, -27.7968f),     // Posición 9 - LUZ BLANCA
	glm::vec3(1.39525f, 12.0955f, -38.7969f)      // Posición 10 - LUZ BLANCA
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

glm::vec3 Light1 = glm::vec3(0);

// Variables para posiciones fijas de los modelos
glm::vec3 dogPos(0.0f, 0.0f, 0.0f);
float dogRot = 0.0f;
// Variable para controlar transparencia de la puerta
float puertaAlpha = 0.5f; // Valor entre 0.0 (totalmente transparente) y 1.0 (opaco)

// Deltatime
GLfloat deltaTime = 0.0f;	// Time between current frame and last frame
GLfloat lastFrame = 0.0f;  	// Time of last frame

// Función para actualizar la animación de la puerta
void UpdateDoorAnimation() {
	if (!doorAnimationActive) return;

	if (doorOpening) {
		doorRotationAngle += doorRotationSpeed * deltaTime * 60.0f;
		if (doorRotationAngle >= MAX_DOOR_ANGLE) {
			doorRotationAngle = MAX_DOOR_ANGLE;
			doorOpening = false;
		}
	}
	else {
		doorRotationAngle -= doorRotationSpeed * deltaTime * 60.0f;
		if (doorRotationAngle <= 0.0f) {
			doorRotationAngle = 0.0f;
			doorOpening = true;
			doorAnimationActive = false; // Detener animación cuando vuelve al inicio
		}
	}
}

// Función para actualizar la animación de las ventanas
void UpdateWindowsAnimation() {
	if (!windowsAnimationActive) return;

	if (!windowsAreOpen) {
		// Abriendo las ventanas
		windowsRotationAngle -= windowsRotationSpeed * deltaTime * 60.0f;
		if (windowsRotationAngle <= -90.0f) {
			windowsRotationAngle = -90.0f;
			windowsAnimationActive = false;
			windowsAreOpen = true;
			std::cout << "Ventanas ABIERTAS" << std::endl;
		}
	}
	else {
		// Cerrando las ventanas
		windowsRotationAngle += windowsRotationSpeed * deltaTime * 60.0f;
		if (windowsRotationAngle >= 0.0f) {
			windowsRotationAngle = 0.0f;
			windowsAnimationActive = false;
			windowsAreOpen = false;
			std::cout << "Ventanas CERRADAS" << std::endl;
		}
	}
}

// Función para actualizar la animación de la campana
void UpdateBellAnimation() {
	if (!bellSwinging) return;

	// Actualizar temporizador
	bellTimer += deltaTime;

	// Detener después de 5 segundos
	if (bellTimer >= BELL_DURATION) {
		bellSwinging = false;
		bellTimer = 0.0f;
		bellRotationAngle = 0.0f; // Volver a posición neutral
		std::cout << "Campana detenida después de 5 segundos" << std::endl;
		return;
	}

	if (bellSwingDirection) {
		// Oscilando hacia la derecha
		bellRotationAngle += bellRotationSpeed * deltaTime * 60.0f;
		if (bellRotationAngle >= MAX_BELL_ANGLE) {
			bellRotationAngle = MAX_BELL_ANGLE;
			bellSwingDirection = false;
		}
	}
	else {
		// Oscilando hacia la izquierda
		bellRotationAngle -= bellRotationSpeed * deltaTime * 60.0f;
		if (bellRotationAngle <= -MAX_BELL_ANGLE) {
			bellRotationAngle = -MAX_BELL_ANGLE;
			bellSwingDirection = true;
		}
	}
}

// Función para actualizar la animación del letrero (VERSIÓN SIMPLIFICADA - SOLO ANIMACIÓN)
void UpdateSignAnimation() {
	if (!signAnimationActive) return;

	switch (signAnimationPhase) {
	case 1: // Rotación de +90 grados en X
		signRotationAngleX += signRotationSpeed * deltaTime * 60.0f;
		if (signRotationAngleX >= 90.0f) {
			signRotationAngleX = 90.0f;
			signAnimationPhase = 2;
			std::cout << "Letrero: Rotación X completada (+90°)" << std::endl;
		}
		break;

	case 2: // Rotación de +180 grados en Y
		signRotationAngleY += signRotationSpeed * deltaTime * 60.0f;
		if (signRotationAngleY >= 180.0f) {
			signRotationAngleY = 180.0f;
			signAnimationPhase = 3;
			std::cout << "Letrero: Rotación Y completada (+180°)" << std::endl;
		}
		break;

	case 3: // Rotación final de -90 grados en X
		signRotationAngleX -= signRotationSpeed * deltaTime * 60.0f;
		if (signRotationAngleX <= 0.0f) {
			signRotationAngleX = 0.0f;
			signRotationAngleY = 0.0f;
			signAnimationActive = false;
			signAnimationPhase = 0;

			// CAMBIAR EL MODELO AL TERMINAR LA ANIMACIÓN
			showSign1 = !showSign1;
			std::cout << "Letrero: Animación completada" << std::endl;
			std::cout << "Ahora mostrando: " << (showSign1 ? "OPEN" : "CLOSED") << std::endl;
		}
		break;
	}
}

// Función para actualizar la animación de las cortinas
void UpdateCortinasAnimation() {
	if (!cortinasAnimationActive) return;

	if (!cortinasAreScaled) {
		// Escalando las cortinas (abriendo)
		cortinasScaleX += cortinasScaleSpeed * deltaTime;
		if (cortinasScaleX >= cortinasTargetScale) {
			cortinasScaleX = cortinasTargetScale;
			cortinasAnimationActive = false;
			cortinasAreScaled = true;
			std::cout << "Cortinas ABIERTAS (Escala: " << cortinasScaleX << ")" << std::endl;
		}
	}
	else {
		// Regresando a escala normal (cerrando)
		cortinasScaleX -= cortinasScaleSpeed * deltaTime;
		if (cortinasScaleX <= 1.0f) {
			cortinasScaleX = 1.0f;
			cortinasAnimationActive = false;
			cortinasAreScaled = false;
			std::cout << "Cortinas CERRADAS (Escala: 1.0)" << std::endl;
		}
	}
}

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
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Proyecto Final - Palafox Jimenez Raúl", nullptr, nullptr);

	if (nullptr == window)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();

		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);
	// Ocultar el cursor
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
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

	//models
	Model fachada((char*)"Models/fachada.obj");
	Model piso((char*)"Models/piso.obj");
	Model toldo((char*)"Models/toldo.obj");
	Model puerta((char*)"Models/puerta.obj");
	Model vidrio((char*)"Models/vidrio.obj");
	Model vidrio2((char*)"Models/vidrio2.obj");
	Model esc((char*)"Models/escritorio.obj");
	Model let((char*)"Models/letrero.obj");        // Letrero OPEN
	Model let2((char*)"Models/letrero2.obj");      // Letrero CLOSED
	Model estanteria((char*)"Models/estant.obj");
	Model frascos((char*)"Models/jar.obj");
	Model campana((char*)"Models/camp.obj");
	Model badajo((char*)"Models/bad.obj");
	Model v1((char*)"Models/ventana1.obj");
	Model v2((char*)"Models/ventana2.obj");
	Model v3((char*)"Models/ventana3.obj");
	Model v4((char*)"Models/ventana4.obj");
	Model sillones((char*)"Models/sillones.obj");
	Model mesas((char*)"Models/mesas.obj");
	Model esc2((char*)"Models/escritorio2.obj");
	Model sujeta((char*)"Models/sujeta.obj");
	Model estufa((char*)"Models/estufa.obj");
	Model cafe((char*)"Models/cafe.obj");
	Model vidlamp((char*)"Models/vidriolamp.obj");
	Model lamp2((char*)"Models/lampara2.obj");
	Model sillas((char*)"Models/sillas.obj");
	Model focos((char*)"Models/focos.obj");
	Model escsup((char*)"Models/escritoriosup.obj");
	Model most((char*)"Models/mostrador.obj");
	Model cama((char*)"Models/cama.obj");
	Model cort((char*)"Models/palo.obj");
	Model cort1((char*)"Models/cortinas1.obj");
	Model cort2((char*)"Models/cortinas2.obj");
	Model sillsup((char*)"Models/sillonsup.obj");
	Model luces((char*)"Models/luces2.obj");
	Model contra((char*)"Models/contra.obj");
	Model pisosup((char*)"Models/pisosup.obj");

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
	glUniform1i(glGetUniformLocation(lightingShader.Program, "material.diffuse"), 0);  // "material" en minúscula y "diffuse" bien escrito
	glUniform1i(glGetUniformLocation(lightingShader.Program, "material.specular"), 1); // "material" en minúscula

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
		UpdateDoorAnimation(); // Actualizar animación de puerta
		UpdateWindowsAnimation(); // Actualizar animación de ventanas
		UpdateBellAnimation(); // Actualizar animación de campana
		UpdateSignAnimation(); // Actualizar animación de letrero
		UpdateCortinasAnimation(); // Actualizar animación de cortinas

		// Clear the colorbuffer
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// OpenGL options
		glEnable(GL_DEPTH_TEST);

		glm::mat4 modelTemp = glm::mat4(1.0f); //Temp

		// Use cooresponding shader when setting uniforms/drawing objects
		lightingShader.Use();

		//glUniform1i(glGetUniformLocation(lightingShader.Program, "diffuse"), 0);

		GLint viewPosLoc = glGetUniformLocation(lightingShader.Program, "viewPos");
		glUniform3f(viewPosLoc, camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);

		// Directional light - INTENSIDAD NOCTURNA
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.direction"), -0.2f, -1.0f, -0.3f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.ambient"), 0.03f, 0.03f, 0.05f);    // CASI OSCURO
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.diffuse"), 0.1f, 0.1f, 0.15f);      // MUY BAJO
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.specular"), 0.02f, 0.02f, 0.02f);   // MÍNIMO

		// === CONFIGURACIÓN DE TODAS LAS LUCES PUNTUALES - INTENSIDAD AUMENTADA ===
// === CONFIGURACIÓN DE TODAS LAS LUCES PUNTUALES - INTENSIDAD MODERADA ===

// === CONFIGURACIÓN DE LUCES 1-7 - INTENSIDAD REDUCIDA ===

// Point light 1 - LUZ AMARILLA PRINCIPAL (PUNTO MEDIO)
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].position"), pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].ambient"), 0.2f, 0.15f, 0.07f);      // PUNTO MEDIO
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].diffuse"), 0.6f, 0.45f, 0.25f);      // PUNTO MEDIO
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].specular"), 0.6f, 0.45f, 0.25f);     // PUNTO MEDIO
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].linear"), 0.07f);      // PUNTO MEDIO
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].quadratic"), 0.017f);   // PUNTO MEDIO

		// Point light 2 - LUZ AMARILLA PRINCIPAL (PUNTO MEDIO)
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].position"), pointLightPositions[1].x, pointLightPositions[1].y, pointLightPositions[1].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].ambient"), 0.2f, 0.15f, 0.07f);      // PUNTO MEDIO
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].diffuse"), 0.6f, 0.45f, 0.25f);      // PUNTO MEDIO
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].specular"), 0.6f, 0.45f, 0.25f);     // PUNTO MEDIO
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].linear"), 0.07f);      // PUNTO MEDIO
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].quadratic"), 0.17f);   // PUNTO MEDIO

		// Point light 3 - LUZ AMARILLA PRINCIPAL (PUNTO MEDIO)
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].position"), pointLightPositions[2].x, pointLightPositions[2].y, pointLightPositions[2].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].ambient"), 0.2f, 0.15f, 0.07f);      // PUNTO MEDIO
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].diffuse"), 0.6f, 0.45f, 0.25f);      // PUNTO MEDIO
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].specular"), 0.6f, 0.45f, 0.25f);     // PUNTO MEDIO
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[2].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[2].linear"), 0.07f);      // PUNTO MEDIO
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[2].quadratic"), 0.017f);   // PUNTO MEDIO

		// Point light 4 - LUZ AMARILLA PRINCIPAL (PUNTO MEDIO)
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].position"), pointLightPositions[3].x, pointLightPositions[3].y, pointLightPositions[3].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].ambient"), 0.2f, 0.15f, 0.07f);      // PUNTO MEDIO
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].diffuse"), 0.6f, 0.45f, 0.25f);      // PUNTO MEDIO
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].specular"), 0.6f, 0.45f, 0.25f);     // PUNTO MEDIO
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[3].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[3].linear"), 0.07f);      // PUNTO MEDIO
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[3].quadratic"), 0.017f);   // PUNTO MEDIO
		// Point light 5 - LUZ MULTICOLOR (ROJO/AMARILLO)
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[4].position"), pointLightPositions[4].x, pointLightPositions[4].y, pointLightPositions[4].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[4].ambient"), 0.1f, 0.06f, 0.02f);      // AMBIENTE ROJO/AMARILLO
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[4].diffuse"), 0.4f, 0.25f, 0.1f);       // DIFUSO ROJO/AMARILLO
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[4].specular"), 0.4f, 0.25f, 0.1f);     // SPECULAR ROJO/AMARILLO
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[4].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[4].linear"), 0.07f);       // BUEN ALCANCE
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[4].quadratic"), 0.017f);   // BUEN ALCANCE

		// Point light 6 - LUZ MULTICOLOR (VERDE/AZUL)
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[5].position"), pointLightPositions[5].x, pointLightPositions[5].y, pointLightPositions[5].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[5].ambient"), 0.02f, 0.1f, 0.08f);      // AMBIENTE VERDE/AZUL
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[5].diffuse"), 0.1f, 0.4f, 0.3f);        // DIFUSO VERDE/AZUL
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[5].specular"), 0.1f, 0.4f, 0.3f);       // SPECULAR VERDE/AZUL
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[5].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[5].linear"), 0.07f);       // BUEN ALCANCE
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[5].quadratic"), 0.017f);   // BUEN ALCANCE

		// Point light 7 - LUZ MULTICOLOR (PÚRPURA/ROSA)
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[6].position"), pointLightPositions[6].x, pointLightPositions[6].y, pointLightPositions[6].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[6].ambient"), 0.08f, 0.04f, 0.1f);      // AMBIENTE PÚRPURA
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[6].diffuse"), 0.3f, 0.15f, 0.4f);       // DIFUSO PÚRPURA
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[6].specular"), 0.3f, 0.15f, 0.4f);      // SPECULAR PÚRPURA
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[6].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[6].linear"), 0.07f);       // BUEN ALCANCE
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[6].quadratic"), 0.017f);   // BUEN ALCANCE
		// Point light 8 - LUZ BLANCA (MÁXIMO ALCANCE)
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[7].position"), pointLightPositions[7].x, pointLightPositions[7].y, pointLightPositions[7].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[7].ambient"), 0.2f, 0.2f, 0.2f);       // AMBIENTE REDUCIDO
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[7].diffuse"), 0.8f, 0.8f, 0.8f);       // DIFUSO REDUCIDO
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[7].specular"), 0.6f, 0.6f, 0.6f);      // SPECULAR REDUCIDO
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[7].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[7].linear"), 0.035f);      // MÁXIMO ALCANCE
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[7].quadratic"), 0.0075f);  // MÁXIMO ALCANCE

		// Point light 9 - LUZ BLANCA (MÁXIMO ALCANCE)
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[8].position"), pointLightPositions[8].x, pointLightPositions[8].y, pointLightPositions[8].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[8].ambient"), 0.2f, 0.2f, 0.2f);       // AMBIENTE REDUCIDO
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[8].diffuse"), 0.8f, 0.8f, 0.8f);       // DIFUSO REDUCIDO
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[8].specular"), 0.6f, 0.6f, 0.6f);      // SPECULAR REDUCIDO
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[8].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[8].linear"), 0.035f);      // MÁXIMO ALCANCE
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[8].quadratic"), 0.0075f);  // MÁXIMO ALCANCE

		// Point light 10 - LUZ BLANCA (MÁXIMO ALCANCE)
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[9].position"), pointLightPositions[9].x, pointLightPositions[9].y, pointLightPositions[9].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[9].ambient"), 0.2f, 0.2f, 0.2f);       // AMBIENTE REDUCIDO
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[9].diffuse"), 0.8f, 0.8f, 0.8f);       // DIFUSO REDUCIDO
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[9].specular"), 0.6f, 0.6f, 0.6f);      // SPECULAR REDUCIDO
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[9].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[9].linear"), 0.035f);      // MÁXIMO ALCANCE
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[9].quadratic"), 0.0075f);  // MÁXIMO ALCANCE
		//Spotlight
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
		//Piso
		model = modelTemp;
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		piso.Draw(lightingShader);

		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);
		//fachada - MATERIAL MUY OPACO
		glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 2.0f);         // MÍNIMO brillo
		glUniform3f(glGetUniformLocation(lightingShader.Program, "material.specular"), 0.05f, 0.05f, 0.05f); // CASI NULO
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		fachada.Draw(lightingShader);
		// Restaurar valores normales para otros modelos
		glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 32.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "material.specular"), 0.5f, 0.5f, 0.5f);

		//toldo
		model = modelTemp;
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		toldo.Draw(lightingShader);

		//escritorio - MATERIAL OPACO
		glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 4.0f);         // MUY BAJO
		glUniform3f(glGetUniformLocation(lightingShader.Program, "material.specular"), 0.08f, 0.08f, 0.08f); // MUY BAJO
		model = modelTemp;
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		esc.Draw(lightingShader);
		// Restaurar valores normales para otros modelos
		glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 32.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "material.specular"), 0.5f, 0.5f, 0.5f);
		//escritorio2
		model = modelTemp;
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		esc2.Draw(lightingShader);

		//estanteria
		model = modelTemp;
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		estanteria.Draw(lightingShader);

		//mesas
		model = modelTemp;
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		mesas.Draw(lightingShader);
		//sillones
		model = modelTemp;
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		sillones.Draw(lightingShader);

		//frascos
		model = modelTemp;
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		frascos.Draw(lightingShader);
		//estufa
		model = modelTemp;
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		estufa.Draw(lightingShader);
		//maquina de cafe
		model = modelTemp;
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		cafe.Draw(lightingShader);
		//Lamparas de mesas
		model = modelTemp;
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		lamp2.Draw(lightingShader);
		//focos
		model = modelTemp;
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		focos.Draw(lightingShader);
		//escritorio superior
		model = modelTemp;
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		escsup.Draw(lightingShader);
		//cama
		model = modelTemp;
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		cama.Draw(lightingShader);
		// palo de cortinas
		model = modelTemp;
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		cort.Draw(lightingShader);

		// === CORTINAS CON ANIMACIÓN DE ESCALA ===
		// Cortina 1 (escala desde pivote derecho)
		glm::mat4 cortina1Model = modelTemp;
		if (cortinasScaleX != 1.0f) {
			cortina1Model = glm::translate(cortina1Model, CORTINA1_PIVOT);
			cortina1Model = glm::scale(cortina1Model, glm::vec3(cortinasScaleX, 1.0f, 1.0f));
			cortina1Model = glm::translate(cortina1Model, -CORTINA1_PIVOT);
		}
		model = cortina1Model;
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		cort1.Draw(lightingShader);

		// Cortina 2 (escala desde pivote izquierdo)
		glm::mat4 cortina2Model = modelTemp;
		if (cortinasScaleX != 1.0f) {
			cortina2Model = glm::translate(cortina2Model, CORTINA2_PIVOT);
			cortina2Model = glm::scale(cortina2Model, glm::vec3(cortinasScaleX, 1.0f, 1.0f));
			cortina2Model = glm::translate(cortina2Model, -CORTINA2_PIVOT);
		}
		model = cortina2Model;
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		cort2.Draw(lightingShader);

		//mostrador
		model = modelTemp;
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		most.Draw(lightingShader);
		//sillonsuperior
		model = modelTemp;
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		sillsup.Draw(lightingShader);
		//luces2
		model = modelTemp;
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		luces.Draw(lightingShader);
		//counter
		model = modelTemp;
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		contra.Draw(lightingShader);
		//piso superior - ABSORCIÓN TOTAL
		glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 2.0f);        // MÍNIMO brillo
		glUniform3f(glGetUniformLocation(lightingShader.Program, "material.specular"), 0.05f, 0.05f, 0.05f); // CASI NULO
		model = modelTemp;
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		pisosup.Draw(lightingShader);
		// Restaurar valores
		glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 32.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "material.specular"), 0.5f, 0.5f, 0.5f);
		//Sillas
		model = modelTemp;
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		sillas.Draw(lightingShader);


		// === VENTANAS CON ANIMACIÓN ===
		// Ventana 1 (hacia afuera)
		glm::mat4 window1Model = modelTemp;
		if (windowsRotationAngle < 0.0f) {
			window1Model = glm::translate(window1Model, V1_PIVOT);
			window1Model = glm::rotate(window1Model, glm::radians(windowsRotationAngle), glm::vec3(0.0f, 1.0f, 0.0f));
			window1Model = glm::translate(window1Model, -V1_PIVOT);
		}
		model = window1Model;
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		v1.Draw(lightingShader);

		// Ventana 2 (hacia adentro - dirección opuesta)
		glm::mat4 window2Model = modelTemp;
		if (windowsRotationAngle < 0.0f) {
			window2Model = glm::translate(window2Model, V2_PIVOT);
			window2Model = glm::rotate(window2Model, glm::radians(-windowsRotationAngle), glm::vec3(0.0f, 1.0f, 0.0f)); // Negativo del ángulo
			window2Model = glm::translate(window2Model, -V2_PIVOT);
		}
		model = window2Model;
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		v2.Draw(lightingShader);

		// Ventana 3 (h hacia afuera)
		glm::mat4 window3Model = modelTemp;
		if (windowsRotationAngle < 0.0f) {
			window3Model = glm::translate(window3Model, V3_PIVOT);
			window3Model = glm::rotate(window3Model, glm::radians(windowsRotationAngle), glm::vec3(0.0f, 1.0f, 0.0f));
			window3Model = glm::translate(window3Model, -V3_PIVOT);
		}
		model = window3Model;
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		v3.Draw(lightingShader);

		// Ventana 4 (hacia adentro - dirección opuesta)
		glm::mat4 window4Model = modelTemp;
		if (windowsRotationAngle < 0.0f) {
			window4Model = glm::translate(window4Model, V4_PIVOT);
			window4Model = glm::rotate(window4Model, glm::radians(-windowsRotationAngle), glm::vec3(0.0f, 1.0f, 0.0f)); // Negativo del ángulo
			window4Model = glm::translate(window4Model, -V4_PIVOT);
		}
		model = window4Model;
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		v4.Draw(lightingShader);

		// === GRUPO DE LA PUERTA CON ANIMACIÓN ===
		glm::mat4 doorGroupModel = modelTemp;

		// Aplicar rotación de la puerta alrededor del pivote
		if (doorRotationAngle > 0.0f) {
			// Mover al pivote, rotar, y volver a la posición original
			doorGroupModel = glm::translate(doorGroupModel, DOOR_PIVOT);
			doorGroupModel = glm::rotate(doorGroupModel, glm::radians(doorRotationAngle), glm::vec3(0.0f, 1.0f, 0.0f)); // Rotación en Y
			doorGroupModel = glm::translate(doorGroupModel, -DOOR_PIVOT);
		}

		//Sujeta campana
		model = modelTemp;
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		sujeta.Draw(lightingShader);

		//Puerta (con animación)
		model = doorGroupModel;
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		puerta.Draw(lightingShader);

		// === GRUPO DE LA CAMPANA CON ANIMACIÓN ===
		glm::mat4 bellGroupModel = modelTemp;

		// Aplicar rotación de la campana alrededor del pivote
		if (bellSwinging && abs(bellRotationAngle) > 0.1f) {
			bellGroupModel = glm::translate(bellGroupModel, BELL_PIVOT);
			bellGroupModel = glm::rotate(bellGroupModel, glm::radians(bellRotationAngle), glm::vec3(0.0f, 0.0f, 1.0f)); // Rotación en Z para oscilar
			bellGroupModel = glm::translate(bellGroupModel, -BELL_PIVOT);
		}

		//Campana (con animación de sonido)
		model = bellGroupModel;
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		campana.Draw(lightingShader);

		//Badajo (con animación de sonido - misma transformación)
		model = bellGroupModel;
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		badajo.Draw(lightingShader);

		// === GRUPO DEL LETRERO CON ANIMACIÓN INDEPENDIENTE ===
		glm::mat4 signGroupModel = doorGroupModel;  // CAMBIAR: usar doorGroupModel en lugar de modelTemp

		// Aplicar animación del letrero si está activa
		if (signAnimationActive) {
			signGroupModel = glm::translate(signGroupModel, glm::vec3(-2.59038f, -0.529044f, -5.78484f));
			signGroupModel = glm::rotate(signGroupModel, glm::radians(signRotationAngleX), glm::vec3(1.0f, 0.0f, 0.0f));
			signGroupModel = glm::rotate(signGroupModel, glm::radians(signRotationAngleY), glm::vec3(0.0f, 1.0f, 0.0f));
			signGroupModel = glm::translate(signGroupModel, -glm::vec3(-2.59038f, -0.529044f, -5.78484f));
		}

		//letrero (con animación independiente) - MOSTRAR EL MODELO CORRECTO
		model = signGroupModel;
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		if (showSign1) {
			let.Draw(lightingShader);  // Mostrar letrero "OPEN"
		}
		else {
			let2.Draw(lightingShader); // Mostrar letrero "CLOSED"
		}

		//letrero (con animación independiente) - MOSTRAR EL MODELO CORRECTO
		model = signGroupModel;
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		if (showSign1) {
			let.Draw(lightingShader);  // Mostrar letrero "OPEN"
		}
		else {
			let2.Draw(lightingShader); // Mostrar letrero "CLOSED"
		}

		// vidrio (hijo de puerta con transparencia - misma transformación)
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glDepthMask(GL_FALSE);

		model = doorGroupModel;
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 1);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "transparencyAlpha"), puertaAlpha);
		vidrio.Draw(lightingShader);

		glDepthMask(GL_TRUE);
		glDisable(GL_BLEND);



		//Vidrio de las lamparas
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glDepthMask(GL_FALSE);

		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 1);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "transparencyAlpha"), puertaAlpha);
		vidlamp.Draw(lightingShader);

		glDepthMask(GL_TRUE);
		glDisable(GL_BLEND);

		//vidrio (de la fachada - SIN animación de puerta)
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glDepthMask(GL_FALSE);

		model = glm::mat4(1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 1);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "transparencyAlpha"), puertaAlpha);
		vidrio2.Draw(lightingShader);

		glDepthMask(GL_TRUE);
		glDisable(GL_BLEND);

		// Also draw the lamp object, again binding the appropriate shader
		lampShader.Use();
		// Get location objects for the matrices on the lamp shader (these could be different on a different shader)
		modelLoc = glGetUniformLocation(lampShader.Program, "model");
		viewLoc = glGetUniformLocation(lampShader.Program, "view");
		projLoc = glGetUniformLocation(lampShader.Program, "projection");

		// Set matrices
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

		// Bind VAO before drawing all lamps
		glBindVertexArray(VAO);

		// Draw all point lights as visual spheres
		for (int i = 0; i < 10; i++) {
			model = glm::mat4(1);
			model = glm::translate(model, pointLightPositions[i]);
			model = glm::scale(model, glm::vec3(0.2f));
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
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
}

// Is called whenever a key is pressed/released via GLFW
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (GLFW_KEY_ESCAPE == key && GLFW_PRESS == action)
	{
		// Mostrar el cursor antes de cerrar
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		glfwSetWindowShouldClose(window, GL_TRUE);
	}


	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			keys[key] = true;

			// Tecla R para activar/desactivar animación de puerta y campana
			if (key == GLFW_KEY_R) {
				doorAnimationActive = !doorAnimationActive;

				if (doorAnimationActive) {
					// Iniciar campana con temporizador
					bellSwinging = true;
					bellTimer = 0.0f; // Resetear temporizador
					std::cout << "Animación de puerta ACTIVADA - Campana sonando por 5 segundos!" << std::endl;
					// Reiniciar animación si estaba en medio
					if (doorRotationAngle <= 0.0f) {
						doorOpening = true;
					}
				}
				else {
					std::cout << "Animación de puerta DESACTIVADA" << std::endl;
				}
			}

			// Tecla Q para toggle abrir/cerrar ventanas
			if (key == GLFW_KEY_Q) {
				windowsAnimationActive = true;
				if (windowsAreOpen) {
					std::cout << "Cerrando ventanas..." << std::endl;
				}
				else {
					std::cout << "Abriendo ventanas..." << std::endl;
				}
			}

			// Tecla P para activar animación de letrero (VERSIÓN CON CAMBIO DE MODELO)
			if (key == GLFW_KEY_P) {
				if (!signAnimationActive) {
					signAnimationActive = true;
					signAnimationPhase = 1;
					std::cout << "Animación de letrero INICIADA" << std::endl;
					std::cout << "Estado actual: " << (showSign1 ? "OPEN" : "CLOSED") << std::endl;
					std::cout << "Cambiando a: " << (!showSign1 ? "OPEN" : "CLOSED") << " después de la animación" << std::endl;
				}
			}

			// Tecla N para toggle abrir/cerrar cortinas
			if (key == GLFW_KEY_N) {
				cortinasAnimationActive = true;
				if (cortinasAreScaled) {
					std::cout << "Cerrando cortinas..." << std::endl;
				}
				else {
					std::cout << "Abriendo cortinas..." << std::endl;
				}
			}
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
			Light1 = glm::vec3(0.2f, 0.8f, 1.0f);
		}
		else
		{
			Light1 = glm::vec3(0);
		}
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
	GLfloat yOffset = lastY - yPos;

	lastX = xPos;
	lastY = yPos;

	camera.ProcessMouseMovement(xOffset, yOffset);
}