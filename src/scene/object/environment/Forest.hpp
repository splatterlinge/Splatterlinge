#ifndef FOREST_HPP
#define FOREST_HPP

#include "../../Scene.hpp"
#include "../WavefrontObject.hpp"
#include "geometry/Terrain.hpp"
#include "qmath.h"

#include <QPointF>
#include <QVector>
#include <QMatrix4x4>

class Forest : public WavefrontObject
{
public:
	Forest( Scene * scene, Terrain * terrain, QString filename, QPointF position, int radius, int number );
	~Forest();

private:
	Terrain * mTerrain;
};

#endif // FOREST_HPP
