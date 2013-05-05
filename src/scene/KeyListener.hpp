#ifndef KEYLISTENER_INCLUDED
#define KEYLISTENER_INCLUDED


#include <QKeyEvent>


class KeyListener
{
public:
	virtual void keyPressEvent( QKeyEvent * event ) = 0;
	virtual void keyReleaseEvent( QKeyEvent * event ) = 0;
};


#endif
