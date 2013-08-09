/*
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

#include "FrustumTest.hpp"


// http://www.crownandcutlass.com/features/technicaldetails/frustum.html


void FrustumTest::sync()
{
	float proj[16];
	float modl[16];
	glGetFloatv( GL_PROJECTION_MATRIX, proj );
	glGetFloatv( GL_MODELVIEW_MATRIX, modl );

	QMatrix4x4 projection(
		proj[0], proj[4], proj[8], proj[12],
		proj[1], proj[5], proj[9], proj[13],
		proj[2], proj[6], proj[10], proj[14],
		proj[3], proj[7], proj[11], proj[15]
	);

	QMatrix4x4 modelView(
		modl[0], modl[4], modl[8], modl[12],
		modl[1], modl[5], modl[9], modl[13],
		modl[2], modl[6], modl[10], modl[14],
		modl[3], modl[7], modl[11], modl[15]
	);

	sync( projection, modelView );
}


void FrustumTest::sync( const QMatrix4x4 & perspective, const QMatrix4x4 & modelView )
{
	QMatrix4x4 clip = perspective * modelView;

	/* Extract the RIGHT plane */
	mFrustum[0] = clip.row(3) - clip.row(0);
	mFrustum[0] /= mFrustum[0].toVector3D().length();

	/* Extract the LEFT plane */
	mFrustum[1] = clip.row(3) + clip.row(0);
	mFrustum[1] /= mFrustum[1].toVector3D().length();

	/* Extract the BOTTOM plane */
	mFrustum[2] = clip.row(3) + clip.row(1);
	mFrustum[2] /= mFrustum[2].toVector3D().length();

	/* Extract the TOP plane */
	mFrustum[3] = clip.row(3) - clip.row(1);
	mFrustum[3] /= mFrustum[3].toVector3D().length();

	/* Extract the FAR plane */
	mFrustum[4] = clip.row(3) - clip.row(2);
	mFrustum[4] /= mFrustum[4].toVector3D().length();

	/* Extract the NEAR plane */
	mFrustum[5] = clip.row(3) + clip.row(2);
	mFrustum[5] /= mFrustum[5].toVector3D().length();
}


bool FrustumTest::isPointInFrustum( QVector3D point ) const
{
	for( int p = 0; p < 6; p++ )
		if( mFrustum[p].x() * point.x() + mFrustum[p].y() * point.y() + mFrustum[p].z() * point.z() + mFrustum[p].w() <= 0 )
			return false;
	return true;
}


bool FrustumTest::isSphereInFrustum( QVector3D center, float radius ) const
{
	for( int p = 0; p < 6; p++ )
	if( mFrustum[p].x() * center.x() + mFrustum[p].y() * center.y() + mFrustum[p].z() * center.z() + mFrustum[p].w() <= -radius )
		return false;
	return true;
}
