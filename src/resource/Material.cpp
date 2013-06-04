#include "Material.hpp"

#include "Shader.hpp"

#include <QSettings>
#include <QGLShaderProgram>

#include <unistd.h>


RESOURCE_CACHE(MaterialData);


GLenum MaterialData::alphaTestFunctionFromString( QString name )
{
	if( name == "GL_NEVER" ) return GL_NEVER;
	else if( name == "GL_LESS" ) return GL_LESS;
	else if( name == "GL_EQUAL" ) return GL_EQUAL;
	else if( name == "GL_LEQUAL" ) return GL_LEQUAL;
	else if( name == "GL_GREATER" ) return GL_GREATER;
	else if( name == "GL_NOTEQUAL" ) return GL_NOTEQUAL;
	else if( name == "GL_GEQUAL" ) return GL_GEQUAL;
	else if( name == "GL_ALWAYS" ) return GL_ALWAYS;
	else return GL_ALWAYS;
}


MaterialData::MaterialData( GLWidget * glWidget, QString name ) :
	AResourceData( name ),
	mGLWidget(glWidget),
	mName(name)
{
}


MaterialData::~MaterialData()
{
	unload();
}


void MaterialData::unload()
{
	if( !loaded() )
		return;
	qDebug() << "-" << this << "MaterialData" << uid();

	QMap<QString, GLuint>::const_iterator i = mTextures.constBegin();
	while( i != mTextures.constEnd() )
	{
		mGLWidget->deleteTexture( i.value() );
		++i;
	}
	mTextures.clear();
	mConstants.clear();
	AResourceData::unload();
}


bool MaterialData::load()
{
	unload();
	qDebug() << "+" << this << "MaterialData" << uid();

	QSettings s( "./data/material/"+mName+"/material.ini", QSettings::IniFormat );

	s.beginGroup( "Textures" );
	{
		QStringList textures = s.allKeys();
		for( QStringList::const_iterator i = textures.constBegin(); i != textures.constEnd(); ++i )
		{
			QString mapPath = "./data/material/"+mName+"/" + s.value( (*i) ).toString();
			QImage map = QImage( mapPath );
			if( map.isNull() )
			{
				qFatal(
					"Texture \"%s\" from file \"%s\" could not be loaded!",
					(*i).toLocal8Bit().constData(),
					mapPath.toLocal8Bit().constData()
				);
			}
			GLuint texture =  mGLWidget->bindTexture( map );
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

	s.beginGroup( "AlphaTest" );
	{
		mAlphaTestEnabled = s.value( "enable", false ).toBool();
		mAlphaTestFunction = alphaTestFunctionFromString( s.value( "function", "GL_ALWAYS" ).toString() );
		mAlphaTestReferenceValue = s.value( "referenceValue", 0.0f ).toFloat();
	}
	s.endGroup();

	s.beginGroup( "Shader" );
	{
		mShaderNames[MaterialQuality::LOW] = s.value( "low", "simple" ).toString();
		mShaderNames[MaterialQuality::MEDIUM] = s.value( "medium", "simple" ).toString();
		mShaderNames[MaterialQuality::HIGH] = s.value( "high", "simple" ).toString();
	}
	s.endGroup();

	return AResourceData::load();
}


MaterialQuality::type MaterialQuality::sMaximum = MaterialQuality::HIGH;


Material::Material( GLWidget * glWidget, QString name, MaterialShaderVariant::type type ) : AResource()
{
	mGLWidget = glWidget;
	mName = name;
	mDefaultQuality = MaterialQuality::maximum();
	mShaderSet[MaterialQuality::LOW].textureUnits.clear();
	mShaderSet[MaterialQuality::LOW].shader = 0;
	mShaderSet[MaterialQuality::LOW].blobMapUniform = -1;
	mShaderSet[MaterialQuality::LOW].cubeMapUniform = -1;
	mShaderSet[MaterialQuality::MEDIUM].textureUnits.clear();
	mShaderSet[MaterialQuality::MEDIUM].shader = 0;
	mShaderSet[MaterialQuality::MEDIUM].blobMapUniform = -1;
	mShaderSet[MaterialQuality::MEDIUM].cubeMapUniform = -1;
	mShaderSet[MaterialQuality::HIGH].textureUnits.clear();
	mShaderSet[MaterialQuality::HIGH].shader = 0;
	mShaderSet[MaterialQuality::HIGH].blobMapUniform = -1;
	mShaderSet[MaterialQuality::HIGH].cubeMapUniform = -1;
	mBlobMap = mCubeMap = -1;

	QSharedPointer<MaterialData> n( new MaterialData( glWidget, name ) );
	cache( n );

	setShader( type );
}


Material::~Material()
{
	delete mShaderSet[MaterialQuality::HIGH].shader;
	delete mShaderSet[MaterialQuality::MEDIUM].shader;
	delete mShaderSet[MaterialQuality::LOW].shader;
}


MaterialQuality::type Material::getBindingQuality()
{
	MaterialQuality::type q = mDefaultQuality;
	if( mDefaultQuality > MaterialQuality::maximum() )
		q = MaterialQuality::maximum();

	if( !mShaderSet[q].shader && q==MaterialQuality::HIGH )
		q = MaterialQuality::MEDIUM;
	if( !mShaderSet[q].shader && q==MaterialQuality::MEDIUM )
		q = MaterialQuality::LOW;

	return q;
}


void Material::bind()
{
	mBoundQuality = getBindingQuality();

	if( !mShaderSet[mBoundQuality].shader )
		return;
	
	mShaderSet[mBoundQuality].shader->bind();

	glPushAttrib( GL_LIGHTING_BIT | GL_ENABLE_BIT );
	
	if( data()->alphaTestEnabled() )
	{
		glAlphaFunc( data()->alphaTestFunction(), data()->alphaTestReferenceValue() );
		glEnable( GL_ALPHA_TEST );
	}

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


void Material::setShader( MaterialQuality::type quality, QString shaderFullName )
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


void Material::setShader( MaterialShaderVariant::type variant )
{
	switch( variant )
	{
		case MaterialShaderVariant::BLOBBING:
			setShader( MaterialQuality::LOW, data()->shaderName(MaterialQuality::LOW)+".blobbing" );
			setShader( MaterialQuality::MEDIUM, data()->shaderName(MaterialQuality::MEDIUM)+".blobbing" );
			setShader( MaterialQuality::HIGH, data()->shaderName(MaterialQuality::HIGH)+".blobbing" );
			break;
		default:
		case MaterialShaderVariant::DEFAULT:
			setShader( MaterialQuality::LOW, data()->shaderName(MaterialQuality::LOW)+".default" );
			setShader( MaterialQuality::MEDIUM, data()->shaderName(MaterialQuality::MEDIUM)+".default" );
			setShader( MaterialQuality::HIGH, data()->shaderName(MaterialQuality::HIGH)+".default" );
			break;
	}
}
