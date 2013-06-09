#include "StaticModel.hpp"

#include <scene/object/AObject.hpp>

#include <QDebug>
#include <QVector3D>
#include <float.h>


RESOURCE_CACHE(StaticModelData);


StaticModelData::StaticModelData( QString name ) :
	AResourceData( name ),
	mName( name )
{
}


StaticModelData::~StaticModelData()
{
	unload();
}


void StaticModelData::unload()
{
	if( !loaded() )
		return;
	qDebug() << "-" << this << "StaticModelData" << uid();

	//TODO: free static data

	StaticModelData::unload();
}


bool StaticModelData::load()
{
	unload();
	qDebug() << "+" << this << "StaticModelData" << uid();

	//TODO: load all static data that can be shared between resource instances

	return AResourceData::load();
}


StaticModel::StaticModel( QString name ) : AResource()
{
	QSharedPointer<StaticModelData> n( new StaticModelData( name ) );
	cache( n );

	//TODO: setup data that is used in each resource instance (optional)
}


StaticModel::~StaticModel()
{
	//TODO: free data
}
