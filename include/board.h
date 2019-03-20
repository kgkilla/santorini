#ifndef BOARD_H
#define BOARD_H

#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

#include "game_defs.h"
#include "tower.h"
#include "player.h"
#include "stb_image.h"

#define BOARD_TOP        0.0f
#define BOARD_BOTTOM    -0.5f
#define BOARD_LEFT      -3.5f
#define BOARD_RIGHT      3.5f

#define TEXTURE_PATH "textures/board.png"

class Board
{
    uint8_t numPlayers;
    Tower towers[BOARD_WIDTH][BOARD_WIDTH];
    Player *players;
    Shader *boardShader;
    unsigned int VAO, VBO;
    unsigned int texture;

    const float vertices[36*5] = {
        BOARD_LEFT,   BOARD_BOTTOM, BOARD_LEFT,     0.0f, 0.0f,
        BOARD_RIGHT,  BOARD_BOTTOM, BOARD_LEFT,     1.0f, 0.0f,
        BOARD_RIGHT,  BOARD_TOP,    BOARD_LEFT,     1.0f, 1.0f,
        BOARD_RIGHT,  BOARD_TOP,    BOARD_LEFT,     1.0f, 1.0f,
        BOARD_LEFT,   BOARD_TOP,    BOARD_LEFT,     0.0f, 1.0f,
        BOARD_LEFT,   BOARD_BOTTOM, BOARD_LEFT,     0.0f, 0.0f,

        BOARD_LEFT,   BOARD_BOTTOM, BOARD_RIGHT,    0.0f, 0.0f,
        BOARD_RIGHT,  BOARD_BOTTOM, BOARD_RIGHT,    1.0f, 0.0f,
        BOARD_RIGHT,  BOARD_TOP,    BOARD_RIGHT,    1.0f, 1.0f,
        BOARD_RIGHT,  BOARD_TOP,    BOARD_RIGHT,    1.0f, 1.0f,
        BOARD_LEFT,   BOARD_TOP,    BOARD_RIGHT,    0.0f, 1.0f,
        BOARD_LEFT,   BOARD_BOTTOM, BOARD_RIGHT,    0.0f, 0.0f,

        BOARD_LEFT,   BOARD_TOP,    BOARD_RIGHT,    1.0f, 0.0f,
        BOARD_LEFT,   BOARD_TOP,    BOARD_LEFT,     1.0f, 1.0f,
        BOARD_LEFT,   BOARD_BOTTOM, BOARD_LEFT,     0.0f, 1.0f,
        BOARD_LEFT,   BOARD_BOTTOM, BOARD_LEFT,     0.0f, 1.0f,
        BOARD_LEFT,   BOARD_BOTTOM, BOARD_RIGHT,    0.0f, 0.0f,
        BOARD_LEFT,   BOARD_TOP,    BOARD_RIGHT,    1.0f, 0.0f,

        BOARD_RIGHT,  BOARD_TOP,    BOARD_RIGHT,    1.0f, 0.0f,
        BOARD_RIGHT,  BOARD_TOP,    BOARD_LEFT,     1.0f, 1.0f,
        BOARD_RIGHT,  BOARD_BOTTOM, BOARD_LEFT,     0.0f, 1.0f,
        BOARD_RIGHT,  BOARD_BOTTOM, BOARD_LEFT,     0.0f, 1.0f,
        BOARD_RIGHT,  BOARD_BOTTOM, BOARD_RIGHT,    0.0f, 0.0f,
        BOARD_RIGHT,  BOARD_TOP,    BOARD_RIGHT,    1.0f, 0.0f,

        BOARD_LEFT,  BOARD_BOTTOM,  BOARD_LEFT,     0.0f, 1.0f,
        BOARD_RIGHT, BOARD_BOTTOM,  BOARD_LEFT,     1.0f, 1.0f,
        BOARD_RIGHT, BOARD_BOTTOM,  BOARD_RIGHT,    1.0f, 0.0f,
        BOARD_RIGHT, BOARD_BOTTOM,  BOARD_RIGHT,    1.0f, 0.0f,
        BOARD_LEFT,  BOARD_BOTTOM,  BOARD_RIGHT,    0.0f, 0.0f,
        BOARD_LEFT,  BOARD_BOTTOM,  BOARD_LEFT,     0.0f, 1.0f,

        BOARD_LEFT,  BOARD_TOP,     BOARD_LEFT,     0.0f, 1.0f,
        BOARD_RIGHT, BOARD_TOP,     BOARD_LEFT,     1.0f, 1.0f,
        BOARD_RIGHT, BOARD_TOP,     BOARD_RIGHT,    1.0f, 0.0f,
        BOARD_RIGHT, BOARD_TOP,     BOARD_RIGHT,    1.0f, 0.0f,
        BOARD_LEFT,  BOARD_TOP,     BOARD_RIGHT,    0.0f, 0.0f,
        BOARD_LEFT,  BOARD_TOP,     BOARD_LEFT,     0.0f, 1.0f
    };

public:
    Board(uint8_t numPlayers = 2) {
        Board::numPlayers = numPlayers;
        Board::players = new Player[numPlayers];
        Board::boardShader = new Shader("shaders/shader.vs", "shaders/shader.fs");

        glGenBuffers(1, &(Board::VBO));
        glGenVertexArrays(1, &(Board::VAO));

        // 1. bind Vertex Array Object
        glBindVertexArray(Board::VAO);
        // 2. copy our vertices array in a buffer for OpenGL to use
        glBindBuffer(GL_ARRAY_BUFFER, Board::VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(Board::vertices), Board::vertices, GL_STATIC_DRAW);
        // 4. then set our vertex attributes pointers
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)(3*sizeof(float)));
        glEnableVertexAttribArray(1);

        glGenTextures(1, &(Board::texture));
        glBindTexture(GL_TEXTURE_2D, Board::texture);
        // set the texture wrapping/filtering options (on the currently bound texture object)
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // load and generate the texture
        int width, height, nrChannels;
        unsigned char *data = stbi_load(TEXTURE_PATH, &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else
        {
            std::cout<<"Failed to load texture"<<std::endl;
        }
        stbi_image_free(data);
    }

    ~Board() {
        delete Board::players;
        delete Board::boardShader;
    }

    int updatePlayer(uint8_t player, uint8_t x, uint8_t y) {
        if(player >= numPlayers)
            return -1;

        if(x >= BOARD_WIDTH || y >= BOARD_WIDTH)
            return -1;

        players[player].setLocation(x, y);
    }

    int updateTower(uint8_t x, uint8_t y) {
        if(x >= BOARD_WIDTH || y >= BOARD_WIDTH)
            return -1;

        towers[x][y].incrementHeight();
    }

    void drawBoard(glm::mat4 model, glm::mat4 view, glm::mat4 projection) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, Board::texture);

        Board::boardShader->use();

        Board::boardShader->setMat4("model", model);
        Board::boardShader->setMat4("view", view);
        Board::boardShader->setMat4("projection", projection);

        glBindVertexArray(Board::VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        // Draw each tower
        glm::mat4 identity = glm::mat4(1.0f);
        float iOffset, jOffset;
        for(int i = 0; i < BOARD_WIDTH; i++)
            for(int j = 0; j < BOARD_WIDTH; j++) {
                iOffset = i * 1.28f;
                jOffset = j * 1.28f;
                glm::mat4 towerModel =
                    glm::translate(identity, glm::vec3(-2.55f + iOffset,
                                                        0.0f,
                                                        2.55f - jOffset));

                towers[i][j].drawTower(towerModel, view, projection);
            }

        // Draw each player
        for(int i = 0; i < numPlayers; i++)
            players[i].drawPlayer(view, projection);
    }
};
#endif
