#include "Shader.hpp"

#include <QDebug>


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
	delete mShader;
}


bool ShaderData::load()
{
	qDebug() << "ShaderData load" << mName;
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
}


void Shader::release()
{
	data()->shader()->release();
}
