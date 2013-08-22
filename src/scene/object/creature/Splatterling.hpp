#ifndef SCENE_OBJECT_CREATURE_SPLATTERLING_INCLUDED
#define SCENE_OBJECT_CREATURE_SPLATTERLING_INCLUDED


#include "ACreature.hpp"
#include "resource/AudioSample.hpp"

static const GLfloat GlobalPositionData[] =
{
	0.0f, 0.0f, 0.0f,
	-1.0f, 1.0f, 8.0f,
	1.0f, 1.0f, 8.0f,
	1.0f, -1.0f, 8.0f,
	-1.0f, -1.0f, 8.0f,
	-1.0f, 1.0f, 8.0f,

	//face
	-1.0f, 1.0f, 8.0f,
	-1.0f, -1.0f, 8.0f,
	1.0f, 1.0f, 8.0f,
	1.0f, -1.0f, 8.0f,

	//wingOne
	0.0f, 0.5f, 4.0f,
	8.0f, 4.0f, -2.0f,
	8.0f, 4.0f, 10.0f,

	//wingTwo
	0.0f, 0.5f, 4.0f,
	-8.0f, 4.0f, -2.0f,
	-8.0f, 4.0f, 10.0f,
};

static const GLfloat TextureCoordData[] =
{
	0.0f, 0.0f,
	0.5f, 0.0f,
	0.5f, 0.5f,
	0.5f, 0.0f,
	0.5f, 0.5f,
	0.5f, 0.0f,

	//face
	0.0f, 1.0f,
	0.0f, 0.5f,
	0.5f, 1.0f,
	0.5f, 0.5f,

	//wingOne
	0.5f, 0.65f,
	1.0f, 0.25f,
	1.0f, 1.0f,

	//wingTwo
	0.5f, 0.65f,
	1.0f, 0.25f,
	1.0f, 1.0f,

};


static const GLubyte ColorData[] =
{
	255, 255, 0,
	255, 255, 0,
	255, 255, 0,
	255, 255, 0,
	255, 255, 0,
	255, 255, 0,

	0, 255, 255,
	0, 255, 255,
	0, 255, 255,
	0, 255, 255,

	0, 0, 255,
	0, 0, 255,
	0, 0, 255,
	0, 0, 255,
	0, 0, 255,
	0, 0, 255,
};

struct GLUquadric;
class Material;


/// Splatterling
class Splatterling : public ACreature
{
public:

	static const GLsizeiptr PositionSize = 16 * 3 * sizeof( GLfloat );
	static const GLsizeiptr ColorSize = 16 * 3 * sizeof( GLubyte );
	static const GLsizeiptr TexSize = 16 * 2 * sizeof( GLfloat );

	static const int BufferSize = 3;

	static const GLsizei BodyVertexCount = 6;
	static const GLsizei HeadVertexCount = 4;
	static const int WingOneYPos = ( ( BodyVertexCount + HeadVertexCount ) * 3 ) + 4;
	static const int WingTwoYPos = ( ( BodyVertexCount + HeadVertexCount ) * 3 ) + ( 3 * 3 ) + 4;
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
		TARGET_WING = 2
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
	GLfloat PositionData[48];
	QVector3D destinationPoint;
	bool wingUpMovement;
	bool playerDetected;
	AudioSample * mWingSound;
	int targetBodyPart;
	float damageMultiplicationFactor[3];
};


#endif
