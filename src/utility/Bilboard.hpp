#ifndef GEOMETRY_BILBOARD_INCLUDED
#define GEOMETRY_BILBOARD_INCLUDED

#include <GLWidget.hpp>


/// Screen-align geometry
/**
 *
 */
class Bilboard
{
public:
	static QMatrix4x4 matrix( const QMatrix4x4 & modelView )
		{ QMatrix4x4 bilboard; bilboard.setColumn( 3, modelView.column(3) ); return bilboard; }

	static void begin( const QMatrix4x4 & modelView )
		{ glPushMatrix(); glLoadMatrix( matrix( modelView ) ); }

	static void end()
		{ glPopMatrix(); }

private:
	Bilboard() {};
	~Bilboard() {};
};


#endif
