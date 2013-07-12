#include "occlusion.hpp"


bool occlusionTest( const QVector3D & point )
{
	GLint viewport[4];
	glGetIntegerv( GL_VIEWPORT, viewport );

	GLdouble mvMatrix[16];
	glGetDoublev( GL_MODELVIEW_MATRIX, mvMatrix );

	GLdouble projMatrix[16];
	glGetDoublev( GL_PROJECTION_MATRIX, projMatrix );

	GLdouble winX = 0.0, winY = 0.0, winZ = 0.0;
	gluProject( point.x(), point.y(), point.z(), mvMatrix, projMatrix, viewport, &winX, &winY, &winZ );

	GLfloat flareZ = 0.0f;
	GLfloat bufferZ = 0.0f;
	flareZ = (GLfloat)winZ;
	if( winX < viewport[0] || winX > viewport[2]+viewport[0] || winY < viewport[1] || winY > viewport[3]+viewport[1] )
		return true;

	glReadPixels( (GLint)winX, (GLint)winY, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &bufferZ );
	if( bufferZ < flareZ )
		return true;

	return false;
}
