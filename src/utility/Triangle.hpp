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
