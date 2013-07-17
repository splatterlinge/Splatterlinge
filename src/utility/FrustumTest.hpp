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

	/// Sync to current modelview/perspective matrix stack.
	void sync();

	/// Visibility test on viewing frustum.
	bool isPointInFrustum( QVector3D point ) const ;
	/// Visibility test on viewing frustum.
	bool isSphereInFrustum( QVector3D center, float radius ) const;

private:
	QVector4D mFrustum[6];
};


#endif
