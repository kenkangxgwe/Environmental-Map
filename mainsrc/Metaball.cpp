//-------------------------------------------------------------
// CAP5705 University of Florida
// copyright 2017 Corey Toler-Franklin 
// Mataball class 
//-------------------------------------------------------------



#include "Metaball.h"



//------------------------------------------
// Initialize Metaball
//------------------------------------------
Metaball::Metaball()
//    :       startNum(3),
//            minNum(1),
//            maxNum(7)
{
}


Metaball::Metaball(glm::vec3 pos, float rad)
://    :	startNum(3),
//        minNum(1),
//        maxNum(7),
        position(pos)
{
	sRadius = rad;
}

Metaball::~Metaball()
{
}

//------------------------------------------
// Compute change location
//------------------------------------------
void Metaball::Change(glm::vec3 pos, float rad)
{
    position = pos;
    sRadius = rad * rad;
}






