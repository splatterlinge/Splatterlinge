#include "WavefrontObject.hpp"

WavefrontObject::WavefrontObject()
{
    vertices = new QList<QVector4D>();
    verticies_textures = new QList<QVector3D>();
    normals = new QList<QVector3D>();
    faces = new QList<FacePoint>();
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

            vertices->append( v );
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

            verticies_textures->append( v );
        }
        else if( fields[0] == "vn" )
        {
            fields.removeFirst();

            QVector3D v;

            v.setX( fields[0].toFloat() );
            v.setY( fields[1].toFloat() );
            v.setZ( fields[2].toFloat() );

            normals->append( v );
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

                faces->append( p );
            }
        }
    }

    qDebug() << "VERTICES";
    qDebug() << vertices->toVector();

    qDebug() << "TEXTURE VERTICES";
    qDebug() << verticies_textures->toVector();

    qDebug() << "NORMALS";
    qDebug() << normals->toVector();

    file.close();

    return true;
}
