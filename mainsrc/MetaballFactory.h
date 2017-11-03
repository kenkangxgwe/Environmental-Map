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





class MetaballFactory
{
public:
                            MetaballFactory             ();
                            ~MetaballFactory            ();


    void                    Update                      (void);

    Metaball                mDefaultMetaball;           // default metaball parameters that we use
    MarchingGrid            mGrid;                      // global grid 
    std::vector<Metaball>   mMetaballs;                 // list of metaballs in the system
    Triag                   mTriangles;                 // triangles for storing computed polygons
                            
private:

    void                    ClearGrid               (void);
    void                    UpdatePositions         (void);
   
};