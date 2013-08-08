#ifndef RESOURCE_STATICMODEL_INCLUDED
#define RESOURCE_STATICMODEL_INCLUDED

#include "AResource.hpp"
#include "Material.hpp"
#include <scene/Scene.hpp>
#include <geometry/Vertex.hpp>

#include <QGLBuffer>
#include <QFile>
#include <QFileInfo>
#include <QMatrix4x4>

class Face
{
public:
	Face() {}

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
	unsigned int start;
	unsigned int count;
	Material * material;
};

/// The model's data
class StaticModelData : public AResourceData
{
public:
	StaticModelData( Scene * scene, QString name );
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
	Scene * mScene;
	QString mName;
	QSizeF mSize;
	QVector<Part> mParts;
	QVector<VertexP3fN3fT2f> mVertices;
	QVector<unsigned int> mIndices;
	QGLBuffer mVertexBuffer;
	QGLBuffer mIndexBuffer;
};


/// A static model
class StaticModel : public AResource<StaticModelData>
{
public:
	StaticModel( Scene * scene, QString name );
	virtual ~StaticModel();

	QSizeF getSize() { return data()->size(); }

	void draw( const QVector<QMatrix4x4> * instances );

private:
	Scene * mScene;
};


#endif
