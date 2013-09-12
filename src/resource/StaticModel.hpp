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

#ifndef RESOURCE_STATICMODEL_INCLUDED
#define RESOURCE_STATICMODEL_INCLUDED

#include <QtDebug>

#include "AResource.hpp"
#include "Material.hpp"
#include <scene/Scene.hpp>
#include <geometry/Vertex.hpp>
#include <utility/Vector.hpp>

#include <QGLBuffer>
#include <QFile>
#include <QFileInfo>
#include <QMatrix4x4>

class Face
{
public:
	Face() {}
	Face( QStringList & fields, QString & material, QVector<QVector3D> * positions, QVector<QVector2D> * texCoords, QVector<QVector3D> * normals );

	QVector<VertexP3fN3fT2f> points;
	QString material;

	bool operator==( const Face & other )
	{
		return points == other.points
			&& material == other.material;
	}
};

class Part
{
public:
	Part() {}
	Part( unsigned int current, unsigned int count, GLWidget * widget, QString & material );

	unsigned int start;
	unsigned int count;
	Material * material;
};

/// The model's data
class StaticModelData : public AResourceData
{
public:
	StaticModelData( GLWidget * glWidget, QString name );
	virtual ~StaticModelData();

	const QString & name() const { return mName; }
	int mode() { return mMode; }
	QVector<Part> & parts() { return mParts; }
	QGLBuffer & vertexBuffer() { return mVertexBuffer; }
	QGLBuffer & indexBuffer() { return mIndexBuffer; }

	bool parse();

	// Overrides:
	virtual bool load();
	virtual void unload();

	static QString baseDirectory() { return AResourceData::baseDirectory()+"model/"; }

protected:
	GLWidget * mGLWidget;
	QString mName;
	GLuint mMode;
	QVector<Part> mParts;
	QVector<VertexP3fN3fT2f> mVertices;
	QVector<unsigned int> mIndices;
	QGLBuffer mVertexBuffer;
	QGLBuffer mIndexBuffer;

	void generateParts( QVector<Face> * faces );
	void generateBuffers();
	QString generateMaterialName( QFile & file, QStringList & fields );
};


/// A static model
class StaticModel : public AResource<StaticModelData>
{
public:
	StaticModel( GLWidget * glWidget, QString name );
	virtual ~StaticModel();

	void draw( const QMatrix4x4 & viewMatrix, const QVector<QMatrix4x4> & instances );
	void draw();
};


#endif
