#include "OcclusionTest.hpp"
#include "RandomNumber.hpp"
#include <geometry/Vertex.hpp>


QGLBuffer OcclusionTest::sRandomVertexInSphereBuffer;


OcclusionTest::OcclusionTest()
{
	if( !sRandomVertexInSphereBuffer.isCreated() )
	{
		VertexP3f randomPointsInSphere[256];
		for( size_t i=0; i<sizeof(randomPointsInSphere); i++ )
			randomPointsInSphere[i].position = RandomNumber::inUnitSphere();
		sRandomVertexInSphereBuffer = QGLBuffer( QGLBuffer::VertexBuffer );
		sRandomVertexInSphereBuffer.create();
		sRandomVertexInSphereBuffer.bind();
		sRandomVertexInSphereBuffer.setUsagePattern( QGLBuffer::StaticDraw );
		sRandomVertexInSphereBuffer.allocate( randomPointsInSphere, sizeof(randomPointsInSphere) );
		sRandomVertexInSphereBuffer.release();
	}
	glGenQueries( 1, &mQuery );
}


OcclusionTest::~OcclusionTest()
{
	glDeleteQueries( 1, &mQuery );
}


bool OcclusionTest::pointVisible( const QVector3D & point )
{
	GLuint sampleCount = 0;

	if( glIsQuery( mQuery ) )	// fetch the result of last frame
		glGetQueryObjectuiv( mQuery, GL_QUERY_RESULT, &sampleCount );

	glPushAttrib( GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT );
	glColorMask( GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE );
	glDepthMask( GL_FALSE );
	glEnable( GL_DEPTH_TEST );

	glBeginQuery( GL_SAMPLES_PASSED, mQuery );
	glBegin( GL_POINTS );
	glVertex( point );
	glEnd();
	glEndQuery( GL_SAMPLES_PASSED );

	glPopAttrib();

	return sampleCount;
}


unsigned char OcclusionTest::randomPointsInUnitSphereVisible( const unsigned char & numPoints )
{
	GLuint sampleCount = 0;

	if( glIsQuery( mQuery ) )	// fetch the result of last frame
		glGetQueryObjectuiv( mQuery, GL_QUERY_RESULT, &sampleCount );

	glPushAttrib( GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT );
	glColorMask( GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE );
	glDepthMask( GL_FALSE );
	glEnable( GL_DEPTH_TEST );

	glBeginQuery( GL_SAMPLES_PASSED, mQuery );
	sRandomVertexInSphereBuffer.bind();
	VertexP3f::glEnableClientState();
	VertexP3f::glPointerVBO();
	glDrawArrays( GL_POINTS, 0, numPoints );
	VertexP3f::glDisableClientState();
	sRandomVertexInSphereBuffer.release();
	glEndQuery( GL_SAMPLES_PASSED );

	glPopAttrib();

	return sampleCount;
}
