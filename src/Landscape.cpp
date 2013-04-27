#include "Landscape.hpp"

#include <QString>
#include <QSettings>


Landscape::Blob::Blob( Landscape * landscape, QRect rect, QString materialName, QVector2D materialScale, QString maskPath )
{
	mLandscape = landscape;
	mRect = rect;
	mMaterialScale = materialScale;
	mMaterial = new Material( mLandscape->getGLWidget(), materialName );
	mMaterial->setShader( "versatileBlob" );
	mMaterial->setMaskMap( maskPath );
}


Landscape::Blob::~Blob()
{
	delete mMaterial;
}


void Landscape::Blob::draw()
{
	mMaterial->bind();
	glMatrixMode( GL_TEXTURE );

	glActiveTexture( GL_TEXTURE0 );	glPushMatrix();
	glScalef( mMaterialScale.x(), mMaterialScale.y(), 1.0f );

	glActiveTexture( GL_TEXTURE1 );	glPushMatrix();
	glScalef( 1.0f/((float)mRect.width()), 1.0f/((float)mRect.height()), 1.0f );
	glTranslatef( -mRect.x(), -mRect.y(), 0.0f );

	mLandscape->getTerrain()->drawPatchMap( mRect );
	glMatrixMode( GL_TEXTURE );

	glActiveTexture( GL_TEXTURE1 );	glPopMatrix();
	glActiveTexture( GL_TEXTURE0 );	glPopMatrix();

	glMatrixMode( GL_TEXTURE );
	mMaterial->release();
}


void Landscape::Blob::drawPatchMap( const QRect & visible )
{
	QRect rectToDraw = mRect.intersected( visible );
	if( rectToDraw.width() <= 1 || rectToDraw.height() <= 1 )
		return;	// nothing to draw

	mMaterial->bind();
	glMatrixMode( GL_TEXTURE );

	glActiveTexture( GL_TEXTURE0 );	glPushMatrix();
	glScalef( mMaterialScale.x(), mMaterialScale.y(), 1.0f );

	glActiveTexture( GL_TEXTURE1 );	glPushMatrix();
	glScalef( 1.0f/((float)mRect.width()), 1.0f/((float)mRect.height()), 1.0f );
	glTranslatef( -mRect.x(), -mRect.y(), 0.0f );

	mLandscape->getTerrain()->drawPatchMap( rectToDraw );
	glMatrixMode( GL_TEXTURE );

	glActiveTexture( GL_TEXTURE1 );	glPopMatrix();
	glActiveTexture( GL_TEXTURE0 );	glPopMatrix();

	glMatrixMode( GL_TEXTURE );
	mMaterial->release();
}


Landscape::Landscape( QGLWidget * glWidget, QString name )
{
	mGLWidget = glWidget;
	mName = name;

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

	int blobNum = s.beginReadArray( "Blob" );
		for( int i=0; i<blobNum; i++ )
		{
			s.setArrayIndex( i );
			Blob * b = new Blob( this,
				s.value("rect").toRect(),
				s.value("material").toString(),
				QVector2D(
					s.value( "materialScaleS", 1.0f ).toFloat(),
					s.value( "materialScaleT", 1.0f ).toFloat()
				),
				"./data/landscape/"+name+"/"+s.value("maskPath").toString()
			);
			mBlobs.append( b );
		}
	s.endArray();

	mTerrain = new Terrain( "./data/landscape/"+name+"/"+heightMapPath, mTerrainSize, mTerrainOffset );
	
	mTerrainMaterial = new Material( glWidget, terrainMaterial );
}


Landscape::~Landscape()
{
	for( int i = 0; i < mBlobs.size(); ++i )
	{
		delete mBlobs[i];
	}
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

	glDepthMask( GL_FALSE );
	glEnable( GL_BLEND );
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
	for( int i = 0; i < mBlobs.size(); ++i )
	{
		mBlobs[i]->draw();
	}
	glDisable( GL_BLEND );
	glDepthMask( GL_TRUE );
}


void Landscape::drawPatchMap( const QRect & visible )
{
	mTerrainMaterial->bind();
	glMatrixMode( GL_TEXTURE );	glPushMatrix();
	glScalef( mTerrainMaterialScale.x(), mTerrainMaterialScale.y(), 1.0f );
	mTerrain->drawPatchMap( visible );
	glMatrixMode( GL_TEXTURE );	glPopMatrix();
	mTerrainMaterial->release();

	glDepthMask( GL_FALSE );
	glEnable( GL_BLEND );
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
	for( int i = 0; i < mBlobs.size(); ++i )
	{
		mBlobs[i]->drawPatchMap( visible );
	}
	glDisable( GL_BLEND );
	glDepthMask( GL_TRUE );
}
