#ifndef TOWER_H
#define TOWER_H

#include "stb_image.h"

#define TOWER_TOP        1.0f
#define TOWER_BOTTOM     0.0f
#define TOWER_LEFT      -0.5f
#define TOWER_RIGHT      0.5f

#define MAX_HEIGHT 3

class Tower
{
    uint8_t height;
    Shader *towerShader;
    unsigned int VAO, VBO;
    unsigned int texture;

    const float vertices[36*5] = {
        TOWER_LEFT,   TOWER_BOTTOM, TOWER_LEFT,  0.0f, 0.0f,
        TOWER_RIGHT,  TOWER_BOTTOM, TOWER_LEFT,  1.0f, 0.0f,
        TOWER_RIGHT,  TOWER_TOP,    TOWER_LEFT,  1.0f, 1.0f,
        TOWER_RIGHT,  TOWER_TOP,    TOWER_LEFT,  1.0f, 1.0f,
        TOWER_LEFT,   TOWER_TOP,    TOWER_LEFT,  0.0f, 1.0f,
        TOWER_LEFT,   TOWER_BOTTOM, TOWER_LEFT,  0.0f, 0.0f,

        TOWER_LEFT,   TOWER_BOTTOM, TOWER_RIGHT,  0.0f, 0.0f,
        TOWER_RIGHT,  TOWER_BOTTOM, TOWER_RIGHT,  1.0f, 0.0f,
        TOWER_RIGHT,  TOWER_TOP,    TOWER_RIGHT,  1.0f, 1.0f,
        TOWER_RIGHT,  TOWER_TOP,    TOWER_RIGHT,  1.0f, 1.0f,
        TOWER_LEFT,   TOWER_TOP,    TOWER_RIGHT,  0.0f, 1.0f,
        TOWER_LEFT,   TOWER_BOTTOM, TOWER_RIGHT,  0.0f, 0.0f,

        TOWER_LEFT,   TOWER_TOP,    TOWER_RIGHT,  1.0f, 0.0f,
        TOWER_LEFT,   TOWER_TOP,    TOWER_LEFT,  1.0f, 1.0f,
        TOWER_LEFT,   TOWER_BOTTOM, TOWER_LEFT,  0.0f, 1.0f,
        TOWER_LEFT,   TOWER_BOTTOM, TOWER_LEFT,  0.0f, 1.0f,
        TOWER_LEFT,   TOWER_BOTTOM, TOWER_RIGHT,  0.0f, 0.0f,
        TOWER_LEFT,   TOWER_TOP,    TOWER_RIGHT,  1.0f, 0.0f,

        TOWER_RIGHT,  TOWER_TOP,    TOWER_RIGHT,  1.0f, 0.0f,
        TOWER_RIGHT,  TOWER_TOP,    TOWER_LEFT,  1.0f, 1.0f,
        TOWER_RIGHT,  TOWER_BOTTOM, TOWER_LEFT,  0.0f, 1.0f,
        TOWER_RIGHT,  TOWER_BOTTOM, TOWER_LEFT,  0.0f, 1.0f,
        TOWER_RIGHT,  TOWER_BOTTOM, TOWER_RIGHT,  0.0f, 0.0f,
        TOWER_RIGHT,  TOWER_TOP,    TOWER_RIGHT,  1.0f, 0.0f,

        TOWER_LEFT,  TOWER_BOTTOM,  TOWER_LEFT,  0.0f, 1.0f,
        TOWER_RIGHT, TOWER_BOTTOM,  TOWER_LEFT,  1.0f, 1.0f,
        TOWER_RIGHT, TOWER_BOTTOM,  TOWER_RIGHT,  1.0f, 0.0f,
        TOWER_RIGHT, TOWER_BOTTOM,  TOWER_RIGHT,  1.0f, 0.0f,
        TOWER_LEFT,  TOWER_BOTTOM,  TOWER_RIGHT,  0.0f, 0.0f,
        TOWER_LEFT,  TOWER_BOTTOM,  TOWER_LEFT,  0.0f, 1.0f,

        TOWER_LEFT,  TOWER_TOP,     TOWER_LEFT,  0.0f, 1.0f,
        TOWER_RIGHT, TOWER_TOP,     TOWER_LEFT,  1.0f, 1.0f,
        TOWER_RIGHT, TOWER_TOP,     TOWER_RIGHT,  1.0f, 0.0f,
        TOWER_RIGHT, TOWER_TOP,     TOWER_RIGHT,  1.0f, 0.0f,
        TOWER_LEFT,  TOWER_TOP,     TOWER_RIGHT,  0.0f, 0.0f,
        TOWER_LEFT,  TOWER_TOP,     TOWER_LEFT,  0.0f, 1.0f
    };

public:
    Tower(void) {
        height = 0;
        towerShader = new Shader("shaders/shader.vs", "shaders/shader.fs");

        glGenBuffers(1, &(VBO));
        glGenVertexArrays(1, &(VAO));

        // 1. bind Vertex Array Object
        glBindVertexArray(VAO);
        // 2. copy our vertices array in a buffer for OpenGL to use
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        // 4. then set our vertex attributes pointers
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)(3*sizeof(float)));
        glEnableVertexAttribArray(1);

        glGenTextures(1, &(texture));
        glBindTexture(GL_TEXTURE_2D, texture);
        // set the texture wrapping/filtering options (on the currently bound texture object)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // load and generate the texture
        int width, height, nrChannels;
        unsigned char *data = stbi_load("textures/awesomeface.png", &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        stbi_image_free(data);
    }
    uint8_t getHeight(void) {
        return height;
    }
    void incrementHeight(void) {
        height += (height < MAX_HEIGHT) ? 1 : 0;
    }

    void drawTower(glm::mat4 model, glm::mat4 view, glm::mat4 projection) {
        // Nothing to do if height is zero
        if(!height)
            return;

        glActiveTexture(GL_TEXTURE0);

        switch(height) {
            case 1:

            case 2:

            case 3:
                glBindTexture(GL_TEXTURE_2D, texture);
                towerShader->use();
                towerShader->setMat4("model", model);
                towerShader->setMat4("view", view);
                towerShader->setMat4("projection", projection);
                break;

            default: break;
        }

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

    }
};
#endif
