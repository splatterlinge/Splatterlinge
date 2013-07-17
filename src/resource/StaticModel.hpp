#ifndef RESOURCE_STATICMODEL_INCLUDED
#define RESOURCE_STATICMODEL_INCLUDED


#include "AResource.hpp"

#include <QDebug>


/// The model's data
class StaticModelData : public AResourceData
{
public:
	StaticModelData( QString name );
	virtual ~StaticModelData();

	const QString & name() const { return mName; }

	// Overrides:
	virtual bool load();
	virtual void unload();

private:
	QString mName;
};


/// A static model
class StaticModel : public AResource<StaticModelData>
{
public:
	StaticModel( QString name );
	virtual ~StaticModel();

private:
};


#endif
