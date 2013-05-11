#ifndef RESOURCE_SHADER_INCLUDED
#define RESOURCE_SHADER_INCLUDED


#include "AResource.hpp"

#include <QtOpenGL>
#include <QDebug>


class GLWidget;


class ShaderData : public AResourceData
{
public:
	ShaderData( GLWidget * glWidget, QString name );
	virtual ~ShaderData();
	bool load();
	
	QGLShaderProgram * program() { return mProgram; }
	const QGLShaderProgram * program() const { return mProgram; }

	const QString & name() const { return mName; }

private:
	GLWidget * mGLWidget;
	QString mName;

	QGLShaderProgram * mProgram;
};


class Shader : public AResource<ShaderData>
{
public:
	Shader( GLWidget * glWidget, QString name );
	virtual ~Shader();

	QGLShaderProgram * program() { return data()->program(); }

	void bind();
	void release();
};


#endif
