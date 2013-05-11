#include "Shader.hpp"

#include <GLWidget.hpp>

#include <QDebug>
#include <GL/glu.h>


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
