#ifndef SCENE_OBJECT_ALIGHTSOURCE_INCLUDED
#define SCENE_OBJECT_ALIGHTSOURCE_INCLUDED


#include <GLWidget.hpp>


class Scene;


/// Abstract base class for light sources
class ALightSource
{
public:
	virtual void updateLightSource( GLenum light ) = 0;
};


#endif
