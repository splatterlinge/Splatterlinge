#include "Landscape.hpp"

#include <QString>
#include <QSettings>


Landscape::Landscape( QGLWidget * glWidget, QString name )
{
	QSettings s( "./data/landscape/"+name+"/landscape.ini", QSettings::IniFormat );

	s.beginGroup( "Terrain" );
		QString heightMapPath = s.value( "heightMapPath" ).toString();
		mTerrainSize = QVector3D(
			s.value( "sizeX", 1000.0f ).toFloat(),
			s.value( "sizeY", 100.0f ).toFloat(),
			s.value( "sizeZ", 1000.0f ).toFloat()
		);
		mTerrainOffset = QVector3D(
			s.value( "offsetX", -500.0f ).toFloat(),
			s.value( "offsetY", -50.0f ).toFloat(),
			s.value( "offsetZ", -500.0f ).toFloat()
		);
		QString terrainMaterial = s.value( "material" ).toString();
		mTerrainMaterialScale = QVector2D(
			s.value( "materialScaleS", 1.0f ).toFloat(),
			s.value( "materialScaleT", 1.0f ).toFloat()
		);
	s.endGroup();
/*
	QList<Blob*> blobs;
	int blobNum = s.beginReadArray( "Blob" );
		for( int i=0; i<blobNum; i++ )
		{
			s.setArrayIndex( i );
			Blob * b = new Blob( s.value("rect").toRect(), s.value("material").toString() );
			blobs.append( b );
		}
	s.endArray();
*/
	mTerrain = new Terrain( "./data/landscape/"+name+"/"+heightMapPath, mTerrainSize, mTerrainOffset );
	
	mTerrainMaterial = new Material( glWidget, terrainMaterial );
}


Landscape::~Landscape()
{
	delete mTerrain;
	delete mTerrainMaterial;
}


void Landscape::draw()
{
	mTerrainMaterial->bind();
	glMatrixMode( GL_TEXTURE );	glPushMatrix();
	glScalef( mTerrainMaterialScale.x(), mTerrainMaterialScale.y(), 1.0f );
	mTerrain->draw();
	glMatrixMode( GL_TEXTURE );	glPopMatrix();
	mTerrainMaterial->release();
}
