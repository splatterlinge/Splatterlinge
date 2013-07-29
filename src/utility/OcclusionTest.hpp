#ifndef UTILITY_OCCLUSIONTEST_INCLUDED
#define UTILITY_OCCLUSIONTEST_INCLUDED

#include "glWrappers.hpp"

#include <QVector4D>


/// Occlusion testing using hardware accelerated occlusion queries
class OcclusionTest
{
public:
	OcclusionTest();
	~OcclusionTest();

	bool pointVisible( QVector3D point=QVector3D(0,0,0) );

private:
	GLuint mQuery;
};


#endif
