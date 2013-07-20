#include "WavefrontModel.hpp"

WavefrontModel::WavefrontModel( GLWidget * widget, QString filename )
{
	mGLWidget = widget;
	mFaces = new QList<Face>();
	mFilename = filename;
	qDebug() << "+" << this << "WavefrontModel:" << filename;
}

int WavefrontModel::load()
{
	if( !mMap.contains( mFilename ) )
	{
		parse();
		render();
	}

	return mMap[mFilename];
}

bool WavefrontModel::parse()
{
	QFile file( mFilename );
	QString line;
	QString keyword;
	QStringList fields;
	QList<QVector3D> * vertices = new QList<QVector3D>();
	QList<QVector2D> * textureVertices = new QList<QVector2D>();
	QList<QVector3D> * normals = new QList<QVector3D>();
	Material * material;
	float x, y, z;
	float u, v;

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
			vertices->append( QVector3D( x, y, z ) );
		}
		else if( keyword == "vt" )
		{
			u = fields.takeFirst().toFloat();
			v = fields.takeFirst().toFloat();
			textureVertices->append( QVector2D( u, v ) );
		}
		else if( keyword == "vn" )
		{
			x = fields.takeFirst().toFloat();
			y = fields.takeFirst().toFloat();
			z = fields.takeFirst().toFloat();
			normals->append( QVector3D( x, y, z ) );
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
					point.vertex = &vertices->at( points.takeFirst().toInt()-1 );
					point.texCoord = &textureVertices->at( points.takeFirst().toInt()-1 );
					point.normal = &normals->at( points.takeFirst().toInt()-1 );

					face.points->append( point );
					face.material = material;
				}
			}
			mFaces->append( face );
		}
		else if( keyword == "usemtl" )
		{
			QFileInfo fileinfo( file );
			material = new Material( mGLWidget, fileinfo.baseName()+"_"+fields.takeFirst() );
		}
	}

	file.close();

	return true;
}

bool WavefrontModel::render()
{
	GLuint index = glGenLists(1);
	Material * lastMat = NULL;

	glNewList( index, GL_COMPILE );

	foreach( Face face, *mFaces )
	{
		if( face.material != NULL )
		{
			if( face.material != lastMat )
			{
				if( lastMat != NULL )
				{
					// TODO fix release!
					//lastMat->release();
				}
				qDebug() << ">>> Change Material";
				face.material->bind();
			}
		}
		glBegin( GL_TRIANGLES );
		foreach( FacePoint fp, *face.points )
		{
			QVector3D normal = *fp.normal;
			QVector3D texture = *fp.texCoord;
			QVector3D vertex = *fp.vertex;

			glNormal3f( normal.x(), normal.y(), normal.z() );
			glTexCoord2f( texture.x(), texture.y() );
			glVertex3f( vertex.x(), vertex.y(), vertex.z() );
		}
		glEnd();
		lastMat = face.material;
	}

	glEndList();

	mMap[mFilename] = index;

	return true;
}
