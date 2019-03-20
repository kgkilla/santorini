#ifndef PLAYER_H
#define PLAYER_H

#include "game_defs.h"

#define PLAYER_TOP 0.75f
#define PLAYER_BOTTOM 0.01f
#define PLAYER_CENTER 0.0f
#define PLAYER_RIGHT 0.25f
#define PLAYER_LEFT -0.25f

class Player
{
    uint8_t x, y;
    glm::mat4 model;
    Shader *playerShader;
    unsigned int VAO, VBO;
    unsigned int texture;

    const float vertices[18*5] = {
        // Top faces
        PLAYER_LEFT,   PLAYER_BOTTOM, PLAYER_LEFT,     0.0f, 0.0f,
        PLAYER_RIGHT,  PLAYER_BOTTOM, PLAYER_LEFT,     1.0f, 0.0f,
        PLAYER_CENTER,  PLAYER_TOP,    PLAYER_CENTER,     1.0f, 1.0f,

        PLAYER_RIGHT,  PLAYER_BOTTOM,    PLAYER_RIGHT,     1.0f, 1.0f,
        PLAYER_LEFT,   PLAYER_BOTTOM,    PLAYER_RIGHT,     0.0f, 1.0f,
        PLAYER_CENTER,   PLAYER_TOP, PLAYER_CENTER,     0.0f, 0.0f,

        PLAYER_LEFT,   PLAYER_BOTTOM, PLAYER_RIGHT,    0.0f, 0.0f,
        PLAYER_RIGHT,  PLAYER_BOTTOM, PLAYER_RIGHT,    1.0f, 0.0f,
        PLAYER_CENTER,  PLAYER_TOP,    PLAYER_CENTER,    1.0f, 1.0f,

        PLAYER_RIGHT,  PLAYER_BOTTOM,    PLAYER_LEFT,    1.0f, 1.0f,
        PLAYER_LEFT,   PLAYER_BOTTOM,    PLAYER_LEFT,    0.0f, 1.0f,
        PLAYER_CENTER,   PLAYER_TOP, PLAYER_CENTER,    0.0f, 0.0f,


        // Bottom faces
        PLAYER_LEFT,   PLAYER_BOTTOM,    PLAYER_LEFT,    1.0f, 0.0f,
        PLAYER_LEFT,   PLAYER_BOTTOM,    PLAYER_RIGHT,     1.0f, 1.0f,
        PLAYER_RIGHT,   PLAYER_BOTTOM, PLAYER_RIGHT,     0.0f, 1.0f,
        PLAYER_RIGHT,   PLAYER_BOTTOM, PLAYER_RIGHT,     0.0f, 1.0f,
        PLAYER_RIGHT,   PLAYER_BOTTOM, PLAYER_RIGHT,    0.0f, 0.0f,
        PLAYER_LEFT,   PLAYER_BOTTOM,    PLAYER_LEFT,    1.0f, 0.0f,
    };

public:
    Player(uint8_t X = 0, uint8_t Y = 0) {
        x = X;
        y = Y;
        model = glm::mat4(1.0f);

        playerShader = new Shader("shaders/shader.vs", "shaders/shader.fs");

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

    ~Player() {
        delete playerShader;
    }

    void setLocation(uint8_t newX, uint8_t newY) {
        x = newX;
        y = newY;
    }

    void drawPlayer(glm::mat4 view, glm::mat4 projection) {

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);

        model = glm::translate(glm::mat4(1.0f), glm::vec3(x, 0.0f, y));

        playerShader->use();
        playerShader->setMat4("model", model);
        playerShader->setMat4("view", view);
        playerShader->setMat4("projection", projection);

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 18);

    }
};
#endif
