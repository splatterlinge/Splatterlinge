#ifndef MATERIAL_INCLUDED
#define MATERIAL_INCLUDED


#include <QtOpenGL>
#include <QDebug>
#include "Resource.hpp"

#include "Shader.hpp"


class MaterialData : public ResourceData
{
public:
	MaterialData( QGLWidget * glWidget, QString name );
	virtual ~MaterialData();
	bool load();

	const QVector4D & ambient() const { return mAmbient; }
	const QVector4D & diffuse() const { return mDiffuse; }
	const QVector4D & specular() const { return mSpecular; }
	const QVector4D & emission() const { return mEmission; }
	const GLfloat & shininess() const { return mShininess; }
	const GLuint & diffuseMap() const { return mDiffuseMap; }
	const GLuint & specularMap() const { return mSpecularMap; }
	const GLuint & normalMap() const { return mNormalMap; }
	Shader * shader() { return mShader; }

private:
	QGLWidget * mGLWidget;
	QString mName;

	QVector4D mAmbient;
	QVector4D mDiffuse;
	QVector4D mSpecular;
	QVector4D mEmission;
	GLfloat mShininess;
	GLuint mDiffuseMap;
	GLuint mSpecularMap;
	GLuint mNormalMap;
	Shader * mShader;
};


class Material : public Resource<MaterialData>
{
public:
	Material( QGLWidget * glWidget, QString name );
	virtual ~Material();
	
	void bind();
	void release();
};


#endif
