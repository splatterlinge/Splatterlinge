#include "TextureRenderer.hpp"


#include <QDebug>


TextureRenderer::TextureRenderer( GLWidget * glWidget, const QSize & size, bool depthBuffer ) :
	mFrameBuffer(0),
	mDepthBuffer(0),
	mTex(0),
	mDepth(0),
	mGLWidget(glWidget),
	mHasDepthBuffer(depthBuffer),
	mSize(size)
{
	glGenTextures( 1, &mTex );
	glBindTexture( GL_TEXTURE_2D, mTex );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexImage2D( GL_TEXTURE_2D, 0, 3, mSize.width(), mSize.height(), 0, GL_RGB, GL_UNSIGNED_BYTE, NULL );

	glGenFramebuffers( 1, &mFrameBuffer );
	glBindFramebuffer( GL_FRAMEBUFFER, mFrameBuffer );
	glFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mTex, 0);

	if( mHasDepthBuffer )
	{
		glGenRenderbuffers( 1, &mDepthBuffer );
		glBindRenderbuffer( GL_RENDERBUFFER, mDepthBuffer );
		glRenderbufferStorage( GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, mSize.width(), mSize.height() );
		glFramebufferRenderbuffer( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, mDepthBuffer );

		glGenTextures( 1, &mDepth );
		glBindTexture( GL_TEXTURE_2D, mDepth );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
		glTexImage2D( GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, mSize.width(), mSize.height(), 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, NULL );

		glFramebufferTexture2D( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, mDepth, 0 );
	}

	GLenum status = glCheckFramebufferStatus( GL_FRAMEBUFFER );
	if( status != GL_FRAMEBUFFER_COMPLETE )
	{
		qFatal( "Could not create FBO (%dx%d%s)\n", mSize.width(), mSize.height(), (mHasDepthBuffer?"+depth":"") );
	}
	glBindFramebuffer( GL_FRAMEBUFFER, 0 );
	glBindRenderbuffer( GL_RENDERBUFFER, 0 );
}


TextureRenderer::~TextureRenderer()
{
	if( mFrameBuffer )
		glDeleteFramebuffers( 1,&mFrameBuffer );
	if( mDepthBuffer )
		glDeleteRenderbuffers( 1,&mDepthBuffer );
	if( mTex )
		glDeleteTextures( 1,&mTex );
	if( mDepth )
		glDeleteTextures( 1, &mDepth );
}


void TextureRenderer::bind()
{
	glBindFramebuffer( GL_FRAMEBUFFER, mFrameBuffer );
	glBindRenderbuffer( GL_RENDERBUFFER, mDepthBuffer );
	glPushAttrib( GL_VIEWPORT_BIT );
	glViewport( 0, 0, size().width(), size().width() );
}


void TextureRenderer::release()
{
	glPopAttrib();
	glBindFramebuffer( GL_FRAMEBUFFER, 0 );
	glBindRenderbuffer( GL_RENDERBUFFER, 0 );
}
