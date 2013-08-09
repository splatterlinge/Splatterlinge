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

#ifndef FOREST_HPP
#define FOREST_HPP

#include "../../Scene.hpp"
#include "../WavefrontObject.hpp"
#include "geometry/Terrain.hpp"
#include "utility/RandomNumber.hpp"
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
