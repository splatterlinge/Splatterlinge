#include "StaticModel.hpp"

#include <scene/object/AObject.hpp>

#include <QDebug>
#include <QVector3D>
#include <float.h>


RESOURCE_CACHE(StaticModelData);


StaticModelData::StaticModelData( Scene * scene, QString name ) :
	AResourceData( name ),
	mScene( scene ),
	mName( name )
{
}


StaticModelData::~StaticModelData()
{
	unload();
}


void StaticModelData::unload()
{
	if( !loaded() )
		return;
	qDebug() << "-" << this << "StaticModelData" << uid();

	foreach( Part part, mParts )
	{
		if( part.material )
		{
			delete part.material;
		}
	}

	mParts.clear();
	mVertices.clear();
	mIndices.clear();

	mVertexBuffer.release();
	mVertexBuffer.destroy();

	mIndexBuffer.release();
	mIndexBuffer.destroy();

	AResourceData::unload();
}


bool StaticModelData::load()
{
	unload();
	qDebug() << "+" << this << "StaticModelData" << uid();

	parse();

	return AResourceData::load();
}

bool StaticModelData::parse()
{
	QFile file( mName );
	QString line;
	QString keyword;
	QStringList fields;
	QVector<Face> faces;
	QVector<QVector3D> positions;
	QVector<QVector2D> texCoords;
	QVector<QVector3D> normals;
	QString material;
	float x, y, z;
	float u, v;

	float minX = 0;
	float maxX = 0;
	float minY = 0;
	float maxY = 0;

	if( !file.open( QIODevice::ReadOnly ) ) {
		qDebug() << file.errorString();
		return false;
	}

	QTextStream in( &file );

	while( !in.atEnd() ) {
		line = in.readLine().trimmed();

		while( line.endsWith( "\\" ) )
		{
			line.truncate( line.size()-1 );
			if( in.atEnd() )
			{
				break;
			}
			line += in.readLine().trimmed();
		}

		if( line.startsWith( "#" ) || line.isEmpty() )
		{
			continue;
		}

		fields = line.split( " ", QString::SkipEmptyParts );
		keyword = fields.takeFirst();

		if( keyword == "v" )
		{
			x = fields.takeFirst().toFloat();
			if( x > maxX )
				maxX = x;
			if( x < minX )
				minX = x;

			y = fields.takeFirst().toFloat();
			if( y > maxY )
				maxY = y;
			if( y < minY )
				minY = y;

			z = fields.takeFirst().toFloat();
			positions.append( QVector3D( x, y, z ) );
		}
		else if( keyword == "vt" )
		{
			u = fields.takeFirst().toFloat();
			v = fields.takeFirst().toFloat();
			texCoords.append( QVector2D( u, v ) );
		}
		else if( keyword == "vn" )
		{
			x = fields.takeFirst().toFloat();
			y = fields.takeFirst().toFloat();
			z = fields.takeFirst().toFloat();
			normals.append( QVector3D( x, y, z ) );
		}
		else if( keyword == "f" )
		{
			Face face;
			VertexP3fN3fT2f vertex;
			QStringList points;

			while( !fields.isEmpty() )
			{
				points = fields.takeFirst().split( "/" );
				while( !points.isEmpty() )
				{
					vertex.position = positions.at( points.takeFirst().toInt()-1 );
					vertex.texCoord = texCoords.at( points.takeFirst().toInt()-1 );
					vertex.normal = normals.at( points.takeFirst().toInt()-1 );

					face.points.append( vertex );
					face.material = material;
				}
			}
			faces.append( face );
		}
		else if( keyword == "usemtl" )
		{
			QFileInfo fileinfo( file );
			material = fileinfo.baseName()+"_"+fields.takeFirst();
		}
	}

	file.close();

	mSize.setWidth(maxX-minX);
	mSize.setHeight(maxY-minY);

	mParts.clear();
	QString lastMat = faces.first().material;
	unsigned int current = 0;
	unsigned int count = 0;

	foreach( Face face, faces )
	{
		if( face.material != lastMat || face == faces.last() )
		{
			Part p;
			p.start = current - count;
			p.count = count;
			p.material = new Material( mScene->glWidget(), lastMat );
			mParts.append(p);

			lastMat = face.material;
			count = 0;
		}

		foreach( VertexP3fN3fT2f vertex, face.points )
		{
			if( mVertices.indexOf( vertex ) == -1 )
			{
				mVertices.append( vertex );
			}
			mIndices.append( mVertices.indexOf( vertex ) );

			current++;
			count++;
		}
	}

	mVertexBuffer = QGLBuffer( QGLBuffer::VertexBuffer );
	mVertexBuffer.create();
	mVertexBuffer.bind();
	mVertexBuffer.setUsagePattern( QGLBuffer::StaticDraw );
	mVertexBuffer.allocate( mVertices.constData(), mVertices.size() * sizeof( VertexP3fN3fT2f ) );
	mVertexBuffer.release();

	mIndexBuffer = QGLBuffer( QGLBuffer::IndexBuffer );
	mIndexBuffer.create();
	mIndexBuffer.bind();
	mIndexBuffer.setUsagePattern( QGLBuffer::StaticDraw );
	mIndexBuffer.allocate( mIndices.constData(), mIndices.size() * sizeof( unsigned int ) );
	mIndexBuffer.release();

	return true;
}


StaticModel::StaticModel( Scene * scene, QString name, QVector<QMatrix4x4> instances ) :
	AResource(),
	mScene( scene ),
	mInstances( instances )
{
	QSharedPointer<StaticModelData> n( new StaticModelData( scene, name ) );
	cache( n );
}


StaticModel::~StaticModel()
{
}

void StaticModel::draw()
{
	data()->vertexBuffer().bind();
	data()->indexBuffer().bind();

	glEnableClientState( GL_INDEX_ARRAY );
	VertexP3fN3fT2f::glEnableClientState();
	VertexP3fN3fT2f::glPointerVBO();

	glPushMatrix();

	foreach( Part part, data()->parts() )
	{
		if( part.material )
		{
			part.material->bind();
		}

		foreach( QMatrix4x4 instance, mInstances )
		{
			glLoadMatrixd( ( mScene->eye()->viewMatrix() * instance ).constData() );

			glDrawElements(
				GL_TRIANGLES,
				part.count,
				GL_UNSIGNED_INT,
				(void*)((size_t)(sizeof(unsigned int)*(	// convert index to pointer
					part.start		// index to start
				) ) )
			);
		}

		if( part.material )
		{
			part.material->release();
		}
	}

	glPopMatrix();

	glDisableClientState( GL_INDEX_ARRAY );
	VertexP3fN3fT2f::glDisableClientState();

	data()->vertexBuffer().release();
	data()->indexBuffer().release();
}
