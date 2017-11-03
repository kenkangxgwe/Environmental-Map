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
bool MarchingGrid::Initialize(int size)
{

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


    return true;
}