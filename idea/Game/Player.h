#pragma once

#include <iostream>
#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Types/EMovement.h"
#include "Game/Entity.h"

class Player : public virtual Entity
{
public:
    glm::vec3 position_;
    glm::vec3 world_up_;
    glm::vec3 front_;
    glm::vec3 right_;
    glm::vec3 up_;
    float movement_speed_;
    float movement_speed_fast_;
    float yaw_;
    float pitch_;
    
    Player(glm::vec3 starting_position = glm::vec3(0.0f, 0.0f, 0.0f),
        TerrainElement terrel = TerrainElement(
                Model("Resources/Models/player/player.obj", true),
                Shader("Resources/Shaders/Model/lowPolyPlayer.vert", "Resources/Shaders/Model/lowPolyPlayer.frag")),
        glm::mat4 world_transform = glm::mat4(1.0f));

    glm::vec3 GetPosition();
    void Draw();

private:
    static const float _YAW_;
    static const float _PITCH_;
    static const float _SPEED_;
    static const float _SPEED_FAST_;
    
    void updatePlayerVectors();
};

