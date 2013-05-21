#ifndef RESOURCE_MATERIAL_INCLUDED
#define RESOURCE_MATERIAL_INCLUDED

#include "AResource.hpp"

#include <GLWidget.hpp>

#include <QVector4D>
#include <QDebug>


class GLWidget;
class Shader;

class MaterialData : public AResourceData
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
	const QMap<QString,GLuint> & textures() const { return mTextures; }
	const QMap<QString,GLfloat> & constants() const { return mConstants; }
	const QString & defaultShaderName() const { return mDefaultShaderName; }
	const QString & blobbingShaderName() const { return mBlobbingShaderName; }

private:
	GLWidget * mGLWidget;
	QString mName;
	QString mDefaultShaderName;
	QString mBlobbingShaderName;

	QVector4D mAmbient;
	QVector4D mDiffuse;
	QVector4D mSpecular;
	QVector4D mEmission;
	GLfloat mShininess;
	QMap<QString,GLuint> mTextures;
	QMap<QString,GLfloat> mConstants;
};


class Material : public AResource<MaterialData>
{
public:
	typedef enum { LOW_QUALITY=0, MEDIUM_QUALITY=1, HIGH_QUALITY=2 } Quality;
	typedef enum { DEFAULT_SHADERTYPE, BLOBBING_SHADERTYPE } ShaderType;
	static void setGlobalMaxQuality( Quality q ) { sGlobalMaxQuality = q; }
	static Quality globalMaxQuality() { return sGlobalMaxQuality; }

	Material( GLWidget * glWidget, QString name, ShaderType type = DEFAULT_SHADERTYPE );
	virtual ~Material();

	void setShader( QString shaderName );
	void setShader( ShaderType type );

	void setBlobMap( GLuint blobMap ) { mBlobMap = blobMap; }
	void setCubeMap( GLuint cubeMap ) { mCubeMap = cubeMap; }

	void bind();
	void release();
	
	void setDefaultQuality( Quality q ) { mDefaultQuality = q; }

private:
	typedef struct
	{
		Shader * shader;
		QVector< QPair<int,GLuint> > textureUnits;
		QVector< QPair<int,GLfloat> > constants;
		int blobMapUniform;
		int cubeMapUniform;
	} ShaderSet;


	static Quality sGlobalMaxQuality;

	GLWidget * mGLWidget;
	QString mName;

	ShaderSet mShaderSet[3];

	GLuint mBlobMap;
	GLuint mCubeMap;

	Quality mDefaultQuality;
	Quality mBoundQuality;

	Quality getBindingQuality();
	void setShader( Quality quality, QString shaderFullName );
};


#endif
