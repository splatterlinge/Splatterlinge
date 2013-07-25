#include "StaticModel.hpp"

#include <scene/object/AObject.hpp>

#include <QDebug>
#include <QVector3D>
#include <float.h>


RESOURCE_CACHE(StaticModelData);


StaticModelData::StaticModelData( QString name ) :
    AResourceData( name ),
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

    //TODO: free static data

    StaticModelData::unload();
}


bool StaticModelData::load()
{
    unload();
    qDebug() << "+" << this << "StaticModelData" << uid();

    mParts.clear();

    mVertexBuffer = QGLBuffer( QGLBuffer::VertexBuffer );
    mVertexBuffer.create();

    mIndexBuffer = QGLBuffer( QGLBuffer::IndexBuffer );
    mIndexBuffer.create();

    parse();

    return AResourceData::load();
}

bool StaticModelData::parse()
{
    qDebug() << "parse";

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
            Vertex vertex;
            QStringList points;

            while( !fields.isEmpty() )
            {
                points = fields.takeFirst().split( "/" );
                while( !points.isEmpty() )
                {
                    vertex.position = positions.at( points.takeFirst().toInt()-1 );
                    vertex.texCoord = texCoords.at( points.takeFirst().toInt()-1 );
                    vertex.normal = normals.at( points.takeFirst().toInt()-1 );

                    face.points->append( vertex );
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
            p.material = lastMat;
            mParts.append(p);

            qDebug() << p.start << " + " << p.count << " | " << p.material;

            lastMat = face.material;
            count = 0;
        }

        foreach( Vertex vertex, *face.points )
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

    qDebug() << mParts.size();

    mVertexBuffer.bind();
    mVertexBuffer.setUsagePattern( QGLBuffer::StaticDraw );
    mVertexBuffer.allocate( mVertices.constData(), mVertices.size() * sizeof( Vertex ) );
    mVertexBuffer.release();

    mIndexBuffer.bind();
    mIndexBuffer.setUsagePattern( QGLBuffer::StaticDraw );
    mIndexBuffer.allocate( mIndices.constData(), mIndices.size() * sizeof( unsigned int ) );
    mIndexBuffer.release();

    return true;
}


StaticModel::StaticModel( GLWidget * widget, QString name ) :
    AResource(),
    mGLWidget( widget )
{
    QSharedPointer<StaticModelData> n( new StaticModelData( name ) );
    cache( n );
}


StaticModel::~StaticModel()
{
    //TODO: free data
}

void StaticModel::draw()
{
    Material * material;

    data()->vertexBuffer().bind();
    data()->indexBuffer().bind();

    glEnableClientState( GL_VERTEX_ARRAY );
    glEnableClientState( GL_TEXTURE_COORD_ARRAY );
    glEnableClientState( GL_NORMAL_ARRAY );
    glEnableClientState( GL_INDEX_ARRAY );

    glVertexPointer( 3, GL_FLOAT, sizeof( Vertex ), (void*)offsetof(Vertex,position) );
    glTexCoordPointer( 2, GL_FLOAT, sizeof( Vertex ), (void*)offsetof(Vertex,texCoord) );
    glNormalPointer( GL_FLOAT, sizeof( Vertex ), (void*)offsetof(Vertex,normal) );

    foreach( Part part, data()->parts() )
    {
        if( part.material != "" )
        {
            material = new Material( mGLWidget, part.material );
            material->bind();
        }

        glDrawElements(
                    GL_TRIANGLES,
                    part.count,
                    GL_UNSIGNED_INT,
                    (void*)((size_t)(sizeof(unsigned int)*(	// convert index to pointer
                        part.start		// index to start
                    ) ) )
        );

        if( part.material != "" )
            material->release();
    }

    glDisableClientState( GL_INDEX_ARRAY );
    glDisableClientState( GL_NORMAL_ARRAY );
    glDisableClientState( GL_TEXTURE_COORD_ARRAY );
    glDisableClientState( GL_VERTEX_ARRAY );

    data()->vertexBuffer().release();
    data()->indexBuffer().release();
}
