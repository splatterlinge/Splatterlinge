#ifndef VERTEX_HPP
#define VERTEX_HPP

#include <QVector2D>
#include <QVector3D>

class Vertex
{
public:
    Vertex();

    bool operator==( const Vertex & other );

    QVector3D position;
    QVector2D texCoord;
    QVector3D normal;
};

#endif // VERTEX_HPP
