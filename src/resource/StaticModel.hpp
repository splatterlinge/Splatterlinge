#ifndef RESOURCE_STATICMODEL_INCLUDED
#define RESOURCE_STATICMODEL_INCLUDED

#include "GLWidget.hpp"
#include "AResource.hpp"
#include "Material.hpp"
#include "utility/Vertex.hpp"

#include <QGLBuffer>
#include <QFile>
#include <QFileInfo>

class Face
{
public:
	Face() {
		points = new QVector<Vertex>();
	}

	QVector<Vertex> * points;
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
	unsigned int start;
	unsigned int count;
	Material * material;
};

/// The model's data
class StaticModelData : public AResourceData
{
public:
	StaticModelData( GLWidget * widget, QString name );
	virtual ~StaticModelData();

	const QString & name() const { return mName; }
	QSizeF size() { return mSize; }
	QVector<Part> & parts() { return mParts; }
	QGLBuffer & vertexBuffer() { return mVertexBuffer; }
	QGLBuffer & indexBuffer() { return mIndexBuffer; }

	bool parse();

	// Overrides:
	virtual bool load();
	virtual void unload();

protected:
	GLWidget * mGLWidget;
	QString mName;
	QSizeF mSize;
	QVector<Part> mParts;
	QVector<Vertex> mVertices;
	QVector<unsigned int> mIndices;
	QGLBuffer mVertexBuffer;
	QGLBuffer mIndexBuffer;
};


/// A static model
class StaticModel : public AResource<StaticModelData>
{
public:
	StaticModel( GLWidget * widget, QString name );
	virtual ~StaticModel();

	void draw();

private:
	GLWidget * mGLWidget;
};


#endif
