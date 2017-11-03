//-----------------------------------------------------------------------------------
// Metaball class framework
// Mataball class 
//-------------------------------------------------------------

#pragma once

#include <glm.hpp>


class Metaball
{
public:
							Metaball			();
							Metaball			(glm::vec3 pos, float rad);
							~Metaball();


			void			Change				(glm::vec3 pos, float rad);
			void			Update				(void);


	glm::vec3 position;
	float sRadius;
	int startNum;
	int minNum;
	int maxNum;

};