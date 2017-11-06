//-------------------------------------------------------------
// CAP5705 University of Florida
// MetaballFactory.h
// copyright Corey Toler-Franklin 2017
// Manages the metaball system parameters.
// Updates the metaball system for re-display.
//-------------------------------------------------------------



#include "MetaballFactory.h"


// I've moved the startNum, minNum and maxNum variables from Metabll class
// to here, so the balls do not have to know how many of itself are produced.
MetaballFactory::MetaballFactory(void)
:       startNum(3),
        minNum(1),
        maxNum(7)
{
	// set up a few metablls when intializing the factory
	//TODO: check for max and min limits in the metaball list
	Metaball leftMetaball = Metaball(glm::vec3(-0.2,0,0), 0.3);
	mDefaultMetaball = Metaball(glm::vec3(0.2,0,0), 0.5);
	// store the metaballs in the list and generate a coresponding list
	// of directions for each metaball to float to.
	mMetaballs.push_back(mDefaultMetaball);
	mMetaballs.push_back(leftMetaball);
	// the param must be an integer
	mGrid.Initialize(20);
}


MetaballFactory::~MetaballFactory(void)
{
}


//==============================================================
// TO DO: Project 4
// clear the surface and normal information in the grid
//================================================================
void MetaballFactory::ClearGrid(void)
{
	mTriangles.points.clear();
	mTriangles.normals.clear();
}



//==============================================================
// TO DO: Project 4
// Move positions in a circle as a function of time
// The rate of motion is inversely proportional to distance (far means slower)
//================================================================
void MetaballFactory::UpdatePositions(void)
{
	Metaball *currBall, *otherBall;
	// the axis the metaballs will move around about
	float angle = 5.0f;   // set the default degrees of moment
	glm::vec3 centerAxis = glm::vec3(1, 1, 1);
	float distance = 0.0f;
	float trackRadius = 0.0f;
	for (int i = 0; i < mMetaballs.size(); i++)
	{
		currBall = &mMetaballs[i];
		trackRadius = glm::length(currBall->position - centerAxis);
		// randomly pick a track for the current moving ball
		for (int j = 0; j < mMetaballs.size(); j++)
		{
			otherBall = &mMetaballs[j];
			if (i != j && (distance == 0 || glm::length(currBall->position - otherBall->position) < distance)) 
			{
				distance = glm::length(currBall->position - otherBall->position);
			}
		}
		//update the degree based on the distance between the balls
		angle /= distance;
		//x
		currBall->position = glm::gtx::rotate_vector::rotate(currBall->position, angle, centerAxis);
	}
}

//=====================================================================
// TO DO:Project 4
// Updates locations
// Updates grid values
// Binds new polygons to the triangle object
// 1. - Remember to clear the surface and normal information in the grid
// 2. - Move the positions of the metaballs circularly as a function of time
//
// 3.- For each metaball and each grid vertex, compute and update the following...
//      -the metaball point - a vector from the metaball to the grid index
//      -normals
//      -vertices on the surface - a function of the squared distance and the radius
// 4. Convert the isosurface to polygons MarchingGrid::IsosurfaceToPolygon
//=====================================================================
void MetaballFactory::Update(void)
{
	Metaball *currBall;
	CubeVert *currCV;

	ClearGrid();

	//currBall->position += mBallDir[i];
	UpdatePositions();
	// update the metaball postions
	for (int i = 0; i < mMetaballs.size(); i++)
	{
		currBall = &mMetaballs[i];
		// update the gribs vertices accordingly
		for (int j = 0; j < mGrid.vertices.size(); j++)
		{
			currCV = &mGrid.vertices[j];
			currCV->surfaceValue = glm::length(currCV->position - currBall->position);
		}
		// if the isosurface is too big or too small for the marching cube
		// to draw, do not update the positions
		int startIndex = mTriangles.points.size();
		if (!mGrid.IsosurfaceToPolygons(currBall->sRadius, mTriangles))
		{
			return;
		}

		// update the normals for the returned triangle mes
		// calculate normal by substracting one cube vertex by the position of the metaball,
		// only calculate the normal for the grid thats intersected by the ball.
		for (int k = startIndex; k < mTriangles.points.size(); k++)
		{
			mTriangles.normals.push_back(glm::normalize(mTriangles.points[k] - currBall->position));
		}

	}
}
