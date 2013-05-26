#include "Shader.hpp"

#include <GLWidget.hpp>

#include <QGLShaderProgram>
#include <QDebug>


RESOURCE_CACHE(ShaderData);


ShaderData::ShaderData( GLWidget * glWidget, QString name ) :
	AResourceData( name ),
	mGLWidget(glWidget),
	mName(name),
	mProgram(0)
{
}


ShaderData::~ShaderData()
{
	unload();
}


void ShaderData::unload()
{
	if( !loaded() )
		return;
	qDebug() << "-" << this << "ShaderData" << uid();

	delete mProgram;
	mProgram = 0;

	AResourceData::unload();
}


bool ShaderData::load()
{
	unload();
	qDebug() << "+" << this << "ShaderData" << uid();

	mProgram = new QGLShaderProgram( mGLWidget );
	mProgram->addShaderFromSourceFile( QGLShader::Vertex, "./data/shader/"+mName+".vert" );
	mProgram->addShaderFromSourceFile( QGLShader::Fragment, "./data/shader/"+mName+".frag" );
	if( !mProgram->link() )
	{
		qWarning() << mProgram->log();
		return false;
	}

	return AResourceData::load();
}


Shader::Shader( GLWidget * glWidget, QString name ) : AResource()
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
}


void Shader::release()
{
	data()->program()->release();
}
