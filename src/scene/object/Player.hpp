#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "AObject.hpp"

#include <GLWidget.hpp>

class Scene;

class Player : public AObject
{
public:
	Player( Scene * scene );
	Player( Player & other );
	virtual ~Player();

	virtual void updateSelf( const double & delta );
	virtual void drawSelf();
	virtual void drawSelfPost();
};

#endif // PLAYER_HPP
