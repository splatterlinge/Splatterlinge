#include "OcclusionTest.hpp"


OcclusionTest::OcclusionTest()
{
	glGenQueries( 1, &mQuery );
}


OcclusionTest::~OcclusionTest()
{
	glDeleteQueries( 1, &mQuery );
}


bool OcclusionTest::pointVisible( QVector3D point )
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
