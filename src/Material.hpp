#ifndef MATERIAL_INCLUDED
#define MATERIAL_INCLUDED


#include <QtOpenGL>
#include <QDebug>
#include "Resource.hpp"
#include "Shader.hpp"


class GLWidget;


class MaterialData : public ResourceData
{
public:
	MaterialData( GLWidget * glWidget, QString name );
	virtual ~MaterialData();
	bool load();

	const QString & name() const { return mName; }
	const QVector4D & ambient() const { return mAmbient; }
	const QVector4D & diffuse() const { return mDiffuse; }
	const QVector4D & specular() const { return mSpecular; }
	const QVector4D & emission() const { return mEmission; }
	const GLfloat & shininess() const { return mShininess; }
	const GLuint & diffuseMap() const { return mDiffuseMap; }
	const GLuint & specularMap() const { return mSpecularMap; }
	const GLuint & normalMap() const { return mNormalMap; }
	const QString & defaultShaderName() const { return mDefaultShaderName; }

private:
	GLWidget * mGLWidget;
	QString mName;
	QString mDefaultShaderName;

	QVector4D mAmbient;
	QVector4D mDiffuse;
	QVector4D mSpecular;
	QVector4D mEmission;
	GLfloat mShininess;
	GLuint mDiffuseMap;
	GLuint mSpecularMap;
	GLuint mNormalMap;
};


class Material : public Resource<MaterialData>
{
private:
	GLWidget * mGLWidget;
	Shader * mShader;
	GLuint mMaskMap;
	GLuint mCubeMap;

public:
	Material( GLWidget * glWidget, QString name );
	virtual ~Material();
	
	void setShader( QString shaderName );
	void setDefaultShader();
	void setMaskMap( QString path );
	void setMaskMap( GLuint maskMap ) { mMaskMap = maskMap; }
	void setCubeMap( GLuint cubeMap ) { mCubeMap = cubeMap; }
	void bind();
	void release();
};


#endif
