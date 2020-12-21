#pragma once

#include<iostream>
#include<vector>
#include<random>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Renderer/Renderer.h"
#include "Terrain/NoiseGenerator.h"

class TerrainGenerator
{
public:
    TerrainGenerator(const int gridSize = 256);

    std::vector<glm::vec3>& GetPositions();
    std::vector<glm::vec3>& GetNormals();
    std::vector<glm::vec3>& GetColors();

private:
    const int gridSize;
    float* heightMap;
    NoiseGenerator noiseGenerator;

    std::vector<glm::vec3> grid;

    std::vector<glm::vec3> positions;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec3> colors;

    void generateHeightMap();
    void generateGrid();
    void generateVertexPositions();
    void generateVertexColors();
    void generateVegetationPositions();

    glm::vec3 calculateTriangleNormal(glm::vec3 v0, glm::vec3 v1, glm::vec3 v2);
};

