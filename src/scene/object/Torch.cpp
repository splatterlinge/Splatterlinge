#include "Torch.hpp"
#include "World.hpp"

#include <utility/occlusion.hpp>
#include <scene/Scene.hpp>


Torch::Torch( Scene * scene, World * world ) :
	AObject( scene, 1.0f ),
	mWorld( world )
{
	mFlareSize = 5.0f;
	mColorCycle = 0.0f;

	QImage flareImage( "./data/effect/flare.png" );
	if( flareImage.isNull() )
	{
		qFatal( "\"%s\" not found!", "./data/effect/flare.png" );
	}
	mFlareMap = scene->glWidget()->bindTexture( flareImage );
	mWorld->addLightSource( this );
}


Torch::~Torch()
{
	mWorld->removeLightSource( this );
}


void Torch::updateLightSource( GLenum light )
{
	glLight( light, GL_POSITION, QVector4D(	position(), 1	) );
	glLight( light, GL_AMBIENT, QVector4D(	0, 0, 0, 1	) );
	glLight( light, GL_DIFFUSE, QVector4D(	color()	) );
	glLight( light, GL_SPECULAR, QVector4D(	color()	) );
	glLight( light, GL_CONSTANT_ATTENUATION, 0.0f );
	glLight( light, GL_LINEAR_ATTENUATION, 0.05f );
	glLight( light, GL_QUADRATIC_ATTENUATION, 0.0f );
	glEnable( light );
}


void Torch::updateSelf( const double & delta )
{
	QColor color = QColor::fromHsvF( mColorCycle, 0.5f, 1.0f, 1.0f );
	mColorCycle += 0.0005f;
	if( mColorCycle >= 1.0f )
		mColorCycle -= 1.0f;
	mColor = QVector4D( color.redF(), color.greenF(), color.blueF(), 1.0f );
/*
	World * world = dynamic_cast<World*>(scene()->root());
	if( world )
	{
		setRotation( QQuaternion::nlerp( rotation(), world->landscape()->terrain()->getNormalRotation(position()), 33.0*delta ) );
	}
*/
}


void Torch::drawSelf()
{
}


void Torch::draw2Self()
{
	if( occlusionTest() )
		return;

	glPushAttrib( GL_VIEWPORT_BIT | GL_DEPTH_BUFFER_BIT );
	glDepthMask( GL_FALSE );
	glDisable( GL_CULL_FACE );
	glDisable( GL_DEPTH_TEST );
	
	glDisable( GL_LIGHTING );
	glEnable( GL_BLEND );
	glBlendFunc( GL_ONE, GL_ONE );
	glActiveTexture( GL_TEXTURE0 );
	glEnable( GL_TEXTURE_2D );
	glBindTexture( GL_TEXTURE_2D, mFlareMap );
	glColor( mColor );

	float m[16];
	glGetFloatv( GL_MODELVIEW_MATRIX, m );
	QVector3D up( m[1], m[5], m[9] );
	QVector3D right( m[0], m[4], m[8] );

	struct _vertices
	{
		QVector3D position;
		QVector2D texCoord;
	};
	struct _vertices vertices[4];
	vertices[0].position = (-right+up) * mFlareSize;
	vertices[0].texCoord = QVector2D(0,0);
	vertices[1].position = ( right+up) * mFlareSize;
	vertices[1].texCoord = QVector2D(1,0);
	vertices[2].position = ( right-up) * mFlareSize;
	vertices[2].texCoord = QVector2D(1,1);
	vertices[3].position = (-right-up) * mFlareSize;
	vertices[3].texCoord = QVector2D(0,1);

	glBindBuffer( GL_ARRAY_BUFFER, 0 );
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );
	glEnableClientState( GL_VERTEX_ARRAY );
	glClientActiveTexture( GL_TEXTURE0 );
	glEnableClientState( GL_TEXTURE_COORD_ARRAY );
	glVertexPointer( 3, GL_FLOAT, sizeof(struct _vertices), &(vertices[0].position) );
	glTexCoordPointer( 2, GL_FLOAT, sizeof(struct _vertices), &(vertices[0].texCoord) );
	glDrawArrays( GL_QUADS, 0, 4 );

	glDisable( GL_BLEND );
	
	glPopAttrib();
}
