#include "WavefrontObject.hpp"

WavefrontObject::WavefrontObject( Scene * scene, const float & size, QString filename ) :
	AObject( scene, size ),
	mSize( size )
{
	mScale = 1.0;
	mVertices = new QList<QVector4D>();
	mTextureVertices = new QList<QVector3D>();
	mNormals = new QList<QVector3D>();
	mFaces = new QList<QList<FacePoint> >();

	parseObj( filename );
}

WavefrontObject::~WavefrontObject()
{
	delete mVertices;
	delete mTextureVertices;
	delete mNormals;
	delete mFaces;
}

bool WavefrontObject::parseObj( QString filename )
{
	QFile file( filename );
	if( !file.open( QIODevice::ReadOnly ) ) {
		qDebug() << file.errorString();
		return false;
	}

	QTextStream in( &file );

	while( !in.atEnd() ) {
		QString line = in.readLine();
		QStringList fields = line.split( " ", QString::SkipEmptyParts );
		QString type = fields[0];
		fields.removeFirst();

		if( type == "mtllib" )
		{
			mMtllib = fields[0];
		}
		else if( type == "v" )
		{
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
		else if( type == "vt" )
		{
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
		else if( type == "vn" )
		{
			QVector3D v;

			v.setX( fields[0].toFloat() );
			v.setY( fields[1].toFloat() );
			v.setZ( fields[2].toFloat() );

			mNormals->append( v );
		}
		else if( type == "f" )
		{
			QList<FacePoint> list;

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

				list.append( p );
			}

			mFaces->append( list );
		}
	}

	foreach( QList<FacePoint> fl, * mFaces )
	{
		qDebug() << "TRIANGLE";
		foreach( FacePoint fp, fl )
		{
			qDebug() << "Vertice: " << mVertices->at( fp.vertex-1 );
			qDebug() << "Texture: " << mTextureVertices->at( fp.texture-1 );
			qDebug() << "Normal:  " << mNormals->at( fp.normal-1 );
		}
	}

	file.close();

	return true;
}

void WavefrontObject::updateSelf( const float & delta )
{
	// TODO
}


void WavefrontObject::drawSelf()
{
	glPushAttrib(GL_ENABLE_BIT | GL_EVAL_BIT);
	glEnable( GL_AUTO_NORMAL );
	glEnable( GL_NORMALIZE );
	glEnable( GL_LIGHTING );
	glEnable( GL_LIGHT0 );

	glPushMatrix();

	glScalef( 0.1*mScale, 0.1*mScale, 0.1*mScale );

	foreach( QList<FacePoint> fl, * mFaces )
	{
		glBegin( GL_TRIANGLES );
		foreach( FacePoint fp, fl )
		{
			QVector3D normal = mNormals->at( fp.normal-1 );
			glNormal3f( normal.x(), normal.y(), normal.z() );

			QVector3D texture = mTextureVertices->at( fp.texture-1 );
			glTexCoord3f( texture.x(), texture.y(), texture.z() );

			QVector4D vertex = mVertices->at( fp.vertex-1 );
			glVertex4f( vertex.x(), vertex.y(), vertex.z(), vertex.w() );
		}
		glEnd();
	}

	glDisable( GL_LIGHTING );
	glDisable( GL_LIGHT0 );

	glColor3f( 1.0, 0.0, 0.0 );
	foreach( QList<FacePoint> fl, * mFaces )
	{
		foreach( FacePoint fp, fl )
		{
			QVector4D vertex = mVertices->at( fp.vertex-1 );
			QVector3D normal = mNormals->at( fp.normal-1 );

			glBegin( GL_LINES );
			glVertex3f( vertex.x(), vertex.y(), vertex.z() );
			glVertex3f( vertex.x()+normal.x()*3, vertex.y()+normal.y()*3, vertex.z()+normal.z()*3 );
			glEnd();
		}
	}

	glPopMatrix();
	glPopAttrib();
}
