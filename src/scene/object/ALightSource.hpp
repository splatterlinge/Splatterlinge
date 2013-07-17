#ifndef SCENE_OBJECT_ALIGHTSOURCE_INCLUDED
#define SCENE_OBJECT_ALIGHTSOURCE_INCLUDED


#include <GLWidget.hpp>

#include <QVector4D>


class Scene;


/// Abstract base class for light sources
class ALightSource
{
public:
	virtual void updateLightSource( GLenum light ) = 0;
};


#endif
