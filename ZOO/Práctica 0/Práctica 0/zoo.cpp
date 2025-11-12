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
const GLuint WIDTH = 800, HEIGHT = 600;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Function prototypes
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow* window, double xPos, double yPos);
void DoMovement();
void AnimationRhino();
void AnimationZebra();
void AnimationWolf();
void AnimationLion();
void AnimationDeer();
void AnimationBear();
unsigned int loadCubemap(std::vector<std::string> faces);

// Camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
bool keys[1024];
GLfloat lastX = 400, lastY = 300;
bool firstMouse = true;

// Light attributes
glm::vec3 lightPos(0.5f, 0.5f, 2.5f);
float movelightPos = 0.0f;
glm::vec3 lightPos1(-0.5f, 0.5f, 2.5f);
float movelightPos1 = 0.0f;
GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;
float rot = 0.0f;
bool activanim = false;

// === VARIABLES DE MOVIMIENTO DE PECES ===
float fishSpeed = 0.002f;   // velocidad de movimiento
float posFishesX = -14.0f; // posición inicial X
float posFishesZ = -20.0f; // posición inicial Z
float rotFish = 90.0f;     // rotación inicial (hacia la derecha)
int fishDir = 0;           // 0 = derecha, 1 = arriba, 2 = izquierda, 3 = abajo

float xMin = -14.0f;
float xMax = -2.0f;
float zMin = -24.5f;
float zMax = -20.0f;


//ANIMACIÓN DEL RINOCERONTE
float rotRhino = 0.0f;
float rhinoFLegs = 0.0f;
float rhinoBLegs = 0.0f;
float rhinoHead = 0.0f;
float rhinoPosX = 0.0f;
float rhinoPosY = -0.5f;
float rhinoPosZ = -5.0f;

//ANIMACIÓN DE LA ZEBRA
float rotZebra = 0.0f;
float zebraFLegs = 0.0f;
float zebraBLegs = 0.0f;
float zebraHead = 0.0f;
float zebraTail = 0.0f;
float zebraPosX = -4.0f;
float zebraPosY = -0.5f;
float zebraPosZ = 2.0f;

//ANIMACIÓN DEL WOLF
float rotWolf = 0.0f;
float wolfFLegs = 0.0f;
float wolfBLegs = 0.0f;
float wolfHead = 0.0f;
float wolfTail = 0.0f;
float wolfPosX = 1.0f;
float wolfPosY = 0.0f;
float wolfPosZ = 0.0f;

//ANIMACIÓN DEL LION
float rotLion = 0.0f;
float lionFLegs = 0.0f;
float lionBLegs = 0.0f;
float lionHead = 0.0f;
float lionTail = 0.0f;
float lionPosX = 4.0f;
float lionPosY = 0.0f;
float lionPosZ = -2.0f;

//ANIMACIÓN DEL DEER
float rotDeer = 0.0f;
float deerFLegs = 0.0f;
float deerBLegs = 0.0f;
float deerHead = 0.0f;
float deerPosX = -2.0f;
float deerPosY = -0.5f;
float deerPosZ = 5.0f;

//ANIMACIÓN DEL BEAR
float rotBear = 0.0f;
float bearFLegs = 0.0f;
float bearBLegs = 0.0f;
float bearHead = 0.0f;
float bearPosX = 0.0f;
float bearPosY = -0.5f;
float bearPosZ = -3.0f;

#define MAX_FRAMES_RHINO 9
int i_max_steps_rhino = 190;
int i_curr_steps_rhino = 0;

typedef struct _frameRhino {
	float rotRhino, rotRhinoInc;
	float rhinoPosX;
	float rhinoPosY;
	float rhinoPosZ;
	float incX;
	float incY;
	float incZ;
	float rhinoHead;
	float rhinoHeadInc;
	float rhinoFLegs;
	float rhinoFLegsInc;
	float rhinoBLegs;
	float rhinoBLegsInc;
} FRAME_RHINO;

FRAME_RHINO KeyFrameRhino[MAX_FRAMES_RHINO];
int FrameIndexRhino = 0;
bool playRhino = false;
int playIndexRhino = 0;

void saveFrameRhino(void)
{
	printf("Rhino KeyFrame %d guardado\n", FrameIndexRhino);
	KeyFrameRhino[FrameIndexRhino].rhinoPosX = rhinoPosX;
	KeyFrameRhino[FrameIndexRhino].rhinoPosY = rhinoPosY;
	KeyFrameRhino[FrameIndexRhino].rhinoPosZ = rhinoPosZ;
	KeyFrameRhino[FrameIndexRhino].rotRhino = rotRhino;
	KeyFrameRhino[FrameIndexRhino].rhinoHead = rhinoHead;
	KeyFrameRhino[FrameIndexRhino].rhinoFLegs = rhinoFLegs;
	KeyFrameRhino[FrameIndexRhino].rhinoBLegs = rhinoBLegs;
	FrameIndexRhino++;
}

void resetElementsRhino(void)
{
	rhinoPosX = KeyFrameRhino[0].rhinoPosX;
	rhinoPosY = KeyFrameRhino[0].rhinoPosY;
	rhinoPosZ = KeyFrameRhino[0].rhinoPosZ;
	rotRhino = KeyFrameRhino[0].rotRhino;
	rhinoHead = KeyFrameRhino[0].rhinoHead;
	rhinoFLegs = KeyFrameRhino[0].rhinoFLegs;
	rhinoBLegs = KeyFrameRhino[0].rhinoBLegs;
}

// Función para cargar animación predefinida del rinoceronte
void loadPredefinedAnimationRhino(void)
{
	// KeyFrame 0 - Posición inicial
	KeyFrameRhino[0].rhinoPosX = 0.0f;
	KeyFrameRhino[0].rhinoPosY = -0.5f;
	KeyFrameRhino[0].rhinoPosZ = -5.0f;
	KeyFrameRhino[0].rotRhino = 0.0f;
	KeyFrameRhino[0].rhinoHead = 0.0f;
	KeyFrameRhino[0].rhinoFLegs = 0.0f;
	KeyFrameRhino[0].rhinoBLegs = 0.0f;

	// KeyFrame 1 - Caminar hacia adelante (lado 1 del cuadrado)
	KeyFrameRhino[1].rhinoPosX = 0.0f;
	KeyFrameRhino[1].rhinoPosY = -0.5f;
	KeyFrameRhino[1].rhinoPosZ = 0.0f;
	KeyFrameRhino[1].rotRhino = 0.0f;
	KeyFrameRhino[1].rhinoHead = 2.0f;
	KeyFrameRhino[1].rhinoFLegs = 5.0f;
	KeyFrameRhino[1].rhinoBLegs = -5.0f;

	// KeyFrame 2 - Girar 90 grados a la derecha
	KeyFrameRhino[2].rhinoPosX = 0.0f;
	KeyFrameRhino[2].rhinoPosY = -0.5f;
	KeyFrameRhino[2].rhinoPosZ = 0.0f;
	KeyFrameRhino[2].rotRhino = 90.0f;
	KeyFrameRhino[2].rhinoHead = -2.0f;
	KeyFrameRhino[2].rhinoFLegs = 0.0f;
	KeyFrameRhino[2].rhinoBLegs = 0.0f;

	// KeyFrame 3 - Caminar hacia la derecha (lado 2 del cuadrado)
	KeyFrameRhino[3].rhinoPosX = 5.0f;
	KeyFrameRhino[3].rhinoPosY = -0.5f;
	KeyFrameRhino[3].rhinoPosZ = 0.0f;
	KeyFrameRhino[3].rotRhino = 90.0f;
	KeyFrameRhino[3].rhinoHead = 2.0f;
	KeyFrameRhino[3].rhinoFLegs = 5.0f;
	KeyFrameRhino[3].rhinoBLegs = -5.0f;

	// KeyFrame 4 - Girar 180 grados (hacia abajo)
	KeyFrameRhino[4].rhinoPosX = 5.0f;
	KeyFrameRhino[4].rhinoPosY = -0.5f;
	KeyFrameRhino[4].rhinoPosZ = 0.0f;
	KeyFrameRhino[4].rotRhino = 180.0f;
	KeyFrameRhino[4].rhinoHead = -50.0f;
	KeyFrameRhino[4].rhinoFLegs = 0.0f;
	KeyFrameRhino[4].rhinoBLegs = 0.0f;

	// KeyFrame 5 - Caminar hacia abajo (lado 3 del cuadrado)
	KeyFrameRhino[5].rhinoPosX = 5.0f;
	KeyFrameRhino[5].rhinoPosY = -0.5f;
	KeyFrameRhino[5].rhinoPosZ = -5.0f;
	KeyFrameRhino[5].rotRhino = 180.0f;
	KeyFrameRhino[5].rhinoHead = 2.0f;
	KeyFrameRhino[5].rhinoFLegs = 5.0f;
	KeyFrameRhino[5].rhinoBLegs = -5.0f;

	// KeyFrame 6 - Girar 270 grados (hacia la izquierda)
	KeyFrameRhino[6].rhinoPosX = 5.0f;
	KeyFrameRhino[6].rhinoPosY = -0.5f;
	KeyFrameRhino[6].rhinoPosZ = -5.0f;
	KeyFrameRhino[6].rotRhino = 270.0f;
	KeyFrameRhino[6].rhinoHead = -3.0f;
	KeyFrameRhino[6].rhinoFLegs = 0.0f;
	KeyFrameRhino[6].rhinoBLegs = 0.0f;

	// KeyFrame 7 - Caminar hacia la izquierda (lado 4 del cuadrado)
	KeyFrameRhino[7].rhinoPosX = 0.0f;
	KeyFrameRhino[7].rhinoPosY = -0.5f;
	KeyFrameRhino[7].rhinoPosZ = -5.0f;
	KeyFrameRhino[7].rotRhino = 270.0f;
	KeyFrameRhino[7].rhinoHead = 2.0f;
	KeyFrameRhino[7].rhinoFLegs = 5.0f;
	KeyFrameRhino[7].rhinoBLegs = -5.0f;

	// KeyFrame 8 - Girar a 0 grados para cerrar el ciclo
	KeyFrameRhino[8].rhinoPosX = 0.0f;
	KeyFrameRhino[8].rhinoPosY = -0.5f;
	KeyFrameRhino[8].rhinoPosZ = -5.0f;
	KeyFrameRhino[8].rotRhino = 360.0f;
	KeyFrameRhino[8].rhinoHead = 0.0f;
	KeyFrameRhino[8].rhinoFLegs = 0.0f;
	KeyFrameRhino[8].rhinoBLegs = 0.0f;

	// Establecer el índice de frames al máximo
	FrameIndexRhino = MAX_FRAMES_RHINO;
	
	printf("Animacion predefinida del rinoceronte cargada: %d keyframes\n", FrameIndexRhino);
}

void interpolationRhino(void)
{
	KeyFrameRhino[playIndexRhino].incX = (KeyFrameRhino[playIndexRhino + 1].rhinoPosX - KeyFrameRhino[playIndexRhino].rhinoPosX) / i_max_steps_rhino;
	KeyFrameRhino[playIndexRhino].incY = (KeyFrameRhino[playIndexRhino + 1].rhinoPosY - KeyFrameRhino[playIndexRhino].rhinoPosY) / i_max_steps_rhino;
	KeyFrameRhino[playIndexRhino].incZ = (KeyFrameRhino[playIndexRhino + 1].rhinoPosZ - KeyFrameRhino[playIndexRhino].rhinoPosZ) / i_max_steps_rhino;
	KeyFrameRhino[playIndexRhino].rotRhinoInc = (KeyFrameRhino[playIndexRhino + 1].rotRhino - KeyFrameRhino[playIndexRhino].rotRhino) / i_max_steps_rhino;
	KeyFrameRhino[playIndexRhino].rhinoHeadInc = (KeyFrameRhino[playIndexRhino + 1].rhinoHead - KeyFrameRhino[playIndexRhino].rhinoHead) / i_max_steps_rhino;
	KeyFrameRhino[playIndexRhino].rhinoFLegsInc = (KeyFrameRhino[playIndexRhino + 1].rhinoFLegs - KeyFrameRhino[playIndexRhino].rhinoFLegs) / i_max_steps_rhino;
	KeyFrameRhino[playIndexRhino].rhinoBLegsInc = (KeyFrameRhino[playIndexRhino + 1].rhinoBLegs - KeyFrameRhino[playIndexRhino].rhinoBLegs) / i_max_steps_rhino;
}
 //procedural
void AnimationRhino()
{
	if (playRhino)
	{
		if (i_curr_steps_rhino >= i_max_steps_rhino)
		{
			playIndexRhino++;
			if (playIndexRhino > FrameIndexRhino - 2)
			{
				// En lugar de detener la animación, reiniciarla para hacer loop
				printf("Ciclo de animacion completado. Reiniciando...\n");
				playIndexRhino = 0;
				resetElementsRhino();
				i_curr_steps_rhino = 0;
				interpolationRhino();
				// playRhino permanece en true para continuar el loop
			}
			else
			{
				i_curr_steps_rhino = 0;
				interpolationRhino();
			}
		}
		else
		{
		
			rhinoPosX += KeyFrameRhino[playIndexRhino].incX;
			rhinoPosY += KeyFrameRhino[playIndexRhino].incY;
			rhinoPosZ += KeyFrameRhino[playIndexRhino].incZ;
			rotRhino += KeyFrameRhino[playIndexRhino].rotRhinoInc;
			rhinoHead += KeyFrameRhino[playIndexRhino].rhinoHeadInc;

			
			bool isMoving = (KeyFrameRhino[playIndexRhino].incX != 0.0f) ||
				(KeyFrameRhino[playIndexRhino].incZ != 0.0f);

			if (isMoving)
			{
				

				float time = (float)glfwGetTime(); 

		
				float legFrequency = 15.0f; 
				float legAmplitude = 45.0f; 

				rhinoFLegs = glm::sin(time * legFrequency) * legAmplitude;
				rhinoBLegs = -glm::sin(time * legFrequency) * legAmplitude; 
			}
			else
			{
				
				rhinoFLegs += KeyFrameRhino[playIndexRhino].rhinoFLegsInc;
				rhinoBLegs += KeyFrameRhino[playIndexRhino].rhinoBLegsInc;
			}
			i_curr_steps_rhino++;
		}
	}
}
////keyframes
//void AnimationRhino()
//{
//	if (playRhino)
//	{
//		if (i_curr_steps_rhino >= i_max_steps_rhino)
//		{
//			playIndexRhino++;
//			if (playIndexRhino > FrameIndexRhino - 2)
//			{
//				// En lugar de detener la animación, reiniciarla para hacer loop
//				printf("Ciclo de animacion completado. Reiniciando...\n");
//				playIndexRhino = 0;
//				resetElementsRhino();
//				i_curr_steps_rhino = 0;
//				interpolationRhino();
//				// playRhino permanece en true para continuar el loop
//			}
//			else
//			{
//				i_curr_steps_rhino = 0;
//				interpolationRhino();
//			}
//		}
//		else
//		{
//			rhinoPosX += KeyFrameRhino[playIndexRhino].incX;
//			rhinoPosY += KeyFrameRhino[playIndexRhino].incY;
//			rhinoPosZ += KeyFrameRhino[playIndexRhino].incZ;
//			rotRhino += KeyFrameRhino[playIndexRhino].rotRhinoInc;
//			rhinoHead += KeyFrameRhino[playIndexRhino].rhinoHeadInc;
//			rhinoFLegs += KeyFrameRhino[playIndexRhino].rhinoFLegsInc;
//			rhinoBLegs += KeyFrameRhino[playIndexRhino].rhinoBLegsInc;
//			i_curr_steps_rhino++;
//		}
//	}
//}

#define MAX_FRAMES_ZEBRA 9
int i_max_steps_zebra = 190;
int i_curr_steps_zebra = 0;

typedef struct _frameZebra {
	float rotZebra, rotZebraInc;
	float zebraPosX;
	float zebraPosY;
	float zebraPosZ;
	float incX;
	float incY;
	float incZ;
	float zebraHead;
	float zebraHeadInc;
	float zebraFLegs;
	float zebraFLegsInc;
	float zebraBLegs;
	float zebraBLegsInc;
	float zebraTail;
	float zebraTailInc;
} FRAME_ZEBRA;

FRAME_ZEBRA KeyFrameZebra[MAX_FRAMES_ZEBRA];
int FrameIndexZebra = 0;
bool playZebra = false;
int playIndexZebra = 0;

void saveFrameZebra(void)
{
	printf("Zebra KeyFrame %d guardado\n", FrameIndexZebra);
	KeyFrameZebra[FrameIndexZebra].zebraPosX = zebraPosX;
	KeyFrameZebra[FrameIndexZebra].zebraPosY = zebraPosY;
	KeyFrameZebra[FrameIndexZebra].zebraPosZ = zebraPosZ;
	KeyFrameZebra[FrameIndexZebra].rotZebra = rotZebra;
	KeyFrameZebra[FrameIndexZebra].zebraHead = zebraHead;
	KeyFrameZebra[FrameIndexZebra].zebraFLegs = zebraFLegs;
	KeyFrameZebra[FrameIndexZebra].zebraBLegs = zebraBLegs;
	KeyFrameZebra[FrameIndexZebra].zebraTail = zebraTail;
	FrameIndexZebra++;
}

void resetElementsZebra(void)
{
	zebraPosX = KeyFrameZebra[0].zebraPosX;
	zebraPosY = KeyFrameZebra[0].zebraPosY;
	zebraPosZ = KeyFrameZebra[0].zebraPosZ;
	rotZebra = KeyFrameZebra[0].rotZebra;
	zebraHead = KeyFrameZebra[0].zebraHead;
	zebraFLegs = KeyFrameZebra[0].zebraFLegs;
	zebraBLegs = KeyFrameZebra[0].zebraBLegs;
	zebraTail = KeyFrameZebra[0].zebraTail;
}

void interpolationZebra(void)
{
	KeyFrameZebra[playIndexZebra].incX = (KeyFrameZebra[playIndexZebra + 1].zebraPosX - KeyFrameZebra[playIndexZebra].zebraPosX) / i_max_steps_zebra;
	KeyFrameZebra[playIndexZebra].incY = (KeyFrameZebra[playIndexZebra + 1].zebraPosY - KeyFrameZebra[playIndexZebra].zebraPosY) / i_max_steps_zebra;
	KeyFrameZebra[playIndexZebra].incZ = (KeyFrameZebra[playIndexZebra + 1].zebraPosZ - KeyFrameZebra[playIndexZebra].zebraPosZ) / i_max_steps_zebra;
	KeyFrameZebra[playIndexZebra].rotZebraInc = (KeyFrameZebra[playIndexZebra + 1].rotZebra - KeyFrameZebra[playIndexZebra].rotZebra) / i_max_steps_zebra;
	KeyFrameZebra[playIndexZebra].zebraHeadInc = (KeyFrameZebra[playIndexZebra + 1].zebraHead - KeyFrameZebra[playIndexZebra].zebraHead) / i_max_steps_zebra;
	KeyFrameZebra[playIndexZebra].zebraFLegsInc = (KeyFrameZebra[playIndexZebra + 1].zebraFLegs - KeyFrameZebra[playIndexZebra].zebraFLegs) / i_max_steps_zebra;
	KeyFrameZebra[playIndexZebra].zebraBLegsInc = (KeyFrameZebra[playIndexZebra + 1].zebraBLegs - KeyFrameZebra[playIndexZebra].zebraBLegs) / i_max_steps_zebra;
	KeyFrameZebra[playIndexZebra].zebraTailInc = (KeyFrameZebra[playIndexZebra + 1].zebraTail - KeyFrameZebra[playIndexZebra].zebraTail) / i_max_steps_zebra;
}

void AnimationZebra()
{
	if (playZebra)
	{
		if (i_curr_steps_zebra >= i_max_steps_zebra)
		{
			playIndexZebra++;
			if (playIndexZebra > FrameIndexZebra - 2)
			{
				printf("Animacion Zebra terminada\n");
				playIndexZebra = 0;
				playZebra = false;
			}
			else
			{
				i_curr_steps_zebra = 0;
				interpolationZebra();
			}
		}
		else
		{
			zebraPosX += KeyFrameZebra[playIndexZebra].incX;
			zebraPosY += KeyFrameZebra[playIndexZebra].incY;
			zebraPosZ += KeyFrameZebra[playIndexZebra].incZ;
			rotZebra += KeyFrameZebra[playIndexZebra].rotZebraInc;
			zebraHead += KeyFrameZebra[playIndexZebra].zebraHeadInc;
			zebraFLegs += KeyFrameZebra[playIndexZebra].zebraFLegsInc;
			zebraBLegs += KeyFrameZebra[playIndexZebra].zebraBLegsInc;
			zebraTail += KeyFrameZebra[playIndexZebra].zebraTailInc;
			i_curr_steps_zebra++;
		}
	}
}

#define MAX_FRAMES_WOLF 9
int i_max_steps_wolf = 190;
int i_curr_steps_wolf = 0;

typedef struct _frameWolf {
	float rotWolf, rotWolfInc;
	float wolfPosX;
	float wolfPosY;
	float wolfPosZ;
	float incX;
	float incY;
	float incZ;
	float wolfHead;
	float wolfHeadInc;
	float wolfFLegs;
	float wolfFLegsInc;
	float wolfBLegs;
	float wolfBLegsInc;
	float wolfTail;
	float wolfTailInc;
} FRAME_WOLF;

FRAME_WOLF KeyFrameWolf[MAX_FRAMES_WOLF];
int FrameIndexWolf = 0;
bool playWolf = false;
int playIndexWolf = 0;

#define MAX_FRAMES_LION 9
int i_max_steps_lion = 190;
int i_curr_steps_lion = 0;

typedef struct _frameLion {
	float rotLion, rotLionInc;
	float lionPosX;
	float lionPosY;
	float lionPosZ;
	float incX;
	float incY;
	float incZ;
	float lionHead;
	float lionHeadInc;
	float lionFLegs;
	float lionFLegsInc;
	float lionBLegs;
	float lionBLegsInc;
	float lionTail;
	float lionTailInc;
} FRAME_LION;

FRAME_LION KeyFrameLion[MAX_FRAMES_LION];
int FrameIndexLion = 0;
bool playLion = false;
int playIndexLion = 0;

void saveFrameWolf(void)
{
	printf("Wolf KeyFrame %d guardado\n", FrameIndexWolf);
	KeyFrameWolf[FrameIndexWolf].wolfPosX = wolfPosX;
	KeyFrameWolf[FrameIndexWolf].wolfPosY = wolfPosY;
	KeyFrameWolf[FrameIndexWolf].wolfPosZ = wolfPosZ;
	KeyFrameWolf[FrameIndexWolf].rotWolf = rotWolf;
	KeyFrameWolf[FrameIndexWolf].wolfHead = wolfHead;
	KeyFrameWolf[FrameIndexWolf].wolfFLegs = wolfFLegs;
	KeyFrameWolf[FrameIndexWolf].wolfBLegs = wolfBLegs;
	KeyFrameWolf[FrameIndexWolf].wolfTail = wolfTail;
	FrameIndexWolf++;
}

void resetElementsWolf(void)
{
	wolfPosX = KeyFrameWolf[0].wolfPosX;
	wolfPosY = KeyFrameWolf[0].wolfPosY;
	wolfPosZ = KeyFrameWolf[0].wolfPosZ;
	rotWolf = KeyFrameWolf[0].rotWolf;
	wolfHead = KeyFrameWolf[0].wolfHead;
	wolfFLegs = KeyFrameWolf[0].wolfFLegs;
	wolfBLegs = KeyFrameWolf[0].wolfBLegs;
	wolfTail = KeyFrameWolf[0].wolfTail;
}

void interpolationWolf(void)
{
	KeyFrameWolf[playIndexWolf].incX = (KeyFrameWolf[playIndexWolf + 1].wolfPosX - KeyFrameWolf[playIndexWolf].wolfPosX) / i_max_steps_wolf;
	KeyFrameWolf[playIndexWolf].incY = (KeyFrameWolf[playIndexWolf + 1].wolfPosY - KeyFrameWolf[playIndexWolf].wolfPosY) / i_max_steps_wolf;
	KeyFrameWolf[playIndexWolf].incZ = (KeyFrameWolf[playIndexWolf + 1].wolfPosZ - KeyFrameWolf[playIndexWolf].wolfPosZ) / i_max_steps_wolf;
	KeyFrameWolf[playIndexWolf].rotWolfInc = (KeyFrameWolf[playIndexWolf + 1].rotWolf - KeyFrameWolf[playIndexWolf].rotWolf) / i_max_steps_wolf;
	KeyFrameWolf[playIndexWolf].wolfHeadInc = (KeyFrameWolf[playIndexWolf + 1].wolfHead - KeyFrameWolf[playIndexWolf].wolfHead) / i_max_steps_wolf;
	KeyFrameWolf[playIndexWolf].wolfFLegsInc = (KeyFrameWolf[playIndexWolf + 1].wolfFLegs - KeyFrameWolf[playIndexWolf].wolfFLegs) / i_max_steps_wolf;
	KeyFrameWolf[playIndexWolf].wolfBLegsInc = (KeyFrameWolf[playIndexWolf + 1].wolfBLegs - KeyFrameWolf[playIndexWolf].wolfBLegs) / i_max_steps_wolf;
	KeyFrameWolf[playIndexWolf].wolfTailInc = (KeyFrameWolf[playIndexWolf + 1].wolfTail - KeyFrameWolf[playIndexWolf].wolfTail) / i_max_steps_wolf;
}

void AnimationWolf()
{
	if (playWolf)
	{
		if (i_curr_steps_wolf >= i_max_steps_wolf)
		{
			playIndexWolf++;
			if (playIndexWolf > FrameIndexWolf - 2)
			{
				printf("Animacion Wolf terminada\n");
				playIndexWolf = 0;
				playWolf = false;
			}
			else
			{
				i_curr_steps_wolf = 0;
				interpolationWolf();
			}
		}
		else
		{
			wolfPosX += KeyFrameWolf[playIndexWolf].incX;
			wolfPosY += KeyFrameWolf[playIndexWolf].incY;
			wolfPosZ += KeyFrameWolf[playIndexWolf].incZ;
			rotWolf += KeyFrameWolf[playIndexWolf].rotWolfInc;
			wolfHead += KeyFrameWolf[playIndexWolf].wolfHeadInc;
			wolfFLegs += KeyFrameWolf[playIndexWolf].wolfFLegsInc;
			wolfBLegs += KeyFrameWolf[playIndexWolf].wolfBLegsInc;
			wolfTail += KeyFrameWolf[playIndexWolf].wolfTailInc;
			i_curr_steps_wolf++;
		}
	}
}

void saveFrameLion(void)
{
	printf("Lion KeyFrame %d guardado\n", FrameIndexLion);
	KeyFrameLion[FrameIndexLion].lionPosX = lionPosX;
	KeyFrameLion[FrameIndexLion].lionPosY = lionPosY;
	KeyFrameLion[FrameIndexLion].lionPosZ = lionPosZ;
	KeyFrameLion[FrameIndexLion].rotLion = rotLion;
	KeyFrameLion[FrameIndexLion].lionHead = lionHead;
	KeyFrameLion[FrameIndexLion].lionFLegs = lionFLegs;
	KeyFrameLion[FrameIndexLion].lionBLegs = lionBLegs;
	KeyFrameLion[FrameIndexLion].lionTail = lionTail;
	FrameIndexLion++;
}

void resetElementsLion(void)
{
	lionPosX = KeyFrameLion[0].lionPosX;
	lionPosY = KeyFrameLion[0].lionPosY;
	lionPosZ = KeyFrameLion[0].lionPosZ;
	rotLion = KeyFrameLion[0].rotLion;
	lionHead = KeyFrameLion[0].lionHead;
	lionFLegs = KeyFrameLion[0].lionFLegs;
	lionBLegs = KeyFrameLion[0].lionBLegs;
	lionTail = KeyFrameLion[0].lionTail;
}

void interpolationLion(void)
{
	KeyFrameLion[playIndexLion].incX = (KeyFrameLion[playIndexLion + 1].lionPosX - KeyFrameLion[playIndexLion].lionPosX) / i_max_steps_lion;
	KeyFrameLion[playIndexLion].incY = (KeyFrameLion[playIndexLion + 1].lionPosY - KeyFrameLion[playIndexLion].lionPosY) / i_max_steps_lion;
	KeyFrameLion[playIndexLion].incZ = (KeyFrameLion[playIndexLion + 1].lionPosZ - KeyFrameLion[playIndexLion].lionPosZ) / i_max_steps_lion;
	KeyFrameLion[playIndexLion].rotLionInc = (KeyFrameLion[playIndexLion + 1].rotLion - KeyFrameLion[playIndexLion].rotLion) / i_max_steps_lion;
	KeyFrameLion[playIndexLion].lionHeadInc = (KeyFrameLion[playIndexLion + 1].lionHead - KeyFrameLion[playIndexLion].lionHead) / i_max_steps_lion;
	KeyFrameLion[playIndexLion].lionFLegsInc = (KeyFrameLion[playIndexLion + 1].lionFLegs - KeyFrameLion[playIndexLion].lionFLegs) / i_max_steps_lion;
	KeyFrameLion[playIndexLion].lionBLegsInc = (KeyFrameLion[playIndexLion + 1].lionBLegs - KeyFrameLion[playIndexLion].lionBLegs) / i_max_steps_lion;
	KeyFrameLion[playIndexLion].lionTailInc = (KeyFrameLion[playIndexLion + 1].lionTail - KeyFrameLion[playIndexLion].lionTail) / i_max_steps_lion;
}

void AnimationLion()
{
	if (playLion)
	{
		if (i_curr_steps_lion >= i_max_steps_lion)
		{
			playIndexLion++;
			if (playIndexLion > FrameIndexLion - 2)
			{
				printf("Animacion Lion terminada\n");
				playIndexLion = 0;
				playLion = false;
			}
			else
			{
				i_curr_steps_lion = 0;
				interpolationLion();
			}
		}
		else
		{
			lionPosX += KeyFrameLion[playIndexLion].incX;
			lionPosY += KeyFrameLion[playIndexLion].incY;
			lionPosZ += KeyFrameLion[playIndexLion].incZ;
			rotLion += KeyFrameLion[playIndexLion].rotLionInc;
			lionHead += KeyFrameLion[playIndexLion].lionHeadInc;
			lionFLegs += KeyFrameLion[playIndexLion].lionFLegsInc;
			lionBLegs += KeyFrameLion[playIndexLion].lionBLegsInc;
			lionTail += KeyFrameLion[playIndexLion].lionTailInc;
			i_curr_steps_lion++;
		}
	}
}

#define MAX_FRAMES_DEER 9
int i_max_steps_deer = 190;
int i_curr_steps_deer = 0;

typedef struct _frameDeer {
	float rotDeer, rotDeerInc;
	float deerPosX;
	float deerPosY;
	float deerPosZ;
	float incX;
	float incY;
	float incZ;
	float deerHead;
	float deerHeadInc;
	float deerFLegs;
	float deerFLegsInc;
	float deerBLegs;
	float deerBLegsInc;
} FRAME_DEER;

FRAME_DEER KeyFrameDeer[MAX_FRAMES_DEER];
int FrameIndexDeer = 0;
bool playDeer = false;
int playIndexDeer = 0;

void saveFrameDeer(void)
{
	printf("Deer KeyFrame %d guardado\n", FrameIndexDeer);
	KeyFrameDeer[FrameIndexDeer].deerPosX = deerPosX;
	KeyFrameDeer[FrameIndexDeer].deerPosY = deerPosY;
	KeyFrameDeer[FrameIndexDeer].deerPosZ = deerPosZ;
	KeyFrameDeer[FrameIndexDeer].rotDeer = rotDeer;
	KeyFrameDeer[FrameIndexDeer].deerHead = deerHead;
	KeyFrameDeer[FrameIndexDeer].deerFLegs = deerFLegs;
	KeyFrameDeer[FrameIndexDeer].deerBLegs = deerBLegs;
	FrameIndexDeer++;
}

void resetElementsDeer(void)
{
	deerPosX = KeyFrameDeer[0].deerPosX;
	deerPosY = KeyFrameDeer[0].deerPosY;
	deerPosZ = KeyFrameDeer[0].deerPosZ;
	rotDeer = KeyFrameDeer[0].rotDeer;
	deerHead = KeyFrameDeer[0].deerHead;
	deerFLegs = KeyFrameDeer[0].deerFLegs;
	deerBLegs = KeyFrameDeer[0].deerBLegs;
}

void interpolationDeer(void)
{
	KeyFrameDeer[playIndexDeer].incX = (KeyFrameDeer[playIndexDeer + 1].deerPosX - KeyFrameDeer[playIndexDeer].deerPosX) / i_max_steps_deer;
	KeyFrameDeer[playIndexDeer].incY = (KeyFrameDeer[playIndexDeer + 1].deerPosY - KeyFrameDeer[playIndexDeer].deerPosY) / i_max_steps_deer;
	KeyFrameDeer[playIndexDeer].incZ = (KeyFrameDeer[playIndexDeer + 1].deerPosZ - KeyFrameDeer[playIndexDeer].deerPosZ) / i_max_steps_deer;
	KeyFrameDeer[playIndexDeer].rotDeerInc = (KeyFrameDeer[playIndexDeer + 1].rotDeer - KeyFrameDeer[playIndexDeer].rotDeer) / i_max_steps_deer;
	KeyFrameDeer[playIndexDeer].deerHeadInc = (KeyFrameDeer[playIndexDeer + 1].deerHead - KeyFrameDeer[playIndexDeer].deerHead) / i_max_steps_deer;
	KeyFrameDeer[playIndexDeer].deerFLegsInc = (KeyFrameDeer[playIndexDeer + 1].deerFLegs - KeyFrameDeer[playIndexDeer].deerFLegs) / i_max_steps_deer;
	KeyFrameDeer[playIndexDeer].deerBLegsInc = (KeyFrameDeer[playIndexDeer + 1].deerBLegs - KeyFrameDeer[playIndexDeer].deerBLegs) / i_max_steps_deer;
}

void AnimationDeer()
{
	if (playDeer)
	{
		if (i_curr_steps_deer >= i_max_steps_deer)
		{
			playIndexDeer++;
			if (playIndexDeer > FrameIndexDeer - 2)
			{
				printf("Animacion Deer terminada\n");
				playIndexDeer = 0;
				playDeer = false;
			}
			else
			{
				i_curr_steps_deer = 0;
				interpolationDeer();
			}
		}
		else
		{
			deerPosX += KeyFrameDeer[playIndexDeer].incX;
			deerPosY += KeyFrameDeer[playIndexDeer].incY;
			deerPosZ += KeyFrameDeer[playIndexDeer].incZ;
			rotDeer += KeyFrameDeer[playIndexDeer].rotDeerInc;
			deerHead += KeyFrameDeer[playIndexDeer].deerHeadInc;
			deerFLegs += KeyFrameDeer[playIndexDeer].deerFLegsInc;
			deerBLegs += KeyFrameDeer[playIndexDeer].deerBLegsInc;
			i_curr_steps_deer++;
		}
	}
}

#define MAX_FRAMES_BEAR 9
int i_max_steps_bear = 190;
int i_curr_steps_bear = 0;

typedef struct _frameBear {
	float rotBear, rotBearInc;
	float bearPosX;
	float bearPosY;
	float bearPosZ;
	float incX;
	float incY;
	float incZ;
	float bearHead;
	float bearHeadInc;
	float bearFLegs;
	float bearFLegsInc;
	float bearBLegs;
	float bearBLegsInc;
} FRAME_BEAR;

FRAME_BEAR KeyFrameBear[MAX_FRAMES_BEAR];
int FrameIndexBear = 0;
bool playBear = false;
int playIndexBear = 0;

void saveFrameBear(void)
{
	printf("Bear KeyFrame %d guardado\n", FrameIndexBear);
	KeyFrameBear[FrameIndexBear].bearPosX = bearPosX;
	KeyFrameBear[FrameIndexBear].bearPosY = bearPosY;
	KeyFrameBear[FrameIndexBear].bearPosZ = bearPosZ;
	KeyFrameBear[FrameIndexBear].rotBear = rotBear;
	KeyFrameBear[FrameIndexBear].bearHead = bearHead;
	KeyFrameBear[FrameIndexBear].bearFLegs = bearFLegs;
	KeyFrameBear[FrameIndexBear].bearBLegs = bearBLegs;
	FrameIndexBear++;
}

void resetElementsBear(void)
{
	bearPosX = KeyFrameBear[0].bearPosX;
	bearPosY = KeyFrameBear[0].bearPosY;
	bearPosZ = KeyFrameBear[0].bearPosZ;
	rotBear = KeyFrameBear[0].rotBear;
	bearHead = KeyFrameBear[0].bearHead;
	bearFLegs = KeyFrameBear[0].bearFLegs;
	bearBLegs = KeyFrameBear[0].bearBLegs;
}

void interpolationBear(void)
{
	KeyFrameBear[playIndexBear].incX = (KeyFrameBear[playIndexBear + 1].bearPosX - KeyFrameBear[playIndexBear].bearPosX) / i_max_steps_bear;
	KeyFrameBear[playIndexBear].incY = (KeyFrameBear[playIndexBear + 1].bearPosY - KeyFrameBear[playIndexBear].bearPosY) / i_max_steps_bear;
	KeyFrameBear[playIndexBear].incZ = (KeyFrameBear[playIndexBear + 1].bearPosZ - KeyFrameBear[playIndexBear].bearPosZ) / i_max_steps_bear;
	KeyFrameBear[playIndexBear].rotBearInc = (KeyFrameBear[playIndexBear + 1].rotBear - KeyFrameBear[playIndexBear].rotBear) / i_max_steps_bear;
	KeyFrameBear[playIndexBear].bearHeadInc = (KeyFrameBear[playIndexBear + 1].bearHead - KeyFrameBear[playIndexBear].bearHead) / i_max_steps_bear;
	KeyFrameBear[playIndexBear].bearFLegsInc = (KeyFrameBear[playIndexBear + 1].bearFLegs - KeyFrameBear[playIndexBear].bearFLegs) / i_max_steps_bear;
	KeyFrameBear[playIndexBear].bearBLegsInc = (KeyFrameBear[playIndexBear + 1].bearBLegs - KeyFrameBear[playIndexBear].bearBLegs) / i_max_steps_bear;
}

void AnimationBear()
{
	if (playBear)
	{
		if (i_curr_steps_bear >= i_max_steps_bear)
		{
			playIndexBear++;
			if (playIndexBear > FrameIndexBear - 2)
			{
				printf("Animacion Bear terminada\n");
				playIndexBear = 0;
				playBear = false;
			}
		 else
			{
				i_curr_steps_bear = 0;
				interpolationBear();
			}
		}
		else
		{
			bearPosX += KeyFrameBear[playIndexBear].incX;
			bearPosY += KeyFrameBear[playIndexBear].incY;
			bearPosZ += KeyFrameBear[playIndexBear].incZ;
			rotBear += KeyFrameBear[playIndexBear].rotBearInc;
			bearHead += KeyFrameBear[playIndexBear].bearHeadInc;
			bearFLegs += KeyFrameBear[playIndexBear].bearFLegsInc;
			bearBLegs += KeyFrameBear[playIndexBear].bearBLegsInc;
			i_curr_steps_bear++;
		}
	}
}

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
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Proyecto Zoologico", nullptr, nullptr);

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
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

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
	Shader modelShader("Shader/modelLoading.vs", "Shader/modelLoading.frag");
	Shader lampshader("Shader/lamp.vs", "Shader/lamp.frag");
	Shader lightingShader("Shader/lighting.vs", "Shader/lighting.frag");
	Shader skyboxShader("Shader/skybox.vs", "Shader/skybox.frag");

	// Rutas del skybox
	std::vector<std::string> faces{
		"images/Skybox/humble_lf.jpg",
		"images/Skybox/humble_rt.jpg",
		"images/Skybox/humble_up.jpg",
		"images/Skybox/humble_dn.jpg",
		"images/Skybox/humble_ft.jpg",
		"images/Skybox/humble_bk.jpg",
	};

	// Cargar cubemap
	unsigned int cubemapTexture = loadCubemap(faces);

	// Asignar el sampler
	skyboxShader.Use();
	glUniform1i(glGetUniformLocation(skyboxShader.Program, "skybox"), 0);

	//Rinoceronte
	Model BodyRhino((char*)"Models/Rhino/BodyRhino.obj");
	Model HeadRhino((char*)"Models/Rhino/HeadRhino.obj");
	Model B_LeftLegRhino((char*)"Models/Rhino/B_LeftLegRhino.obj");
	Model B_RightLegRhino((char*)"Models/Rhino/B_RightLegRhino.obj");
	Model F_LeftLegRhino((char*)"Models/Rhino/F_LeftLegRhino.obj");
	Model F_RightLegRhino((char*)"Models/Rhino/F_RightLegRhino.obj");

	//Leon
	Model BodyLion((char*)"Models/Lion/BodyLion.obj");
	Model HeadLion((char*)"Models/Lion/HeadLion.obj");
	Model B_LeftLegLion((char*)"Models/Lion/B_LeftLegLion.obj");
	Model B_RightLegLion((char*)"Models/Lion/B_RightLegLion.obj");
	Model F_LeftLegLion((char*)"Models/Lion/F_LeftLegLion.obj");
	Model F_RightLegLion((char*)"Models/Lion/F_RightLegLion.obj");
	Model TailLion((char*)"Models/Lion/TailLion.obj");

	//Sebra
	Model BodyZebra((char*)"Models/Zebra/BodyZebra.obj");
	Model HeadZebra((char*)"Models/Zebra/HeadZebra.obj");
	Model B_LeftLegZebra((char*)"Models/Zebra/B_LeftLegZebra.obj");
	Model B_RightLegZebra((char*)"Models/Zebra/B_RightLegZebra.obj");
	Model F_LeftLegZebra((char*)"Models/Zebra/F_LeftLegZebra.obj");
	Model F_RightLegZebra((char*)"Models/Zebra/F_RightLegZebra.obj");
	Model TailZebra((char*)"Models/Zebra/TailZebra.obj");

	//Oso
	Model BodyBear((char*)"Models/Bear/BodyBear.obj");
	Model HeadBear((char*)"Models/Bear/HeadBear.obj");
	Model B_LeftLegBear((char*)"Models/Bear/B_LeftLegBear.obj");
	Model B_RightLegBear((char*)"Models/Bear/B_RightLegBear.obj");
	Model F_LeftLegBear((char*)"Models/Bear/F_LeftLegBear.obj");
	Model F_RightLegBear((char*)"Models/Bear/F_RightLegBear.obj");

	//Venado
	Model BodyDeer((char*)"Models/Deer/BodyDeer.obj");
	Model HeadDeer((char*)"Models/Deer/HeadDeer.obj");
	Model B_LeftLegDeer((char*)"Models/Deer/B_LeftLegDeer.obj");
	Model B_RightLegDeer((char*)"Models/Deer/B_RightLegDeer.obj");
	Model F_LeftLegDeer((char*)"Models/Deer/F_LeftLegDeer.obj");
	Model F_RightLegDeer((char*)"Models/Deer/F_RightLegDeer.obj");

	//Lobo
	Model BodyWolf((char*)"Models/Wolf/BodyWolf.obj");
	Model HeadWolf((char*)"Models/Wolf/HeadWolf.obj");
	Model B_LeftLegWolf((char*)"Models/Wolf/B_LeftLegWolf.obj");
	Model B_RightLegWolf((char*)"Models/Wolf/B_RightLegWolf.obj");
	Model F_LeftLegWolf((char*)"Models/Wolf/F_LeftLegWolf.obj");
	Model F_RightLegWolf((char*)"Models/Wolf/F_RightLegWolf.obj");
	Model TailWolf((char*)"Models/Wolf/TailWolf.obj");

	// Modelos de escenario
	Model wallModel((char*)"Models/Pared/fence.obj");
	Model entranceModel((char*)"Models/Entrada/Gate_obj.obj");
	Model pilar((char*)"Models/EgyptPillar/egypt_pillar.obj");
	Model reja((char*)"Models/Fence/Wire Fence.obj");
	Model Tree((char*)"Models/LotofTrees/OBJ_AS11_Chinaberry_1.obj");
	Model TreeA((char*)"Models/AppleTree/RedDeliciousApple.obj");

	//Modelos del Acuario
	Model goldFish((char*)"Models/Goldfish/Goldfish_Quad.obj");
	Model fishG((char*)"Models/FishG/Fish_Quad.obj");
	Model clownFish((char*)"Models/Clownfish/Clownfish_Quad.obj");
	Model narwal((char*)"Models/Narwal/Narwal_Quad.obj");
	Model seaTurtle((char*)"Models/Turtle/Sea_Turtle_Quad.obj");
	Model seaCrab((char*)"Models/Crab/Sea_Crab_Quad.obj");
	Model corals((char*)"Models/AlienTubes/AlienTubes.obj");

	// ===== CARGAR ANIMACIÓN PREDEFINIDA DEL RINOCERONTE =====
	loadPredefinedAnimationRhino();
	
	// Inicializar KeyFrames del rinoceronte (comentado porque usamos animación predefinida)
	/*
	for (int i = 0; i < MAX_FRAMES_RHINO; i++)
	{
		KeyFrameRhino[i].rhinoPosX = 0;
		KeyFrameRhino[i].rhinoPosY = -0.5f;
		KeyFrameRhino[i].rhinoPosZ = -5.0f;
		KeyFrameRhino[i].incX = 0;
		KeyFrameRhino[i].incY = 0;
		KeyFrameRhino[i].incZ = 0;
		KeyFrameRhino[i].rotRhino = 0;
		KeyFrameRhino[i].rotRhinoInc = 0;
		KeyFrameRhino[i].rhinoHead = 0;
		KeyFrameRhino[i].rhinoHeadInc = 0;
		KeyFrameRhino[i].rhinoFLegs = 0;
		KeyFrameRhino[i].rhinoFLegsInc = 0;
		KeyFrameRhino[i].rhinoBLegs = 0;
		KeyFrameRhino[i].rhinoBLegsInc = 0;
	}
	*/

	// Inicializar KeyFrames de la zebra
	for (int i = 0; i < MAX_FRAMES_ZEBRA; i++)
	{
		KeyFrameZebra[i].zebraPosX = -4.0f;
		KeyFrameZebra[i].zebraPosY = -0.5f;
		KeyFrameZebra[i].zebraPosZ = 2.0f;
		KeyFrameZebra[i].incX = 0;
		KeyFrameZebra[i].incY = 0;
		KeyFrameZebra[i].incZ = 0;
		KeyFrameZebra[i].rotZebra = 0;
		KeyFrameZebra[i].rotZebraInc = 0;
		KeyFrameZebra[i].zebraHead = 0;
		KeyFrameZebra[i].zebraHeadInc = 0;
		KeyFrameZebra[i].zebraFLegs = 0;
		KeyFrameZebra[i].zebraFLegsInc = 0;
		KeyFrameZebra[i].zebraBLegs = 0;
		KeyFrameZebra[i].zebraBLegsInc = 0;
		KeyFrameZebra[i].zebraTail = 0;
		KeyFrameZebra[i].zebraTailInc = 0;
	}

	// Inicializar KeyFrames del lobo
	for (int i = 0; i < MAX_FRAMES_WOLF; i++)
	{
		KeyFrameWolf[i].wolfPosX = 1.0f;
		KeyFrameWolf[i].wolfPosY = 0.0f;
		KeyFrameWolf[i].wolfPosZ = 0.0f;
		KeyFrameWolf[i].incX = 0;
		KeyFrameWolf[i].incY = 0;
		KeyFrameWolf[i].incZ = 0;
		KeyFrameWolf[i].rotWolf = 0;
		KeyFrameWolf[i].rotWolfInc = 0;
		KeyFrameWolf[i].wolfHead = 0;
		KeyFrameWolf[i].wolfHeadInc = 0;
		KeyFrameWolf[i].wolfFLegs = 0;
		KeyFrameWolf[i].wolfFLegsInc = 0;
		KeyFrameWolf[i].wolfBLegs = 0;
		KeyFrameWolf[i].wolfBLegsInc = 0;
		KeyFrameWolf[i].wolfTail = 0;
		KeyFrameWolf[i].wolfTailInc = 0;
	}

	// Inicializar KeyFrames del león
	for (int i = 0; i < MAX_FRAMES_LION; i++)
	{
		KeyFrameLion[i].lionPosX = 4.0f;
		KeyFrameLion[i].lionPosY = 0.0f;
		KeyFrameLion[i].lionPosZ = -2.0f;
		KeyFrameLion[i].incX = 0;
		KeyFrameLion[i].incY = 0;
		KeyFrameLion[i].incZ = 0;
		KeyFrameLion[i].rotLion = 0;
		KeyFrameLion[i].rotLionInc = 0;
		KeyFrameLion[i].lionHead = 0;
		KeyFrameLion[i].lionHeadInc = 0;
		KeyFrameLion[i].lionFLegs = 0;
		KeyFrameLion[i].lionFLegsInc = 0;
		KeyFrameLion[i].lionBLegs = 0;
		KeyFrameLion[i].lionBLegsInc = 0;
		KeyFrameLion[i].lionTail = 0;
		KeyFrameLion[i].lionTailInc = 0;
	}

	// Inicializar KeyFrames del venado
	for (int i = 0; i < MAX_FRAMES_DEER; i++)
	{
		KeyFrameDeer[i].deerPosX = -2.0f;
		KeyFrameDeer[i].deerPosY = -0.5f;
		KeyFrameDeer[i].deerPosZ = 5.0f;
		KeyFrameDeer[i].incX = 0;
		KeyFrameDeer[i].incY = 0;
		KeyFrameDeer[i].incZ = 0;
		KeyFrameDeer[i].rotDeer = 0;
		KeyFrameDeer[i].rotDeerInc = 0;
		KeyFrameDeer[i].deerHead = 0;
		KeyFrameDeer[i].deerHeadInc = 0;
		KeyFrameDeer[i].deerFLegs = 0;
		KeyFrameDeer[i].deerFLegsInc = 0;
		KeyFrameDeer[i].deerBLegs = 0;
		KeyFrameDeer[i].deerBLegsInc = 0;
	}

	// Inicializar KeyFrames del oso
	for (int i = 0; i < MAX_FRAMES_BEAR; i++)
	{
		KeyFrameBear[i].bearPosX = 0.0f;
		KeyFrameBear[i].bearPosY = -0.5f;
		KeyFrameBear[i].bearPosZ = -3.0f;
		KeyFrameBear[i].incX = 0;
		KeyFrameBear[i].incY = 0;
		KeyFrameBear[i].incZ = 0;
		KeyFrameBear[i].rotBear = 0;
		KeyFrameBear[i].rotBearInc = 0;
		KeyFrameBear[i].bearHead = 0;
		KeyFrameBear[i].bearHeadInc = 0;
		KeyFrameBear[i].bearFLegs = 0;
		KeyFrameBear[i].bearFLegsInc = 0;
		KeyFrameBear[i].bearBLegs = 0;
		KeyFrameBear[i].bearBLegsInc = 0;
	}

	glm::mat4 projection = glm::perspective(camera.GetZoom(), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);

	// Skybox setup
	float skyboxVertices[] = {
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f
	};

	unsigned int skyboxVAO, skyboxVBO;
	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glBindVertexArray(0);

	float floorVertices[] = {
		 100.0f, -0.5f,  100.0f,   0.0f, 1.0f, 0.0f,   100.0f, 0.0f,
		-100.0f, -0.5f,  100.0f,   0.0f, 1.0f, 0.0f,   0.0f,   0.0f,
		-100.0f, -0.5f, -100.0f,   0.0f, 1.0f, 0.0f,   0.0f,   100.0f,

		 100.0f, -0.5f,  100.0f,   0.0f, 1.0f, 0.0f,   100.0f, 0.0f,
		-100.0f, -0.5f, -100.0f,   0.0f, 1.0f, 0.0f,   0.0f,   100.0f,
		 100.0f, -0.5f, -100.0f,   0.0f, 1.0f, 0.0f,   100.0f, 100.0f
	};

	unsigned int floorVAO, floorVBO;
	glGenVertexArrays(1, &floorVAO);
	glGenBuffers(1, &floorVBO);
	glBindVertexArray(floorVAO);

	glBindBuffer(GL_ARRAY_BUFFER, floorVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(floorVertices), floorVertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);

	unsigned int floorTexture;
	glGenTextures(1, &floorTexture);
	glBindTexture(GL_TEXTURE_2D, floorTexture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load("images/grass.jpg", &width, &height, &nrChannels, 0);

	if (data)
	{
		GLenum format = (nrChannels == 4) ? GL_RGBA : GL_RGB;
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Error al cargar textura del piso" << std::endl;
	}

	stbi_image_free(data);

	// === Cubo del acuario ===
	float glassVertices[] = {
		// posiciones           // normales           // texcoords
		// Cara frontal
		-1.0f, -0.5f,  1.0f,     0.0f,  0.0f,  1.0f,   0.0f, 0.0f,
		 1.0f, -0.5f,  1.0f,     0.0f,  0.0f,  1.0f,   1.0f, 0.0f,
		 1.0f,  0.5f,  1.0f,     0.0f,  0.0f,  1.0f,   1.0f, 1.0f,
		 1.0f,  0.5f,  1.0f,     0.0f,  0.0f,  1.0f,   1.0f, 1.0f,
		-1.0f,  0.5f,  1.0f,     0.0f,  0.0f,  1.0f,   0.0f, 1.0f,
		-1.0f, -0.5f,  1.0f,     0.0f,  0.0f,  1.0f,   0.0f, 0.0f,

		// Cara trasera
		-1.0f, -0.5f, -1.0f,     0.0f,  0.0f, -1.0f,   0.0f, 0.0f,
		 1.0f, -0.5f, -1.0f,     0.0f,  0.0f, -1.0f,   1.0f, 0.0f,
		 1.0f,  0.5f, -1.0f,     0.0f,  0.0f, -1.0f,   1.0f, 1.0f,
		 1.0f,  0.5f, -1.0f,     0.0f,  0.0f, -1.0f,   1.0f, 1.0f,
		-1.0f,  0.5f, -1.0f,     0.0f,  0.0f, -1.0f,   0.0f, 1.0f,
		-1.0f, -0.5f, -1.0f,     0.0f,  0.0f, -1.0f,   0.0f, 0.0f,

		// Izquierda
		-1.0f,  0.5f,  1.0f,    -1.0f,  0.0f,  0.0f,   1.0f, 0.0f,
		-1.0f,  0.5f, -1.0f,    -1.0f,  0.0f,  0.0f,   1.0f, 1.0f,
		-1.0f, -0.5f, -1.0f,    -1.0f,  0.0f,  0.0f,   0.0f, 1.0f,
		-1.0f, -0.5f, -1.0f,    -1.0f,  0.0f,  0.0f,   0.0f, 1.0f,
		-1.0f, -0.5f,  1.0f,    -1.0f,  0.0f,  0.0f,   0.0f, 0.0f,
		-1.0f,  0.5f,  1.0f,    -1.0f,  0.0f,  0.0f,   1.0f, 0.0f,

		// Derecha
		 1.0f,  0.5f,  1.0f,     1.0f,  0.0f,  0.0f,   1.0f, 0.0f,
		 1.0f,  0.5f, -1.0f,     1.0f,  0.0f,  0.0f,   1.0f, 1.0f,
		 1.0f, -0.5f, -1.0f,     1.0f,  0.0f,  0.0f,   0.0f, 1.0f,
		 1.0f, -0.5f, -1.0f,     1.0f,  0.0f,  0.0f,   0.0f, 1.0f,
		 1.0f, -0.5f,  1.0f,     1.0f,  0.0f,  0.0f,   0.0f, 0.0f,
		 1.0f,  0.5f,  1.0f,     1.0f,  0.0f,  0.0f,   1.0f, 0.0f,

		 // Parte superior
		 -1.0f,  0.5f, -1.0f,     0.0f,  1.0f,  0.0f,   0.0f, 1.0f,
		  1.0f,  0.5f, -1.0f,     0.0f,  1.0f,  0.0f,   1.0f, 1.0f,
		  1.0f,  0.5f,  1.0f,     0.0f,  1.0f,  0.0f,   1.0f, 0.0f,
		  1.0f,  0.5f,  1.0f,     0.0f,  1.0f,  0.0f,   1.0f, 0.0f,
		 -1.0f,  0.5f,  1.0f,     0.0f,  1.0f,  0.0f,   0.0f, 0.0f,
		 -1.0f,  0.5f, -1.0f,     0.0f,  1.0f,  0.0f,   0.0f, 1.0f,

		 // Parte inferior
		 -1.0f, -0.5f, -1.0f,     0.0f, -1.0f,  0.0f,   0.0f, 1.0f,
		  1.0f, -0.5f, -1.0f,     0.0f, -1.0f,  0.0f,   1.0f, 1.0f,
		  1.0f, -0.5f,  1.0f,     0.0f, -1.0f,  0.0f,   1.0f, 0.0f,
		  1.0f, -0.5f,  1.0f,     0.0f, -1.0f,  0.0f,   1.0f, 0.0f,
		 -1.0f, -0.5f,  1.0f,     0.0f, -1.0f,  0.0f,   0.0f, 0.0f,
		 -1.0f, -0.5f, -1.0f,     0.0f, -1.0f,  0.0f,   0.0f, 1.0f
	};

	unsigned int glassVAO, glassVBO;
	glGenVertexArrays(1, &glassVAO);
	glGenBuffers(1, &glassVBO);
	glBindVertexArray(glassVAO);

	glBindBuffer(GL_ARRAY_BUFFER, glassVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glassVertices), glassVertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
	glBindVertexArray(0);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// --- Textura acuario ---
	unsigned int waterTexture;
	glGenTextures(1, &waterTexture);
	glBindTexture(GL_TEXTURE_2D, waterTexture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width1, height1, nrChannels1;
	unsigned char* data1 = stbi_load("images/water.jpg", &width1, &height1, &nrChannels1, 0);
	if (data1)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width1, height1, 0, GL_RGB, GL_UNSIGNED_BYTE, data1);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Fallo al cargar textura de agua" << std::endl;
	}
	stbi_image_free(data1);

	// Game loop
	while (!glfwWindowShouldClose(window))
	{
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		glfwPollEvents();
		DoMovement();
		AnimationRhino();
		AnimationZebra();
		AnimationWolf();
		AnimationLion();
		AnimationDeer();
		AnimationBear();

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//Dibujar Piso
		lightingShader.Use();

		glm::mat4 view = camera.GetViewMatrix();
		lightingShader.setMat4("view", view);
		lightingShader.setMat4("projection", projection);

		glm::mat4 model = glm::mat4(1.0f);
		lightingShader.setMat4("model", model);

		lightingShader.setVec3("viewPos", camera.GetPosition());

		// === Luz direccional ===
		lightingShader.setVec3("dirLight.direction", glm::vec3(-0.2f, -1.0f, -0.3f));
		lightingShader.setVec3("dirLight.ambient", glm::vec3(0.3f, 0.3f, 0.3f));
		lightingShader.setVec3("dirLight.diffuse", glm::vec3(0.8f, 0.8f, 0.8f));
		lightingShader.setVec3("dirLight.specular", glm::vec3(1.0f, 1.0f, 1.0f));

		lightingShader.setVec3("material.ambient", glm::vec3(1.0f));
		lightingShader.setVec3("material.diffuse", glm::vec3(1.0f));
		lightingShader.setVec3("material.specular", glm::vec3(0.5f));
		lightingShader.setFloat("material.shininess", 32.0f);

		lightingShader.setInt("texture_diffuse", 0);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, floorTexture);

		glBindVertexArray(floorVAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);

		//Dibujar
		modelShader.Use();
		modelShader.setMat4("view", view);
		modelShader.setMat4("projection", projection);

		//Cuerpo Rhino
		glm::mat4 modelTemp = glm::mat4(1.0f);
		modelTemp = glm::translate(modelTemp, glm::vec3(rhinoPosX, rhinoPosY, rhinoPosZ));
		modelTemp = glm::rotate(modelTemp, glm::radians(rotRhino), glm::vec3(0.0f, 1.0f, 0.0f));

		model = modelTemp;
		modelShader.setMat4("model", model);
		BodyRhino.Draw(modelShader);

		//Cabeza Rhino
		model = modelTemp;
		model = glm::translate(model, glm::vec3(0.002639f, 1.22896f, 0.903224f));
		model = glm::rotate(model, glm::radians(rhinoHead), glm::vec3(0.0f, 1.0f, 0.0f));
		modelShader.setMat4("model", model);
		HeadRhino.Draw(modelShader);

		// PATA DELANTERA IZQUIERDA Rhino
		model = modelTemp;
		model = glm::translate(model, glm::vec3(-0.357265f, 0.681231f, 0.669307f));
		model = glm::rotate(model, glm::radians(rhinoFLegs), glm::vec3(1.0f, 0.0f, 0.0f));
		modelShader.setMat4("model", model);
		F_LeftLegRhino.Draw(modelShader);

		// PATA DELANTERA DERECHA Rhino
		model = modelTemp;
		model = glm::translate(model, glm::vec3(0.36863f, 0.72705f, 0.664725f));
		model = glm::rotate(model, glm::radians(-rhinoFLegs), glm::vec3(1.0f, 0.0f, 0.0f));
		modelShader.setMat4("model", model);
		F_RightLegRhino.Draw(modelShader);

		// PATA TRASERA IZQUIERDA Rhino
		model = modelTemp;
		model = glm::translate(model, glm::vec3(-0.327688f, 0.809153f, -1.11341f));
		model = glm::rotate(model, glm::radians(-rhinoBLegs), glm::vec3(1.0f, 0.0f, 0.0f));
		modelShader.setMat4("model", model);
		B_LeftLegRhino.Draw(modelShader);

		// PATA TRASERA DERECHA Rhino
		model = modelTemp;
		model = glm::translate(model, glm::vec3(0.318784f, 0.75417f, -1.11341f));
		model = glm::rotate(model, glm::radians(rhinoBLegs), glm::vec3(1.0f, 0.0f, 0.0f));
		modelShader.setMat4("model", model);
		B_RightLegRhino.Draw(modelShader);

		// León
		glm::mat4 modelTempLion = glm::mat4(1.0f);
		modelTempLion = glm::translate(modelTempLion, glm::vec3(lionPosX, lionPosY, lionPosZ));
		modelTempLion = glm::rotate(modelTempLion, glm::radians(rotLion), glm::vec3(0.0f, 1.0f, 0.0f));

		//Cuerpo Lion
		model = modelTempLion;
		modelShader.setMat4("model", model);
		BodyLion.Draw(modelShader);

		//Cabeza Lion
		model = modelTempLion;
		model = glm::translate(model, glm::vec3(0.422815f, 0.240452f, -0.002748f));
		model = glm::rotate(model, glm::radians(lionHead), glm::vec3(0.0f, 1.0f, 0.0f));
		modelShader.setMat4("model", model);
		HeadLion.Draw(modelShader);

		// PATA DELANTERA IZQUIERDA Lion
		model = modelTempLion;
		model = glm::translate(model, glm::vec3(0.206645f, -0.110351f, 0.108954f));
		model = glm::rotate(model, glm::radians(lionFLegs), glm::vec3(0.0f, 0.0f, 1.0f));
		modelShader.setMat4("model", model);
		F_LeftLegLion.Draw(modelShader);

		// PATA DELANTERA DERECHA Lion
		model = modelTempLion;
		model = glm::translate(model, glm::vec3(0.244861f, -0.097645f, -0.109556f));
		model = glm::rotate(model, glm::radians(-lionFLegs), glm::vec3(0.0f, 0.0f, 1.0f));
		modelShader.setMat4("model", model);
		F_RightLegLion.Draw(modelShader);

		// PATA TRASERA IZQUIERDA Lion
		model = modelTempLion;
		model = glm::translate(model, glm::vec3(-0.281848f, -0.00993f, 0.08262f));
		model = glm::rotate(model, glm::radians(-lionBLegs), glm::vec3(0.0f, 0.0f, 1.0f));
		modelShader.setMat4("model", model);
		B_LeftLegLion.Draw(modelShader);

		// PATA TRASERA DERECHA Lion
		model = modelTempLion;
		model = glm::translate(model, glm::vec3(-0.251413f, -0.00014f, -0.093842f));
		model = glm::rotate(model, glm::radians(lionBLegs), glm::vec3(0.0f, 0.0f, 1.0f));
		modelShader.setMat4("model", model);
		B_RightLegLion.Draw(modelShader);

		// COLA Lion
		model = modelTempLion;
		model = glm::translate(model, glm::vec3(-0.40091f , 0.12949f, -0.010439f));
		model = glm::rotate(model, glm::radians(lionTail), glm::vec3(0.0f, 0.0f, 1.0f));
		modelShader.setMat4("model", model);
		TailLion.Draw(modelShader);

		// Cebra
		glm::mat4 modelTempZebra = glm::mat4(1.0f);
		modelTempZebra = glm::translate(modelTempZebra, glm::vec3(zebraPosX, zebraPosY, zebraPosZ));
		modelTempZebra = glm::rotate(modelTempZebra, glm::radians(rotZebra), glm::vec3(0.0f, 1.0f, 0.0f));

		//Cuerpo Cebra
		model = modelTempZebra;
		modelShader.setMat4("model", model);
		BodyZebra.Draw(modelShader);

		//Cabeza Cebra
		model = modelTempZebra;
		model = glm::translate(model, glm::vec3(0.002846f, 1.2333f, 0.48328f));
		model = glm::rotate(model, glm::radians(zebraHead), glm::vec3(1.0f, 0.0f, 0.0f));
		modelShader.setMat4("model", model);
		HeadZebra.Draw(modelShader);


		// PATA DELANTERA IZQUIERDA Zebra
		model = modelTempZebra;
		
		model = glm::translate(model, glm::vec3(-0.18725f, 0.793251f, 0.321389f));
		model = glm::rotate(model, glm::radians(zebraFLegs), glm::vec3(1.0f, 0.0f, 0.0f));
		modelShader.setMat4("model", model);
		F_LeftLegZebra.Draw(modelShader);

		// PATA DELANTERA DERECHA Zebra
		model = modelTempZebra;
		model = glm::translate(model, glm::vec3(0.187179f, 0.775141f, 0.309626f));
		model = glm::rotate(model, glm::radians(-zebraFLegs), glm::vec3(1.0f, 0.0f, 0.0f));
		modelShader.setMat4("model", model);
		F_RightLegZebra.Draw(modelShader);

		// PATA TRASERA IZQUIERDA Zebra
		model = modelTempZebra;
		model = glm::translate(model, glm::vec3(-0.156607f, 0.777266f, -0.693926f));
		model = glm::rotate(model, glm::radians(-zebraBLegs), glm::vec3(1.0f, 0.0f, 0.0f));
		modelShader.setMat4("model", model);
		B_LeftLegZebra.Draw(modelShader);

		// PATA TRASERA DERECHA Zebra
		model = modelTempZebra;
		model = glm::translate(model, glm::vec3(0.162712f, 0.772383f, -0.699344f));
		model = glm::rotate(model, glm::radians(zebraBLegs), glm::vec3(1.0f, 0.0f, 0.0f));
		modelShader.setMat4("model", model);
		B_RightLegZebra.Draw(modelShader);

		// COLA Zebra
		model = modelTempZebra;
		model = glm::translate(model, glm::vec3(-0.003215f, 0.972246f, -0.891742f));
		model = glm::rotate(model, glm::radians(zebraTail), glm::vec3(1.0f, 0.0f, 0.0f));
		modelShader.setMat4("model", model);
		TailZebra.Draw(modelShader);

		//Venado (Deer)
		glm::mat4 modelTempDeer = glm::mat4(1.0f);
		modelTempDeer = glm::translate(modelTempDeer, glm::vec3(deerPosX, deerPosY, deerPosZ));
		modelTempDeer = glm::rotate(modelTempDeer, glm::radians(rotDeer), glm::vec3(0.0f, 1.0f, 0.0f));

		//Cuerpo Deer
		model = modelTempDeer;
		modelShader.setMat4("model", model);
		BodyDeer.Draw(modelShader);

		//Cabeza Deer
		model = modelTempDeer;
		//-0.446126f
		model = glm::translate(model, glm::vec3(0.004444f, 0.934631f, 0.446126f));
		model = glm::rotate(model, glm::radians(deerHead), glm::vec3(0.0f, 1.0f, 0.0f));
		modelShader.setMat4("model", model);
		HeadDeer.Draw(modelShader);

		// PATA DELANTERA IZQUIERDA Deer
		model = modelTempDeer;
		model = glm::translate(model, glm::vec3(-0.124788f, 0.661562f, 0.233486f));
		model = glm::rotate(model, glm::radians(deerFLegs), glm::vec3(1.0f, 0.0f, 0.0f));
		modelShader.setMat4("model", model);
		F_LeftLegDeer.Draw(modelShader);

		// PATA DELANTERA DERECHA Deer
		model = modelTempDeer;
		model = glm::translate(model, glm::vec3(0.118221f, 0.646354f, 0.233681f));
		model = glm::rotate(model, glm::radians(-deerFLegs), glm::vec3(1.0f, 0.0f, 0.0f));
		modelShader.setMat4("model", model);
		F_RightLegDeer.Draw(modelShader);

		// PATA TRASERA IZQUIERDA Deer
		model = modelTempDeer;
		model = glm::translate(model, glm::vec3(-0.116149f, 0.670526f, -0.384063f));
		model = glm::rotate(model, glm::radians(-deerBLegs), glm::vec3(1.0f, 0.0f, 0.0f));
		modelShader.setMat4("model", model);
		B_LeftLegDeer.Draw(modelShader);

		// PATA TRASERA DERECHA Deer
		model = modelTempDeer;
		model = glm::translate(model, glm::vec3(0.112376f, 0.654912f, -0.361251f));
		model = glm::rotate(model, glm::radians(deerBLegs), glm::vec3(1.0f, 0.0f, 0.0f));
		modelShader.setMat4("model", model);
		B_RightLegDeer.Draw(modelShader);

		//Lobo (Wolf)
		glm::mat4 modelTempWolf = glm::mat4(1.0f);
		modelTempWolf = glm::translate(modelTempWolf, glm::vec3(wolfPosX, wolfPosY, wolfPosZ));
		modelTempWolf = glm::rotate(modelTempWolf, glm::radians(rotWolf), glm::vec3(0.0f, 1.0f, 0.0f));

		//Cuerpo Wolf
		model = modelTempWolf;
		modelShader.setMat4("model", model);
		BodyWolf.Draw(modelShader);

		//Cabeza Wolf
		model = modelTempWolf;
		model = glm::translate(model, glm::vec3(0.323833f, 0.21796f, 0.006356f));
		model = glm::rotate(model, glm::radians(wolfHead), glm::vec3(0.0f, 1.0f, 0.0f));
		modelShader.setMat4("model", model);
		HeadWolf.Draw(modelShader);

		// PATA DELANTERA IZQUIERDA Wolf
		model = modelTempWolf;
		model = glm::translate(model, glm::vec3(0.189673f, -0.062067f, 0.122766f));
		model = glm::rotate(model, glm::radians(wolfFLegs), glm::vec3(0.0f, 0.0f, 1.0f));
		modelShader.setMat4("model", model);
		F_LeftLegWolf.Draw(modelShader);

		// PATA DELANTERA DERECHA Wolf
		model = modelTempWolf;
		model = glm::translate(model, glm::vec3(0.20445f, -0.065344f, -0.109262f));
		model = glm::rotate(model, glm::radians(-wolfFLegs), glm::vec3(0.0f, 0.0f, 1.0f));
		modelShader.setMat4("model", model);
		F_RightLegWolf.Draw(modelShader);

		// PATA TRASERA IZQUIERDA Wolf
		model = modelTempWolf;
		model = glm::translate(model, glm::vec3(-0.30265f, -0.062009f, 0.115517f));
		model = glm::rotate(model, glm::radians(-wolfBLegs), glm::vec3(0.0f, 0.0f, 1.0f));
		modelShader.setMat4("model", model);
		B_LeftLegWolf.Draw(modelShader);

		// PATA TRASERA DERECHA Wolf
		model = modelTempWolf;
		model = glm::translate(model, glm::vec3(-0.312346f, -0.073358f, -0.121459f));
		model = glm::rotate(model, glm::radians(wolfBLegs), glm::vec3(0.0f, 0.0f, 1.0f));
		modelShader.setMat4("model", model);
		B_RightLegWolf.Draw(modelShader);

		// COLA Wolf
		model = modelTempWolf;
		model = glm::translate(model, glm::vec3(-0.414132f, 0.064711f, 0.005788f));
		model = glm::rotate(model, glm::radians(wolfTail), glm::vec3(0.0f, 0.0f, 1.0f));
		modelShader.setMat4("model", model);
		TailWolf.Draw(modelShader);

		//Oso (Bear)
		glm::mat4 modelTempBear = glm::mat4(1.0f);
		modelTempBear = glm::translate(modelTempBear, glm::vec3(bearPosX, bearPosY, bearPosZ));
		modelTempBear = glm::rotate(modelTempBear, glm::radians(rotBear), glm::vec3(0.0f, 1.0f, 0.0f));

		//Cuerpo Bear
		model = modelTempBear;
		modelShader.setMat4("model", model);
		BodyBear.Draw(modelShader);

		//Cabeza Bear
		model = modelTempBear;
		model = glm::translate(model, glm::vec3(-0.002199f, 0.973857f, 0.529054f));
		model = glm::rotate(model, glm::radians(bearHead), glm::vec3(0.0f, 1.0f, 0.0f));
		modelShader.setMat4("model", model);
		HeadBear.Draw(modelShader);

		// PATA DELANTERA IZQUIERDA Bear
		model = modelTempBear;
		model = glm::translate(model, glm::vec3(-0.263069f, 0.614662f, 0.224646f));
		model = glm::rotate(model, glm::radians(bearFLegs), glm::vec3(1.0f, 0.0f, 0.0f));
		modelShader.setMat4("model", model);
		F_LeftLegBear.Draw(modelShader);

		// PATA DELANTERA DERECHA Bear
		model = modelTempBear;
		model = glm::translate(model, glm::vec3(0.27788f, 0.618621f, 0.241813f));
		model = glm::rotate(model, glm::radians(-bearFLegs), glm::vec3(1.0f, 0.0f, 0.0f));
		modelShader.setMat4("model", model);
		F_RightLegBear.Draw(modelShader);

		// PATA TRASERA IZQUIERDA Bear
		model = modelTempBear;
		model = glm::translate(model, glm::vec3(-0.261244f, 0.621932f, -0.551798f));
		model = glm::rotate(model, glm::radians(-bearBLegs), glm::vec3(1.0f, 0.0f, 0.0f));
		modelShader.setMat4("model", model);
		B_LeftLegBear.Draw(modelShader);

		// PATA TRASERA DERECHA Bear
		model = modelTempBear;
		model = glm::translate(model, glm::vec3(0.250998f, 0.596885f, -0.541412f));
		model = glm::rotate(model, glm::radians(bearBLegs), glm::vec3(1.0f, 0.0f, 0.0f));
		modelShader.setMat4("model", model);
		B_RightLegBear.Draw(modelShader);

		//Dibujar Paredes y Entrada (desde el segundo código)
		modelTemp = glm::mat4(1.0f);

		// Entrada
		glm::mat4 ent = modelTemp;
		ent = glm::translate(ent, glm::vec3(2.8f, -0.55f, 25.5f));
		ent = glm::scale(ent, glm::vec3(0.1f));
		modelShader.setMat4("model", ent);
		entranceModel.Draw(modelShader);

		// Paredes Entrada
		glm::mat4 wall1 = modelTemp;
		wall1 = glm::translate(wall1, glm::vec3(12.0f, -0.5f, 25.5f));
		wall1 = glm::scale(wall1, glm::vec3(2.5f));
		modelShader.setMat4("model", wall1);
		wallModel.Draw(modelShader);

		glm::mat4 wall2 = modelTemp;
		wall2 = glm::translate(wall2, glm::vec3(19.0f, -0.5f, 25.5f));
		wall2 = glm::scale(wall2, glm::vec3(2.5f));
		modelShader.setMat4("model", wall2);
		wallModel.Draw(modelShader);

		glm::mat4 wall8 = modelTemp;
		wall8 = glm::translate(wall8, glm::vec3(-6.5f, -0.5f, 25.5f));
		wall8 = glm::scale(wall8, glm::vec3(2.5f));
		modelShader.setMat4("model", wall8);
		wallModel.Draw(modelShader);

		glm::mat4 wall9 = modelTemp;
		wall9 = glm::translate(wall9, glm::vec3(-13.5f, -0.5f, 25.5f));
		wall9 = glm::scale(wall9, glm::vec3(2.5f));
		modelShader.setMat4("model", wall9);
		wallModel.Draw(modelShader);

		// Paredes Izquierdas
		glm::mat4 wall3 = modelTemp;
		wall3 = glm::translate(wall3, glm::vec3(22.5f, -0.5f, 22.0f));
		wall3 = glm::rotate(wall3, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		wall3 = glm::scale(wall3, glm::vec3(2.5f));
		modelShader.setMat4("model", wall3);
		wallModel.Draw(modelShader);

		glm::mat4 wall4 = modelTemp;
		wall4 = glm::translate(wall4, glm::vec3(22.5f, -0.5f, 15.0f));
		wall4 = glm::rotate(wall4, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		wall4 = glm::scale(wall4, glm::vec3(2.5f));
		modelShader.setMat4("model", wall4);
		wallModel.Draw(modelShader);

		glm::mat4 wall5 = modelTemp;
		wall5 = glm::translate(wall5, glm::vec3(22.5f, -0.5f, 8.0f));
		wall5 = glm::rotate(wall5, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		wall5 = glm::scale(wall5, glm::vec3(2.5f));
		modelShader.setMat4("model", wall5);
		wallModel.Draw(modelShader);

		glm::mat4 wall6 = modelTemp;
		wall6 = glm::translate(wall6, glm::vec3(22.5f, -0.5f, 1.0f));
		wall6 = glm::rotate(wall6, glm::radians(90.5f), glm::vec3(0.0f, 1.0f, 0.0f));
		wall6 = glm::scale(wall6, glm::vec3(2.5f));
		modelShader.setMat4("model", wall6);
		wallModel.Draw(modelShader);

		glm::mat4 wall7 = modelTemp;
		wall7 = glm::translate(wall7, glm::vec3(22.5f, -0.5f, -6.0f));
		wall7 = glm::rotate(wall7, glm::radians(90.5f), glm::vec3(0.0f, 1.0f, 0.0f));
		wall7 = glm::scale(wall7, glm::vec3(2.5f));
		modelShader.setMat4("model", wall7);
		wallModel.Draw(modelShader);

		glm::mat4 wall15 = modelTemp;
		wall15 = glm::translate(wall15, glm::vec3(22.5f, -0.5f, -13.0f));
		wall15 = glm::rotate(wall15, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		wall15 = glm::scale(wall15, glm::vec3(2.5f));
		modelShader.setMat4("model", wall15);
		wallModel.Draw(modelShader);

		glm::mat4 wall16 = modelTemp;
		wall16 = glm::translate(wall16, glm::vec3(22.5f, -0.5f, -20.0f));
		wall16 = glm::rotate(wall16, glm::radians(90.5f), glm::vec3(0.0f, 1.0f, 0.0f));
		wall16 = glm::scale(wall16, glm::vec3(2.5f));
		modelShader.setMat4("model", wall16);
		wallModel.Draw(modelShader);

		glm::mat4 wall17 = modelTemp;
		wall17 = glm::translate(wall17, glm::vec3(22.5f, -0.5f, -27.0f));
		wall17 = glm::rotate(wall17, glm::radians(90.5f), glm::vec3(0.0f, 1.0f, 0.0f));
		wall17 = glm::scale(wall17, glm::vec3(2.5f));
		modelShader.setMat4("model", wall17);
		wallModel.Draw(modelShader);

		// Paredes Derechas
		glm::mat4 wall10 = modelTemp;
		wall10 = glm::translate(wall10, glm::vec3(-17.0f, -0.5f, 22.0f));
		wall10 = glm::rotate(wall10, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		wall10 = glm::scale(wall10, glm::vec3(2.5f));
		modelShader.setMat4("model", wall10);
		wallModel.Draw(modelShader);

		glm::mat4 wall11 = modelTemp;
		wall11 = glm::translate(wall11, glm::vec3(-17.0f, -0.5f, 15.0f));
		wall11 = glm::rotate(wall11, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		wall11 = glm::scale(wall11, glm::vec3(2.5f));
		modelShader.setMat4("model", wall11);
		wallModel.Draw(modelShader);

		glm::mat4 wall12 = modelTemp;
		wall12 = glm::translate(wall12, glm::vec3(-17.0f, -0.5f, 8.0f));
		wall12 = glm::rotate(wall12, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		wall12 = glm::scale(wall12, glm::vec3(2.5f));
		modelShader.setMat4("model", wall12);
		wallModel.Draw(modelShader);

		glm::mat4 wall13 = modelTemp;
		wall13 = glm::translate(wall13, glm::vec3(-17.0f, -0.5f, 1.0f));
		wall13 = glm::rotate(wall13, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		wall13 = glm::scale(wall13, glm::vec3(2.5f));
		modelShader.setMat4("model", wall13);
		wallModel.Draw(modelShader);

		glm::mat4 wall14 = modelTemp;
		wall14 = glm::translate(wall14, glm::vec3(-17.0f, -0.5f, -6.0f));
		wall14 = glm::rotate(wall14, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		wall14 = glm::scale(wall14, glm::vec3(2.5f));
		modelShader.setMat4("model", wall14);
		wallModel.Draw(modelShader);

		glm::mat4 wall18 = modelTemp;
		wall18 = glm::translate(wall18, glm::vec3(-17.0f, -0.5f, -13.0f));
		wall18 = glm::rotate(wall18, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		wall18 = glm::scale(wall18, glm::vec3(2.5f));
		modelShader.setMat4("model", wall18);
		wallModel.Draw(modelShader);

		glm::mat4 wall19 = modelTemp;
		wall19 = glm::translate(wall19, glm::vec3(-17.0f, -0.5f, -20.0f));
		wall19 = glm::rotate(wall19, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		wall19 = glm::scale(wall19, glm::vec3(2.5f));
		modelShader.setMat4("model", wall19);
		wallModel.Draw(modelShader);

		glm::mat4 wall20 = modelTemp;
		wall20 = glm::translate(wall20, glm::vec3(-17.0f, -0.5f, -27.0f));
		wall20 = glm::rotate(wall20, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		wall20 = glm::scale(wall20, glm::vec3(2.5f));
		modelShader.setMat4("model", wall20);
		wallModel.Draw(modelShader);

		//Paredes del Fondo
		glm::mat4 wall21 = modelTemp;
		wall21 = glm::translate(wall21, glm::vec3(12.0f, -0.5f, -30.5f));
		wall21 = glm::scale(wall21, glm::vec3(2.5f));
		modelShader.setMat4("model", wall21);
		wallModel.Draw(modelShader);

		glm::mat4 wall22 = modelTemp;
		wall22 = glm::translate(wall22, glm::vec3(19.0f, -0.5f, -30.5f));
		wall22 = glm::scale(wall22, glm::vec3(2.5f));
		modelShader.setMat4("model", wall22);
		wallModel.Draw(modelShader);

		glm::mat4 wall23 = modelTemp;
		wall23 = glm::translate(wall23, glm::vec3(-6.5f, -0.5f, -30.5f));
		wall23 = glm::scale(wall23, glm::vec3(2.5f));
		modelShader.setMat4("model", wall23);
		wallModel.Draw(modelShader);

		glm::mat4 wall24 = modelTemp;
		wall24 = glm::translate(wall24, glm::vec3(-13.5f, -0.5f, -30.5f));
		wall24 = glm::scale(wall24, glm::vec3(2.5f));
		modelShader.setMat4("model", wall24);
		wallModel.Draw(modelShader);

		glm::mat4 wall25 = modelTemp;
		wall25 = glm::translate(wall25, glm::vec3(3.5f, -0.5f, -30.5f));
		wall25 = glm::scale(wall25, glm::vec3(4.0f, 2.5f, 2.5f));
		modelShader.setMat4("model", wall25);
		wallModel.Draw(modelShader);

		//Pilares acuario
		glm::mat4 pillar1 = modelTemp;
		pillar1 = glm::translate(pillar1, glm::vec3(-16.0f, 0.0f, -18.5f));
		pillar1 = glm::scale(pillar1, glm::vec3(0.7f));
		modelShader.setMat4("model", pillar1);
		pilar.Draw(modelShader);

		glm::mat4 pillar2 = modelTemp;
		pillar2 = glm::translate(pillar2, glm::vec3(0.0f, 0.0f, -18.5f));
		pillar2 = glm::scale(pillar2, glm::vec3(0.7f));
		modelShader.setMat4("model", pillar2);
		pilar.Draw(modelShader);

		glm::mat4 pillar3 = modelTemp;
		pillar3 = glm::translate(pillar3, glm::vec3(-16.0f, 0.0f, -27.5f));
		pillar3 = glm::scale(pillar3, glm::vec3(0.7f));
		modelShader.setMat4("model", pillar3);
		pilar.Draw(modelShader);

		glm::mat4 pillar4 = modelTemp;
		pillar4 = glm::translate(pillar4, glm::vec3(0.0f, 0.0f, -27.5f));
		pillar4 = glm::scale(pillar4, glm::vec3(0.7f));
		modelShader.setMat4("model", pillar4);
		pilar.Draw(modelShader);

		// === CARGA DE MODELOS Y ANIMACIÓN DE PECES EN CUADRADO ===
		// === MOVIMIENTO RECTANGULAR DE PECES ===
		if (fishDir == 0) { // Derecha
			posFishesX += fishSpeed;
			if (posFishesX >= xMax) {
				fishDir = 1;
				rotFish = 0.0f;
			}
		}
		else if (fishDir == 1) { // Arriba
			posFishesZ -= fishSpeed;
			if (posFishesZ <= zMin) {
				fishDir = 2;
				rotFish = -90.0f; // izquierda
			}
		}
		else if (fishDir == 2) { // Izquierda
			posFishesX -= fishSpeed;
			if (posFishesX <= xMin) {
				fishDir = 3;
				rotFish = 180.0f; // abajo
			}
		}
		else if (fishDir == 3) { // Abajo
			posFishesZ += fishSpeed;
			if (posFishesZ >= zMax) {
				fishDir = 0;
				rotFish = 90.0f; // derecha (reinicia)
			}
		}


		glm::mat4 clown = modelTemp; 
		clown = glm::translate(clown, glm::vec3(posFishesX, 0.0f, posFishesZ)); 
		clown = glm::rotate(clown, glm::radians(rotFish), glm::vec3(0.0f, 1.0f, 0.0f)); 
		clown = glm::scale(clown, glm::vec3(10.0f)); 
		modelShader.setMat4("model", clown); 
		clownFish.Draw(modelShader); 
		
		glm::mat4 pez = modelTemp; 
		pez = glm::translate(pez, glm::vec3(posFishesX - 1.0f, 0.0f, posFishesZ + 0.5)); 
		pez = glm::rotate(pez, glm::radians(rotFish), glm::vec3(0.0f, 1.0f, 0.0f)); 
		pez = glm::scale(pez, glm::vec3(10.0f)); modelShader.setMat4("model", pez); 
		fishG.Draw(modelShader); 
		
		glm::mat4 goldf = modelTemp; 
		goldf = glm::translate(goldf, glm::vec3(posFishesX - 1.0f, 0.0f, posFishesZ - 0.5)); 
		goldf = glm::rotate(goldf, glm::radians(rotFish), glm::vec3(0.0f, 1.0f, 0.0f)); 
		goldf = glm::scale(goldf, glm::vec3(10.0f)); 
		modelShader.setMat4("model", goldf); 
		goldFish.Draw(modelShader);

		// === NARVAL ===
		{
			glm::mat4 narval = modelTemp;

			float timeValue = glfwGetTime();
			float floatY = sin(timeValue * 0.8f) * 0.2f;   // movimiento suave en Y
			float tilt = sin(timeValue * 0.4f) * glm::radians(5.0f); // ligera inclinación

			narval = glm::translate(narval, glm::vec3(-9.0f, 0.65f + floatY, -23.5f)); 
			narval = glm::rotate(narval, tilt, glm::vec3(1.0f, 0.0f, 0.0f)); 
			narval = glm::scale(narval, glm::vec3(0.8f)); 
			modelShader.setMat4("model", narval);
			narwal.Draw(modelShader);
		}

		// === TORTUGA Y CANGREJO ===
		{
			glm::mat4 turtleBig = modelTemp;

			float timeValue = glfwGetTime();
			float totalDistance = 8.0f; 
			float movX = fmod(timeValue * 0.02f, totalDistance);
			float floatY = sin(timeValue * 0.5f) * 0.05f;

			turtleBig = glm::translate(turtleBig, glm::vec3(-12.0f + movX, -0.3f + floatY, -23.0f));
			turtleBig = glm::rotate(turtleBig, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			turtleBig = glm::scale(turtleBig, glm::vec3(3.0f));
			modelShader.setMat4("model", turtleBig);
			seaTurtle.Draw(modelShader);

			glm::mat4 turtle = modelTemp;
			turtle = glm::translate(turtle, glm::vec3(-14.0f + movX, -0.3f + floatY, -25.0f));
			turtle = glm::rotate(turtle, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			turtle = glm::scale(turtle, glm::vec3(0.8f));
			modelShader.setMat4("model", turtle);
			seaTurtle.Draw(modelShader);

			glm::mat4 crab = modelTemp;
			crab = glm::translate(crab, glm::vec3(-14.0f + movX, -0.3f + floatY, -24.0f));
			crab = glm::rotate(crab, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			crab = glm::scale(crab, glm::vec3(1.2f));
			modelShader.setMat4("model", crab);
			seaCrab.Draw(modelShader);
		}

		// === Corales ===
		glm::mat4 coral = modelTemp;
		coral = glm::translate(coral, glm::vec3(-8.0f, -0.3f, -26.0f));
		coral = glm::scale(coral, glm::vec3(0.05f));
		modelShader.setMat4("model", coral);
		corals.Draw(modelShader);
		
		//=== Habitat Aviario ===
		glm::mat4 rejaA = modelTemp;
		rejaA = glm::translate(rejaA, glm::vec3(12.0f, -0.8f, -28.0f));
		rejaA = glm::scale(rejaA, glm::vec3(7.0f, 6.0f, 5.0f));
		modelShader.setMat4("model", rejaA);
		reja.Draw(modelShader);

		glm::mat4 rejaB = modelTemp;
		rejaB = glm::translate(rejaB, glm::vec3(3.0f, -0.8f, -21.5f));
		rejaB = glm::rotate(rejaB, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		rejaB = glm::scale(rejaB, glm::vec3(5.0f, 6.0f, 5.0f));
		modelShader.setMat4("model", rejaB);
		reja.Draw(modelShader);

		glm::mat4 rejaA1 = modelTemp;
		rejaA1 = glm::translate(rejaA1, glm::vec3(12.0f, -0.8f, -15.0f));
		rejaA1 = glm::scale(rejaA1, glm::vec3(7.0f, 6.0f, 5.0f));
		modelShader.setMat4("model", rejaA1);
		reja.Draw(modelShader);

		glm::mat4 rejaB1 = modelTemp;
		rejaB1 = glm::translate(rejaB1, glm::vec3(20.5f, -0.8f, -21.5f));
		rejaB1 = glm::rotate(rejaB1, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		rejaB1 = glm::scale(rejaB1, glm::vec3(5.0f, 6.0f, 5.0f));
		modelShader.setMat4("model", rejaB1);
		reja.Draw(modelShader);

		//Arbolitos del acuario
		glm::mat4 tree1 = modelTemp;
		tree1 = glm::translate(tree1, glm::vec3(-2.0f, -1.0f, -28.5f));
		tree1 = glm::scale(tree1, glm::vec3(0.3f, 0.3f, 0.2f));
		modelShader.setMat4("model", tree1);
		TreeA.Draw(modelShader);

		glm::mat4 tree4 = modelTemp;
		tree4 = glm::translate(tree4, glm::vec3(-5.0f, -1.0f, -28.5f));
		tree4 = glm::scale(tree4, glm::vec3(0.3f, 0.3f, 0.2f));
		modelShader.setMat4("model", tree4);
		TreeA.Draw(modelShader);

		glm::mat4 tree5 = modelTemp;
		tree5 = glm::translate(tree5, glm::vec3(1.0f, -1.0f, -28.5f));
		tree5 = glm::scale(tree5, glm::vec3(0.3f, 0.3f, 0.2f));
		modelShader.setMat4("model", tree5);
		TreeA.Draw(modelShader);

		glm::mat4 tree6 = modelTemp;
		tree6 = glm::translate(tree6, glm::vec3(-8.0f, -1.0f, -28.5f));
		tree6 = glm::scale(tree6, glm::vec3(0.3f, 0.3f, 0.2f));
		modelShader.setMat4("model", tree6);
		TreeA.Draw(modelShader);

		glm::mat4 tree7 = modelTemp;
		tree7 = glm::translate(tree7, glm::vec3(-11.0f, -1.0f, -28.5f));
		tree7 = glm::scale(tree7, glm::vec3(0.3f, 0.3f, 0.2f));
		modelShader.setMat4("model", tree7);
		TreeA.Draw(modelShader);

		glm::mat4 tree8 = modelTemp;
		tree8 = glm::translate(tree8, glm::vec3(-14.0f, -1.0f, -28.5f));
		tree8 = glm::scale(tree8, glm::vec3(0.3f, 0.3f, 0.2f));
		modelShader.setMat4("model", tree8);
		TreeA.Draw(modelShader);

		glm::mat4 tree9 = modelTemp;
		tree9 = glm::translate(tree9, glm::vec3(1.5f, -1.0f, -25.5f));
		tree9 = glm::rotate(tree9, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		tree9 = glm::scale(tree9, glm::vec3(0.3f, 0.3f, 0.2f));
		modelShader.setMat4("model", tree9);
		TreeA.Draw(modelShader);

		glm::mat4 tree10 = modelTemp;
		tree10 = glm::translate(tree10, glm::vec3(1.5f, -1.0f, -22.5f));
		tree10 = glm::rotate(tree10, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		tree10 = glm::scale(tree10, glm::vec3(0.3f, 0.3f, 0.2f));
		modelShader.setMat4("model", tree10);
		TreeA.Draw(modelShader);

		glm::mat4 tree11 = modelTemp;
		tree11 = glm::translate(tree11, glm::vec3(1.5f, -1.0f, -19.5f));
		tree11 = glm::rotate(tree11, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		tree11 = glm::scale(tree11, glm::vec3(0.3f, 0.3f, 0.2f));
		modelShader.setMat4("model", tree11);
		TreeA.Draw(modelShader);

		//=== Modelos aviario ===
		glm::mat4 tree2 = modelTemp;
		tree2 = glm::translate(tree2, glm::vec3(18.0f, -1.0f, -25.0f));
		tree2 = glm::rotate(tree2, glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		tree2 = glm::scale(tree2, glm::vec3(0.03f));
		modelShader.setMat4("model", tree2);
		Tree.Draw(modelShader);

		glm::mat4 tree3 = modelTemp;
		tree3 = glm::translate(tree3, glm::vec3(6.0f, -1.0f, -26.0f));
		tree3 = glm::rotate(tree3, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		tree3 = glm::scale(tree3, glm::vec3(0.02f));
		modelShader.setMat4("model", tree3);
		Tree.Draw(modelShader);

		// === Dibujar acuario con textura de agua ===
		lightingShader.Use();
		lightingShader.setMat4("view", view);
		lightingShader.setMat4("projection", projection);

		// Posición y tamaño del acuario
		glm::mat4 glassModel = glm::mat4(1.0f);
		glassModel = glm::translate(glassModel, glm::vec3(-8.0f, 0.0f, -23.0f));
		glassModel = glm::scale(glassModel, glm::vec3(8.0f, 3.0f, 4.5f));
		lightingShader.setMat4("model", glassModel);

		// Configurar materiales (color base + brillo)
		lightingShader.setVec3("material.ambient", glm::vec3(0.5f, 0.6f, 0.9f));
		lightingShader.setVec3("material.diffuse", glm::vec3(0.5f, 0.7f, 1.0f));
		lightingShader.setVec3("material.specular", glm::vec3(0.8f));
		lightingShader.setFloat("material.shininess", 16.0f);

		// Transparencia
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		lightingShader.setFloat("opacity", 0.4f);

		// Textura del agua
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, waterTexture);
		lightingShader.setInt("texture_diffuse", 0);

		// Dibujar cubo del acuario
		glBindVertexArray(glassVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);

		glDisable(GL_BLEND);

		//Dibujar Skybox
		glDepthFunc(GL_LEQUAL);
		skyboxShader.Use();

		glm::mat4 viewSkybox = glm::mat4(glm::mat3(camera.GetViewMatrix()));
		skyboxShader.setMat4("view", viewSkybox);
		skyboxShader.setMat4("projection", projection);

		glBindVertexArray(skyboxVAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		glDepthFunc(GL_LESS);

		glfwSwapBuffers(window);
	}

	glDeleteVertexArrays(1, &floorVAO);
	glDeleteBuffers(1, &floorVBO);
	glDeleteVertexArrays(1, &glassVAO);
	glDeleteBuffers(1, &glassVBO);

	glfwTerminate();
	return 0;
}

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

	// Controles unificados para Rhino, Zebra, Wolf, Lion, Deer y Bear
	if (keys[GLFW_KEY_2])
	{
		rotRhino += 0.5f;
		rotZebra += 0.5f;
		rotWolf += 0.5f;
		rotLion += 0.5f;
		rotDeer += 0.5f;
		rotBear += 0.5f;
	}
	if (keys[GLFW_KEY_3])
	{
		rotRhino -= 0.5f;
		rotZebra -= 0.5f;
		rotWolf -= 0.5f;
		rotLion -= 0.5f;
		rotDeer -= 0.5f;
		rotBear -= 0.5f;
	}
	if (keys[GLFW_KEY_4])
	{
		rhinoHead += 0.5f;
		zebraHead += 0.5f;
		wolfHead += 0.5f;
		lionHead += 0.5f;
		deerHead += 0.5f;
		bearHead += 0.5f;
	}
	if (keys[GLFW_KEY_5])
	{
		rhinoHead -= 0.5f;
		zebraHead -= 0.5f;
		wolfHead -= 0.5f;
		lionHead -= 0.5f;
		deerHead -= 0.5f;
		bearHead -= 0.5f;
	}
	if (keys[GLFW_KEY_6])
	{
		rhinoFLegs += 0.5f;
		zebraFLegs += 0.5f;
		wolfFLegs += 0.5f;
		lionFLegs += 0.5f;
		deerFLegs += 0.5f;
		bearFLegs += 0.5f;
	}
	if (keys[GLFW_KEY_7])
	{
		rhinoFLegs -= 0.5f;
		zebraFLegs -= 0.5f;
		wolfFLegs -= 0.5f;
		lionFLegs -= 0.5f;
		deerFLegs -= 0.5f;
		bearFLegs -= 0.5f;
	}
	if (keys[GLFW_KEY_8])
	{
		rhinoBLegs += 0.5f;
		zebraBLegs += 0.5f;
		wolfBLegs += 0.5f;
		lionBLegs += 0.5f;
		deerBLegs += 0.5f;
		bearBLegs += 0.5f;
	}
	if (keys[GLFW_KEY_9])
	{
		rhinoBLegs -= 0.5f;
		zebraBLegs -= 0.5f;
		wolfBLegs -= 0.5f;
		lionBLegs -= 0.5f;
		deerBLegs -= 0.5f;
		bearBLegs -= 0.5f;
	}

	// Control de cola (solo para Zebra, Wolf y Lion)
	if (keys[GLFW_KEY_0])
	{
		zebraTail += 0.5f;
		wolfTail += 0.5f;
		lionTail += 0.5f;
	}
	if (keys[GLFW_KEY_1])
	{
		zebraTail -= 0.5f;
		wolfTail -= 0.5f;
		lionTail -= 0.5f;
	}

	if (keys[GLFW_KEY_H])
	{
		rhinoPosZ += 0.01f;
		zebraPosZ += 0.01f;
		wolfPosZ += 0.01f;
		lionPosZ += 0.01f;
		deerPosZ += 0.01f;
		bearPosZ += 0.01f;
	}
	if (keys[GLFW_KEY_Y])
	{
		rhinoPosZ -= 0.01f;
		zebraPosZ -= 0.01f;
		wolfPosZ -= 0.01f;
		lionPosZ -= 0.01f;
		deerPosZ -= 0.01f;
		bearPosZ -= 0.01f;
	}
	if (keys[GLFW_KEY_G])
	{
		rhinoPosX -= 0.01f;
		zebraPosX -= 0.01f;
		wolfPosX -= 0.01f;
		lionPosX -= 0.01f;
		deerPosX -= 0.01f;
		bearPosX -= 0.01f;
	}
	if (keys[GLFW_KEY_J])
	{
		rhinoPosX += 0.01f;
		zebraPosX += 0.01f;
		wolfPosX += 0.01f;
		lionPosX += 0.01f;
		deerPosX += 0.01f;
		bearPosX += 0.01f;
	}

	if (activanim)
	{
		if (rot > -90.0f)
			rot -= 0.1f;
	}

	if (camera.GetPosition().y < 0.0f)
		camera = Camera(glm::vec3(camera.GetPosition().x, 0.0f, camera.GetPosition().z));
}

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

	if (keys[GLFW_KEY_O])
	{
		movelightPos += 0.1f;
	}

	if (keys[GLFW_KEY_I])
	{
		movelightPos1 += 0.1f;
	}

	// Guardar KeyFrame
	if (keys[GLFW_KEY_K] && action == GLFW_PRESS)
	{
		if (FrameIndexRhino < MAX_FRAMES_RHINO)
		{
			saveFrameRhino();
		}
	}

	// Reproducir Animación
	if (keys[GLFW_KEY_L] && action == GLFW_PRESS)
	{
		if (playRhino == false && (FrameIndexRhino > 1))
		{
			resetElementsRhino();
			interpolationRhino();
			playRhino = true;
			playIndexRhino = 0;
			i_curr_steps_rhino = 0;
			std::cout << "Reproduciendo animacion predefinida del rinoceronte..." << std::endl;
		}
		else
		{
			playRhino = false;
			std::cout << "Animacion pausada" << std::endl;
		}
	}
	
	// Presiona P para iniciar animación predefinida automática (con loop)
	if (keys[GLFW_KEY_P] && action == GLFW_PRESS)
	{
		if (!playRhino && FrameIndexRhino > 1)
		{
			resetElementsRhino();
			interpolationRhino();
			playRhino = true;
			playIndexRhino = 0;
			i_curr_steps_rhino = 0;
			std::cout << "Iniciando animacion predefinida en loop..." << std::endl;
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

unsigned int loadCubemap(std::vector<std::string> faces)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(false);

	for (unsigned int i = 0; i < faces.size(); i++)
	{
		unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			// === ROTACIÓN DE LAS CARAS UP y DOWN ===
			if (i == 2 || i == 3)
			{
				int channels = nrChannels;
				unsigned char* rotated = new unsigned char[width * height * channels];
				for (int y = 0; y < height; ++y)
				{
					for (int x = 0; x < width; ++x)
					{
						for (int c = 0; c < channels; ++c)
						{
							rotated[(x * height + (height - y - 1)) * channels + c] =
								data[(y * width + x) * channels + c];
						}
					}
				}
				stbi_image_free(data);
				data = rotated;
				std::swap(width, height);
			}

			GLenum format = (nrChannels == 4) ? GL_RGBA : GL_RGB;
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);

			delete[] data;
		}
		else
		{
			std::cout << "Error al cargar: " << faces[i] << std::endl;
			stbi_image_free(data);
		}
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return textureID;
}