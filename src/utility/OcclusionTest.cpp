/*
 * Copyright (C) 2013
 * Branimir Djordjevic <branimir.djordjevic@gmail.com>
 * Tobias Himmer <provisorisch@online.de>
 * Michael Wydler <michael.wydler@gmail.com>
 * Karl-Heinz Zimmermann <karlzimmermann3787@gmail.com>
 *
 * This file is part of Splatterlinge.
 *
 * Splatterlinge is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * Splatterlinge is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Splatterlinge. If not, see <http://www.gnu.org/licenses/>.
 */

#include "OcclusionTest.hpp"
#include "RandomNumber.hpp"
#include <geometry/Vertex.hpp>


QGLBuffer OcclusionTest::sRandomVertexInSphereBuffer;


OcclusionTest::OcclusionTest()
{
	if( !sRandomVertexInSphereBuffer.isCreated() )
	{
		VertexP3f randomPointsInSphere[256];
		for( size_t i=0; i<sizeof(randomPointsInSphere)/sizeof(VertexP3f); i++ )
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
	GLuint samplesPerFragment;
	glGetIntegerv( GL_SAMPLES_ARB, (GLint*)&samplesPerFragment );
	if( samplesPerFragment == 0 )
		samplesPerFragment = 1;

	if( glIsQuery( mQuery ) )	// fetch the result of last frame
		glGetQueryObjectuiv( mQuery, GL_QUERY_RESULT, &sampleCount );

	glPushAttrib( GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT | GL_ENABLE_BIT );
	glColorMask( GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE );	// don't draw anything
	glDepthMask( GL_FALSE );	// don't write the depth of our testing points to the depth buffer
	glEnable( GL_DEPTH_TEST );	// essential for occlusion query
	glDisable( GL_MULTISAMPLE );	// multisampling would cause the query to report too many passed samples

	glBeginQuery( GL_SAMPLES_PASSED, mQuery );
	glBegin( GL_POINTS );
	glVertex( point );
	glEnd();
	glEndQuery( GL_SAMPLES_PASSED );

	glPopAttrib();

	return sampleCount/samplesPerFragment;
}


unsigned char OcclusionTest::randomPointsInUnitSphereVisible( const unsigned char & numPoints )
{
	GLuint sampleCount = 0;
	GLuint samplesPerFragment;
	glGetIntegerv( GL_SAMPLES_ARB, (GLint*)&samplesPerFragment );
	if( samplesPerFragment == 0 )
		samplesPerFragment = 1;

	if( glIsQuery( mQuery ) )	// fetch the result of last frame
		glGetQueryObjectuiv( mQuery, GL_QUERY_RESULT, &sampleCount );

	glPushAttrib( GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT | GL_ENABLE_BIT );
	glColorMask( GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE );	// don't draw anything
	glDepthMask( GL_FALSE );	// don't write the depth of our testing points to the depth buffer
	glEnable( GL_DEPTH_TEST );	// essential for occlusion query
	glDisable( GL_MULTISAMPLE );	// multisampling would cause the query to report too many passed samples

	glBeginQuery( GL_SAMPLES_PASSED, mQuery );
	sRandomVertexInSphereBuffer.bind();
	VertexP3f::glEnableClientState();
	VertexP3f::glPointerVBO();
	glDrawArrays( GL_POINTS, 0, numPoints );
	VertexP3f::glDisableClientState();
	sRandomVertexInSphereBuffer.release();
	glEndQuery( GL_SAMPLES_PASSED );

	glPopAttrib();

	return sampleCount/samplesPerFragment;
}
