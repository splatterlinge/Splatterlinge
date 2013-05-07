#include "Material.hpp"

#include "Shader.hpp"

#include <GLWidget.hpp>

#include <unistd.h>


RESOURCE_CACHE(MaterialData);


MaterialData::MaterialData( GLWidget * glWidget, QString name ) :
	AResourceData( name ),
	mGLWidget(glWidget),
	mName(name),
	mDiffuseMap(-1),
	mSpecularMap(-1),
	mNormalMap(-1)
{
}


MaterialData::~MaterialData()
{
	if( loaded() )
		qDebug() << "-" << this << "MaterialData" << uid();
}


bool MaterialData::load()
{
	qDebug() << "+" << this << "MaterialData" << uid();

	QSettings s( "./data/material/"+mName+"/material.ini", QSettings::IniFormat );

	s.beginGroup( "Material" );
		QString diffuseMapPath = "./data/material/"+mName+"/" + s.value( "diffuseMapPath", "diffuse.png" ).toString();
		QString specularMapPath = "./data/material/"+mName+"/" + s.value( "specularMapPath", "specular.png" ).toString();
		QString normalMapPath = "./data/material/"+mName+"/" + s.value( "normalMapPath", "normal.png" ).toString();
		mAmbient = QVector4D(
			s.value( "ambientRed", 0.2f ).toFloat(),
			s.value( "ambientGreen", 0.2f ).toFloat(),
			s.value( "ambientBlue", 0.2f ).toFloat(),
			s.value( "ambientAlpha", 1.0f ).toFloat()
		);
		mDiffuse = QVector4D(
			s.value( "diffuseRed", 0.8f ).toFloat(),
			s.value( "diffuseGreen", 0.8f ).toFloat(),
			s.value( "diffuseBlue", 0.8f ).toFloat(),
			s.value( "diffuseAlpha", 1.0f ).toFloat()
		);
		mSpecular = QVector4D(
			s.value( "specularRed", 0.0f ).toFloat(),
			s.value( "specularGreen", 0.0f ).toFloat(),
			s.value( "specularBlue", 0.0f ).toFloat(),
			s.value( "specularAlpha", 1.0f ).toFloat()
		);
		mEmission = QVector4D(
			s.value( "emissionRed", 0.0f ).toFloat(),
			s.value( "emissionGreen", 0.0f ).toFloat(),
			s.value( "emissionBlue", 0.0f ).toFloat(),
			s.value( "emissionAlpha", 1.0f ).toFloat()
		);
		mShininess = s.value( "shininess", 0 ).toFloat();
		mDefaultShaderName = s.value( "shader", "versatile" ).toString();
	s.endGroup();
	
	QPixmap diffuseMap = QPixmap( diffuseMapPath );
	if( diffuseMap.isNull() )
	{
		qFatal( "\"%s\" not found!", diffuseMapPath.toLocal8Bit().constData() );
	}
	QPixmap specularMap = QPixmap( specularMapPath );
	if( specularMapPath.isNull() )
	{
		qFatal( "\"%s\" not found!", specularMapPath.toLocal8Bit().constData() );
	}
	QPixmap normalMap = QPixmap( normalMapPath );
	if( normalMap.isNull() )
	{
		qFatal( "\"%s\" not found!", normalMapPath.toLocal8Bit().constData() );
	}

	mDiffuseMap =  mGLWidget->bindTexture( diffuseMap, GL_TEXTURE_2D, GL_RGBA, QGLContext::MipmapBindOption | QGLContext::LinearFilteringBindOption );
	mSpecularMap =  mGLWidget->bindTexture( specularMap, GL_TEXTURE_2D, GL_RGBA, QGLContext::MipmapBindOption | QGLContext::LinearFilteringBindOption );
	mNormalMap =  mGLWidget->bindTexture( normalMap, GL_TEXTURE_2D, GL_RGB, QGLContext::MipmapBindOption | QGLContext::LinearFilteringBindOption );

	return AResourceData::load();
}


Material::Quality Material::sGlobalMaxQuality = Material::HIGH_QUALITY;


Material::Material( GLWidget * glWidget, QString name ) : AResource()
{
	mGLWidget = glWidget;
	mMaskMap = -1;
	mCubeMap = -1;
	mDefaultQuality = HIGH_QUALITY;
	mShader[HIGH_QUALITY] = 0;
	mShader[MEDIUM_QUALITY] = 0;
	mShader[LOW_QUALITY] = 0;
	QSharedPointer<MaterialData> n( new MaterialData( glWidget, name ) );
	cache( n );
	setDefaultShader();
}


Material::~Material()
{
	delete mShader[HIGH_QUALITY];
	delete mShader[MEDIUM_QUALITY];
	delete mShader[LOW_QUALITY];
}


Material::Quality Material::getBindingQuality()
{
	Quality q = mDefaultQuality;
	if( mDefaultQuality > sGlobalMaxQuality )
		q = sGlobalMaxQuality;

	if( !mShader[q] && q==HIGH_QUALITY )
		q = MEDIUM_QUALITY;
	if( !mShader[q] && q==MEDIUM_QUALITY )
		q = LOW_QUALITY;

	return q;
}


void Material::bind()
{
	mBoundQuality = getBindingQuality();

	if( !mShader[mBoundQuality] )
		return;
	
	mShader[mBoundQuality]->bind();

	glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT, reinterpret_cast<const GLfloat*>( &(data()->ambient()) ) );
	glMaterialfv( GL_FRONT_AND_BACK, GL_DIFFUSE, reinterpret_cast<const GLfloat*>( &(data()->diffuse()) ) );
	glMaterialfv( GL_FRONT_AND_BACK, GL_SPECULAR, reinterpret_cast<const GLfloat*>( &(data()->specular()) ) );
	glMaterialfv( GL_FRONT_AND_BACK, GL_EMISSION, reinterpret_cast<const GLfloat*>( &(data()->emission()) ) );
	glMaterialf( GL_FRONT_AND_BACK, GL_SHININESS, data()->shininess() );

	if( mCubeMap >= 0 && mShader[mBoundQuality]->hasCubeMap() )
	{
		mGLWidget->glActiveTexture( GL_TEXTURE0+(mShader[mBoundQuality]->texUnit_cubeMap()) );
		glBindTexture( GL_TEXTURE_2D, mCubeMap );
	}
	if( mMaskMap >= 0 && mShader[mBoundQuality]->hasMaskMap() )
	{
		mGLWidget->glActiveTexture( GL_TEXTURE0+(mShader[mBoundQuality]->texUnit_maskMap()) );
		glBindTexture( GL_TEXTURE_2D, mMaskMap );
	}
	if( data()->normalMap() >= 0 && mShader[mBoundQuality]->hasNormalMap() )
	{
		mGLWidget->glActiveTexture( GL_TEXTURE0+(mShader[mBoundQuality]->texUnit_normalMap()) );
		glBindTexture( GL_TEXTURE_2D, data()->normalMap() );
	}
	if( data()->specularMap() >= 0 && mShader[mBoundQuality]->hasSpecularMap() )
	{
		mGLWidget->glActiveTexture( GL_TEXTURE0+(mShader[mBoundQuality]->texUnit_specularMap()) );
		glBindTexture( GL_TEXTURE_2D, data()->specularMap() );
	}
	if( data()->diffuseMap() >= 0 && mShader[mBoundQuality]->hasDiffuseMap() )
	{
		mGLWidget->glActiveTexture( GL_TEXTURE0+(mShader[mBoundQuality]->texUnit_diffuseMap()) );
		glBindTexture( GL_TEXTURE_2D, data()->diffuseMap() );
	}
}


void Material::release()
{
	if( !mShader[mBoundQuality] )
		return;

	mShader[mBoundQuality]->release();
}


void Material::setShader( QString shaderName )
{
	delete mShader[HIGH_QUALITY];
	delete mShader[MEDIUM_QUALITY];
	delete mShader[LOW_QUALITY];

	if( access( QString("./data/shader/"+shaderName+".high.vert").toLocal8Bit().constData(), R_OK ) == 0 )
	{
		mShader[HIGH_QUALITY] = new Shader( mGLWidget, shaderName+".high" );
	} else {
		mShader[HIGH_QUALITY] = 0;
	}

	if( access( QString("./data/shader/"+shaderName+".medium.vert").toLocal8Bit().constData(), R_OK ) == 0 )
	{
		mShader[MEDIUM_QUALITY] = new Shader( mGLWidget, shaderName+".medium" );
	} else {
		mShader[MEDIUM_QUALITY] = 0;
	}

	if( access( QString("./data/shader/"+shaderName+".low.vert").toLocal8Bit().constData(), R_OK ) == 0 )
	{
		mShader[LOW_QUALITY] = new Shader( mGLWidget, shaderName+".low" );
	} else {
		mShader[LOW_QUALITY] = 0;
	}
}


void Material::setDefaultShader()
{
	setShader( data()->defaultShaderName() );
}


void Material::setMaskMap( QString path )
{
	QPixmap maskMap = QPixmap( path );
	if( maskMap.isNull() )
	{
		qFatal( "\"%s\" not found!", path.toLocal8Bit().constData() );
	}

	mMaskMap =  mGLWidget->bindTexture( maskMap, GL_TEXTURE_2D, GL_LUMINANCE, QGLContext::MipmapBindOption | QGLContext::LinearFilteringBindOption );
}
