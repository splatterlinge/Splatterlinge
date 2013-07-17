#ifndef RESOURCE_SHADER_INCLUDED
#define RESOURCE_SHADER_INCLUDED


#include "AResource.hpp"

#include <GLWidget.hpp>

#include <QDebug>


class QGLShaderProgram;


/// Compiled shader program data
class ShaderData : public AResourceData
{
public:
	ShaderData( GLWidget * glWidget, QString name );
	virtual ~ShaderData();

	QGLShaderProgram * program() { return mProgram; }
	const QGLShaderProgram * program() const { return mProgram; }

	const QString & name() const { return mName; }

	// Overrides:
	virtual bool load();
	virtual void unload();

private:
	GLWidget * mGLWidget;
	QString mName;

	QGLShaderProgram * mProgram;
};


/// Shader program
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
