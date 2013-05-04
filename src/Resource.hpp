#ifndef RESOURCE_INCLUDED
#define RESOURCE_INCLUDED


#include <QHash>
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
	static QHash< QString, QWeakPointer<T> > cached;
	QSharedPointer<T> mData;

protected:
	QSharedPointer<T> & data() { return mData; }
	void cache( QSharedPointer<T> data )
	{
		if( cached.contains(data->uid()) && !(cached[data->uid()].isNull()) )
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
	const QSharedPointer<T> & constData() const { return mData; }
};


#define RESOURCE_CACHE( ResourceDataType ) \
	template<> QHash< QString, QWeakPointer<ResourceDataType> > Resource<ResourceDataType>::cached \
		= QHash< QString, QWeakPointer<ResourceDataType> >()


#endif
