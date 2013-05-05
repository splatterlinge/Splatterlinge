#ifndef SHADER_INCLUDED
#define SHADER_INCLUDED


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
	GLWidget * mGLWidget;
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


class Shader : public AResource<ShaderData>
{
public:
	Shader( GLWidget * glWidget, QString name );
	virtual ~Shader();

	bool hasDiffuseMap() const { return constData()->uniform_diffuseMap() >= 0; }
	bool hasSpecularMap() const { return constData()->uniform_specularMap() >= 0; }
	bool hasNormalMap() const { return constData()->uniform_normalMap() >= 0; }
	bool hasMaskMap() const { return constData()->uniform_maskMap() >= 0; }
	bool hasCubeMap() const { return constData()->uniform_cubeMap() >= 0; }

	int texUnit_diffuseMap() const { return constData()->texUnit_diffuseMap(); }
	int texUnit_specularMap() const { return constData()->texUnit_specularMap(); }
	int texUnit_normalMap() const { return constData()->texUnit_normalMap(); }
	int texUnit_maskMap() const { return constData()->texUnit_maskMap(); }
	int texUnit_cubeMap() const { return constData()->texUnit_cubeMap(); }

	QGLShaderProgram * program() { return data()->program(); }

	void bind();
	void release();
};


#endif
