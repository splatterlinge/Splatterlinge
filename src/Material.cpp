#include "Material.hpp"

#include <QCryptographicHash>
#include <stdio.h>


RESOURCE_CACHE(MaterialData);


MaterialData::MaterialData( QGLWidget * glWidget, QString name ) :
	ResourceData( name ),
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
		qWarning() << diffuseMapPath << "not found!";
	}
	QPixmap specularMap = QPixmap( specularMapPath );
	if( specularMapPath.isNull() )
	{
		qWarning() << specularMapPath << "not found!";
	}
	QPixmap normalMap = QPixmap( normalMapPath );
	if( normalMap.isNull() )
	{
		qWarning() << normalMapPath << "not found!";
	}

	mDiffuseMap =  mGLWidget->bindTexture( diffuseMap, GL_TEXTURE_2D, GL_RGBA, QGLContext::MipmapBindOption | QGLContext::LinearFilteringBindOption );
	mSpecularMap =  mGLWidget->bindTexture( specularMap, GL_TEXTURE_2D, GL_RGBA, QGLContext::MipmapBindOption | QGLContext::LinearFilteringBindOption );
	mNormalMap =  mGLWidget->bindTexture( normalMap, GL_TEXTURE_2D, GL_RGB, QGLContext::MipmapBindOption | QGLContext::LinearFilteringBindOption );

	return ResourceData::load();
}


Material::Material( QGLWidget * glWidget, QString name ) : Resource()
{
	mGLWidget = glWidget;
	mMaskMap = -1;
	mCubeMap = -1;
	mShader = 0;
	QSharedPointer<MaterialData> n( new MaterialData( glWidget, name ) );
	cache( n );
	setDefaultShader();
}


Material::~Material()
{
	delete mShader;
}


void Material::bind()
{
	if( !mShader )
		return;
	
	mShader->bind();

	glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT, reinterpret_cast<const GLfloat*>( &(data()->ambient()) ) );
	glMaterialfv( GL_FRONT_AND_BACK, GL_DIFFUSE, reinterpret_cast<const GLfloat*>( &(data()->diffuse()) ) );
	glMaterialfv( GL_FRONT_AND_BACK, GL_SPECULAR, reinterpret_cast<const GLfloat*>( &(data()->specular()) ) );
	glMaterialfv( GL_FRONT_AND_BACK, GL_EMISSION, reinterpret_cast<const GLfloat*>( &(data()->emission()) ) );
	glMaterialf( GL_FRONT_AND_BACK, GL_SHININESS, data()->shininess() );

	if( mCubeMap >= 0 && mShader->hasCubeMap() )
		glActiveTexture( GL_TEXTURE0+(mShader->texUnit_cubeMap()) );	glBindTexture( GL_TEXTURE_2D, mCubeMap );

	if( mMaskMap >= 0 && mShader->hasMaskMap() )
		glActiveTexture( GL_TEXTURE0+(mShader->texUnit_maskMap()) );	glBindTexture( GL_TEXTURE_2D, mMaskMap );

	if( data()->normalMap() >= 0 && mShader->hasNormalMap() )
		glActiveTexture( GL_TEXTURE0+(mShader->texUnit_normalMap()) );	glBindTexture( GL_TEXTURE_2D, data()->normalMap() );

	if( data()->specularMap() >= 0 && mShader->hasSpecularMap() )
		glActiveTexture( GL_TEXTURE0+(mShader->texUnit_specularMap()) );	glBindTexture( GL_TEXTURE_2D, data()->specularMap() );

	if( data()->diffuseMap() >= 0 && mShader->hasDiffuseMap() )
		glActiveTexture( GL_TEXTURE0+(mShader->texUnit_diffuseMap()) );	glBindTexture( GL_TEXTURE_2D, data()->diffuseMap() );
}


void Material::release()
{
	if( !mShader )
		return;
	
	mShader->release();
}


void Material::setShader( QString shaderName )
{
	delete mShader;
	mShader = new Shader( mGLWidget, shaderName );
}


void Material::setDefaultShader()
{
	delete mShader;
	mShader = new Shader( mGLWidget, data()->defaultShaderName() );
}


void Material::setMaskMap( QString path )
{
	QPixmap maskMap = QPixmap( path );
	if( maskMap.isNull() )
	{
		qWarning() << path << "not found!";
	}

	mMaskMap =  mGLWidget->bindTexture( maskMap, GL_TEXTURE_2D, GL_LUMINANCE, QGLContext::MipmapBindOption | QGLContext::LinearFilteringBindOption );
}
