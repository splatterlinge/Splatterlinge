#include "WavefrontModel.hpp"

WavefrontModel::WavefrontModel( GLWidget * widget, QString filename )
{
    mGLWidget = widget;
    mFilename = filename;
    mSize = QSizeF(0,0);
    qDebug() << "+" << this << "WavefrontModel:" << filename;

    parse();
    render();
}

bool WavefrontModel::parse()
{
    QFile file( mFilename );
    QString line;
    QString keyword;
    QStringList fields;
    QVector<Face> faces;
    QVector<QVector3D> positions;
    QVector<QVector2D> texCoords;
    QVector<QVector3D> normals;
    Material * material = NULL;
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
            material = new Material( mGLWidget, fileinfo.baseName()+"_"+fields.takeFirst() );
        }
    }

    file.close();

    mSize.setWidth(maxX-minX);
    mSize.setHeight(maxY-minY);

    foreach( Face face, faces )
    {
        foreach( Vertex vertex, *face.points )
        {
            if( mVertices.indexOf( vertex ) == -1 )
            {
                mVertices.append( vertex );
            }
            mIndices.append( mVertices.indexOf( vertex ) );
        }
    }

    mVertexBuffer = QGLBuffer( QGLBuffer::VertexBuffer );
    mVertexBuffer.create();
    mVertexBuffer.bind();
    mVertexBuffer.setUsagePattern( QGLBuffer::StaticDraw );
    mVertexBuffer.allocate( mVertices.constData(), mVertices.size() * sizeof( Vertex ) );
    mVertexBuffer.release();

    mIndexBuffer = QGLBuffer( QGLBuffer::IndexBuffer );
    mIndexBuffer.create();
    mIndexBuffer.bind();
    mIndexBuffer.setUsagePattern( QGLBuffer::StaticDraw );
    mIndexBuffer.allocate( mIndices.constData(), mIndices.size() * sizeof( unsigned int ) );
    mIndexBuffer.release();

    return true;
}

bool WavefrontModel::render()
{
    //Material * lastMat = NULL;

    mVertexBuffer.bind();
    mIndexBuffer.bind();

    glEnableClientState( GL_VERTEX_ARRAY );
    glEnableClientState( GL_INDEX_ARRAY );

    glVertexPointer( 3, GL_FLOAT, sizeof( Vertex ), (void*)offsetof(Vertex,position) );
    glTexCoordPointer( 2, GL_FLOAT, sizeof( Vertex ), (void*)offsetof(Vertex,texCoord) );
    glNormalPointer( GL_FLOAT, sizeof( Vertex ), (void*)offsetof(Vertex,normal) );

    glDrawElements(
                GL_TRIANGLES,
                mIndices.size(),
                GL_UNSIGNED_INT,
                0
    );

    glDisableClientState( GL_INDEX_ARRAY );
    glDisableClientState( GL_VERTEX_ARRAY );

    mVertexBuffer.release();
    mIndexBuffer.release();

    return true;
}
