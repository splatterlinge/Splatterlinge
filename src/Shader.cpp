#include "Shader.hpp"

#include <QDebug>
#include <GL/glu.h>


RESOURCE_CACHE(ShaderData);


ShaderData::ShaderData( QGLWidget * glWidget, QString name ) :
	ResourceData( name ),
	mGLWidget(glWidget),
	mName(name),
	mShader(0)
{
}


ShaderData::~ShaderData()
{
	qDebug() << "- ShaderData" << uid();
	delete mShader;
	mShader = 0;
}


bool ShaderData::load()
{
	qDebug() << "+ ShaderData" << uid();
	delete mShader;
	mShader = new QGLShaderProgram( mGLWidget );
	mShader->addShaderFromSourceFile( QGLShader::Vertex, "./data/shader/"+mName+".vsh" );
	mShader->addShaderFromSourceFile( QGLShader::Fragment, "./data/shader/"+mName+".fsh" );
	if( !mShader->link() )
	{
		qWarning() << mShader->log();
		return false;
	}

	mUniform_diffuseMap = mShader->uniformLocation("diffuseMap");
	if( mUniform_diffuseMap < 0 )
	{
		qDebug() << "Uniform \"diffuseMap\" not found - disabled.";
	}
	mUniform_specularMap = mShader->uniformLocation("specularMap");
	if( mUniform_diffuseMap < 0 )
	{
		qDebug() << "Uniform \"specularMap\" not found - disabled.";
	}
	mUniform_normalMap = mShader->uniformLocation("normalMap");
	if( mUniform_diffuseMap < 0 )
	{
		qDebug() << "Uniform \"normalMap\" not found - disabled.";
	}
	mUniform_maskMap = mShader->uniformLocation("maskMap");
	if( mUniform_maskMap < 0 )
	{
		qDebug() << "Uniform \"maskMap\" not found - disabled.";
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
	data()->shader()->bind();
	if( data()->uniform_diffuseMap() >= 0 )
		data()->shader()->setUniformValue( data()->uniform_diffuseMap(), 0 );
	if( data()->uniform_specularMap() >= 0 )
		data()->shader()->setUniformValue( data()->uniform_specularMap(), 1 );
	if( data()->uniform_normalMap() >= 0 )
		data()->shader()->setUniformValue( data()->uniform_normalMap(), 2 );
	if( data()->uniform_maskMap() >= 0 )
		data()->shader()->setUniformValue( data()->uniform_maskMap(), 3 );
}


void Shader::release()
{
	data()->shader()->release();
}
