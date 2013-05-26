#include "Landscape.hpp"

#include <scene/Scene.hpp>
#include <scene/TextureRenderer.hpp>
#include <geometry/Terrain.hpp>

#include <resource/Material.hpp>
#include <resource/Shader.hpp>

#include <QString>
#include <QSettings>
#include <QGLShaderProgram>


Landscape::Landscape( Scene * scene, QString name ) :
	AObject( scene )
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
	mTerrainFilter = new Filter( this, QSize( 3, 3 ) );
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
	delete mTerrainFilter;
	delete mTerrainMaterial;
	delete mReflectionRenderer;
	delete mRefractionRenderer;
	delete mWaterShader;
}


void Landscape::updateSelf( const double & delta )
{
}


void Landscape::drawSelf()
{
}


void Landscape::drawSelfPost()
{
	mTerrainFilter->draw();
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
	if( scene()->eye()->position().y()>mWaterHeight )
		mirroredEye.setClippingPlane( 0, QVector4D(0,1,0, -mWaterHeight+mWaterClippingPlaneOffset) );
	else
		mirroredEye.setClippingPlane( 0, QVector4D(0,-1,0, mWaterHeight+mWaterClippingPlaneOffset) );
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
	if( scene()->eye()->position().y()>mWaterHeight )
		scene()->eye()->setClippingPlane( 0, QVector4D(0,-1,0, mWaterHeight+mWaterClippingPlaneOffset) );
	else
		scene()->eye()->setClippingPlane( 0, QVector4D(0,1,0, -mWaterHeight+mWaterClippingPlaneOffset) );
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

	glDisable( GL_CULL_FACE );
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
	glEnable( GL_CULL_FACE );
}


void Landscape::drawPatch( const QRectF & rect )
{
	mTerrainMaterial->bind();
	glMatrixMode( GL_TEXTURE );	glActiveTexture( GL_TEXTURE0 );	glPushMatrix();
		glScalef( mTerrainMaterialScale.x(), -mTerrainMaterialScale.y(), 1.0f );
		mTerrain->drawPatch( rect );
	glMatrixMode( GL_TEXTURE );	glPopMatrix();	glMatrixMode( GL_MODELVIEW );
	mTerrainMaterial->release();

	glDepthMask( GL_FALSE );
	glEnable( GL_BLEND );
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
	for( int i = 0; i < mBlobs.size(); ++i )
	{
		mBlobs[i]->drawPatch( rect );
	}
	glDisable( GL_BLEND );
	glDepthMask( GL_TRUE );
}




////////////////////////////////////////////////////////////////
// Blob


Landscape::Blob::Blob( Landscape * landscape, QRect rect, QString materialName, QVector2D materialScale, QString blobMapPath )
{
	mGLWidget = landscape->scene()->glWidget();
	mLandscape = landscape;
	mRect = rect;
	mMaterialScale = materialScale;
	mMaterial = new Material( mGLWidget, materialName, MaterialShaderVariant::BLOBBING );
	QImage blobMap = QImage( blobMapPath );
	if( blobMap.isNull() )
	{
		qFatal( "BlobMap from file \"%s\" could not be loaded!", blobMapPath.toLocal8Bit().constData() );
	}
	mBlobMap =  mGLWidget->bindTexture( blobMap );
	mMaterial->setBlobMap( mBlobMap );
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

	glActiveTexture( GL_TEXTURE0 );	glPushMatrix();
	glScaled( mMaterialScale.x(), -mMaterialScale.y(), 1.0 );

	glActiveTexture( GL_TEXTURE1 );	glPushMatrix();
	glScaled( 1.0/((double)mRect.width()), -1.0/((double)mRect.height()), 1.0 );
	glTranslated( -mRect.x(), -mRect.y(), 0.0 );

	mLandscape->terrain()->drawPatchMap( rectToDraw );

	glMatrixMode( GL_TEXTURE );
	glActiveTexture( GL_TEXTURE1 );	glPopMatrix();
	glActiveTexture( GL_TEXTURE0 );	glPopMatrix();

	glMatrixMode( GL_MODELVIEW );
	mMaterial->release();
}




////////////////////////////////////////////////////////////////
// Filter


Landscape::Filter::Patch::Patch( QVector3D position, QVector3D size ) :
	mPosition(position),
	mSize(size)
{
	mCenter = mPosition + mSize/2.0;
	mBoundingSphereRadius = size.length()/2.0f;
}


Landscape::Filter::Filter( Landscape * landscape, QSize filterSize ) :
	mLandscape(landscape),
	mFilterSize(filterSize)
{
	/* example for filterSize 3x3
	 *
	 * .---------size.x()---------.
	 *                   .-stepX--.
	 * |--------|--------|--------|-    -
	 * |        |        |        | |    |
	 * |        |        |        |stepZ |
	 * |        |        |        | |    |
	 * |--------|--------|--------|-     |
	 * |        |        |        |      |
	 * |        |  Eye   |        |     size.z()
	 * |        |        |        |      |
	 * |--------|--------|--------|      |
	 * |        |        |        |      |
	 * |        |        |        |      |
	 * |        |        |        |      |
	 * |--------|--------|--------|     -
	 * ^
	 * from
	 * '-----far----'
	 */
	//TODO: maybe do these calculations in map coordinates ...
	float farPlane = landscape->scene()->eye()->farPlane();
	QVector3D from = QVector3D( -farPlane, landscape->terrain()->offset().y(), -farPlane );
	QVector3D size = QVector3D( 2.0f*farPlane, landscape->terrain()->size().y(), 2.0f*farPlane );
	float stepX =  size.x()/(float)filterSize.width();
	float stepZ =  size.z()/(float)filterSize.height();
	QVector3D patchSize = QVector3D( stepX, landscape->terrain()->size().y(), stepZ );
	mPatches.clear();
	for( int z=0; z<mFilterSize.height(); ++z )
	{
		for( int x=0; x<mFilterSize.width(); ++x )
		{
			QVector3D pos = from + QVector3D( (float)x*stepX, 0.0f, (float)z*stepZ );
			mPatches.push_back( Patch( pos, patchSize ) );
		}
	}
}


Landscape::Filter::~Filter()
{

}


void Landscape::Filter::draw()
{
	for( int z=0; z<mFilterSize.height(); ++z )
	{
		QRectF mergedRect;	// we will merge patches along the x axis as this is optimal for the terrain mesh VBO
		for( int x=0; x<mFilterSize.width(); ++x )
		{
			Patch & patch = mPatches[z*mFilterSize.width()+x];
			QVector3D eyePosition = QVector3D( mLandscape->scene()->eye()->position().x(), 0.0f, mLandscape->scene()->eye()->position().z() );
			QVector3D spherePosition = eyePosition + patch.center();
/*
			GLUquadric * q = gluNewQuadric();
			glPushAttrib( GL_POLYGON_BIT );
			glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
			glColor3f( 1, 1, 1 );
			glPushMatrix();
			glTranslate( spherePosition );
			gluSphere( q, patch.boundingSphereRadius(), 32, 32 );
			glPopMatrix();
			glPopAttrib();
			gluDeleteQuadric( q );
*/
			if( mLandscape->scene()->eye()->isSphereInFrustum( spherePosition, patch.boundingSphereRadius() ) )
			{
				/*
				mLandscape->drawPatch(
					QRectF(
						eyePosition.x() + patch.position().x(),
						eyePosition.z() + patch.position().z(),
						patch.size().x(),
						patch.size().z()
					)
				);
				*/
				mergedRect = mergedRect.united(
					QRectF(
						eyePosition.x() + patch.position().x(),
						eyePosition.z() + patch.position().z(),
						patch.size().x(),
						patch.size().z()
					)
				);
			}
		}
		if( !mergedRect.isNull() )
		{
			mLandscape->drawPatch( mergedRect );
		}
	}
}
