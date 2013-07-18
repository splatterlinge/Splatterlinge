#ifndef UTILITY_GLWRAPPERS_INCLUDED
#define UTILITY_GLWRAPPERS_INCLUDED

#include <GL/glew.h>

#include <QVector2D>
#include <QVector3D>
#include <QVector4D>
#include <QMatrix4x4>
#include <QColor>
#include <QPointF>


/* TODO:
Q_STATIC_ASSERT(sizeof(QVector2D) == 2*sizeof(float));
Q_STATIC_ASSERT(sizeof(QVector3D) == 3*sizeof(float));
Q_STATIC_ASSERT(sizeof(QVector4D) == 4*sizeof(float));
Q_STATIC_ASSERT(sizeof(QPointF) == 2*sizeof(qreal));
*/


inline void glMultMatrix( const GLfloat * m )		{ glMultMatrixf( m ); }
inline void glMultMatrix( const GLdouble * m )		{ glMultMatrixd( m ); }
inline void glMultMatrix( const QMatrix4x4 & m )	{ glMultMatrix( m.constData() ); }

inline void glLoadMatrix( const GLfloat * m )		{ glLoadMatrixf( m ); }
inline void glLoadMatrix( const GLdouble * m )		{ glLoadMatrixd( m ); }
inline void glLoadMatrix( const QMatrix4x4 & m )	{ glLoadMatrix( m.constData() ); }

inline void glVertex( const GLshort & x, const GLshort & y )	{ glVertex2s( x, y ); }
inline void glVertex( const GLint & x, const GLint & y )	{ glVertex2i( x, y ); }
inline void glVertex( const GLfloat & x, const GLfloat & y )	{ glVertex2f( x, y ); }
inline void glVertex( const GLdouble & x, const GLdouble & y )	{ glVertex2d( x, y ); }
inline void glVertex( const GLshort & x, const GLshort & y, const GLshort & z )		{ glVertex3s( x, y, z ); }
inline void glVertex( const GLint & x, const GLint & y, const GLint & z )		{ glVertex3s( x, y, z ); }
inline void glVertex( const GLfloat & x, const GLfloat & y, const GLfloat & z )		{ glVertex3s( x, y, z ); }
inline void glVertex( const GLdouble & x, const GLdouble & y, const GLdouble & z )	{ glVertex3s( x, y, z ); }
inline void glVertex( const GLshort & x, const GLshort & y, const GLshort & z, const GLshort & w )	{ glVertex4s( x, y, z, w ); }
inline void glVertex( const GLint & x, const GLint & y, const GLint & z, const GLshort & w )		{ glVertex4s( x, y, z, w ); }
inline void glVertex( const GLfloat & x, const GLfloat & y, const GLfloat & z, const GLshort & w )	{ glVertex4s( x, y, z, w ); }
inline void glVertex( const GLdouble & x, const GLdouble & y, const GLdouble & z, const GLshort & w )	{ glVertex4s( x, y, z, w ); }
inline void glVertex2v( const GLfloat * v )	{ glVertex2fv( v ); }
inline void glVertex2v( const GLdouble * v )	{ glVertex2dv( v ); }
inline void glVertex3v( const GLfloat * v )	{ glVertex3fv( v ); }
inline void glVertex3v( const GLdouble * v )	{ glVertex3dv( v ); }
inline void glVertex4v( const GLfloat * v )	{ glVertex4fv( v ); }
inline void glVertex4v( const GLdouble * v )	{ glVertex4dv( v ); }
inline void glVertex( const QVector2D & v )	{ glVertex2v( reinterpret_cast<const float*>(&v) ); }
inline void glVertex( const QVector3D & v )	{ glVertex3v( reinterpret_cast<const float*>(&v) ); }
inline void glVertex( const QVector4D & v )	{ glVertex4v( reinterpret_cast<const float*>(&v) ); }
inline void glVertex( const QPointF & v )	{ glVertex2v( reinterpret_cast<const qreal*>(&v) ); }

inline void glNormal( const GLbyte & x, const GLbyte & y, const GLbyte & z )		{ glNormal3b( x, y, z ); }
inline void glNormal( const GLdouble & x, const GLdouble & y, const GLdouble & z )	{ glNormal3d( x, y, z ); }
inline void glNormal( const GLfloat & x, const GLfloat & y, const GLfloat & z )		{ glNormal3f( x, y, z ); }
inline void glNormal( const GLint & x, const GLint & y, const GLint & z )		{ glNormal3i( x, y, z ); }
inline void glNormal( const GLshort & x, const GLshort & y, const GLshort & z )		{ glNormal3s( x, y, z ); }
inline void glNormal3v( const GLbyte * v )	{ glNormal3bv( v ); }
inline void glNormal3v( const GLdouble * v )	{ glNormal3dv( v ); }
inline void glNormal3v( const GLfloat * v )	{ glNormal3fv( v ); }
inline void glNormal3v( const GLint * v )	{ glNormal3iv( v ); }
inline void glNormal3v( const GLshort * v )	{ glNormal3sv( v ); }
inline void glNormal( const QVector3D & v )	{ glNormal3v( reinterpret_cast<const float*>(&v) ); }

inline void glRotate( const GLfloat & angle, const GLfloat & x, const GLfloat & y, const GLfloat & z )		{ glRotatef( angle, x, y, z ); }
inline void glRotate( const GLdouble & angle, const GLdouble & x, const GLdouble & y, const GLdouble & z )	{ glRotated( angle, x, y, z ); }
inline void glRotate( const GLfloat & angle, const QVector3D & axis )	{ glRotate( angle, static_cast<GLfloat>(axis.x()), static_cast<GLfloat>(axis.y()), static_cast<GLfloat>(axis.z()) ); }
inline void glRotate( const GLdouble & angle, const QVector3D & axis )	{ glRotate( angle, static_cast<GLdouble>(axis.x()), static_cast<GLdouble>(axis.y()), static_cast<GLdouble>(axis.z()) ); }

inline void glTranslate( const GLfloat & x, const GLfloat & y, const GLfloat & z )	{ glTranslatef( x, y, z ); }
inline void glTranslate( const GLdouble & x, const GLdouble & y, const GLdouble & z )	{ glTranslated( x, y, z ); }
inline void glTranslate( const QVector3D & translation )	{ glTranslated( translation.x(), translation.y(), translation.z() ); }

inline void glScale( const GLfloat & x, const GLfloat & y, const GLfloat & z )		{ glScalef( x, y, z ); }
inline void glScale( const GLdouble & x, const GLdouble & y, const GLdouble & z )	{ glScaled( x, y, z ); }
inline void glScale( const QVector3D & scale )			{ glScale( scale.x(), scale.y(), scale.z() ); }

inline void glColor( const GLbyte & red, const GLbyte & green, const GLbyte & blue )		{ glColor3b( red, green, blue ); }
inline void glColor( const GLshort & red, const GLshort & green, const GLshort & blue )		{ glColor3s( red, green, blue ); }
inline void glColor( const GLint & red, const GLint & green, const GLint & blue )		{ glColor3i( red, green, blue ); }
inline void glColor( const GLubyte & red, const GLubyte & green, const GLubyte & blue )		{ glColor3ub( red, green, blue ); }
inline void glColor( const GLushort & red, const GLushort & green, const GLushort & blue )	{ glColor3us( red, green, blue ); }
inline void glColor( const GLuint & red, const GLuint & green, const GLuint & blue )		{ glColor3ui( red, green, blue ); }
inline void glColor( const GLfloat & red, const GLfloat & green, const GLfloat & blue )		{ glColor3f( red, green, blue ); }
inline void glColor( const GLdouble & red, const GLdouble & green, const GLdouble & blue )	{ glColor3d( red, green, blue ); }
inline void glColor( const GLbyte & red, const GLbyte & green, const GLbyte & blue, const GLbyte & alpha )	{ glColor4b( red, green, blue, alpha ); }
inline void glColor( const GLshort & red, const GLshort & green, const GLshort & blue, const GLshort & alpha )	{ glColor4s( red, green, blue, alpha ); }
inline void glColor( const GLint & red, const GLint & green, const GLint & blue, const GLint & alpha )		{ glColor4i( red, green, blue, alpha ); }
inline void glColor( const GLubyte & red, const GLubyte & green, const GLubyte & blue, const GLubyte & alpha )		{ glColor4ub( red, green, blue, alpha ); }
inline void glColor( const GLushort & red, const GLushort & green, const GLushort & blue, const GLushort & alpha )	{ glColor4us( red, green, blue, alpha ); }
inline void glColor( const GLuint & red, const GLuint & green, const GLuint & blue, const GLuint & alpha )		{ glColor4ui( red, green, blue, alpha ); }
inline void glColor( const GLfloat & red, const GLfloat & green, const GLfloat & blue, const GLfloat & alpha )		{ glColor4f( red, green, blue, alpha ); }
inline void glColor( const GLdouble & red, const GLdouble & green, const GLdouble & blue, const GLdouble & alpha )	{ glColor4d( red, green, blue, alpha ); }
inline void glColor3v( const GLbyte * v )	{ glColor3bv( v ); }
inline void glColor3v( const GLshort * v )	{ glColor3sv( v ); }
inline void glColor3v( const GLint * v )	{ glColor3iv( v ); }
inline void glColor3v( const GLfloat * v )	{ glColor3fv( v ); }
inline void glColor3v( const GLdouble * v )	{ glColor3dv( v ); }
inline void glColor3v( const GLubyte * v )	{ glColor3ubv( v ); }
inline void glColor3v( const GLushort * v )	{ glColor3usv( v ); }
inline void glColor3v( const GLuint * v )	{ glColor3uiv( v ); }
inline void glColor4v( const GLbyte * v )	{ glColor4bv( v ); }
inline void glColor4v( const GLshort * v )	{ glColor4sv( v ); }
inline void glColor4v( const GLint * v )	{ glColor4iv( v ); }
inline void glColor4v( const GLfloat * v )	{ glColor4fv( v ); }
inline void glColor4v( const GLdouble * v )	{ glColor4dv( v ); }
inline void glColor4v( const GLubyte * v )	{ glColor4ubv( v ); }
inline void glColor4v( const GLushort * v )	{ glColor4usv( v ); }
inline void glColor4v( const GLuint * v )	{ glColor4uiv( v ); }
inline void glColor( const QColor & color )	{ glColor( color.redF(), color.greenF(), color.blueF(), color.alphaF() ); }
inline void glColor( const QRgb & color )	{ glColor( qRed(color), qGreen(color), qBlue(color), qAlpha(color) ); }
inline void glColor( const QVector3D & color )	{ glColor3v( reinterpret_cast<const float*>(&color) ); }
inline void glColor( const QVector4D & color )	{ glColor4v( reinterpret_cast<const float*>(&color) ); }

inline void glLight( const GLenum & light, const GLenum & pname, const GLfloat & param )	{ glLightf( light, pname, param ); }
inline void glLight( const GLenum & light, const GLenum & pname, const GLint & param )		{ glLighti( light, pname, param ); }
inline void glLightv( const GLenum & light, const GLenum & pname, const GLfloat * params )	{ glLightfv( light, pname, params ); }
inline void glLightv( const GLenum & light, const GLenum & pname, const GLint * params )	{ glLightiv( light, pname, params ); }
inline void glLight( const GLenum & light, const GLenum & pname, const QVector4D & v )		{ glLightv( light, pname, reinterpret_cast<const float*>(&v) ); }

inline void glFog( const GLenum & pname, const GLfloat & param )	{ glFogf( pname, param ); }
inline void glFog( const GLenum & pname, const GLint & param )		{ glFogi( pname, param ); }
inline void glFogv( const GLenum & pname, const GLfloat * params )	{ glFogfv( pname, params ); }
inline void glFogv( const GLenum & pname, const GLint * params )	{ glFogiv( pname, params ); }
inline void glFog( const GLenum & pname, const QVector4D & v )		{ glFogv( pname, reinterpret_cast<const float*>(&v) ); }

inline void glMaterial( const GLenum & face, const GLenum & pname, const GLfloat & param )	{ glMaterialf( face, pname, param ); }
inline void glMaterial( const GLenum & face, const GLenum & pname, const GLint & param )	{ glMateriali( face, pname, param ); }
inline void glMaterialv( const GLenum & face, const GLenum & pname, const GLfloat * param )	{ glMaterialfv( face, pname, param ); }
inline void glMaterialv( const GLenum & face, const GLenum & pname, const GLint * param )	{ glMaterialiv( face, pname, param ); }
inline void glMaterial( const GLenum & face, const GLenum & pname, const QVector4D & v )	{ glMaterialv( face, pname, reinterpret_cast<const float*>(&v) ); }


QString glGetErrorString( const GLenum & error );
GLenum glGetAlphaTestFunctionFromString( const QString & name );


#endif
