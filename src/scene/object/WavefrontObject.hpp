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

#ifndef WAVEFRONTOBJECT_HPP
#define WAVEFRONTOBJECT_HPP

#include <scene/Scene.hpp>
#include <resource/StaticModel.hpp>

#include <QString>
#include <QVector>
#include <QVectorIterator>
#include <QMatrix4x4>

class WavefrontObject : public AObject
{
public:
	WavefrontObject( Scene * scene, QString filename );
	WavefrontObject( Scene * scene, QString filename, QVector<QMatrix4x4> * instances );
	virtual ~WavefrontObject();

	virtual void updateSelf( const double & delta );
	virtual void drawSelf();

protected:
	StaticModel * mModel;
	Scene * mScene;
	QString mFilename;
	QVector<QMatrix4x4> * mInstances;
};

#endif // WAVEFRONTOBJECT_HPP
