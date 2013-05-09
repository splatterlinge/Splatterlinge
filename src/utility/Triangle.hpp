#ifndef UTILITY_TRIANGLE_INCLUDED
#define UTILITY_TRIANGLE_INCLUDED


#include <QVector3D>


class Triangle
{
public:
	Triangle( const QVector3D & p, const QVector3D & q, const QVector3D & r ) : mP(p), mQ(q), mR(r) {}
	const QVector3D & p() const { return mP; }
	const QVector3D & q() const { return mQ; }
	const QVector3D & r() const { return mR; }
	void setP( const QVector3D & point ) { mP = point; }
	void setQ( const QVector3D & point ) { mP = point; }
	void setR( const QVector3D & point ) { mP = point; }

	bool intersectRay( const QVector3D & origin, const QVector3D & direction, float * len );

private:
	QVector3D mP;
	QVector3D mQ;
	QVector3D mR;
};


#endif
