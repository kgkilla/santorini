#ifndef CAMERA_H
#define CAMERA_H

#include <iostream>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

// Default camera values
const float SPEED       =  250.f;
const float SENSITIVITY =  0.1f;
const float ZOOM        =  5.0f;

// An abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
class Camera
{
    int corner;
    bool spinning;
    bool birdsEye;

    // Camera Attributes
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;
    glm::vec3 Target;

public:

    float Zoom;

    // Constructor with vectors
    Camera(glm::vec3 position = glm::vec3(60.0f, 30.0f, 60.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f))
    {
        spinning = false;
        corner = 0;
        Zoom = ZOOM;

        Position = position;
        WorldUp = up;
        Target = glm::vec3(0.f, 0.f, 0.f);
        updateCameraVectors();
    }

    // Returns the view matrix calculated using Euler Angles and the LookAt Matrix
    glm::mat4 GetViewMatrix()
    {
        return glm::lookAt(Position, glm::vec3(0.f, 0.f, 0.f), Up);
    }

    // Returns true when completed
    bool spinLeft(float deltaTime) {
        // Initialize spinning if idle
        if(!spinning) {
            spinning = true;
        }
        // Already spinning, check to see if we can stop
        else if(spinning) {
            if((corner == 0 || corner == 2) && (Position.x < -60.0f || Position.x > 60.0f)) {
                Position.x = clampDirection(Position.x, -60.0f, 60.0f);
                spinning = false;
                corner++;
                updateCameraVectors();
                return true;
            }
            else if((corner == 1 || corner == 3) && (Position.z < -60.0f || Position.z > 60.0f)) {
                Position.z = clampDirection(Position.z, -60.0f, 60.0f);
                spinning = false;
                corner = (corner == 1) ? (corner+1) : 0;
                updateCameraVectors();
                return true;
            }
        }

        // Movement direction based on corner the camera is in
        switch(corner) {
            case 0: Position.x -= (SPEED * deltaTime); break;
            case 1: Position.z -= (SPEED * deltaTime); break;
            case 2: Position.x += (SPEED * deltaTime); break;
            case 3: Position.z += (SPEED * deltaTime); break;
        }

        updateCameraVectors();
        return false;
    }

    // Returns true when completed
    bool spinRight(float deltaTime) {
        // Initialize spinning if idle
        if(!spinning) {
            spinning = true;
        }
        // Already spinning, check to see if we can stop
        else if(spinning) {
            if((corner == 1 || corner == 3) && (Position.x < -60.0f || Position.x > 60.0f)) {
                Position.x = clampDirection(Position.x, -60.0f, 60.0f);
                spinning = false;
                corner--;
                updateCameraVectors();
                return true;
            }
            else if((corner == 0 || corner == 2) && (Position.z < -60.0f || Position.z > 60.0f)) {
                Position.z = clampDirection(Position.z, -60.0f, 60.0f);
                spinning = false;
                corner = (corner == 0) ? 3 : (corner-1);
                updateCameraVectors();
                return true;
            }
        }

        // Movement direction based on corner the camera is in
        switch(corner) {
            case 0: Position.z -= (SPEED * deltaTime); break;
            case 1: Position.x += (SPEED * deltaTime); break;
            case 2: Position.z += (SPEED * deltaTime); break;
            case 3: Position.x -= (SPEED * deltaTime); break;
        }

        updateCameraVectors();
        return false;
    }

    // Returns true when completed
    bool spinUp(float deltaTime) {
        bool ret = false;

        // Initialize movement if idle and not already in birds eye view
        if(!spinning && !birdsEye) {
            spinning = true;
            birdsEye = true;
        }
        // Already moving, check to see if we can stop
        else {
            if(Position.x < -1.0f)
                Position.x += (SPEED * deltaTime);
            else
                Position.x -= (SPEED * deltaTime);

            if(Position.z < -1.0f)
                Position.z += (SPEED * deltaTime);
            else
                Position.z -= (SPEED * deltaTime);

            Zoom = (Zoom >= 25.0f) ? 25.0f : (Zoom + (SPEED * deltaTime/3));

            switch(corner) {
                case 0:
                    if(Position.z < 1.0f) {
                        Position.x = clampDirection(Position.x, 1.0f, 60.0f);
                        Position.z = clampDirection(Position.z, 1.0f, 60.0f);
                        spinning = false;
                        ret = true;
                    } break;
                case 1:
                    if(Position.z < 1.0f) {
                        Position.x = clampDirection(Position.x, -60.0f, -1.0f);
                        Position.z = clampDirection(Position.z, 1.0f, 60.0f);
                        spinning = false;
                        ret = true;
                    } break;
                case 2:
                    if(Position.z > -1.0f) {
                        Position.x = clampDirection(Position.x, -60.0f, -1.0f);
                        Position.z = clampDirection(Position.z, -60.0f, -1.0f);
                        spinning = false;
                        ret = true;
                    } break;
                case 3:
                    if(Position.z > -1.0f) {
                        Position.x = clampDirection(Position.x, 1.0f, 60.0f);
                        Position.z = clampDirection(Position.z, -60.0f, -1.0f);
                        spinning = false;
                        ret = true;
                    } break;
            }
        }

        updateCameraVectors();
        return ret;
    }

    // Returns true when completed
    bool spinDown(float deltaTime) {
        // Initialize movement if idle and currently in birds eye view
        if(!spinning && birdsEye) {
            spinning = true;
            birdsEye = false;
        }
        // Already moving, check to see if we can stop
        else {
            Zoom = (Zoom <= ZOOM) ? ZOOM : (Zoom - (SPEED * deltaTime/3));

            switch(corner) {
                case 0:
                    Position.x += (SPEED * deltaTime);
                    Position.z += (SPEED * deltaTime);
                    if(Position.z > 60.0f) {
                        Position.x = clampDirection(Position.x, 1.0f, 60.0f);
                        Position.z = clampDirection(Position.z, 1.0f, 60.0f);
                        spinning = false;
                    } break;
                case 1:
                    Position.x -= (SPEED * deltaTime);
                    Position.z += (SPEED * deltaTime);
                    if(Position.z > 60.0f) {
                        Position.x = clampDirection(Position.x, -60.0f, -1.0f);
                        Position.z = clampDirection(Position.z, 1.0f, 60.0f);
                        spinning = false;
                    } break;
                case 2:
                    Position.x -= (SPEED * deltaTime);
                    Position.z -= (SPEED * deltaTime);
                    if(Position.z < -60.0f) {
                        Position.x = clampDirection(Position.x, -60.0f, -1.0f);
                        Position.z = clampDirection(Position.z, -60.0f, -1.0f);
                        spinning = false;
                    } break;
                case 3:
                    Position.x += (SPEED * deltaTime);
                    Position.z -= (SPEED * deltaTime);
                    if(Position.z < -60.0f) {
                        Position.x = clampDirection(Position.x, 1.0f, 60.0f);
                        Position.z = clampDirection(Position.z, -60.0f, -1.0f);
                        spinning = false;
                    } break;
            }
        }

        updateCameraVectors();
        return spinning ? false : true;
    }

private:
    // Calculates the front vector from the Camera's (updated) Euler Angles
    void updateCameraVectors()
    {
        Front = glm::normalize(Target - Position);
        Right = glm::normalize(glm::cross(Front, WorldUp));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
        Up    = glm::normalize(glm::cross(Right, Front));
    }

    float clampDirection(float value, float lower, float upper) {
        if(value > upper)
            return upper;
        else if(value < lower)
            return lower;
        else
            return value;
    }
};
#endif
