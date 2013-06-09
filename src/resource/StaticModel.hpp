#ifndef RESOURCE_STATICMODEL_INCLUDED
#define RESOURCE_STATICMODEL_INCLUDED


#include "AResource.hpp"

#include <QDebug>


class StaticModelData : public AResourceData
{
public:
	StaticModelData( QString name );
	virtual ~StaticModelData();
	virtual bool load();
	virtual void unload();

	const QString & name() const { return mName; }

private:
	QString mName;
};


class StaticModel : public AResource<StaticModelData>
{
public:
	StaticModel( QString name );
	virtual ~StaticModel();

private:
};


#endif
