#include "Eye.hpp"


void Eye::updateMatrix() const
{
	mMatrix.setToIdentity();
	mMatrix.rotate( -rotation() );
	mMatrix.translate( -position() );
}
