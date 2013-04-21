#include "Landscape.hpp"

#include <QString>
#include <QSettings>


Landscape::Landscape( QGLWidget * glWidget, QString name )
{
	QSettings s( "./data/landscape/"+name+"/landscape.ini", QSettings::IniFormat );

	s.beginGroup( "Terrain" );
		QString heightMapPath = s.value( "heightMapPath" ).toString();
		QString terrainMaterial = s.value( "material" ).toString();
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
	mTerrain = new Terrain( "./data/landscape/"+name+"/"+heightMapPath, QVector3D(1000,200,1000), QVector3D(-500,-50,-500) );
	
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
	mTerrain->draw();
	mTerrainMaterial->release();
}
