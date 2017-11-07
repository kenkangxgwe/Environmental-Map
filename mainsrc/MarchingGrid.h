//--------------------------------------------------------------------------------------
// Interpolates a set of polygons from an implicit function. 
// CAP5705 Marching Cube implementation
// copyright Corey Toler-Franklin 2017
//--------------------------------------------------------------------------------------


#pragma once



#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm.hpp>

#include<iostream>
#include<vector>

class Triag {
public:
    std::vector<glm::vec3> points;
    std::vector<glm::vec3> normals;
};

//simple class to easily hold 
class EdgeVert {
public:
    glm::vec3 position;
    glm::vec3 normal;
};

class CubeVert {
public:
    glm::vec3 position;
    glm::vec3 normal;
    float surfaceValue;
};

class GridCube {
public:
    CubeVert* cVertices[8];	
};

class MarchingGrid {
public:


    MarchingGrid();
    ~MarchingGrid();

    int mMinSize;
    int mMaxSize;
    int mGridSize;
    int mLevel;

	bool Initialize(int size);
    bool IsosurfaceToPolygons(float level, Triag &triangles);
    std::vector<CubeVert> vertices;
    std::vector<GridCube> cubes;


};

