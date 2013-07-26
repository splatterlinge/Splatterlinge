#include "Vertex.hpp"

Vertex::Vertex()
{
}

bool Vertex::operator==( const Vertex & other )
{
    return position == other.position
            && texCoord == other.texCoord
            && normal == other.normal;
}
