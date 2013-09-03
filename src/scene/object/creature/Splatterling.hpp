#ifndef SCENE_OBJECT_CREATURE_SPLATTERLING_INCLUDED
#define SCENE_OBJECT_CREATURE_SPLATTERLING_INCLUDED


#include "ACreature.hpp"
#include "resource/AudioSample.hpp"


struct GLUquadric;
class Material;


/// Splatterling
class Splatterling : public ACreature
{
public:

	static const GLsizeiptr PositionSize = 72 * 3 * sizeof( GLfloat );
	static const GLsizeiptr ColorSize = 72 * 3 * sizeof( GLubyte );
	static const GLsizeiptr TexSize = 72 * 2 * sizeof( GLfloat );


	static const float SplatterlingSizeFactor = 0.5f;
	static const int BufferSize = 3;
	static const float SplatterlingLength = 8.8f;
	static const float SplatterlingBoundingSphereSize = 18.0f;

	static const GLsizei BodyVertexCount = 34;
	static const GLsizei HeadVertexCount = 32;
	static const int WingOneYPos = ( ( BodyVertexCount + HeadVertexCount ) * 3 ) + 4;
	static const int WingTwoYPos = ( ( BodyVertexCount + HeadVertexCount ) * 3 ) + ( 3 * 3 ) + 4;
	static const double RotationStepSize = 2.0;
	enum
	{
		POSITION_OBJECT = 0,
		COLOR_OBJECT = 1,
		TEXTURE_OBJECT = 2
	};
	enum
	{
		TARGET_BODY = 0,
		TARGET_HEAD = 1,
		TARGET_WING_RIGHT = 2,
		TARGET_WING_LEFT = 3
	};

	GLuint BufferName[BufferSize];


	Splatterling( World * world );
	~Splatterling();

	virtual void updateSelf( const double & delta );
	virtual void drawSelf();

	virtual AObject * intersectLine( const AObject * exclude, const QVector3D & origin, const QVector3D & direction,
					 float & length, QVector3D * normal = NULL );

	virtual void receiveDamage( int damage, const QVector3D * position = NULL, const QVector3D * direction = NULL );
	virtual void recalculateWingPosition( );

	virtual bool intersectBody(const QVector3D & origin, const QVector3D & direction, float * intersectionDistance);
	virtual bool intersectWing(const QVector3D & origin, const QVector3D & direction, float * intersectionDistance);
	virtual bool intersectHead(const QVector3D & origin, const QVector3D & direction, float * intersectionDistance);

private:
	virtual void randomDestinationPoint();
	GLUquadric * mQuadric;
	Material * mMaterial;
	QVector3D mTarget;
	float mVelocityY;
	float mHeightAboveGround;
	GLuint vboId;
	GLfloat PositionData[PositionSize/sizeof( GLfloat )];
	QVector3D destinationPoint;
	bool wingUpMovement;
	bool playerDetected;
	AudioSample * mWingSound;
	AudioSample * mSnapSound;
	int targetBodyPart;
	float damageMultiplicationFactor[4];
	float mCoolDown;
	bool recalculationOfRotationAngle;
	float rotationAroundPlayer;
};


#endif
