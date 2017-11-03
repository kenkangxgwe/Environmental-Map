///////////////////////////////////////////////////////////////////////////////////
// copyright 2015 Corey Toler-Franklin, University of Florida
// Geometry Header - object descriptions
//////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <string>
#include <vector>
#include "VertexObj.h"




class Geometry
{

public:

						Geometry					(void);
						~Geometry					(void);


	void				Initialize					(std::string filename);
	void				Load						(std::string filename);
	void				CreateBuffers				(void);

	void				Draw						(GLint vertex, GLint normal);
	void				DeleteBuffers				(void);


private:

	std::vector<VertexObj>		m_vertices;
	std::vector<VertexObj>		m_texcoords;
	std::vector<VertexObj>		m_normals;
	std::vector<VertexObj>		m_parameters;
	std::vector<FaceObj>		m_faces;
	GLuint						m_vbo_vertices;
	GLuint						m_vbo_indices;
};