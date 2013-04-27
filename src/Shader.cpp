#include "Shader.hpp"

#include <QDebug>
#include <GL/glu.h>


RESOURCE_CACHE(ShaderData);


ShaderData::ShaderData( QGLWidget * glWidget, QString name ) :
	ResourceData( name ),
	mGLWidget(glWidget),
	mName(name),
	mProgram(0)
{
}


ShaderData::~ShaderData()
{
	if( loaded() )
	{
		qDebug() << "-" << this << "ShaderData" << uid();
		delete mProgram;
		mProgram = 0;
	}
}


bool ShaderData::load()
{
	delete mProgram;
	mProgram = new QGLShaderProgram( mGLWidget );
	qDebug() << "+" << this << "ShaderData" << uid();
	mProgram->addShaderFromSourceFile( QGLShader::Vertex, "./data/shader/"+mName+".vsh" );
	mProgram->addShaderFromSourceFile( QGLShader::Fragment, "./data/shader/"+mName+".fsh" );
	if( !mProgram->link() )
	{
		qWarning() << mProgram->log();
		return false;
	}

	unsigned int texUnit = 0;
	mUniform_diffuseMap = mProgram->uniformLocation("diffuseMap");
	if( mUniform_diffuseMap < 0 )
	{
		qDebug() << "Uniform \"diffuseMap\" not found - texture disabled.";
	} else {
		mTexUnit_diffuseMap = texUnit++;
		qDebug() << "Texture \"diffuseMap\" on Unit" << mTexUnit_diffuseMap;
	}
	mUniform_specularMap = mProgram->uniformLocation("specularMap");
	if( mUniform_specularMap < 0 )
	{
		qDebug() << "Uniform \"specularMap\" not found - texture disabled.";
	} else {
		mTexUnit_specularMap = texUnit++;
		qDebug() << "Texture \"specularMap\" on Unit" << mTexUnit_specularMap;
	}
	mUniform_normalMap = mProgram->uniformLocation("normalMap");
	if( mUniform_normalMap < 0 )
	{
		qDebug() << "Uniform \"normalMap\" not found - texture disabled.";
	} else {
		mTexUnit_normalMap = texUnit++;
		qDebug() << "Texture \"normalMap\" on Unit" << mTexUnit_normalMap;
	}
	mUniform_maskMap = mProgram->uniformLocation("maskMap");
	if( mUniform_maskMap < 0 )
	{
		qDebug() << "Uniform \"maskMap\" not found - texture disabled.";
	} else {
		mTexUnit_maskMap = texUnit++;
		qDebug() << "Texture \"maskMap\" on Unit" << mTexUnit_maskMap;
	}
	mUniform_cubeMap = mProgram->uniformLocation("cubeMap");
	if( mUniform_cubeMap < 0 )
	{
		qDebug() << "Uniform \"cubeMap\" not found - texture disabled.";
	} else {
		mTexUnit_cubeMap = texUnit++;
		qDebug() << "Texture \"cubeMap\" on Unit" << mTexUnit_cubeMap;
	}

	return ResourceData::load();
}


Shader::Shader( QGLWidget * glWidget, QString name ) : Resource()
{
	QSharedPointer<ShaderData> n( new ShaderData( glWidget, name ) );
	cache( n );
}


Shader::~Shader()
{
}


void Shader::bind()
{
	data()->program()->bind();
	if( data()->uniform_diffuseMap() >= 0 )
		data()->program()->setUniformValue( data()->uniform_diffuseMap(), data()->texUnit_diffuseMap() );
	if( data()->uniform_specularMap() >= 0 )
		data()->program()->setUniformValue( data()->uniform_specularMap(), data()->texUnit_specularMap() );
	if( data()->uniform_normalMap() >= 0 )
		data()->program()->setUniformValue( data()->uniform_normalMap(), data()->texUnit_normalMap() );
	if( data()->uniform_maskMap() >= 0 )
		data()->program()->setUniformValue( data()->uniform_maskMap(), data()->texUnit_maskMap() );
	if( data()->uniform_cubeMap() >= 0 )
		data()->program()->setUniformValue( data()->uniform_cubeMap(), data()->texUnit_cubeMap() );
}


void Shader::release()
{
	data()->program()->release();
}
