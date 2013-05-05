#include "Landscape.hpp"

#include <QString>
#include <QSettings>

#include "GLScene.hpp"
#include "TextureRenderer.hpp"


Landscape::Blob::Blob( Landscape * landscape, QRect rect, QString materialName, QVector2D materialScale, QString maskPath )
{
	mGLWidget = landscape->scene()->glWidget();
	mLandscape = landscape;
	mRect = rect;
	mMaterialScale = materialScale;
	mMaterial = new Material( mGLWidget, materialName );
	mMaterial->setShader( "versatileBlob" );
	mMaterial->setMaskMap( maskPath );
}


Landscape::Blob::~Blob()
{
	delete mMaterial;
}


void Landscape::Blob::drawPatchMap( const QRect & visible )
{
	QRect rectToDraw = mRect.intersected( visible );
	if( rectToDraw.width() <= 1 || rectToDraw.height() <= 1 )
		return;	// nothing to draw

	mMaterial->bind();
	glMatrixMode( GL_TEXTURE );

	mGLWidget->glActiveTexture( GL_TEXTURE0 );	glPushMatrix();
	glScaled( mMaterialScale.x(), mMaterialScale.y(), 1.0 );

	mGLWidget->glActiveTexture( GL_TEXTURE1 );	glPushMatrix();
	glScaled( 1.0/((double)mRect.width()), 1.0/((double)mRect.height()), 1.0 );
	glTranslated( -mRect.x(), -mRect.y(), 0.0 );

	mLandscape->terrain()->drawPatchMap( rectToDraw );

	glMatrixMode( GL_TEXTURE );
	mGLWidget->glActiveTexture( GL_TEXTURE1 );	glPopMatrix();
	mGLWidget->glActiveTexture( GL_TEXTURE0 );	glPopMatrix();

	glMatrixMode( GL_MODELVIEW );
	mMaterial->release();
}


Landscape::Landscape( GLScene * scene, QString name ) :
	Object3D( scene )
{
	mName = name;

	QSettings s( "./data/landscape/"+name+"/landscape.ini", QSettings::IniFormat );

	s.beginGroup( "Terrain" );
		QString heightMapPath = s.value( "heightMapPath", "height.png" ).toString();
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
	s.beginGroup( "Water" );
		mWaterHeight = s.value( "height", 0.0f ).toFloat();
		mWaterClippingPlaneOffset = s.value( "clippingPlaneOffset", 0.05f ).toFloat();
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
	mTerrainMaterial = new Material( scene->glWidget(), terrainMaterial );

	mWaterShader = new Shader( scene->glWidget(), "water" );
	mReflectionRenderer = new TextureRenderer( scene->glWidget(), QSize(512,512), true );
	mRefractionRenderer = new TextureRenderer( scene->glWidget(), QSize(512,512), true );
}


Landscape::~Landscape()
{
	for( int i = 0; i < mBlobs.size(); ++i )
	{
		delete mBlobs[i];
	}
	delete mTerrain;
	delete mTerrainMaterial;
	delete mReflectionRenderer;
	delete mRefractionRenderer;
}


void Landscape::updateSelf( const float & delta )
{
}


void Landscape::drawSelf()
{
}


void Landscape::drawSelfPost()
{
	QRectF visible( scene()->eye()->position().x() - scene()->eye()->farPlane(),
			scene()->eye()->position().z() - scene()->eye()->farPlane(),
			scene()->eye()->farPlane()*2,
			scene()->eye()->farPlane()*2 );

	mTerrainMaterial->bind();
	glMatrixMode( GL_TEXTURE );	glPushMatrix();
	glScalef( mTerrainMaterialScale.x(), mTerrainMaterialScale.y(), 1.0f );
	mTerrain->drawPatchMap( mTerrain->toMap(visible) );
	glMatrixMode( GL_TEXTURE );	glPopMatrix();
	glMatrixMode( GL_MODELVIEW );
	mTerrainMaterial->release();

	glDepthMask( GL_FALSE );
	glEnable( GL_BLEND );
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
	for( int i = 0; i < mBlobs.size(); ++i )
	{
		mBlobs[i]->drawPatchMap( mTerrain->toMap(visible) );
	}
	glDisable( GL_BLEND );
	glDepthMask( GL_TRUE );
}


void Landscape::renderReflection()
{
	mReflectionRenderer->bind();
	glClear( GL_DEPTH_BUFFER_BIT );
	glMatrixMode( GL_PROJECTION );	glPushMatrix();	glLoadIdentity();
	glMatrixMode( GL_MODELVIEW );	glPushMatrix();	glLoadIdentity();

	Eye * sceneEye = scene()->eye();
	Eye mirroredEye( *sceneEye );
	QQuaternion r = mirroredEye.rotation();
	r.setX( -r.x() );
	r.setZ( -r.z() );
	mirroredEye.setRotation( r );
	mirroredEye.setPositionY( -mirroredEye.position().y() +2* mWaterHeight);
	mirroredEye.setClippingPlane( 0, QVector4D(0,1,0, -mWaterHeight+mWaterClippingPlaneOffset) );

	scene()->setEye( &mirroredEye );
	glScalef( 1,-1,1 );
	glCullFace( GL_FRONT );
	mirroredEye.draw();
	glCullFace( GL_BACK );
	scene()->setEye( sceneEye );

	glMatrixMode( GL_PROJECTION ); glPopMatrix();
	glMatrixMode( GL_MODELVIEW ); glPopMatrix();
	mReflectionRenderer->release();
}


void Landscape::renderRefraction()
{
	mRefractionRenderer->bind();
	glClear( GL_DEPTH_BUFFER_BIT );
	glMatrixMode( GL_PROJECTION );	glPushMatrix();	glLoadIdentity();
	glMatrixMode( GL_MODELVIEW );	glPushMatrix();	glLoadIdentity();
	scene()->eye()->setClippingPlane( 0, QVector4D(0,-1,0, mWaterHeight+mWaterClippingPlaneOffset) );
	scene()->eye()->draw();
	scene()->eye()->setClippingPlane( 0 );
	glMatrixMode( GL_PROJECTION );	glPopMatrix();
	glMatrixMode( GL_MODELVIEW );	glPopMatrix();
	mRefractionRenderer->release();
}


void Landscape::drawSelfPostProc()
{
//	Material::Quality defaultQuality = Material::globalMaxQuality();
//	Material::setGlobalMaxQuality( Material::LOW_QUALITY );

	renderReflection();
	renderRefraction();

//	Material::setGlobalMaxQuality( defaultQuality );

	glPushMatrix();
	glTranslatef( scene()->eye()->position().x(), mWaterHeight, scene()->eye()->position().z() );
	mWaterShader->bind();
	mWaterShader->program()->setUniformValue( "reflectionMap", 0 );
	mWaterShader->program()->setUniformValue( "refractionMap", 1 );
	glActiveTexture( GL_TEXTURE1 );	glBindTexture( GL_TEXTURE_2D, mRefractionRenderer->texID() );
	glActiveTexture( GL_TEXTURE0 );	glBindTexture( GL_TEXTURE_2D, mReflectionRenderer->texID() );
	glBegin( GL_QUADS );
		glVertex3f(-scene()->eye()->farPlane(), 0, scene()->eye()->farPlane() );
		glVertex3f( scene()->eye()->farPlane(), 0, scene()->eye()->farPlane() );
		glVertex3f( scene()->eye()->farPlane(), 0,-scene()->eye()->farPlane() );
		glVertex3f(-scene()->eye()->farPlane(), 0,-scene()->eye()->farPlane() );
	glEnd();
	mWaterShader->release();
	glPopMatrix();
}
