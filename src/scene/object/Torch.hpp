#ifndef SCENE_OBJECT_TORCH_INCLUDED
#define SCENE_OBJECT_TORCH_INCLUDED

#include "AWorldObject.hpp"
#include "ALightSource.hpp"

#include <GLWidget.hpp>
#include <utility/OcclusionTest.hpp>
#include <utility/Bilboard.hpp>

#include <QGLBuffer>


/// A torch
/**
 *
 */
class Torch : public AWorldObject, public ALightSource
{
public:
	Torch( World * world );
	virtual ~Torch();

	virtual void updateSelf( const double & delta );
	virtual void drawSelf();
	virtual void draw2Self();

	virtual void updateLightSource( GLenum light );

	const QVector4D & color() const { return mColor; }

private:
	static const GLfloat sQuadVertices[];
	static QGLBuffer sQuadVertexBuffer;

	OcclusionTest mOcclusionTest;
	GLuint mFlareMap;
	float mFlareSize;
	float mColorCycle;
	QVector4D mColor;
	float mFlareRotation;
};


#endif
