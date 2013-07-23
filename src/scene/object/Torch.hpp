#ifndef SCENE_OBJECT_TORCH_INCLUDED
#define SCENE_OBJECT_TORCH_INCLUDED

#include "AWorldObject.hpp"
#include "ALightSource.hpp"


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
	GLuint mFlareMap;
	float mFlareSize;
	float mColorCycle;
	QVector4D mColor;
	float mFlareRotation;
};


#endif
