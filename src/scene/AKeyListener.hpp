#ifndef SCENE_AKEYLISTENER_INCLUDED
#define SCENE_AKEYLISTENER_INCLUDED


#include <QKeyEvent>


/// Abstract key listener
/**
 * Register at Scene to receive keyboard events.
 */
class AKeyListener
{
public:
	virtual void keyPressEvent( QKeyEvent * event ) = 0;
	virtual void keyReleaseEvent( QKeyEvent * event ) = 0;
};


#endif
