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

//ANIMACIÓN DEL RINOCERONTE
float rotRhino = 0.0f;
float rhinoFLegs = 0.0f;
float rhinoBLegs = 0.0f;
float rhinoHead = 0.0f;
float rhinoPosX = 0.0f;
float rhinoPosY = -0.5f;
float rhinoPosZ = -5.0f;

//Ajuste de pivotes
float headPivotX = 0.0f, headPivotY = 0.15f, headPivotZ = 0.45f;
float legPivotX = 0.15f, legPivotY = -0.05f, legPivotZ = 0.25f;

//ANIMACIÓN DE LA ZEBRA
float rotZebra = 0.0f;
float zebraFLegs = 0.0f;
float zebraBLegs = 0.0f;
float zebraHead = 0.0f;
float zebraTail = 0.0f;
float zebraPosX = -4.0f;
float zebraPosY = -0.5f;
float zebraPosZ = 2.0f;

//Ajuste de pivotes zebra
float zebraHeadPivotX = 0.0f, zebraHeadPivotY = 0.15f, zebraHeadPivotZ = 0.45f;
float zebraTailPivotX = 0.0f, zebraTailPivotY = 0.1f, zebraTailPivotZ = -0.3f;

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

void AnimationRhino()
{
    if (playRhino)
    {
      if (i_curr_steps_rhino >= i_max_steps_rhino)
     {
    playIndexRhino++;
  if (playIndexRhino > FrameIndexRhino - 2)
         {
 printf("Animacion terminada\n");
 playIndexRhino = 0;
      playRhino = false;
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
          rhinoFLegs += KeyFrameRhino[playIndexRhino].rhinoFLegsInc;
       rhinoBLegs += KeyFrameRhino[playIndexRhino].rhinoBLegsInc;
            i_curr_steps_rhino++;
}
    }
}

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
    Model LionModel((char*)"Models/Lion/uploads_files_5354356_model.obj");

	//Sebra
    //Model ZebraModel((char*)"Models/Zebra/uploads_files_5014568_Zebra_Quad.obj");
    Model BodyZebra((char*)"Models/Zebra/BodyZebra.obj");
    Model HeadZebra((char*)"Models/Zebra/HeadZebra.obj");
    Model B_LeftLegZebra((char*)"Models/Zebra/B_LeftLegZebra.obj");
    Model B_RightLegZebra((char*)"Models/Zebra/B_RightLegZebra.obj");
    Model F_LeftLegZebra((char*)"Models/Zebra/F_LeftLegZebra.obj");
    Model F_RightLegZebra((char*)"Models/Zebra/F_RightLegZebra.obj");
    Model TailZebra((char*)"Models/Zebra/TailZebra.obj");

    //Oso
    Model BearModel((char*)"Models/Bear/bear.bland.obj");

    //Venado
    Model DeerModel((char*)"Models/Deer/uploads_files_5014765_Deer_Quad.obj");

    //Lobo
    Model WolfModel((char*)"Models/Wolf/uploads_files_5222363_model.obj");

    
    
    // Modelos de escenario
    Model wallModel((char*)"Models/Pared/fence.obj");
    Model entranceModel((char*)"Models/Entrada/Gate_obj.obj");

    // Inicializar KeyFrames del rinoceronte
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
        -1.0f,1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,1.0f,
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
        lightingShader.setVec3("light.position", lightPos);
        lightingShader.setVec3("light.position1", lightPos1);
        lightingShader.setVec3("light.ambient", glm::vec3(0.3f));
        lightingShader.setVec3("light.diffuse", glm::vec3(0.7f));
        lightingShader.setVec3("light.specular", glm::vec3(1.0f));

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

        //Dibujar Rinoceronte
        modelShader.Use();
        modelShader.setMat4("view", view);
        modelShader.setMat4("projection", projection);

        //Cuerpo
        glm::mat4 modelTemp = glm::mat4(1.0f);
        modelTemp = glm::translate(modelTemp, glm::vec3(rhinoPosX, rhinoPosY, rhinoPosZ));
        modelTemp = glm::rotate(modelTemp, glm::radians(rotRhino), glm::vec3(0.0f, 1.0f, 0.0f));
        model = modelTemp;
        modelShader.setMat4("model", model);
        BodyRhino.Draw(modelShader);

		//Cabeza
        model = modelTemp;
        model = glm::translate(model, glm::vec3(headPivotX, headPivotY, headPivotZ));
        model = glm::rotate(model, glm::radians(rhinoHead), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::translate(model, glm::vec3(-headPivotX, -headPivotY, -headPivotZ));
        modelShader.setMat4("model", model);
        HeadRhino.Draw(modelShader);

        // PATA DELANTERA IZQUIERDA
        model = modelTemp;
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(rhinoFLegs), glm::vec3(1.0f, 0.0f, 0.0f));
        modelShader.setMat4("model", model);
        F_LeftLegRhino.Draw(modelShader);

        // PATA DELANTERA DERECHA
        model = modelTemp;
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(-rhinoFLegs), glm::vec3(1.0f, 0.0f, 0.0f));
        modelShader.setMat4("model", model);
        F_RightLegRhino.Draw(modelShader);

        // PATA TRASERA IZQUIERDA
        model = modelTemp;
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(-rhinoBLegs), glm::vec3(1.0f, 0.0f, 0.0f));
        modelShader.setMat4("model", model);
        B_LeftLegRhino.Draw(modelShader);

        // PATA TRASERA DERECHA
        model = modelTemp;
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(rhinoBLegs), glm::vec3(1.0f, 0.0f, 0.0f));
        modelShader.setMat4("model", model);
        B_RightLegRhino.Draw(modelShader);

        // León
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        modelShader.setMat4("model", model);
        LionModel.Draw(modelShader);

        // Cebra
        glm::mat4 modelTempZebra = glm::mat4(1.0f);
        modelTempZebra = glm::translate(modelTempZebra, glm::vec3(zebraPosX, zebraPosY, zebraPosZ));
        modelTempZebra = glm::rotate(modelTempZebra, glm::radians(rotZebra), glm::vec3(0.0f, 1.0f, 0.0f));
        
  //Cuerpo Zebra
      model = modelTempZebra;
        modelShader.setMat4("model", model);
     BodyZebra.Draw(modelShader);

    //Cabeza Zebra
        model = modelTempZebra;
        model = glm::translate(model, glm::vec3(zebraHeadPivotX, zebraHeadPivotY, zebraHeadPivotZ));
   model = glm::rotate(model, glm::radians(zebraHead), glm::vec3(0.0f, 1.0f, 0.0f));
 model = glm::translate(model, glm::vec3(-zebraHeadPivotX, -zebraHeadPivotY, -zebraHeadPivotZ));
        modelShader.setMat4("model", model);
     HeadZebra.Draw(modelShader);

        // PATA DELANTERA IZQUIERDA Zebra
 model = modelTempZebra;
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(zebraFLegs), glm::vec3(1.0f, 0.0f, 0.0f));
  modelShader.setMat4("model", model);
        F_LeftLegZebra.Draw(modelShader);

        // PATA DELANTERA DERECHA Zebra
        model = modelTempZebra;
  model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(-zebraFLegs), glm::vec3(1.0f, 0.0f, 0.0f));
        modelShader.setMat4("model", model);
        F_RightLegZebra.Draw(modelShader);

        // PATA TRASERA IZQUIERDA Zebra
        model = modelTempZebra;
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(-zebraBLegs), glm::vec3(1.0f, 0.0f, 0.0f));
        modelShader.setMat4("model", model);
 B_LeftLegZebra.Draw(modelShader);

        // PATA TRASERA DERECHA Zebra
        model = modelTempZebra;
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
  model = glm::rotate(model, glm::radians(zebraBLegs), glm::vec3(1.0f, 0.0f, 0.0f));
        modelShader.setMat4("model", model);
        B_RightLegZebra.Draw(modelShader);

   // COLA Zebra
        model = modelTempZebra;
        model = glm::translate(model, glm::vec3(zebraTailPivotX, zebraTailPivotY, zebraTailPivotZ));
        model = glm::rotate(model, glm::radians(zebraTail), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::translate(model, glm::vec3(-zebraTailPivotX, -zebraTailPivotY, -zebraTailPivotZ));
  modelShader.setMat4("model", model);
        TailZebra.Draw(modelShader);


        //Oso
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, -0.5f, -3.0f));
        model = glm::rotate(model, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f));
        modelShader.setMat4("model", model);
        BearModel.Draw(modelShader);

        //Ciervo
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, -0.5f, 3.0f));
        model = glm::rotate(model, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.5f));
        modelShader.setMat4("model", model);
        DeerModel.Draw(modelShader);

        //Lobo
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
       
        modelShader.setMat4("model", model);
        WolfModel.Draw(modelShader);


        //Dibujar Pared y Entrada
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, -0.5f, -10.0f));
        model = glm::scale(model, glm::vec3(1.0f));
        modelShader.setMat4("model", model);
        wallModel.Draw(modelShader);

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(3.0f, -0.5f, 25.0f));
        model = glm::scale(model, glm::vec3(0.1f));
        modelShader.setMat4("model", model);
        entranceModel.Draw(modelShader);

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

    // Rhino controls
    if (keys[GLFW_KEY_2])
        rotRhino += 0.1f;
    if (keys[GLFW_KEY_3])
        rotRhino -= 0.1f;
    if (keys[GLFW_KEY_4])
        rhinoHead += 0.1f;
    if (keys[GLFW_KEY_5])
        rhinoHead -= 0.1f;
    if (keys[GLFW_KEY_6])
        rhinoFLegs += 0.1f;
    if (keys[GLFW_KEY_7])
      rhinoFLegs -= 0.1f;
    if (keys[GLFW_KEY_8])
        rhinoBLegs += 0.1f;
    if (keys[GLFW_KEY_9])
        rhinoBLegs -= 0.1f;
    if (keys[GLFW_KEY_H])
        rhinoPosZ += 0.01f;
    if (keys[GLFW_KEY_Y])
        rhinoPosZ -= 0.01f;
    if (keys[GLFW_KEY_G])
        rhinoPosX -= 0.01f;
    if (keys[GLFW_KEY_J])
        rhinoPosX += 0.01f;

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
            std::cout << "Reproduciendo animacion..." << std::endl;
        }
        else
        {
            playRhino = false;
            std::cout << "Animacion pausada" << std::endl;
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