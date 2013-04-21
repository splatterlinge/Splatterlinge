#ifndef SHADER_INCLUDED
#define SHADER_INCLUDED


#include <QtOpenGL>
#include <QDebug>

#include "Resource.hpp"


class ShaderData : public ResourceData
{
public:
	ShaderData( QGLWidget * glWidget, QString name );
	virtual ~ShaderData();
	bool load();
	
	QGLShaderProgram * shader() { return mShader; }
	int uniform_diffuseMap() const { return mUniform_diffuseMap; }
	int uniform_specularMap() const { return mUniform_specularMap; }
	int uniform_normalMap() const { return mUniform_normalMap; }

private:
	QGLWidget * mGLWidget;
	QString mName;

	QGLShaderProgram * mShader;
	int mUniform_diffuseMap;
	int mUniform_specularMap;
	int mUniform_normalMap;
};


class Shader : public Resource<ShaderData>
{
public:
	Shader( QGLWidget * glWidget, QString name );
	virtual ~Shader();
	
	void bind();
	void release();
};


#endif
