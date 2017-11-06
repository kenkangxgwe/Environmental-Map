//-----------------------------------------------------
//MarchingGrid Framework
// CAP5705 University of Florida
// CAP5705 Marching Cube implementation
// copyright 2017 Corey Toler-Franklin
//------------------------------------------------------
#include "MarchingGrid.h"
#include "GridData.h"



MarchingGrid::MarchingGrid()
    : mMinSize  (10),
      mMaxsize  (100),
      mGridSize (30),
      mLevel    (1.0f)

{

}


MarchingGrid::~MarchingGrid()
{
}


//---------------------------------------------------
// TO DO: Project 4
// Initialize the vertices and build the cubes.
// Given the size, populate the veritices in the grid.
// Distance apart is influenced by the total number of cubes
// and the grid size.
// There are 8 vertices for each cube.
//----------------------------------------------------
bool MarchingGrid::Initialize(float size)
{
	mGridSize = 100 / size + 1;
	float cellSize = 1 / mGridSize;
	for (int i = 0; i <= mGridSize; i++) {
		for (int j = 0; j <= mGridSize; j++) {
			for (int k = 0; k <= mGridSize; k++) {
				CubeVert newVert;
				newVert.position = glm::vec3(k, i, j) * float(mGridSize);
				vertices.push_back(newVert);
			}
		}
	}
	cubes.reserve(mGridSize*mGridSize*mGridSize);
	for (int i = 0; i < cubes.size(); i++) {
		int offset = i / (mGridSize + 1);
		cubes[i].cVertices[0] = &vertices[i - offset];
		cubes[i].cVertices[1] = &vertices[i - offset + 1];
		cubes[i].cVertices[2] = &vertices[i - offset + mGridSize + 2];
		cubes[i].cVertices[3] = &vertices[i - offset + mGridSize + 1];
		cubes[i].cVertices[4] = &vertices[i - offset + (mGridSize + 1) * (mGridSize + 1)];
		cubes[i].cVertices[5] = &vertices[i - offset + (mGridSize + 1) * (mGridSize + 1) + 1];
		cubes[i].cVertices[6] = &vertices[i - offset + (mGridSize + 1) * (mGridSize + 2) + 1];
		cubes[i].cVertices[6] = &vertices[i - offset + (mGridSize + 1) * (mGridSize + 2)];
	}

    return(true);
}


//---------------------------------------------------
// TO DO: Project 4
// The edge table data is already populated - See GridData.h
// 1. For all the cubes
//    - Find which vertices are on the surface.
//    - Store an edge list of all the computed vertices.
//    - Surface values must lie within a pre-set threshold
//
// 2. Compute an 8bit index, for the first edge table.
//    - Find the edge to interpolate on
//    - If the cube is either all in or out, the edgeTable will return a 0 or 255
//    - Move to the next edge.
//
// 3. Update the relevant edges.
//   - Cycle through the twelve edges.
//   - Use bitand and shift operators to find the relevant edges based on the edgeTable lookup.
//   - Use the vertices table to retrieve the relevant vertices from the right edges and populate.
//     - Compute the point P between the two vertices vertex2 and vertex1
//       This is  the level of the cube minus the value at the vertex 1, divided
//       by the change denoted by P between vertex2 and vertex1 values
//
//  4. Update the triangle object with the edge information
//---------------------------------------------------------------------------------
bool MarchingGrid::IsosurfaceToPolygons(float level, Triag &triangles)
{
	for (auto &cube : cubes) {
		int edgeIndex = 0;
		for (int i = 8; i > 0; i--) {
			edgeIndex |= (cube.cVertices[i]->surfaceValue < level);
			edgeIndex <<= 1;
		}
		int edgeList = edgeTable[edgeIndex];
		for (int i = 0; i < 12; i++) {
			if (edgeList & 1) {
				float v1 = cube.cVertices[cubeEdgeVerts[2 * i]]->surfaceValue;
				float v2 = cube.cVertices[cubeEdgeVerts[2 * i + 1]]->surfaceValue;
				assert((v1 - level) * (v2 - level) <= 0);
				glm::vec3 p1 = cube.cVertices[cubeEdgeVerts[2 * i]]->position;
				glm::vec3 p2 = cube.cVertices[cubeEdgeVerts[2 * i + 1]]->position;
				glm::vec3 intersection = p2 + (v2 - level) / (v2 - v1) * (p1 - p2);
				triangles.points.push_back(intersection);
			}
			edgeList >>= 1;
		}
	}

	return true;
}
