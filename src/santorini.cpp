#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<math.h>
#include<stdio.h>
#include<iostream>
#include<stdbool.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

#include"shader.h"
#include"camera.h"
#include"board.h"

#define SCR_WIDTH 1280
#define SCR_HEIGHT 720
#define GAME_NAME "Santorini"

static float mixValue = 0.2f;
static unsigned int newWidth = SCR_WIDTH;
static unsigned int newHeight = SCR_HEIGHT;

static float deltaTime = 0.0f;	// Time between current frame and last frame
static float lastFrame = 0.0f; // Time of last frame

// camera
Camera camera(glm::vec3(60.0f, 30.0f, 60.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;

static bool g_updateTower = false;
static bool g_updatePlayer = false;
static bool g_birdsEye = false;

static bool g_cameraSpinLeft = false;
static bool g_cameraSpinRight = false;
static bool g_cameraSpinUp = false;
static bool g_cameraSpinDown = false;

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
    newWidth = width;
    newHeight = height;
}

void processInput(GLFWwindow *window)
{
    bool cameraMoving = g_cameraSpinUp || g_cameraSpinDown || g_cameraSpinLeft || g_cameraSpinRight;

    // Close window
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    // Move camera
    else if(glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS && !cameraMoving) {
        g_cameraSpinLeft = true;
    }
    else if(glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS && !cameraMoving) {
        g_cameraSpinRight = true;
    }
    else if(glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS && !cameraMoving) {
        g_cameraSpinUp = true;
    }
    else if(glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS && !cameraMoving) {
        g_cameraSpinDown = true;
    }

    // Move player or make tower (SHOULD EVENTUALLY BE MOUSE INPUT)
    else {
        if(glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
            g_updateTower = true;
        if(glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
            g_updatePlayer = true;
    }
}

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, GAME_NAME, NULL, NULL);
    if (window == NULL)
    {
        std::cout<<"Failed to create GLFW window"<<std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout<<"Failed to initialize GLAD"<<std::endl;
        return -1;
    }
    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);

    // Window resizing
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    glEnable(GL_DEPTH_TEST);

    Board board(2);

    // Render loop
    while(!glfwWindowShouldClose(window))
    {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Clear buffer
        glClearColor(0.8f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Inputs
        processInput(window);

        if(g_updateTower) {
            board.updateTower(0,0);
            board.updateTower(1,1);
            board.updateTower(2,2);
            board.updateTower(3,3);
            board.updateTower(4,4);
        }

        // Process camera movement
        if(g_cameraSpinLeft) {
            if(!g_birdsEye) {
                if(camera.spinLeft(deltaTime)) {
                    g_cameraSpinLeft = false;
                }
            } else g_cameraSpinLeft = false;
        }
        else if(g_cameraSpinRight) {
            if(!g_birdsEye) {
                if(camera.spinRight(deltaTime)) {
                    g_cameraSpinRight = false;
                }
            } else g_cameraSpinRight = false;
        }
        else if(g_cameraSpinUp) {
            if(!g_birdsEye) {
                if(camera.spinUp(deltaTime)) {
                    g_cameraSpinUp = false;
                    g_birdsEye = true;
                }
            }
            else {
                g_cameraSpinUp = false;
            }
        }
        else if(g_cameraSpinDown) {
            if(g_birdsEye) {
                if(camera.spinDown(deltaTime)) {
                    g_cameraSpinDown = false;
                    g_birdsEye = false;
                }
            }
            else {
                g_cameraSpinDown = false;
            }
        }

        /*********************
        /* Rendering commands
        /********************/
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)newWidth / (float)newHeight, 0.1f, 200.0f);
        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 model = glm::mat4(1.0f);

        g_updateTower = false;
        g_updatePlayer = false;
        board.drawBoard(model, view, projection);

        // Check for events and swap buffers
        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    glfwTerminate();

    return 0;
}
