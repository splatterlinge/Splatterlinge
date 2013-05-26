#ifndef SCENE_OBJECT_LANDSCAPE_INCLUDED
#define SCENE_OBJECT_LANDSCAPE_INCLUDED

#include "AObject.hpp"
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
class Landscape : public AObject
{
public:
	Landscape( Scene * scene, QString rootDir );
	~Landscape();

	virtual void updateSelf( const double & delta );
	virtual void drawSelf();
	virtual void drawSelfPost();
	virtual void drawSelfPostProc();
	
	void drawPatch( const QRectF & rect );

	Terrain * terrain() { return mTerrain; }
	const Terrain * terrain() const { return mTerrain; }

private:
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
	public:
		Blob( Landscape * landscape, QRect rect, QString material, QVector2D materialScale, QString blobMapPath );
		~Blob();
		void drawPatchMap( const QRect & visible );
		void drawPatch( const QRectF & visible )
			{ drawPatchMap( mLandscape->terrain()->toMap(visible) ); }
	};

	/// Splits the Terrain into a grid of patches and applies frustum culling.
	class Filter
	{
	public:
		Filter( Landscape * landscape, QSize filterSize );
		~Filter();
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
	
	void renderReflection();
	void renderRefraction();
};


#endif
