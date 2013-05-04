#ifndef WORLD_INCLUDED
#define WORLD_INCLUDED


#include "Object3D.hpp"
#include "KeyListener.hpp"
#include "MouseListener.hpp"


class Sky;
class Landscape;
class Shader;
class TextureRenderer;


class World : public Object3D, public KeyListener
{
public:
	World( GLScene * scene );
	virtual ~World();
	
	virtual void updateSelf( const float & delta );
	virtual void drawSelf();
	virtual void drawSelfPost();

	virtual void keyPressEvent( QKeyEvent * event );
	virtual void keyReleaseEvent( QKeyEvent * event );

private:
	bool mTimeLapse;
	float mTimeOfDay;
	Sky * mSky;
	QSharedPointer<Landscape> mLandscape;
};


#endif
