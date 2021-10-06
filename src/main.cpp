#include <OpenGLPrj.hpp>
#include <GLFW/glfw3.h>

#include <Camera.hpp>
#include <Shader.hpp>

#include <iostream>
#include <string>
#include <vector>

//Shooting States
enum Shooting_State
{
  CNOSHOOT,
  LNOSHOOT,
  CSHOOT,
  LSHOOT
};
int clawShot = CNOSHOOT, lightShot = LNOSHOOT;

const std::string program_name = ("SouthPark-FBW Multiplayer");

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);
void pukanje(Shooting_State fire);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 800;

// camera
static Camera camera(glm::vec3(-0.496057f, 6.04088f, 23.1115f));
static float lastX = SCR_WIDTH / 2.0f;
static float lastY = SCR_HEIGHT / 2.0f;
static bool firstMouse = true;

// timing
static float deltaTime = 0.0f; // time between current frame and last frame
static float lastFrame = 0.0f;

//Characters initial position
static float x1 = -4.0f, y1 = 0.1f, z1 = 9.5f; //(-4.0f,0.1f,9.5f)
static float x2 = 27.0f, y2 = 0.1f, z2 = 9.5f; //(27.0f, 0.1f, 9.5f)

//Characters powerups positions
static float cx = 0.0f, cy = -10.0f, cz = 16.5f;  //-4.0f,0.1f,11.5f
static float lx = 18.0f, ly = -10.0f, lz = 16.5f; //18.0f, -9.6f, 1.2f
float br = x1, br2 = x2;

bool CHIT = false, LHIT = false;

//health counter> COON | CHAOS
float count1 = 26.0f, count2 = 26.0f;

//WASTED
bool deadcoon = false, deadchaos = false;

int main()
{
  // glfw: initialize and configure
  // ------------------------------
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
  glfwWindowHint(
      GLFW_OPENGL_FORWARD_COMPAT,
      GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

  // glfw window creation
  // --------------------
  GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT,
                                        program_name.c_str(), nullptr, nullptr);
  if (window == nullptr)
  {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  // tell GLFW to capture our mouse
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  // glad: load all OpenGL function pointers
  // ---------------------------------------
  if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
  {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return -1;
  }

  // configure global opengl state
  // -----------------------------
  glEnable(GL_DEPTH_TEST);

  // build and compile our shader program
  // ------------------------------------
  std::string shader_location("../res/shaders/");

  std::string used_shaders("shader");
  std::string used_leftsprites("levsprite");
  std::string used_ritghtsprites("desensprite");
  std::string used_claws("claw");
  std::string used_lightning("lightning");
  std::string used_bar("healthbar1");
  std::string used_bar2("healthbar2");

  Shader ourShader(shader_location + used_shaders + std::string(".vert"),
                   shader_location + used_shaders + std::string(".frag"));

  Shader spriteShader1(shader_location + used_leftsprites + std::string(".vert"),
                       shader_location + used_leftsprites + std::string(".frag"));

  Shader spriteShader2(shader_location + used_ritghtsprites + std::string(".vert"),
                       shader_location + used_ritghtsprites + std::string(".frag"));

  Shader clawShader(shader_location + used_claws + std::string(".vert"),
                    shader_location + used_claws + std::string(".frag"));

  Shader lightShader(shader_location + used_lightning + std::string(".vert"),
                     shader_location + used_lightning + std::string(".frag"));

  Shader healthbar1(shader_location + used_bar + std::string(".vert"),
                    shader_location + used_bar + std::string(".frag"));

  Shader healthbar2(shader_location + used_bar2 + std::string(".vert"),
                    shader_location + used_bar2 + std::string(".frag"));

  // set up vertex data (and buffer(s)) and configure vertex attributes
  // ------------------------------------------------------------------
  float vertices[] = {
      -25.0f, -1.0f, -25.0f, 0.0f, 1.0f,
      25.0f, -1.0f, -25.0f, 1.0f, 1.0f,
      25.0f, -1.0f, 25.0f, 1.0f, 0.0f,
      25.0f, -1.0f, 25.0f, 1.0f, 0.0f,
      -25.0f, -1.0f, 25.0f, 0.0f, 0.0f,
      -25.0f, -1.0f, -25.0f, 0.0f, 1.0f};
  float vertices2[] = {///LEV KARAKTER
                       // pos      // tex
                       -15.5f, -8.5f, -15.5f, 0.0f, 1.0f,
                       -8.5f, -8.5f, -15.5f, 1.0f, 1.0f,
                       -8.5f, -8.5f, -8.5f, 1.0f, 0.0f,
                       -8.5f, -8.5f, -8.5f, 1.0f, 0.0f,
                       -15.5f, -8.5f, -8.5f, 0.0f, 0.0f,
                       -15.5f, -8.5f, -15.5f, 0.0f, 1.0f

  };

  float vertices3[] = {///DESEN KARAKTER
                       // pos      // tex
                       -15.5f, -8.5f, -15.5f, 0.0f, 1.0f,
                       -8.5f, -8.5f, -15.5f, 1.0f, 1.0f,
                       -8.5f, -8.5f, -8.5f, 1.0f, 0.0f,
                       -8.5f, -8.5f, -8.5f, 1.0f, 0.0f,
                       -15.5f, -8.5f, -8.5f, 0.0f, 0.0f,
                       -15.5f, -8.5f, -15.5f, 0.0f, 1.0f};
  float claws[] = {///lev KARAKTER POWERUP
                   // pos      // tex
                   -15.5f, -11.5f, -15.5f, 0.0f, 1.0f,
                   -11.5f, -11.5f, -15.5f, 1.0f, 1.0f,
                   -11.5f, -11.5f, -11.5f, 1.0f, 0.0f,
                   -11.5f, -11.5f, -11.5f, 1.0f, 0.0f,
                   -15.5f, -11.5f, -11.5f, 0.0f, 0.0f,
                   -15.5f, -11.5f, -11.5f, 0.0f, 1.0f};
  float lightning[] = {///DESEN KARAKTER POWERUP
                       // pos      // tex
                       -15.5f, -11.5f, -15.5f, 0.0f, 1.0f,
                       -11.5f, -11.5f, -15.5f, 1.0f, 1.0f,
                       -11.5f, -11.5f, -11.5f, 1.0f, 0.0f,
                       -11.5f, -11.5f, -11.5f, 1.0f, 0.0f,
                       -15.5f, -11.5f, -11.5f, 0.0f, 0.0f,
                       -15.5f, -11.5f, -11.5f, 0.0f, 1.0f};
  float healthbarCOON[] = {
      0.6f, 1.0f, 0.0f, 0.0f, 1.0f,
      0.6f, -1.0f, 0.0f, 1.0f, 1.0f,
      2.6f, -1.0f, 0.0f, 1.0f, 0.0f,

      0.6f, 1.0f, 0.0f, 1.0f, 0.0f,
      2.6f, 1.0f, 0.0f, 0.0f, 0.0f,
      2.6f, -1.0f, 0.0f, 0.0f, 1.0f,
      //vtor
      2.6f, 1.0f, 0.0f, 0.0f, 0.0f,
      2.6f, -1.0f, 0.0f, 0.0f, 1.0f,
      4.6f, -1.0f, 0.0f, 0.0f, 1.0f,

      2.6f, 1.0f, 0.0f, 0.0f, 0.0f,
      4.6f, -1.0f, 0.0f, 0.0f, 1.0f,
      4.6f, 1.0f, 0.0f, 0.0f, 0.0,
      //tret
      4.6f, 1.0f, 0.0f, 0.0f, 0.0f,
      4.6f, -1.0f, 0.0f, 0.0f, 1.0f,
      6.6f, -1.0f, 0.0f, 0.0f, 1.0f,

      4.6f, 1.0f, 0.0f, 0.0f, 0.0f,
      6.6f, -1.0f, 0.0f, 0.0f, 1.0f,
      6.6f, 1.0f, 0.0f, 0.0f, 0.0,
      //4
      6.6f, 1.0f, 0.0f, 0.0f, 0.0f,
      6.6f, -1.0f, 0.0f, 0.0f, 1.0f,
      8.6f, -1.0f, 0.0f, 0.0f, 1.0f,

      6.6f, 1.0f, 0.0f, 0.0f, 0.0f,
      8.6f, -1.0f, 0.0f, 0.0f, 1.0f,
      8.6f, 1.0f, 0.0f, 0.0f, 0.0,
      //5
      8.6f, 1.0f, 0.0f, 0.0f, 0.0f,
      8.6f, -1.0f, 0.0f, 0.0f, 1.0f,
      10.6f, -1.0f, 0.0f, 0.0f, 1.0f,

      8.6f, 1.0f, 0.0f, 0.0f, 0.0f,
      10.6f, -1.0f, 0.0f, 0.0f, 1.0f,
      10.6f, 1.0f, 0.0f, 0.0f, 0.0

  };
  float healthbarCHAOS[] = {
      0.6f, 1.0f, 0.0f, 0.0f, 1.0f,
      0.6f, -1.0f, 0.0f, 1.0f, 1.0f,
      2.6f, -1.0f, 0.0f, 1.0f, 0.0f,

      0.6f, 1.0f, 0.0f, 1.0f, 0.0f,
      2.6f, 1.0f, 0.0f, 0.0f, 0.0f,
      2.6f, -1.0f, 0.0f, 0.0f, 1.0f,
      //vtor
      2.6f, 1.0f, 0.0f, 0.0f, 0.0f,
      2.6f, -1.0f, 0.0f, 0.0f, 1.0f,
      4.6f, -1.0f, 0.0f, 0.0f, 1.0f,

      2.6f, 1.0f, 0.0f, 0.0f, 0.0f,
      4.6f, -1.0f, 0.0f, 0.0f, 1.0f,
      4.6f, 1.0f, 0.0f, 0.0f, 0.0,
      //tret
      4.6f, 1.0f, 0.0f, 0.0f, 0.0f,
      4.6f, -1.0f, 0.0f, 0.0f, 1.0f,
      6.6f, -1.0f, 0.0f, 0.0f, 1.0f,

      4.6f, 1.0f, 0.0f, 0.0f, 0.0f,
      6.6f, -1.0f, 0.0f, 0.0f, 1.0f,
      6.6f, 1.0f, 0.0f, 0.0f, 0.0,
      //4
      6.6f, 1.0f, 0.0f, 0.0f, 0.0f,
      6.6f, -1.0f, 0.0f, 0.0f, 1.0f,
      8.6f, -1.0f, 0.0f, 0.0f, 1.0f,

      6.6f, 1.0f, 0.0f, 0.0f, 0.0f,
      8.6f, -1.0f, 0.0f, 0.0f, 1.0f,
      8.6f, 1.0f, 0.0f, 0.0f, 0.0,
      //5
      8.6f, 1.0f, 0.0f, 0.0f, 0.0f,
      8.6f, -1.0f, 0.0f, 0.0f, 1.0f,
      10.6f, -1.0f, 0.0f, 0.0f, 1.0f,

      8.6f, 1.0f, 0.0f, 0.0f, 0.0f,
      10.6f, -1.0f, 0.0f, 0.0f, 1.0f,
      10.6f, 1.0f, 0.0f, 0.0f, 0.0};
  unsigned int VBO1, VBO2, VBO3, VBO4, VBO5, VBO6, VBO7, VAO1, VAO2, VAO3, VAO4, VAO5, VAO6, VAO7;
  glGenVertexArrays(1, &VAO1);
  glGenVertexArrays(1, &VAO2);
  glGenVertexArrays(1, &VAO3);
  glGenVertexArrays(1, &VAO4);
  glGenVertexArrays(1, &VAO5);
  glGenVertexArrays(1, &VAO6);
  glGenVertexArrays(1, &VAO7);
  glGenBuffers(1, &VBO1);
  glGenBuffers(1, &VBO2);
  glGenBuffers(1, &VBO3);
  glGenBuffers(1, &VBO4);
  glGenBuffers(1, &VBO5);
  glGenBuffers(1, &VBO6);
  glGenBuffers(1, &VBO7);
  ////RAMNINA//////////////////////////////////
  glBindVertexArray(VAO1);

  glBindBuffer(GL_ARRAY_BUFFER, VBO1);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  // texture coord attribute// position attribute
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), nullptr);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                        reinterpret_cast<void *>(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  //2D SPRITE LEV///////////////////////////////////////////

  glBindVertexArray(VAO2);

  glBindBuffer(GL_ARRAY_BUFFER, VBO2);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);

  // texture coord attribute// position attribute
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), nullptr);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                        reinterpret_cast<void *>(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  //2D SPRTE DESEN////////////////////////////////////////////
  glBindVertexArray(VAO3);

  glBindBuffer(GL_ARRAY_BUFFER, VBO3);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices3), vertices3, GL_STATIC_DRAW);

  // texture coord attribute// position attribute
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), nullptr);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                        reinterpret_cast<void *>(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  ///CLAWS////////////////////////////////////////////
  glBindVertexArray(VAO4);

  glBindBuffer(GL_ARRAY_BUFFER, VBO4);
  glBufferData(GL_ARRAY_BUFFER, sizeof(claws), claws, GL_STATIC_DRAW);

  // texture coord attribute// position attribute
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), nullptr);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                        reinterpret_cast<void *>(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  ///LIGHTNING////////////////////////////////////////////
  glBindVertexArray(VAO5);

  glBindBuffer(GL_ARRAY_BUFFER, VBO5);
  glBufferData(GL_ARRAY_BUFFER, sizeof(lightning), lightning, GL_STATIC_DRAW);

  // texture coord attribute// position attribute
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), nullptr);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                        reinterpret_cast<void *>(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  ///HEALTHBAR FOR THE COON///////////////////////////////////////////
  glBindVertexArray(VAO6);

  glBindBuffer(GL_ARRAY_BUFFER, VBO6);
  glBufferData(GL_ARRAY_BUFFER, sizeof(healthbarCOON), healthbarCOON, GL_STATIC_DRAW);

  // texture coord attribute// position attribute
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), nullptr);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                        reinterpret_cast<void *>(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  ///HEALTHBAR FOR PROF CHAOS///////////////////////////////////////////
  glBindVertexArray(VAO7);

  glBindBuffer(GL_ARRAY_BUFFER, VBO7);
  glBufferData(GL_ARRAY_BUFFER, sizeof(healthbarCHAOS), healthbarCHAOS, GL_STATIC_DRAW);

  // texture coord attribute// position attribute
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), nullptr);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                        reinterpret_cast<void *>(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  // load and create a texture
  // -------------------------
  unsigned int texture1, texture2, texture3, texture4, texture5, texture6;
  // texture 1
  // ---------
  glGenTextures(1, &texture1);
  glBindTexture(GL_TEXTURE_2D, texture1);
  // set the texture wrapping parameters
  glTexParameteri(
      GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
      GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  // set texture filtering parameters
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  // load image, create texture and generate mipmaps
  int width, height, nrChannels;
  stbi_set_flip_vertically_on_load(
      true); // tell stb_image.h to flip loaded texture's on the y-axis.
  // The FileSystem::getPath(...) is part of the GitHub repository so we can
  // find files on any IDE/platform; replace it with your own image path.
  unsigned char *data = stbi_load("../res/textures/blue.jpg", &width,
                                  &height, &nrChannels, 0);
  if (data)
  {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
                 GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
  }
  else
  {
    std::cout << "Failed to load texture" << std::endl;
  }
  stbi_image_free(data);
  // texture 2
  // ---------
  glGenTextures(1, &texture2);
  glBindTexture(GL_TEXTURE_2D, texture2);
  // set the texture wrapping parameters
  glTexParameteri(
      GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
      GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  // set texture filtering parameters
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  // load image, create texture and generate mipmaps
  data = stbi_load("../res/textures/chaos.png", &width, &height,
                   &nrChannels, 0);
  if (data)
  {
    //   // note that the awesomeface.png has transparency and thus an alpha channel,
    //   // so make sure to tell OpenGL the data type is of GL_RGBA
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
                 GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
  }
  else
  {
    std::cout << "Failed to load texture" << std::endl;
  }
  stbi_image_free(data);

  //texture 3 TRANSPARENT
  // ---------
  glGenTextures(1, &texture3);
  glBindTexture(GL_TEXTURE_2D, texture3);
  // set the texture wrapping parameters
  glTexParameteri(
      GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
      GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  // set texture filtering parameters
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  // load image, create texture and generate mipmaps
  data = stbi_load("../res/textures/transp.png", &width, &height,
                   &nrChannels, 0);
  if (data)
  {
    //   // note that the awesomeface.png has transparency and thus an alpha channel,
    //   // so make sure to tell OpenGL the data type is of GL_RGBA
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
                 GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
  }
  else
  {
    std::cout << "Failed to load texture" << std::endl;
  }
  stbi_image_free(data);

  //texture 4 THE COON
  // ---------
  glGenTextures(1, &texture4);
  glBindTexture(GL_TEXTURE_2D, texture4);
  // set the texture wrapping parameters
  glTexParameteri(
      GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
      GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  // set texture filtering parameters
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  // load image, create texture and generate mipmaps
  data = stbi_load("../res/textures/coon.png", &width, &height,
                   &nrChannels, 0);
  if (data)
  {
    //   // note that the awesomeface.png has transparency and thus an alpha channel,
    //   // so make sure to tell OpenGL the data type is of GL_RGBA
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
                 GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
  }
  else
  {
    std::cout << "Failed to load texture" << std::endl;
  }
  stbi_image_free(data);

  //texture 5 THE COON CLAWS---////////////////////////////////
  // ---------
  glGenTextures(1, &texture5);
  glBindTexture(GL_TEXTURE_2D, texture5);
  // set the texture wrapping parameters
  glTexParameteri(
      GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
      GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  // set texture filtering parameters
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  // load image, create texture and generate mipmaps
  data = stbi_load("../res/textures/claws.png", &width, &height,
                   &nrChannels, 0);
  if (data)
  {
    //   // note that the awesomeface.png has transparency and thus an alpha channel,
    //   // so make sure to tell OpenGL the data type is of GL_RGBA
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
                 GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
  }
  else
  {
    std::cout << "Failed to load texture" << std::endl;
  }
  stbi_image_free(data);

  //texture 6 THE CHAOS LIGHTNING---------------////////////////////
  // ---------
  glGenTextures(1, &texture6);
  glBindTexture(GL_TEXTURE_2D, texture6);
  // set the texture wrapping parameters
  glTexParameteri(
      GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
      GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  // set texture filtering parameters
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  // load image, create texture and generate mipmaps
  data = stbi_load("../res/textures/lightning.png", &width, &height,
                   &nrChannels, 0);
  if (data)
  {
    //   // note that the awesomeface.png has transparency and thus an alpha channel,
    //   // so make sure to tell OpenGL the data type is of GL_RGBA
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
                 GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
  }
  else
  {
    std::cout << "Failed to load texture" << std::endl;
  }
  stbi_image_free(data);
  // tell opengl for each sampler to which texture unit it belongs to (only has
  // to be done once)
  // -------------------------------------------------------------------------------------------
  ourShader.use();
  ourShader.setInt("texture1", 0);

  spriteShader1.use();
  spriteShader1.setInt("texture3", 1);
  spriteShader1.setInt("texture4", 2);

  spriteShader2.use();
  spriteShader2.setInt("texture3", 3);
  spriteShader2.setInt("texture2", 4);

  clawShader.use();
  clawShader.setInt("texture5", 5);

  lightShader.use();
  lightShader.setInt("texture6", 6);

  while (!glfwWindowShouldClose(window))
  {
    CHIT = false, LHIT = false;
    // per-frame time logic
    // --------------------
    float currentFrame = static_cast<float>(glfwGetTime());
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    // input
    // -----
    processInput(window);

    // render
    // ------
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    ourShader.use(); // activate shader
    // bind textures on corresponding texture units
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture1);

    // pass projection matrix to shader (note that in this case it could change
    // every frame)
    glm::mat4 projection = glm::perspective(
        glm::radians(camera.Zoom), SCR_WIDTH * 1.0f / SCR_HEIGHT, 0.1f, 100.0f);
    ourShader.setMat4("projection", projection);

    // camera/view transformation
    glm::mat4 view = camera.GetViewMatrix();
    ourShader.setMat4("view", view);

    glBindVertexArray(VAO1);

    // calculate the model matrix for each object and pass it to shader before drawing
    glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    float angle = 20.0f * 0;
    model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
    ourShader.setMat4("model", model);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    ////////////////////////////VTOR SHADER///////////////////
    spriteShader1.use();
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture3);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, texture4);

    glBindVertexArray(VAO2);
    // pass projection matrix to shader (note that in this case it could change
    // every frame)
    glm::mat4 projection2 = glm::perspective(
        glm::radians(camera.Zoom), SCR_WIDTH * 1.0f / SCR_HEIGHT, 0.1f, 100.0f);
    spriteShader1.setMat4("projection", projection2);

    // camera/view transformation
    glm::mat4 view2 = camera.GetViewMatrix();
    spriteShader1.setMat4("view", view2);

    glm::mat4 model2 = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first

    model2 = glm::rotate(model2, glm::radians(90.0f), glm::vec3(40.0f, -0.3f, 0.5f));
    if (count1 < 6)
    {
      deadcoon = true;
      model2 = glm::translate(model2, glm::vec3(5.0f, -10.0f, 16.5f));
    }
    else
      model2 = glm::translate(model2, glm::vec3(x1, y1, z1));
    spriteShader1.setMat4("model", model2);

    glDrawArrays(GL_TRIANGLES, 0, 6);

    ////////////////////////////TRET SHADER///////////////////
    spriteShader2.use();
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, texture3);
    glActiveTexture(GL_TEXTURE4);
    glBindTexture(GL_TEXTURE_2D, texture2);

    glBindVertexArray(VAO3);
    // pass projection matrix to shader (note that in this case it could change
    // every frame)
    glm::mat4 projection3 = glm::perspective(
        glm::radians(camera.Zoom), SCR_WIDTH * 1.0f / SCR_HEIGHT, 0.1f, 100.0f);
    spriteShader2.setMat4("projection", projection3);

    // camera/view transformation
    glm::mat4 view3 = camera.GetViewMatrix();
    spriteShader2.setMat4("view", view3);

    glm::mat4 model3 = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
                                        //float angle2 = 90.0f;
    model3 = glm::rotate(model3, glm::radians(90.0f), glm::vec3(40.0f, -0.3f, 0.5f));
    if (count2 < 6)
    {
      deadchaos = true;
      model3 = glm::translate(model3, glm::vec3(5.0f, -10.0f, 16.5f));
    }
    else
      model3 = glm::translate(model3, glm::vec3(x2, y2, z2));
    spriteShader2.setMat4("model", model3);

    glDrawArrays(GL_TRIANGLES, 0, 6);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    /////////////////////////////CLAWS SHADER///////////////////////
    clawShader.use();
    glActiveTexture(GL_TEXTURE5);
    glBindTexture(GL_TEXTURE_2D, texture5);

    glBindVertexArray(VAO4);
    // pass projection matrix to shader (note that in this case it could change
    // every frame)
    glm::mat4 projection4 = glm::perspective(
        glm::radians(camera.Zoom), SCR_WIDTH * 1.0f / SCR_HEIGHT, 0.1f, 100.0f);
    clawShader.setMat4("projection", projection4);

    // camera/view transformation
    glm::mat4 view4 = camera.GetViewMatrix();
    clawShader.setMat4("view", view4);

    glm::mat4 model4 = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first

    model4 = glm::rotate(model4, glm::radians(90.0f), glm::vec3(40.0f, -0.3f, 0.5f));
    if (clawShot == CNOSHOOT)
    {
      model4 = glm::translate(model4, glm::vec3(0.0f, -10.0f, 16.5f));
    }
    else if (clawShot == CSHOOT)
    {
      br += 0.05f;
      if (br >= 35.4301)
      {
        clawShot = CNOSHOOT;
      }
      if ((br >= x2 - 0.5) && (br <= x2 + 0.5) && (cy >= y2 - 0.5) && (cy <= y2 + 0.5))
      {

        LHIT = true;
        clawShot = CNOSHOOT;
        br = x1;
      }
      model4 = glm::translate(model4, glm::vec3(br, cy, cz));
    }

    clawShader.setMat4("model", model4);

    glDrawArrays(GL_TRIANGLES, 0, 6);

    ////////////////////////LIGHTNING SHADER////////////////////////////
    lightShader.use();
    glActiveTexture(GL_TEXTURE6);
    glBindTexture(GL_TEXTURE_2D, texture6);

    glBindVertexArray(VAO5);

    // pass projection matrix to shader (note that in this case it could change
    // every frame)
    glm::mat4 projection5 = glm::perspective(
        glm::radians(camera.Zoom), SCR_WIDTH * 1.0f / SCR_HEIGHT, 0.1f, 100.0f);
    lightShader.setMat4("projection", projection5);

    // camera/view transformation
    glm::mat4 view5 = camera.GetViewMatrix();
    lightShader.setMat4("view", view5);

    glm::mat4 model5 = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first

    model5 = glm::rotate(model5, glm::radians(90.0f), glm::vec3(40.0f, -0.3f, 0.5f));

    if (lightShot == LNOSHOOT)
    {
      model5 = glm::translate(model5, glm::vec3(5.0f, -10.0f, 16.5f));
    }
    else if (lightShot == LSHOOT)
    {
      br2 -= 0.05f;
      if (br2 <= -35.1)
      {
        lightShot = LNOSHOOT;
      }
      if ((br2 >= x1 - 0.5) && (br2 <= x1 + 0.5) && (ly >= y1 - 0.5) && (ly <= y1 + 0.5))
      {

        CHIT = true;
        lightShot = LNOSHOOT;
        br2 = x2;
      }
      model5 = glm::translate(model5, glm::vec3(br2, ly, lz));
    }
    lightShader.setMat4("model", model5);

    glDrawArrays(GL_TRIANGLES, 0, 6);

    ////////////////////////////HEALTHBAR COON SHADER///////////////////
    healthbar1.use();

    glBindVertexArray(VAO6);
    // pass projection matrix to shader (note that in this case it could change
    // every frame)
    glm::mat4 projection6 = glm::perspective(
        glm::radians(camera.Zoom), SCR_WIDTH * 1.0f / SCR_HEIGHT, 0.1f, 100.0f);
    healthbar1.setMat4("projection", projection6);

    // camera/view transformation
    glm::mat4 view6 = camera.GetViewMatrix();
    healthbar1.setMat4("view", view6);

    glm::mat4 model6 = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    //float angle2 = 90.0f;
    model6 = glm::rotate(model6, glm::radians(180.0f), glm::vec3(0.0f, 10.0f, 0.0f));
    model6 = glm::translate(model6, glm::vec3(-0.55f, 0.0f, -12.05f));
    healthbar1.setMat4("model", model6);
    if (CHIT)
    {
      count1 -= 6;
    }
    glDrawArrays(GL_TRIANGLES, 0, count1);

    ////////////////////////////HEALTHBAR CHAOS SHADER///////////////////
    healthbar2.use();

    glBindVertexArray(VAO7);
    // pass projection matrix to shader (note that in this case it could change
    // every frame)
    glm::mat4 projection7 = glm::perspective(
        glm::radians(camera.Zoom), SCR_WIDTH * 1.0f / SCR_HEIGHT, 0.1f, 100.0f);
    healthbar2.setMat4("projection", projection7);

    // camera/view transformation
    glm::mat4 view7 = camera.GetViewMatrix();
    healthbar2.setMat4("view", view7);

    glm::mat4 model7 = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    //float angle2 = 90.0f;
    // model7 = glm::rotate(model7, glm::radians(180.0f), glm::vec3(55.0f, 55.3f, -0.5f));
    model7 = glm::translate(model7, glm::vec3(-0.6f, 0.0f, 12.0f));
    healthbar2.setMat4("model", model7);

    if (LHIT)
    {
      count2 -= 6;
    }
    glDrawArrays(GL_TRIANGLES, 0, count2);

    // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved
    // etc.)
    // -------------------------------------------------------------------------------
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  // optional: de-allocate all resources once they've outlived their purpose:
  // ------------------------------------------------------------------------
  glDeleteVertexArrays(1, &VAO1);
  glDeleteBuffers(1, &VBO1);
  glDeleteVertexArrays(1, &VAO2);
  glDeleteBuffers(1, &VBO2);
  glDeleteVertexArrays(1, &VAO3);
  glDeleteBuffers(1, &VBO3);
  glDeleteVertexArrays(1, &VAO4);
  glDeleteBuffers(1, &VBO4);
  glDeleteVertexArrays(1, &VAO5);
  glDeleteBuffers(1, &VBO5);
  glDeleteVertexArrays(1, &VAO6);
  glDeleteBuffers(1, &VBO6);
  glDeleteVertexArrays(1, &VAO7);
  glDeleteBuffers(1, &VBO7);
  // glfw: terminate, clearing all previously allocated GLFW resources.
  // ------------------------------------------------------------------
  glfwTerminate();
  return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this
// frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
  /////////////////////COON MOVEMENT///////////////////////
  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
  {
    if (y1 < -12.3302)
    {
      y1 = y1;
    }
    else
      y1 = y1 - 0.01;
  }
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
  {
    if (y1 > 8.3501)
    {
      y1 = y1;
    }
    else
      y1 = y1 + 0.01;
  }
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
  {
    if (x1 < -9.86013)
    {
      x1 = x1;
    }
    else
      x1 = x1 - 0.01;
  }
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
  {
    if (x1 > 8.8701)
    {
      x1 = x1;
    }
    else
      x1 = x1 + 0.01;
  }
  /////////////////////CHAOS MOVEMENT////////////////////////
  if (glfwGetKey(window, GLFW_KEY_KP_8) == GLFW_PRESS)
  {
    if (y2 < -12.3302)
    {
      y2 = y2;
    }
    else
      y2 = y2 - 0.01;
  }
  if (glfwGetKey(window, GLFW_KEY_KP_5) == GLFW_PRESS)
  {
    if (y2 > 8.3501)
    {
      y2 = y2;
    }
    else
      y2 = y2 + 0.01;
  }
  if (glfwGetKey(window, GLFW_KEY_KP_4) == GLFW_PRESS)
  {
    if (x2 < 14.9997)
    {
      x2 = x2;
    }
    else
      x2 = x2 - 0.01;
  }
  if (glfwGetKey(window, GLFW_KEY_KP_6) == GLFW_PRESS)
  {
    if (x2 > 33.4999)
    {
      x2 = x2;
    }
    else
      x2 = x2 + 0.01;
  }
  /////////////CLAWS SHOOT////////////////////////
  if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
  {
    if (deadcoon)
    {
      clawShot = CNOSHOOT;
    }
    else
    {
      cy = y1;
      cz = z1;
      br = x1;
      clawShot = CSHOOT;
    }
  }
  /////////////LIGHTNING STRIKE////////////////////
  if (glfwGetKey(window, GLFW_KEY_BACKSPACE) == GLFW_PRESS)
  {
    if (deadchaos)
    {
      lightShot = LNOSHOOT;
    }
    else
    {
      ly = y2;
      lz = z2;
      br2 = x2;
      lightShot = LSHOOT;
    }
  }
  ///////////////////CAMERA MOVEMENT//////////////////////
  if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
    camera.ProcessKeyboard(FORWARD, deltaTime);
  if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
    camera.ProcessKeyboard(BACKWARD, deltaTime);
  if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
    camera.ProcessKeyboard(LEFT, deltaTime);
  if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
    camera.ProcessKeyboard(RIGHT, deltaTime);
}

// glfw: whenever the window size changed (by OS or user resize) this callback
// function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
  // make sure the viewport matches the new window dimensions; note that width
  // and height will be significantly larger than specified on retina displays.
  glViewport(0, 0, width, height);
}
