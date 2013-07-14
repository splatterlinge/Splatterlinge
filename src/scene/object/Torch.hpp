#ifndef SCENE_OBJECT_TORCH_INCLUDED
#define SCENE_OBJECT_TORCH_INCLUDED

#include "AObject.hpp"


/// A torch
/**
 * 
 */
class Torch : public AObject
{
public:
	Torch( Scene * scene );
	virtual ~Torch();
	
	virtual void updateSelf( const double & delta );
	virtual void drawSelf();
	virtual void drawAfterSelf();
	
	const QVector4D & color() const { return mColor; }

private:
	GLuint mFlareMap;
	float mFlareSize;
	float mColorCycle;
	QVector4D mColor;
};


#endif
