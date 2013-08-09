/*
 * This file is part of Splatterlinge.
 *
 * Splatterlinge is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * Splatterlinge is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Splatterlinge. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef RESOURCE_MATERIAL_INCLUDED
#define RESOURCE_MATERIAL_INCLUDED

#include "AResource.hpp"

#include <GLWidget.hpp>

#include <QVector4D>


class GLWidget;
class Shader;


/// Material's quality settings
class MaterialQuality
{
	MaterialQuality() {}
	~MaterialQuality() {}
public:
	enum Type
	{
		LOW	= 0,
		MEDIUM	= 1,
		HIGH	= 2
	};
	const static int num = 3;

	static Type fromString( const QString & name );
	static QString toString( const Type & quality );
	static const Type & maximum() { return sMaximum; }
	static void setMaximum( const Type & max ) { sMaximum = max; }

private:
	static Type sMaximum;
};


/// Material's shader variants
namespace MaterialShaderVariant
{
	enum Type
	{
		DEFAULT		= 0,
		BLOBBING	= 1
	};
	const static int num = 2;
};


/// Material's data
class MaterialData : public AResourceData
{
public:
	MaterialData( GLWidget * glWidget, QString name );
	virtual ~MaterialData();

	const QString & name() const { return mName; }
	const QVector4D & ambient() const { return mAmbient; }
	const QVector4D & diffuse() const { return mDiffuse; }
	const QVector4D & specular() const { return mSpecular; }
	const QVector4D & emission() const { return mEmission; }
	const GLfloat & shininess() const { return mShininess; }
	const QMap<QString,GLuint> & textures() const { return mTextures; }
	const QMap<QString,GLfloat> & constants() const { return mConstants; }
	const QString shaderName( MaterialQuality::Type quality ) const { return mShaderNames[quality]; }

	const GLclampf & alphaTestReferenceValue() const { return mAlphaTestReferenceValue; }
	const GLenum & alphaTestFunction() const { return mAlphaTestFunction; }
	const bool & alphaTestEnabled() const { return mAlphaTestEnabled; }

	// Overrides:
	virtual bool load();
	virtual void unload();

private:
	GLWidget * mGLWidget;
	QString mName;

	QString mShaderNames[MaterialQuality::num];

	QVector4D mAmbient;
	QVector4D mDiffuse;
	QVector4D mSpecular;
	QVector4D mEmission;
	GLfloat mShininess;
	QMap<QString,GLuint> mTextures;
	QMap<QString,GLfloat> mConstants;
	bool mAlphaTestEnabled;
	GLclampf mAlphaTestReferenceValue;
	GLenum mAlphaTestFunction;
};


/// A material
class Material : public AResource<MaterialData>
{
public:
	Material( GLWidget * glWidget, QString name, MaterialShaderVariant::Type variant = MaterialShaderVariant::DEFAULT );
	virtual ~Material();

	void setShader( MaterialShaderVariant::Type variant );

	void overrideAmbient( const QVector4D & ambient ) { glMaterial( GL_FRONT_AND_BACK, GL_AMBIENT, ambient ); }
	void overrideDiffuse( const QVector4D & diffuse ) { glMaterial( GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse ); }
	void overrideSpecular( const QVector4D & specular ) { glMaterial( GL_FRONT_AND_BACK, GL_SPECULAR, specular ); }
	void overrideEmission( const QVector4D & emission ) { glMaterial( GL_FRONT_AND_BACK, GL_EMISSION, emission ); }
	void overrideAlphaTestReferenceValue( const float & referenceValue ) { glAlphaFunc( data()->alphaTestFunction(), referenceValue ); }

	void setBlobMap( GLuint blobMap ) { mBlobMap = blobMap; }
	void setCubeMap( GLuint cubeMap ) { mCubeMap = cubeMap; }

	void bind();
	void release();

	void setDefaultQuality( MaterialQuality::Type q ) { mDefaultQuality = q; }

	static float filterAnisotropyMaximum() { GLfloat maxAnisotropy; glGetFloatv( GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAnisotropy ); return maxAnisotropy; }
	static void setFilterAnisotropy( float maxAnisotropy );
	static float filterAnisotropy() { return sFilterAnisotropy; }

private:
	typedef struct
	{
		Shader * shader;
		QVector< QPair<int,GLuint> > textureUnits;
		QVector< QPair<int,GLfloat> > constants;
		int blobMapUniform;
		int cubeMapUniform;
	} ShaderSet;

	GLWidget * mGLWidget;
	QString mName;

	float mOpacity;

	GLuint mBlobMap;
	GLuint mCubeMap;

	GLclampf mAlphaTestReferenceValueOverride;
	const GLclampf * mUsedAlphaTestReferenceValue;

	ShaderSet mShaderSet[MaterialQuality::num];
	MaterialQuality::Type mDefaultQuality;
	MaterialQuality::Type mBoundQuality;

	MaterialQuality::Type getBindingQuality();
	void setShader( MaterialQuality::Type quality, QString shaderFullName );

	static float sFilterAnisotropy;
};


#endif
