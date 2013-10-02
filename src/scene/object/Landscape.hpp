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

#ifndef SCENE_OBJECT_LANDSCAPE_INCLUDED
#define SCENE_OBJECT_LANDSCAPE_INCLUDED

#include "AWorldObject.hpp"
#include <geometry/Terrain.hpp>

#include <QString>
#include <QRect>
#include <QSizeF>
#include <QVector2D>
#include <QDebug>


class GLWidget;
class Shader;
class TextureRenderer;
class Material;


/// A Landscape consisting of terrain and water
/**
 *
 */
class Landscape : public AWorldObject
{
public:
	Landscape( World * world, QString rootDir );
	~Landscape();

	virtual void updateSelf( const double & delta );
	virtual void drawSelf();
	virtual void drawSelfPost();
	virtual void draw2SelfPost();

	virtual const AObject * intersectLine( const AObject * exclude, const QVector3D & origin, const QVector3D & direction,
		float & length, QVector3D * normal = NULL ) const;

	virtual QVector<const AObject*> collideSphere( const AObject * exclude, const float & radius,
		QVector3D & center, QVector3D * normal = NULL ) const;

	void drawPatch( const QRectF & rect );

	Terrain * terrain() { return mTerrain; }
	const Terrain * terrain() const { return mTerrain; }
	const float & waterHeight() const { return mWaterHeight; }
	const bool & drawingReflection() const { return mDrawingReflection; }
	const bool & drawingRefraction() const { return mDrawingRefraction; }

	/// Draws a part of the Terrain using another Material.
	class Blob
	{
		GLWidget * mGLWidget;
		Landscape * mLandscape;
		Material * mMaterial;
		QVector2D mMaterialScale;
		QString mBlobMapPath;
		GLuint mBlobMap;
		QRect mRect;
		int mPriority;

		static int sQuality;
	public:
		Blob( Landscape * landscape, QRect rect, QString material, QVector2D materialScale, int priority, QString blobMapPath );
		~Blob();
		void drawPatchMap( const QRect & visible );
		void drawPatch( const QRectF & visible )
			{ drawPatchMap( mLandscape->terrain()->toMap(visible) ); }

		static int quality() { return sQuality; }
		static void setQuality( int quality ) { sQuality = quality; }
	};

private:
	/// Splits the Terrain into a grid of patches and applies frustum culling.
	class Filter
	{
	public:
		Filter( Landscape * landscape, QSize filterSize );
		~Filter();
		void update();
		void draw();
	private:
		class Patch
		{
		public:
			Patch() : mPosition(), mSize(), mCenter(), mBoundingSphereRadius(0.0f) {};
			Patch( QVector3D position, QVector3D size );
			const QVector3D & position() { return mPosition; }
			const QVector3D & size() { return mSize; }
			const QVector3D & center() { return mCenter; }
			const float & boundingSphereRadius() { return mBoundingSphereRadius; }
		private:
			QVector3D mPosition;
			QVector3D mSize;
			QVector3D mCenter;
			float mBoundingSphereRadius;
		};
		Landscape * mLandscape;
		QSize mFilterSize;
		QVector<Patch> mPatches;
	};

	QString mName;
	QVector<Blob*> mBlobs;
	QVector< QSharedPointer<AObject> > mVegetation;
	QVector< QSharedPointer<AObject> > mPowerUps;
	Terrain * mTerrain;
	Filter * mTerrainFilter;
	Material * mTerrainMaterial;
	QVector3D mTerrainSize;
	QVector3D mTerrainOffset;
	QVector2D mTerrainMaterialScale;
	float mWaterHeight;
	Shader * mWaterShader;
	TextureRenderer * mReflectionRenderer;
	TextureRenderer * mRefractionRenderer;
	float mWaterClippingPlaneOffset;
	bool mDrawingReflection;
	bool mDrawingRefraction;
	GLuint mWaterMap;

	void drawInfinitePlane( const float & height );
	void renderReflection();
	void renderRefraction();
};


#endif
