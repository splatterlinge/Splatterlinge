#ifndef WORLD_INCLUDED
#define WORLD_INCLUDED


#include "Object3D.hpp"
#include "KeyListener.hpp"

class Teapot;

class Sky;
class Landscape;


class World : public Object3D, public KeyListener
{
public:
	World( GLScene * scene, const Object3D * parent = 0 );
	virtual ~World();
	
	virtual void updateSelf( const float & delta );
	virtual void drawSelf();

	virtual void keyPressEvent( QKeyEvent * event );
	virtual void keyReleaseEvent( QKeyEvent * event );

private:
	bool mTimeLapse;
	float mTimeOfDay;
	Sky * mSky;
	Landscape * mLandscape;
};


#endif
