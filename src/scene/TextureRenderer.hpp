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

#ifndef SCENE_TEXTURERENDERER_INCLUDED
#define SCENE_TEXTURERENDERER_INCLUDED


#include <GLWidget.hpp>


/// FBO texture renderer.
class TextureRenderer
{
	private:
		GLuint mLastFrameBuffer;
		GLuint mLastRenderBuffer;
		GLuint mFrameBuffer;
		GLuint mDepthBuffer;
		GLuint mTex;
		GLuint mDepth;
		GLWidget * mGLWidget;
		bool mHasDepthBuffer;
		QSize mSize;

	public:
		TextureRenderer( GLWidget * glWidget, const QSize & size, bool depthBuffer );
		~TextureRenderer();

		void bind();
		void release();

		GLuint texID() const { return mTex; }
		GLuint depthID() const { return mDepth; }
		bool hasDepthBuffer() const { return mHasDepthBuffer; }
		const QSize & size() const { return mSize; }
};


#endif
