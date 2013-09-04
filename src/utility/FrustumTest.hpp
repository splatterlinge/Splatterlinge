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

#ifndef UTILITY_FRUSTUMTEST_INCLUDED
#define UTILITY_FRUSTUMTEST_INCLUDED

#include "glWrappers.hpp"

#include <QVector4D>


/// Frustum culling
class FrustumTest
{
public:
	FrustumTest() {}
	~FrustumTest() {}

	/// Sync to current OpenGL modelview/perspective matrix stack.
	void sync();
	/// Sync to modelview/perspective matrix stack.
	void sync( const QMatrix4x4 & perspective, const QMatrix4x4 & modelView );

	/// Visibility test on viewing frustum.
	bool isPointInFrustum( QVector3D point ) const ;
	/// Visibility test on viewing frustum.
	bool isSphereInFrustum( QVector3D center, float radius ) const;

private:
	QVector4D mFrustum[6];
};


#endif
