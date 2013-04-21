#include "Material.hpp"

#include <QCryptographicHash>
#include <stdio.h>


RESOURCE_CACHE(MaterialData);


MaterialData::MaterialData( QGLWidget * glWidget, QString name ) :
	ResourceData( name ),
	mGLWidget(glWidget),
	mName(name),
	mShader(0)
{
}


MaterialData::~MaterialData()
{
	delete mShader;
}


bool MaterialData::load()
{
	qDebug() << "MaterialData load" << mName;
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
		QString shaderName = s.value( "shader", "" ).toString();
	s.endGroup();
	
	QPixmap diffuseMap = QPixmap(diffuseMapPath);
	if( diffuseMap.isNull() )
	{
		qWarning() << diffuseMapPath << " not found!";
	}
	QPixmap specularMap = QPixmap(specularMapPath);
	if( specularMapPath.isNull() )
	{
		qWarning() << specularMapPath << " not found!";
	}
	QPixmap normalMap = QPixmap(normalMapPath);
	if( normalMap.isNull() )
	{
		qWarning() << normalMapPath << " not found!";
	}

	mDiffuseMap =  mGLWidget->bindTexture( diffuseMap, GL_TEXTURE_2D, GL_RGBA, QGLContext::MipmapBindOption | QGLContext::LinearFilteringBindOption );
	mSpecularMap =  mGLWidget->bindTexture( specularMap, GL_TEXTURE_2D, GL_RGBA, QGLContext::MipmapBindOption | QGLContext::LinearFilteringBindOption );
	mNormalMap =  mGLWidget->bindTexture( normalMap, GL_TEXTURE_2D, GL_RGB, QGLContext::MipmapBindOption | QGLContext::LinearFilteringBindOption );
	mShader = new Shader( mGLWidget, shaderName );

	return ResourceData::load();
}


Material::Material( QGLWidget * glWidget, QString name ) : Resource()
{
	QSharedPointer<MaterialData> n( new MaterialData( glWidget, name ) );
	cache( n );
}


Material::~Material()
{
}


void Material::bind()
{
	data()->shader()->bind();

	glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT, reinterpret_cast<const GLfloat*>( &(data()->ambient()) ) );
	glMaterialfv( GL_FRONT_AND_BACK, GL_DIFFUSE, reinterpret_cast<const GLfloat*>( &(data()->diffuse()) ) );
	glMaterialfv( GL_FRONT_AND_BACK, GL_SPECULAR, reinterpret_cast<const GLfloat*>( &(data()->specular()) ) );
	glMaterialfv( GL_FRONT_AND_BACK, GL_EMISSION, reinterpret_cast<const GLfloat*>( &(data()->emission()) ) );
	glMaterialf( GL_FRONT_AND_BACK, GL_SHININESS, data()->shininess() );

	glActiveTexture( GL_TEXTURE2 );	glBindTexture( GL_TEXTURE_2D, data()->normalMap() );
	glActiveTexture( GL_TEXTURE1 );	glBindTexture( GL_TEXTURE_2D, data()->specularMap() );
	glActiveTexture( GL_TEXTURE0 );	glBindTexture( GL_TEXTURE_2D, data()->diffuseMap() );
}


void Material::release()
{
	data()->shader()->release();
}
