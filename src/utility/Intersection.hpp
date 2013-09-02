#ifndef INTERSECTION_HPP
#define INTERSECTION_HPP

#include <QVector4D>
#include <QVector3D>
#include <qmatrix4x4.h>
#include <QtGlobal>
#include <GLWidget.hpp>

#include "Triangle.hpp"

namespace Intersection
{
	bool intersectTriangleFan(const GLfloat PositionData[], const int firstVertexPos, const int lastVertexPos,
							  const QMatrix4x4 & modelMatrix,const QVector3D & origin, const QVector3D & direction,
							  float * intersectionDistance );
	bool intersectTriangleStrip(const GLfloat PositionData[], const int firstVertexPos, const int lastVertexPos,
							  const QMatrix4x4 & modelMatrix,const QVector3D & origin, const QVector3D & direction,
							  float * intersectionDistance );
}
#endif // INTERSECTION_HPP
