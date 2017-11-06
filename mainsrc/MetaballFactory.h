//-----------------------------------------------------------------------------------
// Metaball Factory class framework
// CAP5705 University of Florida
// copyright Corey Toler-Franklin 2017
// Manages the metaball system parameters.
// Updates the metaball system for re-display.
//-----------------------------------------------------------------------------------

#pragma once

#include<vector>


#include "MarchingGrid.h"
#include "Metaball.h"
#include <glm/gtx/rotate_vector.hpp>
#include <stdlib.h>     /* srand, rand */




class MetaballFactory
{
public:
                            MetaballFactory             ();
                            ~MetaballFactory            ();


    void                    Update                      (void);

    Metaball                mDefaultMetaball;           // default metaball parameters that we use
    MarchingGrid            mGrid;                      // global grid 
    std::vector<Metaball>   mMetaballs;                 // list of metaballs in the system
	std::vector<glm::vec3>  mRotaAxis;                   // list of rotating axis of metablls, the index matches the index of mMetaballs
    Triag                   mTriangles;                 // triangles for storing computed polygons

	int startNum;
	int minNum;
	int maxNum;
                            
private:
    void                    ClearGrid               (void);
    void                    UpdatePositions         (void);
	glm::vec3               genRandAxis             (void);
   
};