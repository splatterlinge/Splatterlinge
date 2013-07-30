#ifndef UTILITY_OCCLUSIONTEST_INCLUDED
#define UTILITY_OCCLUSIONTEST_INCLUDED

#include "glWrappers.hpp"

#include <QVector4D>
#include <QGLBuffer>


/// Occlusion testing using hardware accelerated occlusion queries
class OcclusionTest
{
public:
	OcclusionTest();
	~OcclusionTest();

	bool pointVisible( const QVector3D & point=QVector3D(0,0,0) );
	unsigned char randomPointsInUnitSphereVisible( const unsigned char & numPoints );

private:
	GLuint mQuery;
	static QGLBuffer sRandomVertexInSphereBuffer;
};


#endif
