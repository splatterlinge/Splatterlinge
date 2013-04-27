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
	
	QGLShaderProgram * program() { return mProgram; }
	const QGLShaderProgram * program() const { return mProgram; }

	const QString & name() const { return mName; }

	int uniform_diffuseMap() const { return mUniform_diffuseMap; }
	int uniform_specularMap() const { return mUniform_specularMap; }
	int uniform_normalMap() const { return mUniform_normalMap; }
	int uniform_maskMap() const { return mUniform_maskMap; }
	int uniform_cubeMap() const { return mUniform_cubeMap; }

	int texUnit_diffuseMap() const { return mTexUnit_diffuseMap; }
	int texUnit_specularMap() const { return mTexUnit_specularMap; }
	int texUnit_normalMap() const { return mTexUnit_normalMap; }
	int texUnit_maskMap() const { return mTexUnit_maskMap; }
	int texUnit_cubeMap() const { return mTexUnit_cubeMap; }

private:
	QGLWidget * mGLWidget;
	QString mName;

	QGLShaderProgram * mProgram;
	int mUniform_diffuseMap;
	int mUniform_specularMap;
	int mUniform_normalMap;
	int mUniform_maskMap;
	int mUniform_cubeMap;
	int mTexUnit_diffuseMap;
	int mTexUnit_specularMap;
	int mTexUnit_normalMap;
	int mTexUnit_maskMap;
	int mTexUnit_cubeMap;
};


class Shader : public Resource<ShaderData>
{
public:
	Shader( QGLWidget * glWidget, QString name );
	virtual ~Shader();

	bool hasDiffuseMap() const { return data()->uniform_diffuseMap() >= 0; }
	bool hasSpecularMap() const { return data()->uniform_specularMap() >= 0; }
	bool hasNormalMap() const { return data()->uniform_normalMap() >= 0; }
	bool hasMaskMap() const { return data()->uniform_maskMap() >= 0; }
	bool hasCubeMap() const { return data()->uniform_cubeMap() >= 0; }

	int texUnit_diffuseMap() const { return data()->texUnit_diffuseMap(); }
	int texUnit_specularMap() const { return data()->texUnit_specularMap(); }
	int texUnit_normalMap() const { return data()->texUnit_normalMap(); }
	int texUnit_maskMap() const { return data()->texUnit_maskMap(); }
	int texUnit_cubeMap() const { return data()->texUnit_cubeMap(); }

	QGLShaderProgram * program() { return data()->program(); }

	void bind();
	void release();
};


#endif
