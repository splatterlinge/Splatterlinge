#ifndef RESOURCE_SHADER_INCLUDED
#define RESOURCE_SHADER_INCLUDED


#include "AResource.hpp"

#include <GLWidget.hpp>

#include <QDebug>


class QGLShaderProgram;


class ShaderData : public AResourceData
{
public:
	ShaderData( GLWidget * glWidget, QString name );
	virtual ~ShaderData();
	virtual bool load();
	virtual void unload();
	
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
