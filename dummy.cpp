/*
 * dummy.cpp
 *
 *  Created on: Nov 25, 2019
 *      Author: little
 */


#include <cstring>

#include "dummy.h"




using namespace little;



static GLfloat DummyVertices[] =
{
		0.0f, -0.5f,  0.0f, 1.0f, 0.0f, 0.0f, // 0
		0.5f, -0.5f,  0.0f, 0.0f, 1.0f, 0.0f, // 1
		1.0f, -0.5f,  0.0f, 0.0f, 0.0f, 1.0f, // 2
		0.0f,  0.0f,  0.0f, 0.5f, 0.0f, 0.0f, // 3
		0.5f,  0.0f,  0.0f, 0.0f, 0.5f, 0.0f, // 4
		1.0f,  0.0f,  0.0f, 0.0f, 0.0f, 0.5f, // 5
		0.0f,  0.5f,  0.0f, 0.0f, 0.0f, 1.0f, // 6
		0.5f,  0.5f,  0.0f, 0.0f, 1.0f, 0.0f, // 7
		1.0f,  0.5f,  0.0f, 1.0f, 1.0f, 1.0f  // 8
};

static INDEX_T DummyIndexes[] =
{
		// Winding order
		//               |Degenerate triangles|
		0, 3, 1, 4, 2, 5,         5, 3,        3, 6, 4, 7, 5, 8
};




void DummyMesh::Initialize()
{
	memcpy(this->vertices, DummyVertices, sizeof(DummyVertices));
	SAFE_DEL_ARRAY(this->indexes);
	this->indexes = new INDEX_T[this->NumOfIndexes()]();
	memcpy(this->indexes, DummyIndexes, sizeof(DummyIndexes));
}






