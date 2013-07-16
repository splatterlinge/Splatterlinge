#ifndef SCENE_OBJECT_ALIGHTSOURCE_INCLUDED
#define SCENE_OBJECT_ALIGHTSOURCE_INCLUDED


#include <GLWidget.hpp>

#include <QVector4D>


class Scene;


class ALightSource
{
public:
	virtual void updateLightSource( GLenum light ) = 0;
};

#endif
