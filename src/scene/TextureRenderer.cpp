/*
 * Copyright (C) 2013
 * Branimir Djordjevic <branimir.djordjevic@gmail.com>
 * Tobias Himmer <provisorisch@online.de>
 * Michael Wydler <michael.wydler@gmail.com>
 * Karl-Heinz Zimmermann <karlzimmermann3787@gmail.com>
 *
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

#include "TextureRenderer.hpp"


#include <QDebug>


TextureRenderer::TextureRenderer( GLWidget * glWidget, const QSize & size, bool depthBuffer ) :
	mLastFrameBuffer( 0 ),
	mLastRenderBuffer( 0 ),
	mFrameBuffer( 0 ),
	mDepthBuffer( 0 ),
	mTex( 0 ),
	mDepth( 0 ),
	mGLWidget( glWidget ),
	mHasDepthBuffer( depthBuffer ),
	mSize( size )
{
	glGenTextures( 1, &mTex );
	glBindTexture( GL_TEXTURE_2D, mTex );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexImage2D( GL_TEXTURE_2D, 0, 3, mSize.width(), mSize.height(), 0, GL_RGB, GL_UNSIGNED_BYTE, NULL );

	glGenFramebuffers( 1, &mFrameBuffer );
	glBindFramebuffer( GL_FRAMEBUFFER, mFrameBuffer );
	glFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mTex, 0 );

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
	glBindTexture( GL_TEXTURE_2D, 0 );
}


TextureRenderer::~TextureRenderer()
{
	if( mFrameBuffer )
		glDeleteFramebuffers( 1,&mFrameBuffer );
	if( mDepthBuffer )
		glDeleteRenderbuffers( 1,&mDepthBuffer );
	if( mTex )
		glDeleteTextures( 1, &mTex );
	if( mDepth )
		glDeleteTextures( 1, &mDepth );
}


void TextureRenderer::bind()
{
	glGetIntegerv( GL_FRAMEBUFFER_BINDING, (GLint*)&mLastFrameBuffer );
	glGetIntegerv( GL_RENDERBUFFER_BINDING, (GLint*)&mLastRenderBuffer );
	glBindFramebuffer( GL_FRAMEBUFFER, mFrameBuffer );
	glBindRenderbuffer( GL_RENDERBUFFER, mDepthBuffer );
	glPushAttrib( GL_VIEWPORT_BIT );
	glViewport( 0, 0, size().width(), size().height() );
}


void TextureRenderer::release()
{
	glPopAttrib();
	glBindFramebuffer( GL_FRAMEBUFFER, mLastFrameBuffer );
	glBindRenderbuffer( GL_RENDERBUFFER, mLastRenderBuffer );
}
