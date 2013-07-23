#include "Torch.hpp"
#include "World.hpp"

#include <utility/occlusion.hpp>


Torch::Torch( World * world ) :
	AWorldObject( world, 1.0f )
{
	mFlareSize = 5.0f;
	mColorCycle = 0.0f;
	mFlareRotation = 0.0f;

	QImage flareImage( "./data/effect/flare.png" );
	if( flareImage.isNull() )
	{
		qFatal( "\"%s\" not found!", "./data/effect/flare.png" );
	}
	mFlareMap = scene()->glWidget()->bindTexture( flareImage );
	world->addLightSource( this );
}


Torch::~Torch()
{
	world()->removeLightSource( this );
}


void Torch::updateLightSource( GLenum light )
{
	glLight( light, GL_POSITION, QVector4D(	worldPosition(), 1	) );
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

	mFlareRotation += 20.0f * delta;
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
	glEnable( GL_TEXTURE_2D );
	glBindTexture( GL_TEXTURE_2D, mFlareMap );

	QVector3D dir( eyeDirection() );
	QVector3D up( eyeUp() );
	QVector3D left( eyeLeft() );

	struct _vertices
	{
		QVector3D position;
		QVector2D texCoord;
	};
	struct _vertices vertices[4];
	vertices[0].position = (-left+up) * mFlareSize;
	vertices[0].texCoord = QVector2D(0,0);
	vertices[1].position = ( left+up) * mFlareSize;
	vertices[1].texCoord = QVector2D(1,0);
	vertices[2].position = ( left-up) * mFlareSize;
	vertices[2].texCoord = QVector2D(1,1);
	vertices[3].position = (-left-up) * mFlareSize;
	vertices[3].texCoord = QVector2D(0,1);

	glBindBuffer( GL_ARRAY_BUFFER, 0 );
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );

	glClientActiveTexture( GL_TEXTURE0 );

	glEnableClientState( GL_VERTEX_ARRAY );
	glEnableClientState( GL_TEXTURE_COORD_ARRAY );

	glVertexPointer( 3, GL_FLOAT, sizeof(struct _vertices), &(vertices[0].position) );
	glTexCoordPointer( 2, GL_FLOAT, sizeof(struct _vertices), &(vertices[0].texCoord) );

	glPushMatrix();
	glColor( mColor/2.0f );
	glRotate( mFlareRotation, dir );
	glDrawArrays( GL_QUADS, 0, 4 );
	glRotate( -mFlareRotation*2.7f, dir );
	glDrawArrays( GL_QUADS, 0, 4 );
	glPopMatrix();

	glDisableClientState( GL_TEXTURE_COORD_ARRAY );
	glDisableClientState( GL_VERTEX_ARRAY );

	glDisable( GL_BLEND );

	glPopAttrib();
}
