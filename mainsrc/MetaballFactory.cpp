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
	Metaball leftMetaball = Metaball(glm::vec3(-0.5,0,0), 0.3);
	mDefaultMetaball = Metaball(glm::vec3(0,0,0), 0.5);
	// store the metaballs in the list and generate a coresponding list
	// of directions for each metaball to float to.
	mMetaballs.push_back(mDefaultMetaball);
	mBallDir.push_back(getRandomDir(mDefaultMetaball));
	mMetaballs.push_back(leftMetaball);
	mBallDir.push_back(getRandomDir(leftMetaball));
	mGrid.Initialize(0.3);
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
	Metaball currBall, otherBall;
	float distance = 0;
	float trackRadius = 0;
	float angle = 100;   // set the default degrees of moment
	for (int i = 0; i < mMetaballs.size(); i++)
	{
		currBall = mMetaballs.at(i);
		// randomly pick a track for the current moving ball
		trackRadius = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (1 - currBall.sRadius)));
		for (int j = 0; mMetaballs.size(); j++)
		{
			otherBall = mMetaballs.at(j);
			if (currBall.position != otherBall.position && currBall.sRadius != otherBall.sRadius
				 && (currBall.position - otherBall.position).length() < distance)
			{
				distance = (currBall.position - otherBall.position).length();

				//x
				currBall.position.x = currBall.position.x + cos(angle * (1 - distance)) * trackRadius;
				//y
				currBall.position.x = currBall.position.x + sin(angle * (1 - distance)) * trackRadius;
			}
		}
	}
}

//==============================================================
// TO DO: Project 4
// Randomly generate a moving vector for each metaball
//================================================================
glm::vec3 MetaballFactory::getRandomDir(Metaball ball)
{
	glm::vec3 dir;
	dir.x = rand() % 1;
	dir.y = rand() % 1;
	dir.z = rand() % 1;

	return dir;
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
	std::vector<GridCube> cubes = mGrid.cubes;
	Metaball currBall;
	CubeVert currCV;
	glm::vec3 currVer, currNor;

	ClearGrid();
	// update the metaball postions
	for (int i = 0; i < mMetaballs.size(); i++)
	{
		//TODO: update the positions inversely to the propertion of distance
		currBall = mMetaballs.at(i);
		currBall.position = currBall.position + mBallDir.at(i);

		// update the distance between each vertex in the marching grid cube
		// to its center

		// update the gribs vertices accordingly
		for (int j = 0; mGrid.vertices.size(); j++)
		{
			currCV = mGrid.vertices.at(j);
			currCV.surfaceValue = (currCV.position - currBall.position).length();
		}
		// if the isosurface is too big or too small for the marching cube
		// to draw, do not update the positions
		if (!mGrid.IsosurfaceToPolygons(currBall.sRadius, mTriangles))
		{
			return;
		}

		// update the normals for the returned triangle mes
		// calculate normal by substracting one cube vertex by the position of the metaball,
	    // only calculate the normal for the grid thats intersected by the ball.
		for (int k = 0; k < mTriangles.points.size(); k++)
		{
			currVer = mTriangles.points.at(k);
			currNor = mTriangles.normals.at(k);
			currNor = glm::normalize(currVer - currBall.position);
		}

	}
}
