#include "WavefrontObject.hpp"

WavefrontObject::WavefrontObject( Scene * scene, const float & size, QString filename ) :
	AObject( scene, size ),
	mSize( size )
{
	mScene = scene;
	mScale = 0.4;
	mVertices = new QList<QVector3D>();
	mTextureVertices = new QList<QVector3D>();
	mNormals = new QList<QVector3D>();
	mFaces = new QList<Face>();

	parseObj( filename );

	qDebug() << "+" << this << "WavefrontData:" << filename;
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
	QString line;
	QString keyword;
	QStringList fields;
	float x, y, z;
	float u, v, w;

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
			y = fields.takeFirst().toFloat();
			z = fields.takeFirst().toFloat();
			mVertices->append( QVector3D( x, y, z ) );
		}
		else if( keyword == "vt" )
		{
			u = fields.takeFirst().toFloat();
			v = fields.takeFirst().toFloat();
			w = 1.0f;
			mTextureVertices->append( QVector3D( u, v, w ) );
		}
		else if( keyword == "vn" )
		{
			x = fields.takeFirst().toFloat();
			y = fields.takeFirst().toFloat();
			z = fields.takeFirst().toFloat();
			mNormals->append( QVector3D( x, y, z ) );
		}
		else if( keyword == "f" )
		{
			Face face;
			FacePoint point;
			QStringList points;

			while( !fields.isEmpty() )
			{
				points = fields.takeFirst().split( "/" );
				while( !points.isEmpty() )
				{
					point.vertex = &mVertices->at( points.takeFirst().toInt()-1 );
					point.texCoord = &mTextureVertices->at( points.takeFirst().toInt()-1 );
					point.normal = &mNormals->at( points.takeFirst().toInt()-1 );

					face.points->append( point );
					face.material = mMaterial;
				}
			}
			mFaces->append( face );
		}
		else if( keyword == "usemtl" )
		{
			QFileInfo fileinfo( file );
			mMaterial = new Material( mScene->glWidget(), fileinfo.baseName()+"_"+fields.takeFirst() );
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

	glScalef( 1.0*mScale, 1.0*mScale, 1.0*mScale );
	glColor3f( 1.0f, 1.0f, 1.0f );

	foreach( Face face, *mFaces )
	{
		if( face.material != 0 )
		{
			face.material->bind();
		}
		glBegin( GL_TRIANGLES );
		foreach( FacePoint fp, *face.points )
		{
			QVector3D normal = *fp.normal;
			QVector3D texture = *fp.texCoord;
			QVector3D vertex = *fp.vertex;

			glNormal3f( normal.x(), normal.y(), normal.z() );
			glTexCoord3f( texture.x(), texture.y(), texture.z() );
			glVertex3f( vertex.x(), vertex.y(), vertex.z() );
		}
		glEnd();
		if( face.material != 0 )
		{
			face.material->release();
		}
	}

	glDisable( GL_LIGHTING );
	glDisable( GL_LIGHT0 );

	glColor3f( 1.0, 0.0, 0.0 );
	foreach( Face face, *mFaces )
	{
		foreach( FacePoint fp, *face.points )
		{
			QVector3D normal = *fp.normal;
			QVector3D vertex = *fp.vertex;

			glBegin( GL_LINES );
			glVertex3f( vertex.x(), vertex.y(), vertex.z() );
			glVertex3f( vertex.x()+normal.x()*3, vertex.y()+normal.y()*3, vertex.z()+normal.z()*3 );
			glEnd();
		}
	}

	glPopMatrix();
	glPopAttrib();
}
