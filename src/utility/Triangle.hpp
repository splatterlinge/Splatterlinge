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

#ifndef UTILITY_TRIANGLE_INCLUDED
#define UTILITY_TRIANGLE_INCLUDED


#include <QVector3D>


/// Triangle
class Triangle
{
public:
	Triangle() : mP(), mQ(), mR() {}
	Triangle( const QVector3D & p, const QVector3D & q, const QVector3D & r ) : mP(p), mQ(q), mR(r) {}
	const QVector3D & p() const { return mP; }
	const QVector3D & q() const { return mQ; }
	const QVector3D & r() const { return mR; }
	void setP( const QVector3D & point ) { mP = point; }
	void setQ( const QVector3D & point ) { mQ = point; }
	void setR( const QVector3D & point ) { mR = point; }

	QVector3D normal() const { return QVector3D::normal( mP, mQ, mR); }

	bool intersectRay( const QVector3D & origin, const QVector3D & direction, float * intersectionDistance ) const
		{ return intersectRay( mP, mQ, mR, origin, direction, intersectionDistance ); }

	static bool intersectRay( const QVector3D & p, const QVector3D & q, const QVector3D & r, const QVector3D & origin, const QVector3D & direction, float * intersectionDistance );

private:
	QVector3D mP;
	QVector3D mQ;
	QVector3D mR;
};


#endif
