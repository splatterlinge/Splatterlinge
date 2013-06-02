#include "Player.hpp"

Player::Player( Scene * scene ) :
	AObject( scene )
{
}

Player::Player( Player & other ) :
	AObject( other )
{
}

Player::~Player()
{
}

void Player::updateSelf( const double & delta )
{
}

void Player::drawSelf()
{
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	glColor3f( 0.0f, 1.0f, 0.0f );
	glBegin(GL_LINES);
	glVertex3f( 0.0f, 0.05f, 0.0f);
	glVertex3f( 0.0f, 0.1f, 0.0f);
	glVertex3f( 0.0f,-0.05f, 0.0f);
	glVertex3f( 0.0f,-0.1f, 0.0f);
	glVertex3f( 0.05f, 0.0f, 0.0f);
	glVertex3f( 0.15f, 0.0f, 0.0f);
	glVertex3f(-0.05f, 0.0f, 0.0f);
	glVertex3f(-0.15f, 0.0f, 0.0f);
	glEnd();
}

void Player::drawSelfPost()
{
}
