#ifndef SCENE_OBJECT_TORCH_INCLUDED
#define SCENE_OBJECT_TORCH_INCLUDED

#include "AObject.hpp"
#include "ALightSource.hpp"


class World;


/// A torch
/**
 *
 */
class Torch : public AObject, public ALightSource
{
public:
	Torch( Scene * scene, World * world );
	virtual ~Torch();

	virtual void updateSelf( const double & delta );
	virtual void drawSelf();
	virtual void draw2Self();

	virtual void updateLightSource( GLenum light );

	const QVector4D & color() const { return mColor; }

private:
	World * mWorld;
	GLuint mFlareMap;
	float mFlareSize;
	float mColorCycle;
	QVector4D mColor;
};


#endif
