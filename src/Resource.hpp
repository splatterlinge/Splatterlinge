#ifndef RESOURCE_INCLUDED
#define RESOURCE_INCLUDED


#include <QMap>
#include <QWeakPointer>
#include <QString>


class ResourceData
{
private:
	QString mUID;
	bool mLoaded;

public:
	ResourceData( const QString & uid ) : mUID(uid), mLoaded(false) {}
	virtual ~ResourceData() = 0;

	const QString & uid() const { return mUID; }
	bool loaded() const { return mLoaded; }
	virtual bool load() { mLoaded = true; return true; }

	virtual bool operator==( const ResourceData & rhs ) const { return mUID==rhs.mUID; }
	virtual bool operator!=( const ResourceData & rhs ) const { return !(*this==rhs); }
};


template< class T >
class Resource
{
private:
	static QMap< QString, QWeakPointer<T> > cached;
	QSharedPointer<T> mData;

protected:
	QSharedPointer<T> & data() { return mData; }
	void cache( QSharedPointer<T> data )
	{
		if( cached.contains(data->uid()) )
		{
			mData = cached[data->uid()];
		} else {
			cached[data->uid()] = data;
			mData = data;
			mData->load();
		}
	}

public:
	Resource() {};
	virtual ~Resource() {};
};


#define RESOURCE_CACHE( ResourceDataType ) \
	template<> QMap< QString, QWeakPointer<ResourceDataType> > Resource<ResourceDataType>::cached \
		= QMap< QString, QWeakPointer<ResourceDataType> >()


#endif
