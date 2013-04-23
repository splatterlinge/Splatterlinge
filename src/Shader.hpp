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
	const QString & name() const { return mName; }
	int uniform_diffuseMap() const { return mUniform_diffuseMap; }
	int uniform_specularMap() const { return mUniform_specularMap; }
	int uniform_normalMap() const { return mUniform_normalMap; }
	int uniform_maskMap() const { return mUniform_maskMap; }

private:
	QGLWidget * mGLWidget;
	QString mName;

	QGLShaderProgram * mShader;
	int mUniform_diffuseMap;
	int mUniform_specularMap;
	int mUniform_normalMap;
	int mUniform_maskMap;
};


class Shader : public Resource<ShaderData>
{
public:
	Shader( QGLWidget * glWidget, QString name );
	virtual ~Shader();
	
	bool supportsMaskMap() { return data()->uniform_maskMap() >= 0; }
	
	void bind();
	void release();
};


#endif
