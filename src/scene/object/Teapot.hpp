#ifndef SCENE_OBJECT_TEAPOT_INCLUDED
#define SCENE_OBJECT_TEAPOT_INCLUDED

#include "AObject.hpp"


class Material;
class AudioSample;


/// It's a Teapot
/**
 * 
 */
class Teapot : public AObject
{
public:
	Teapot( Scene * scene, const float & size );
	virtual ~Teapot();
	
	virtual void updateSelf( const double & delta );
	virtual void drawSelf();

private:
	Material * mMaterial;
	AudioSample * mAudioSample;
	float mSize;
};


#endif
