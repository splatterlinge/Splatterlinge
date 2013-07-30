#ifndef GEOMETRY_VERTEX_INCLUDED
#define GEOMETRY_VERTEX_INCLUDED


#include <QVector3D>
#include <QVector2D>
#include <utility/glWrappers.hpp>


class VertexP3f
{
public:
	QVector3D position;

	VertexP3f() :
		position(0,0,0)
	{}

	~VertexP3f() {}

	bool operator==( const VertexP3f & other )
	{
		return position == other.position;
	}

	bool operator!=( const VertexP3f & other )
	{
		return !(*this==other);
	}

	static size_t size() { return sizeof( VertexP3f ); }
	static size_t positionOffset() { return offsetof( VertexP3f, position ); }
	static void * positionOffsetPTR() { return (void*)positionOffset(); }
	static void glEnableClientState()
	{
		::glEnableClientState( GL_VERTEX_ARRAY );
	}
	static void glDisableClientState()
	{
		::glDisableClientState( GL_VERTEX_ARRAY );
	}
	static void glPointerVBO()
	{
		glVertexPointer( 3, GL_FLOAT, size(), positionOffsetPTR() );
	}
};


class VertexP3fT2f
{
public:
	QVector3D position;
	QVector2D texCoord;

	VertexP3fT2f() :
		position(0,0,0),
		texCoord(0,0)
	{}

	~VertexP3fT2f() {}

	bool operator==( const VertexP3fT2f & other )
	{
		return position == other.position
			&& texCoord == other.texCoord;
	}

	bool operator!=( const VertexP3fT2f & other )
	{
		return !(*this==other);
	}

	static size_t size() { return sizeof( VertexP3fT2f ); }
	static size_t positionOffset() { return offsetof( VertexP3fT2f, position ); }
	static size_t texCoordOffset() { return offsetof( VertexP3fT2f, texCoord ); }
	static void * positionOffsetPTR() { return (void*)positionOffset(); }
	static void * texCoordOffsetPTR() { return (void*)texCoordOffset(); }
	static void glEnableClientState()
	{
		::glEnableClientState( GL_VERTEX_ARRAY );
		::glEnableClientState( GL_TEXTURE_COORD_ARRAY );
	}
	static void glDisableClientState()
	{
		::glDisableClientState( GL_VERTEX_ARRAY );
		::glDisableClientState( GL_TEXTURE_COORD_ARRAY );
	}
	static void glPointerVBO()
	{
		glVertexPointer( 3, GL_FLOAT, size(), positionOffsetPTR() );
		glTexCoordPointer( 2, GL_FLOAT, size(), texCoordOffsetPTR() );
	}
};


class VertexP3fN3fT2f
{
public:
	QVector3D position;
	QVector3D normal;
	QVector2D texCoord;

	VertexP3fN3fT2f() :
		position(0,0,0),
		normal(0,1,0),
		texCoord(0,0)
	{}

	~VertexP3fN3fT2f() {}

	bool operator==( const VertexP3fN3fT2f & other )
	{
		return position == other.position
			&& normal == other.normal
			&& texCoord == other.texCoord;
	}

	bool operator!=( const VertexP3fN3fT2f & other )
	{
		return !(*this==other);
	}

	static size_t size() { return sizeof( VertexP3fN3fT2f ); }
	static size_t positionOffset() { return offsetof( VertexP3fN3fT2f, position ); }
	static size_t normalOffset() { return offsetof( VertexP3fN3fT2f, normal ); }
	static size_t texCoordOffset() { return offsetof( VertexP3fN3fT2f, texCoord ); }
	static void * positionOffsetPTR() { return (void*)positionOffset(); }
	static void * normalOffsetPTR() { return (void*)normalOffset(); }
	static void * texCoordOffsetPTR() { return (void*)texCoordOffset(); }
	static void glEnableClientState()
	{
		::glEnableClientState( GL_VERTEX_ARRAY );
		::glEnableClientState( GL_NORMAL_ARRAY );
		::glEnableClientState( GL_TEXTURE_COORD_ARRAY );
	}
	static void glDisableClientState()
	{
		::glDisableClientState( GL_VERTEX_ARRAY );
		::glDisableClientState( GL_NORMAL_ARRAY );
		::glDisableClientState( GL_TEXTURE_COORD_ARRAY );
	}
	static void glPointerVBO()
	{
		glVertexPointer( 3, GL_FLOAT, size(), positionOffsetPTR() );
		glNormalPointer( GL_FLOAT, size(), normalOffsetPTR() );
		glTexCoordPointer( 2, GL_FLOAT, size(), texCoordOffsetPTR() );
	}
};


#endif
