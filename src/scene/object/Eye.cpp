#include "Eye.hpp"

#include <scene/Scene.hpp>

#include <utility/glWrappers.hpp>
#include <utility/alWrappers.hpp>


Eye::Eye( Scene * scene ) :
	AObject( scene ),
	mFOV(60.0f),
	mNearPlane(0.1f),
	mFarPlane(500.0f)
{
}


Eye::Eye( Eye & other ) :
	AObject(other),
	mFOV(other.mFOV),
	mNearPlane(other.mNearPlane),
	mFarPlane(other.mFarPlane)
{
}


Eye::~Eye()
{
}


void Eye::updateMatrix() const
{
	mMatrix.setToIdentity();
	mMatrix.rotate( 180.0f, QVector3D( 0, 1, 0 ) );
	mMatrix.rotate( -rotation() );
	mMatrix.translate( -position() );
}


void Eye::updateSelf( const double & delta )
{
	ALfloat listenerOri[]={(ALfloat)-direction().x(),(ALfloat)-direction().y(),(ALfloat)-direction().z(),
		(ALfloat)up().x(),(ALfloat)up().y(),(ALfloat)up().z()};
	alListener( AL_POSITION, position() );
	alListenerv( AL_ORIENTATION, listenerOri );
//	ALfloat listenerVel[]={0.0,0.0,0.0};
//	alListenerfv( AL_VELOCITY, listenerVel);
}


void Eye::updateSelfPost( const double & delta )
{
	if( !mAttached.isNull() )
	{
		setPosition( mAttached.data()->position() );
		setRotation( mAttached.data()->rotation() );
	}
}


void Eye::drawSelf()
{
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	gluPerspective( mFOV, (float)scene()->width()/scene()->height(), mNearPlane, mFarPlane );
	glMatrixMode( GL_MODELVIEW );
	prepareFrustum();
	enableClippingPlanes();
}


void Eye::drawSelfPost()
{
	disableClippingPlanes();
}


void Eye::setClippingPlane( int n, QVector4D plane )
{
	if( plane.isNull() )
	{
		mClippingPlanes.remove( n );
	} else {
		mClippingPlanes[n] = plane;
	}
}


void Eye::enableClippingPlanes()
{
	QMap<int,QVector4D>::const_iterator i = mClippingPlanes.constBegin();
	while( i != mClippingPlanes.constEnd() )
	{
		double plane[4]={ i.value().x(), i.value().y(), i.value().z(), i.value().w() };
		glClipPlane( GL_CLIP_PLANE0 + i.key(), plane );
		glEnable( GL_CLIP_PLANE0 + i.key() );
		++i;
	}
}


void Eye::disableClippingPlanes()
{
	QMap<int,QVector4D>::const_iterator i = mClippingPlanes.constBegin();
	while( i != mClippingPlanes.constEnd() )
	{
		glDisable( GL_CLIP_PLANE0 + i.key() );
		++i;
	}
}


// http://www.crownandcutlass.com/features/technicaldetails/frustum.html
void Eye::prepareFrustum()
{
	float   proj[16];
	float   modl[16];
	glGetFloatv( GL_PROJECTION_MATRIX, proj );
	glGetFloatv( GL_MODELVIEW_MATRIX, modl );

	float   clip[16];
	/* Combine the two matrices (multiply projection by modelview) */
	clip[ 0] = modl[ 0] * proj[ 0] + modl[ 1] * proj[ 4] + modl[ 2] * proj[ 8] + modl[ 3] * proj[12];
	clip[ 1] = modl[ 0] * proj[ 1] + modl[ 1] * proj[ 5] + modl[ 2] * proj[ 9] + modl[ 3] * proj[13];
	clip[ 2] = modl[ 0] * proj[ 2] + modl[ 1] * proj[ 6] + modl[ 2] * proj[10] + modl[ 3] * proj[14];
	clip[ 3] = modl[ 0] * proj[ 3] + modl[ 1] * proj[ 7] + modl[ 2] * proj[11] + modl[ 3] * proj[15];

	clip[ 4] = modl[ 4] * proj[ 0] + modl[ 5] * proj[ 4] + modl[ 6] * proj[ 8] + modl[ 7] * proj[12];
	clip[ 5] = modl[ 4] * proj[ 1] + modl[ 5] * proj[ 5] + modl[ 6] * proj[ 9] + modl[ 7] * proj[13];
	clip[ 6] = modl[ 4] * proj[ 2] + modl[ 5] * proj[ 6] + modl[ 6] * proj[10] + modl[ 7] * proj[14];
	clip[ 7] = modl[ 4] * proj[ 3] + modl[ 5] * proj[ 7] + modl[ 6] * proj[11] + modl[ 7] * proj[15];

	clip[ 8] = modl[ 8] * proj[ 0] + modl[ 9] * proj[ 4] + modl[10] * proj[ 8] + modl[11] * proj[12];
	clip[ 9] = modl[ 8] * proj[ 1] + modl[ 9] * proj[ 5] + modl[10] * proj[ 9] + modl[11] * proj[13];
	clip[10] = modl[ 8] * proj[ 2] + modl[ 9] * proj[ 6] + modl[10] * proj[10] + modl[11] * proj[14];
	clip[11] = modl[ 8] * proj[ 3] + modl[ 9] * proj[ 7] + modl[10] * proj[11] + modl[11] * proj[15];

	clip[12] = modl[12] * proj[ 0] + modl[13] * proj[ 4] + modl[14] * proj[ 8] + modl[15] * proj[12];
	clip[13] = modl[12] * proj[ 1] + modl[13] * proj[ 5] + modl[14] * proj[ 9] + modl[15] * proj[13];
	clip[14] = modl[12] * proj[ 2] + modl[13] * proj[ 6] + modl[14] * proj[10] + modl[15] * proj[14];
	clip[15] = modl[12] * proj[ 3] + modl[13] * proj[ 7] + modl[14] * proj[11] + modl[15] * proj[15];

	/* Extract the RIGHT plane */
	mFrustum[0] = QVector4D(
		clip[ 3] - clip[ 0],
		clip[ 7] - clip[ 4],
		clip[11] - clip[ 8],
		clip[15] - clip[12]
	);
	mFrustum[0] /= mFrustum[0].toVector3D().length();

	/* Extract the LEFT plane */
	mFrustum[1] = QVector4D(
		clip[ 3] + clip[ 0],
		clip[ 7] + clip[ 4],
		clip[11] + clip[ 8],
		clip[15] + clip[12]
	);
	mFrustum[1] /= mFrustum[1].toVector3D().length();

	/* Extract the BOTTOM plane */
	mFrustum[2] = QVector4D(
		clip[ 3] + clip[ 1],
		clip[ 7] + clip[ 5],
		clip[11] + clip[ 9],
		clip[15] + clip[13]
	);
	mFrustum[2] /= mFrustum[2].toVector3D().length();

	/* Extract the TOP plane */
	mFrustum[3] = QVector4D(
		clip[ 3] - clip[ 1],
		clip[ 7] - clip[ 5],
		clip[11] - clip[ 9],
		clip[15] - clip[13]
	);
	mFrustum[3] /= mFrustum[3].toVector3D().length();

	/* Extract the FAR plane */
	mFrustum[4] = QVector4D(
		clip[ 3] - clip[ 2],
		clip[ 7] - clip[ 6],
		clip[11] - clip[10],
		clip[15] - clip[14]
	);
	mFrustum[4] /= mFrustum[4].toVector3D().length();

	/* Extract the NEAR plane */
	mFrustum[5] = QVector4D(
		clip[ 3] + clip[ 2],
		clip[ 7] + clip[ 6],
		clip[11] + clip[10],
		clip[15] + clip[14]
	);
	mFrustum[5] /= mFrustum[5].toVector3D().length();
}


bool Eye::isPointInFrustum( QVector3D point ) const
{
	for( int p = 0; p < 6; p++ )
		if( mFrustum[p].x() * point.x() + mFrustum[p].y() * point.y() + mFrustum[p].z() * point.z() + mFrustum[p].w() <= 0 )
			return false;
	return true;
}


bool Eye::isSphereInFrustum( QVector3D center, float radius ) const
{
	for( int p = 0; p < 6; p++ )
	if( mFrustum[p].x() * center.x() + mFrustum[p].y() * center.y() + mFrustum[p].z() * center.z() + mFrustum[p].w() <= -radius )
		return false;
	return true;
}


void Eye::attach( QWeakPointer< AObject > object )
{
	mAttached = object;
}
