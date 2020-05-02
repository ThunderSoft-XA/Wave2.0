/*
 * dummy.h
 *
 *  Created on: Nov 25, 2019
 *      Author: little
 */

#ifndef LIB_INCLUDE_DUMMY_H_
#define LIB_INCLUDE_DUMMY_H_

#include "mesh.h"




namespace little
{



class DummyMesh: public AnimatedMesh
{
public:
	DummyMesh():
		AnimatedMesh(3, 3)
	{
		FOOTPRINT();
	}

	virtual ~DummyMesh()
	{
		FOOTPRINT();
	}

public:
	DummyMesh & operator=(const DummyMesh &) = default;

public:
	virtual void Initialize(void);
	virtual void Update(void)      {}

protected:
	virtual void UpdateZvalue(int x, int y, int pos)   {}
	virtual void ColorizeVertex(int x, int y, int pos) {}
	virtual void ComputeIndex(int x, int y)            {}
};



} // namespace little


#endif /* LIB_INCLUDE_DUMMY_H_ */
