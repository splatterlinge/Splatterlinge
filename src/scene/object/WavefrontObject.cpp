#include "WavefrontObject.hpp"

WavefrontObject::WavefrontObject( Scene * scene, const float & size, QString filename ) :
    AObject( scene, size ),
    mSize( size )
{
    mVertices = new QList<QVector4D>();
    mTextureVertices = new QList<QVector3D>();
    mNormals = new QList<QVector3D>();
    mFaces = new QList<FacePoint>();

    load( filename );
}

WavefrontObject::~WavefrontObject()
{
    delete mVertices;
    delete mTextureVertices;
    delete mNormals;
    delete mFaces;
}

bool WavefrontObject::load( QString filename )
{
    QFile file( filename );
    if( !file.open( QIODevice::ReadOnly ) ) {
        QMessageBox::information( 0, "Error", file.errorString() );
        return false;
    }

    QTextStream in( &file );

    while( !in.atEnd() ) {
        QString line = in.readLine();
        QStringList fields = line.split( " ", QString::SkipEmptyParts );

        if( fields[0] == "v" )
        {
            fields.removeFirst();

            QVector4D v;
            v.setX( fields[0].toFloat() );
            v.setY( fields[1].toFloat() );
            v.setZ( fields[2].toFloat() );

            if( fields.length() == 4 )
            {
                v.setW( fields[3].toFloat() );
            }
            else
            {
                v.setW( 1.0f );
            }

            mVertices->append( v );
        }
        else if( fields[0] == "vt" )
        {
            fields.removeFirst();

            QVector3D v;
            v.setX( fields[0].toFloat() );

            if( fields.length() >= 2 )
            {
                v.setY( fields[1].toFloat() );
            }
            if( fields.length() == 3 )
            {
                v.setZ( fields[2].toFloat() );
            }

            mTextureVertices->append( v );
        }
        else if( fields[0] == "vn" )
        {
            fields.removeFirst();

            QVector3D v;

            v.setX( fields[0].toFloat() );
            v.setY( fields[1].toFloat() );
            v.setZ( fields[2].toFloat() );

            mNormals->append( v );
        }
        else if( fields[0] == "f" )
        {
            fields.removeFirst();
            foreach( QString s, fields)
            {
                QStringList fp = s.split( "/" );
                FacePoint p;
                p.vertex = fp[0].toInt();
                if( fp.length() >= 2)
                {
                    p.texture = fp[1].toInt();
                }
                if( fp.length() == 3)
                {
                    p.normal = fp[2].toInt();
                }

                mFaces->append( p );
            }
        }
    }

    qDebug() << "VERTICES";
    qDebug() << mVertices->toVector();

    qDebug() << "TEXTURE VERTICES";
    qDebug() << mTextureVertices->toVector();

    qDebug() << "NORMALS";
    qDebug() << mNormals->toVector();

    file.close();

    return true;
}

void WavefrontObject::updateSelf( const float & delta )
{
    // TODO
}


void WavefrontObject::drawSelf()
{
    // TODO
}
