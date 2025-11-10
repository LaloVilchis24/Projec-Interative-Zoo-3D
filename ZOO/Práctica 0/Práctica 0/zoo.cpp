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

// ===== ANIMACIÓN DEL RINOCERONTE =====
float rotRhino = 0.0f;
float rhinoFLegs = 0.0f;
float rhinoBLegs = 0.0f;
float rhinoHead = 0.0f;
float rhinoPosX = 0.0f;
float rhinoPosY = -0.5f;
float rhinoPosZ = -5.0f;

// DEBUG: Ajuste de pivotes
float headPivotX = 0.0f, headPivotY = 0.15f, headPivotZ = 0.45f;
float legPivotX = 0.15f, legPivotY = -0.05f, legPivotZ = 0.25f;

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
    glfwSetInputMode( window, GLFW_CURSOR, GLFW_CURSOR_DISABLED );

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

    // Imagenes Skybox
    std::vector<std::string> faces{
        "images/Skybox/humble_lf.jpg",
        "images/Skybox/humble_rt.jpg",
        "images/Skybox/humble_up.jpg",
        "images/Skybox/humble_dn.jpg",
        "images/Skybox/humble_ft.jpg",
        "images/Skybox/humble_bk.jpg",        
    };

    // Cargar skybox
    unsigned int cubemapTexture = loadCubemap(faces);
    skyboxShader.Use();
    glUniform1i(glGetUniformLocation(skyboxShader.Program, "skybox"), 0);

    // Load models
    Model rhinoModel((char*)"Models/uploads_files_5014602_Rhino_Quad.obj");
    Model BodyRhino((char*)"Models/BodyRhino.obj");
    Model HeadRhino((char*)"Models/HeadRhino.obj");
    Model B_LeftLegRhino((char*)"Models/B_LeftLegRhino.obj");
    Model B_RightLegRhino((char*)"Models/B_RightLegRhino.obj");
    Model F_LeftLegRhino((char*)"Models/F_LeftLegRhino.obj");
    Model F_RightLegRhino((char*)"Models/F_RightLegRhino.obj");
    
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

    glm::mat4 projection = glm::perspective(camera.GetZoom(), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);

    // --- SKYBOX setup --- 
    float skyboxVertices[] = {
        // positions
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
        // posiciones             // normales         // texturas 
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

    // posiciones
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // normales
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // coordenadas de textura
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);

    unsigned int floorTexture;
    glGenTextures(1, &floorTexture);
    glBindTexture(GL_TEXTURE_2D, floorTexture);

    // parámetros de repetición y filtrado
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

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // --- Textura acuario ---
    unsigned int waterTexture;
    glGenTextures(1, &waterTexture);
    glBindTexture(GL_TEXTURE_2D, waterTexture);

    // parámetros de envoltura y filtrado
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // cargar imagen
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

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // === Dibujar Piso ===
        lightingShader.Use();

        glm::mat4 view = camera.GetViewMatrix();
        lightingShader.setMat4("view", view);
        lightingShader.setMat4("projection", projection);

        glm::mat4 model = glm::mat4(1.0f);
        lightingShader.setMat4("model", model);

        // === Luz direccional ===
        lightingShader.Use();
        lightingShader.setVec3("dirLight.direction", glm::vec3(-0.2f, -1.0f, -0.3f));  // dirección del sol
        lightingShader.setVec3("dirLight.ambient", glm::vec3(0.3f, 0.3f, 0.3f));       // luz ambiental suave
        lightingShader.setVec3("dirLight.diffuse", glm::vec3(0.8f, 0.8f, 0.8f));       // intensidad del sol
        lightingShader.setVec3("dirLight.specular", glm::vec3(1.0f, 1.0f, 1.0f));      // reflejo fuerte

        // --- Activar textura del piso ---
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, floorTexture);

        // --- Dibujar el plano ---
        glBindVertexArray(floorVAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);

        // === Dibujar Rinoceronte ===
        modelShader.Use();
        modelShader.setMat4("view", view);
        modelShader.setMat4("projection", projection);

        // CUERPO (BASE)
        glm::mat4 modelTemp = glm::mat4(1.0f);
        modelTemp = glm::translate(modelTemp, glm::vec3(rhinoPosX, rhinoPosY, rhinoPosZ));
        modelTemp = glm::rotate(modelTemp, glm::radians(rotRhino), glm::vec3(0.0f, 1.0f, 0.0f));
        model = modelTemp;
        modelShader.setMat4("model", model);
        BodyRhino.Draw(modelShader);

        // CABEZA
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
        //model = glm::translate(model, glm::vec3(-0.15f, 0.05f, -0.25f));
        modelShader.setMat4("model", model);
        F_LeftLegRhino.Draw(modelShader);

        // PATA DELANTERA DERECHA
        model = modelTemp;
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(-rhinoFLegs), glm::vec3(1.0f, 0.0f, 0.0f));
        //model = glm::translate(model, glm::vec3(0.15f, 0.05f, -0.25f));
        modelShader.setMat4("model", model);
        F_RightLegRhino.Draw(modelShader);

        // PATA TRASERA IZQUIERDA
        model = modelTemp;
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(-rhinoBLegs), glm::vec3(1.0f, 0.0f, 0.0f));
        //model = glm::translate(model, glm::vec3(-0.15f, 0.05f, 0.25f));
        modelShader.setMat4("model", model);
        B_LeftLegRhino.Draw(modelShader);

        // PATA TRASERA DERECHA
        model = modelTemp;
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(rhinoBLegs), glm::vec3(1.0f, 0.0f, 0.0f));
        //model = glm::translate(model, glm::vec3(0.15f, 0.05f, 0.25f));
        modelShader.setMat4("model", model);
        B_RightLegRhino.Draw(modelShader);

        // === Dibujar Pared y Entrada ===
        modelTemp = glm::translate(modelTemp, glm::vec3(0.0f, 0.0f, 0.0f));
        // Entrada
        glm::mat4 ent = modelTemp;
        ent = glm::translate(ent, glm::vec3(2.8f, -0.55f, 25.5f));
        ent = glm::scale(ent, glm::vec3(0.1f));
        modelShader.setMat4("model", ent);
        entranceModel.Draw(modelShader);
        
        // Paredes Entrada
        glm::mat4 wall1 = modelTemp;
        wall1 = glm::translate(wall1, glm::vec3(12.0f, 0.0f, 25.5f));
        wall1 = glm::scale(wall1, glm::vec3(2.5f));
        modelShader.setMat4("model", wall1);
        wallModel.Draw(modelShader);

        glm::mat4 wall2 = modelTemp;
        wall2 = glm::translate(wall2, glm::vec3(19.0f, 0.0f, 25.5f));
        wall2 = glm::scale(wall2, glm::vec3(2.5f));
        modelShader.setMat4("model", wall2);
        wallModel.Draw(modelShader);

        glm::mat4 wall8 = modelTemp;
        wall8 = glm::translate(wall8, glm::vec3(-6.5f, 0.0f, 25.5f));
        wall8 = glm::scale(wall8, glm::vec3(2.5f));
        modelShader.setMat4("model", wall8);
        wallModel.Draw(modelShader);

        glm::mat4 wall9 = modelTemp;
        wall9 = glm::translate(wall9, glm::vec3(-13.5f, 0.0f, 25.5f));
        wall9 = glm::scale(wall9, glm::vec3(2.5f));
        modelShader.setMat4("model", wall9);
        wallModel.Draw(modelShader);

        // Paredes Izquierdas
        glm::mat4 wall3 = modelTemp;
        wall3 = glm::translate(wall3, glm::vec3(22.5f, 0.0f, 22.0f));
        wall3 = glm::rotate(wall3, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        wall3 = glm::scale(wall3, glm::vec3(2.5f));
        modelShader.setMat4("model", wall3);
        wallModel.Draw(modelShader);

        glm::mat4 wall4 = modelTemp;
        wall4 = glm::translate(wall4, glm::vec3(22.5f, 0.0f, 15.0f));
        wall4 = glm::rotate(wall4, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        wall4 = glm::scale(wall4, glm::vec3(2.5f));
        modelShader.setMat4("model", wall4);
        wallModel.Draw(modelShader);

        glm::mat4 wall5 = modelTemp;
        wall5 = glm::translate(wall5, glm::vec3(22.5f, 0.0f, 8.0f));
        wall5 = glm::rotate(wall5, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        wall5 = glm::scale(wall5, glm::vec3(2.5f));
        modelShader.setMat4("model", wall5);
        wallModel.Draw(modelShader);

        glm::mat4 wall6 = modelTemp;
        wall6 = glm::translate(wall6, glm::vec3(22.5f, 0.0f, 1.0f));
        wall6 = glm::rotate(wall6, glm::radians(90.5f), glm::vec3(0.0f, 1.0f, 0.0f));
        wall6 = glm::scale(wall6, glm::vec3(2.5f));
        modelShader.setMat4("model", wall6);
        wallModel.Draw(modelShader);

        glm::mat4 wall7 = modelTemp;
        wall7 = glm::translate(wall7, glm::vec3(22.5f, 0.0f, -6.0f));
        wall7 = glm::rotate(wall7, glm::radians(90.5f), glm::vec3(0.0f, 1.0f, 0.0f));
        wall7 = glm::scale(wall7, glm::vec3(2.5f));
        modelShader.setMat4("model", wall7);
        wallModel.Draw(modelShader);

        glm::mat4 wall15 = modelTemp;
        wall15 = glm::translate(wall15, glm::vec3(22.5f, 0.0f, -13.0f));
        wall15 = glm::rotate(wall15, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        wall15 = glm::scale(wall15, glm::vec3(2.5f));
        modelShader.setMat4("model", wall15);
        wallModel.Draw(modelShader);

        glm::mat4 wall16 = modelTemp;
        wall16 = glm::translate(wall16, glm::vec3(22.5f, 0.0f, -20.0f));
        wall16 = glm::rotate(wall16, glm::radians(90.5f), glm::vec3(0.0f, 1.0f, 0.0f));
        wall16 = glm::scale(wall16, glm::vec3(2.5f));
        modelShader.setMat4("model", wall16);
        wallModel.Draw(modelShader);

        glm::mat4 wall17 = modelTemp;
        wall17 = glm::translate(wall17, glm::vec3(22.5f, 0.0f, -27.0f));
        wall17 = glm::rotate(wall17, glm::radians(90.5f), glm::vec3(0.0f, 1.0f, 0.0f));
        wall17 = glm::scale(wall17, glm::vec3(2.5f));
        modelShader.setMat4("model", wall17);
        wallModel.Draw(modelShader);

        // Paredes Dereches
        glm::mat4 wall10 = modelTemp;
        wall10 = glm::translate(wall10, glm::vec3(-17.0f, 0.0f, 22.0f));
        wall10 = glm::rotate(wall10, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        wall10 = glm::scale(wall10, glm::vec3(2.5f));
        modelShader.setMat4("model", wall10);
        wallModel.Draw(modelShader);

        glm::mat4 wall11 = modelTemp;
        wall11 = glm::translate(wall11, glm::vec3(-17.0f, 0.0f, 15.0f));
        wall11 = glm::rotate(wall11, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        wall11 = glm::scale(wall11, glm::vec3(2.5f));
        modelShader.setMat4("model", wall11);
        wallModel.Draw(modelShader);

        glm::mat4 wall12 = modelTemp;
        wall12 = glm::translate(wall12, glm::vec3(-17.0f, 0.0f, 8.0f));
        wall12 = glm::rotate(wall12, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        wall12 = glm::scale(wall12, glm::vec3(2.5f));
        modelShader.setMat4("model", wall12);
        wallModel.Draw(modelShader);

        glm::mat4 wall13 = modelTemp;
        wall13 = glm::translate(wall13, glm::vec3(-17.0f, 0.0f, 1.0f));
        wall13 = glm::rotate(wall13, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        wall13 = glm::scale(wall13, glm::vec3(2.5f));
        modelShader.setMat4("model", wall13);
        wallModel.Draw(modelShader);

        glm::mat4 wall14 = modelTemp;
        wall14 = glm::translate(wall14, glm::vec3(-17.0f, 0.0f, -6.0f));
        wall14 = glm::rotate(wall14, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        wall14 = glm::scale(wall14, glm::vec3(2.5f));
        modelShader.setMat4("model", wall14);
        wallModel.Draw(modelShader);

        glm::mat4 wall18 = modelTemp;
        wall18 = glm::translate(wall18, glm::vec3(-17.0f, 0.0f, -13.0f));
        wall18 = glm::rotate(wall18, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        wall18 = glm::scale(wall18, glm::vec3(2.5f));
        modelShader.setMat4("model", wall18);
        wallModel.Draw(modelShader);

        glm::mat4 wall19 = modelTemp;
        wall19 = glm::translate(wall19, glm::vec3(-17.0f, 0.0f, -20.0f));
        wall19 = glm::rotate(wall19, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        wall19 = glm::scale(wall19, glm::vec3(2.5f));
        modelShader.setMat4("model", wall19);
        wallModel.Draw(modelShader);

        glm::mat4 wall20 = modelTemp;
        wall20 = glm::translate(wall20, glm::vec3(-17.0f, 0.0f, -27.0f));
        wall20 = glm::rotate(wall20, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        wall20 = glm::scale(wall20, glm::vec3(2.5f));
        modelShader.setMat4("model", wall20);
        wallModel.Draw(modelShader);

        //Paredes del Fondo
        glm::mat4 wall21 = modelTemp;
        wall21 = glm::translate(wall21, glm::vec3(12.0f, 0.0f, -30.5f));
        wall21 = glm::scale(wall21, glm::vec3(2.5f));
        modelShader.setMat4("model", wall21);
        wallModel.Draw(modelShader);

        glm::mat4 wall22 = modelTemp;
        wall22 = glm::translate(wall22, glm::vec3(19.0f, 0.0f, -30.5f));
        wall22 = glm::scale(wall22, glm::vec3(2.5f));
        modelShader.setMat4("model", wall22);
        wallModel.Draw(modelShader);

        glm::mat4 wall23 = modelTemp;
        wall23 = glm::translate(wall23, glm::vec3(-6.5f, 0.0f, -30.5f));
        wall23 = glm::scale(wall23, glm::vec3(2.5f));
        modelShader.setMat4("model", wall23);
        wallModel.Draw(modelShader);

        glm::mat4 wall24 = modelTemp;
        wall24 = glm::translate(wall24, glm::vec3(-13.5f, 0.0f, -30.5f));
        wall24 = glm::scale(wall24, glm::vec3(2.5f));
        modelShader.setMat4("model", wall24);
        wallModel.Draw(modelShader);

        glm::mat4 wall25 = modelTemp;
        wall25 = glm::translate(wall25, glm::vec3(3.5f, 0.0f, -30.5f));
        wall25 = glm::scale(wall25, glm::vec3(4.0f, 2.5f, 2.5f));
        modelShader.setMat4("model", wall25);
        wallModel.Draw(modelShader);

        /*glm::mat4 wall26 = modelTemp;
        wall26 = glm::translate(wall26, glm::vec3(-0.5f, 0.0f, -30.5f));
        wall26 = glm::scale(wall26, glm::vec3(2.5f));
        modelShader.setMat4("model", wall26);
        wallModel.Draw(modelShader);*/

        // === Dibujar acuario con textura de agua ===
        lightingShader.Use();
        lightingShader.setMat4("view", view);
        lightingShader.setMat4("projection", projection);

        // Posición y tamaño del cubo
        glm::mat4 glassModel = glm::mat4(1.0f);
        glassModel = glm::translate(glassModel, glm::vec3(0.0f, 0.0f, -25.0f));
        glassModel = glm::scale(glassModel, glm::vec3(5.0f, 3.0f, 5.0f));
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

        // === Dibujar Skybox ===
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

    // Guardar KeyFrame (K)
    if (keys[GLFW_KEY_K] && action == GLFW_PRESS)
    {
        if (FrameIndexRhino < MAX_FRAMES_RHINO)
        {
            saveFrameRhino();
        }
    }

    // Reproducir Animación (L)
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
    GLfloat yOffset = lastY - yPos;  // Reversed since y-coordinates go from bottom to left

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
                // Rotar 90° sentido horario
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
                std::swap(width, height); // intercambiar dimensiones tras rotación
            }

            GLenum format = (nrChannels == 4) ? GL_RGBA : GL_RGB;
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);

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