#ifndef SCENE_OBJECT_CREATURE_SPLATTERLING_INCLUDED
#define SCENE_OBJECT_CREATURE_SPLATTERLING_INCLUDED


#include "ACreature.hpp"


struct GLUquadric;
class Material;


/// Splatterling
class Splatterling : public ACreature
{
public:
    Splatterling( World * world );
    ~Splatterling();

	virtual void updateSelf( const double & delta );
	virtual void drawSelf();

	virtual AObject * intersectLine( const AObject * exclude, const QVector3D & origin, const QVector3D & direction,
		float & length, QVector3D * normal = NULL );

	virtual void receiveDamage( int damage, const QVector3D * position=NULL, const QVector3D * direction=NULL );
    virtual void recalculateWingPosition( );
private:
	GLUquadric * mQuadric;
	Material * mMaterial;
	QVector3D mTarget;
	float mVelocityY;
	float mHeightAboveGround;
    GLuint vboId;
    bool wingUpMovement;
};


#endif
