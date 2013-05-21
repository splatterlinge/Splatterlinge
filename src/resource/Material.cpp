#include "Material.hpp"

#include "Shader.hpp"

#include <QSettings>
#include <QGLShaderProgram>

#include <unistd.h>


RESOURCE_CACHE(MaterialData);


MaterialData::MaterialData( GLWidget * glWidget, QString name ) :
	AResourceData( name ),
	mGLWidget(glWidget),
	mName(name)
{
}


MaterialData::~MaterialData()
{
	if( loaded() )
		qDebug() << "-" << this << "MaterialData" << uid();
}


bool MaterialData::load()
{
	qDebug() << "+" << this << "MaterialData" << uid();

	QSettings s( "./data/material/"+mName+"/material.ini", QSettings::IniFormat );

	s.beginGroup( "Textures" );
	{
		QStringList textures = s.allKeys();
		QStringList::const_iterator i;
		for( i = textures.constBegin(); i != textures.constEnd(); ++i )
		{
			QString mapPath = "./data/material/"+mName+"/" + s.value( (*i) ).toString();
			QPixmap map = QPixmap( mapPath );
			if( map.isNull() )
			{
				qFatal(
					"Texture \"%s\" from file \"%s\" could not be loaded!",
					(*i).toLocal8Bit().constData(),
					mapPath.toLocal8Bit().constData()
				);
			}
			GLuint texture =  mGLWidget->bindTexture( map, GL_TEXTURE_2D, GL_RGBA, QGLContext::MipmapBindOption | QGLContext::LinearFilteringBindOption );
			mTextures[(*i)] = texture;
		}
	}
	s.endGroup();

	s.beginGroup( "Constants" );
	{
		QStringList constants = s.allKeys();
		QStringList::const_iterator i;
		for( i = constants.constBegin(); i != constants.constEnd(); ++i )
		{
			mConstants[(*i)] = s.value( (*i) ).toFloat();
		}
	}
	s.endGroup();

	s.beginGroup( "Model" );
	{
		mAmbient = QVector4D(
			s.value( "ambientRed", 0.2f ).toFloat(),
			s.value( "ambientGreen", 0.2f ).toFloat(),
			s.value( "ambientBlue", 0.2f ).toFloat(),
			s.value( "ambientAlpha", 1.0f ).toFloat()
		);
		mDiffuse = QVector4D(
			s.value( "diffuseRed", 0.8f ).toFloat(),
			s.value( "diffuseGreen", 0.8f ).toFloat(),
			s.value( "diffuseBlue", 0.8f ).toFloat(),
			s.value( "diffuseAlpha", 1.0f ).toFloat()
		);
		mSpecular = QVector4D(
			s.value( "specularRed", 0.0f ).toFloat(),
			s.value( "specularGreen", 0.0f ).toFloat(),
			s.value( "specularBlue", 0.0f ).toFloat(),
			s.value( "specularAlpha", 1.0f ).toFloat()
		);
		mEmission = QVector4D(
			s.value( "emissionRed", 0.0f ).toFloat(),
			s.value( "emissionGreen", 0.0f ).toFloat(),
			s.value( "emissionBlue", 0.0f ).toFloat(),
			s.value( "emissionAlpha", 1.0f ).toFloat()
		);
		mShininess = s.value( "shininess", 0 ).toFloat();
	}
	s.endGroup();

	s.beginGroup( "Shader" );
	{
		mDefaultShaderName = s.value( "defaultShader", "versatile" ).toString();
		mBlobbingShaderName = s.value( "blobbingShader", "versatileBlob" ).toString();
	}
	s.endGroup();

	return AResourceData::load();
}


Material::Quality Material::sGlobalMaxQuality = Material::HIGH_QUALITY;


Material::Material( GLWidget * glWidget, QString name, ShaderType type ) : AResource()
{
	mGLWidget = glWidget;
	mName = name;
	mDefaultQuality = sGlobalMaxQuality;
	mShaderSet[LOW_QUALITY].textureUnits.clear();
	mShaderSet[LOW_QUALITY].shader = 0;
	mShaderSet[LOW_QUALITY].blobMapUniform = -1;
	mShaderSet[LOW_QUALITY].cubeMapUniform = -1;
	mShaderSet[MEDIUM_QUALITY].textureUnits.clear();
	mShaderSet[MEDIUM_QUALITY].shader = 0;
	mShaderSet[MEDIUM_QUALITY].blobMapUniform = -1;
	mShaderSet[MEDIUM_QUALITY].cubeMapUniform = -1;
	mShaderSet[HIGH_QUALITY].textureUnits.clear();
	mShaderSet[HIGH_QUALITY].shader = 0;
	mShaderSet[HIGH_QUALITY].blobMapUniform = -1;
	mShaderSet[HIGH_QUALITY].cubeMapUniform = -1;
	mBlobMap = mCubeMap = -1;

	QSharedPointer<MaterialData> n( new MaterialData( glWidget, name ) );
	cache( n );

	setShader( type );
}


Material::~Material()
{
	delete mShaderSet[HIGH_QUALITY].shader;
	delete mShaderSet[MEDIUM_QUALITY].shader;
	delete mShaderSet[LOW_QUALITY].shader;
}


Material::Quality Material::getBindingQuality()
{
	Quality q = mDefaultQuality;
	if( mDefaultQuality > sGlobalMaxQuality )
		q = sGlobalMaxQuality;

	if( !mShaderSet[q].shader && q==HIGH_QUALITY )
		q = MEDIUM_QUALITY;
	if( !mShaderSet[q].shader && q==MEDIUM_QUALITY )
		q = LOW_QUALITY;

	return q;
}


void Material::bind()
{
	mBoundQuality = getBindingQuality();

	if( !mShaderSet[mBoundQuality].shader )
		return;
	
	mShaderSet[mBoundQuality].shader->bind();

	glPushAttrib( GL_LIGHTING_BIT | GL_ENABLE_BIT );

	glMaterial( GL_FRONT_AND_BACK, GL_AMBIENT, data()->ambient() );
	glMaterial( GL_FRONT_AND_BACK, GL_DIFFUSE, data()->diffuse() );
	glMaterial( GL_FRONT_AND_BACK, GL_SPECULAR, data()->specular() );
	glMaterial( GL_FRONT_AND_BACK, GL_EMISSION, data()->emission() );
	glMaterial( GL_FRONT_AND_BACK, GL_SHININESS, data()->shininess() );

	for( int i = 0; i < mShaderSet[mBoundQuality].constants.size(); i++ )
	{
		mShaderSet[mBoundQuality].shader->program()->setUniformValue(
			mShaderSet[mBoundQuality].constants[i].first,
			mShaderSet[mBoundQuality].constants[i].second
		);
	}

	int texUnit;
	for( texUnit = 0; texUnit < mShaderSet[mBoundQuality].textureUnits.size(); texUnit++ )
	{
		glActiveTexture( GL_TEXTURE0 + texUnit );
		glBindTexture( GL_TEXTURE_2D, mShaderSet[mBoundQuality].textureUnits[texUnit].second );
		mShaderSet[mBoundQuality].shader->program()->setUniformValue( mShaderSet[mBoundQuality].textureUnits[texUnit].first, texUnit );
	}

	if( mBlobMap >= 0 && mShaderSet[mBoundQuality].blobMapUniform >= 0 )
	{
		glActiveTexture( GL_TEXTURE0 + texUnit );
		glBindTexture( GL_TEXTURE_2D, mBlobMap );
		mShaderSet[mBoundQuality].shader->program()->setUniformValue( mShaderSet[mBoundQuality].blobMapUniform, texUnit );
		texUnit++;
	}

	if( mCubeMap >= 0 && mShaderSet[mBoundQuality].cubeMapUniform >= 0 )
	{
		glActiveTexture( GL_TEXTURE0 + texUnit );
		glBindTexture( GL_TEXTURE_2D, mCubeMap );
		mShaderSet[mBoundQuality].shader->program()->setUniformValue( mShaderSet[mBoundQuality].blobMapUniform, texUnit );
		texUnit++;
	}

}


void Material::release()
{
	if( !mShaderSet[mBoundQuality].shader )
		return;

	glPopAttrib();

	mShaderSet[mBoundQuality].shader->release();
}


void Material::setShader( Quality quality, QString shaderFullName )
{
	delete mShaderSet[quality].shader;
	mShaderSet[quality].shader = 0;

	if( access( QString("./data/shader/"+shaderFullName+".vert").toLocal8Bit().constData(), R_OK ) != 0 )
		return;

	mShaderSet[quality].shader = new Shader( mGLWidget, shaderFullName );

	mShaderSet[quality].textureUnits.clear();
	{
		QMap<QString, GLuint>::const_iterator i = data()->textures().constBegin();
		while( i != data()->textures().constEnd() )
		{
			const QString & uniformName = i.key();
			const GLuint & texID = i.value();
			int uniform = mShaderSet[quality].shader->program()->uniformLocation( uniformName );
			if( uniform >=0 )
			{
				mShaderSet[quality].textureUnits.push_back( QPair<int,GLuint>( uniform, texID ) );
			}
			++i;
		}
	}

	mShaderSet[quality].constants.clear();
	{
		QMap<QString, GLfloat>::const_iterator i = data()->constants().constBegin();
		while( i != data()->constants().constEnd() )
		{
			const QString & uniformName = i.key();
			const GLfloat & value = i.value();
			int uniform = mShaderSet[quality].shader->program()->uniformLocation( uniformName );
			if( uniform >=0 )
			{
				mShaderSet[quality].constants.push_back( QPair<int,GLfloat>( uniform, value ) );
			}
			++i;
		}
	}

	mShaderSet[quality].blobMapUniform = mShaderSet[quality].shader->program()->uniformLocation( "blobMap" );
	mShaderSet[quality].cubeMapUniform = mShaderSet[quality].shader->program()->uniformLocation( "cubeMap" );
}


void Material::setShader( QString shaderName )
{
	setShader( LOW_QUALITY, shaderName+".low" );
	setShader( MEDIUM_QUALITY, shaderName+".medium" );
	setShader( HIGH_QUALITY, shaderName+".high" );
}


void Material::setShader( ShaderType type )
{
	switch( type )
	{
		case BLOBBING_SHADERTYPE:
			setShader( data()->blobbingShaderName() );
			break;
		default:
		case DEFAULT_SHADERTYPE:
			setShader( data()->defaultShaderName() );
			break;
	}
}
