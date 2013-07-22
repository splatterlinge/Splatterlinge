#include "WavefrontModel.hpp"

WavefrontModel::WavefrontModel( GLWidget * widget, QString filename )
{
	mGLWidget = widget;
	mFilename = filename;
	mIndex = 0;
	mFaces = new QList<Face>();
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
	QList<QVector3D> vertices;
	QList<QVector2D> textureVertices;
	QList<QVector3D> normals;
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
			vertices.append( QVector3D( x, y, z ) );
		}
		else if( keyword == "vt" )
		{
			u = fields.takeFirst().toFloat();
			v = fields.takeFirst().toFloat();
			textureVertices.append( QVector2D( u, v ) );
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
			FacePoint point;
			QStringList points;

			while( !fields.isEmpty() )
			{
				points = fields.takeFirst().split( "/" );
				while( !points.isEmpty() )
				{
					point.vertex = vertices.at( points.takeFirst().toInt()-1 );
					point.texCoord = textureVertices.at( points.takeFirst().toInt()-1 );
					point.normal = normals.at( points.takeFirst().toInt()-1 );

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

	mSize.setWidth(maxX-minX);
	mSize.setHeight(maxY-minY);

	return true;
}

bool WavefrontModel::render()
{
	mIndex = glGenLists(1);
	Material * lastMat = NULL;

	glNewList( mIndex, GL_COMPILE );

	foreach( Face face, *mFaces )
	{
		if( face.material != NULL )
		{
			if( face.material != lastMat )
			{
				if( lastMat != NULL )
				{
					lastMat->release();
				}
				face.material->bind();
				lastMat = face.material;
			}
		}
		glBegin( GL_TRIANGLES );
		foreach( FacePoint fp, *face.points )
		{
			glNormal( fp.normal );
			glTexCoord( fp.texCoord );
			glVertex( fp.vertex );
		}
		glEnd();
	}
	if(lastMat)
		lastMat->release();
	glEndList();

	return true;
}
