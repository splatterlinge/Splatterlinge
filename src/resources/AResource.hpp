#ifndef ARESOURCE_INCLUDED
#define ARESOURCE_INCLUDED


#include <QHash>
#include <QWeakPointer>
#include <QString>


class AResourceData
{
private:
	QString mUID;
	bool mLoaded;

public:
	AResourceData( const QString & uid ) : mUID(uid), mLoaded(false) {}
	virtual ~AResourceData() = 0;

	const QString & uid() const { return mUID; }
	bool loaded() const { return mLoaded; }
	virtual bool load() { mLoaded = true; return true; }

	virtual bool operator==( const AResourceData & rhs ) const { return mUID==rhs.mUID; }
	virtual bool operator!=( const AResourceData & rhs ) const { return !(*this==rhs); }
};


template< class T >
class AResource
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
	AResource() {};
	virtual ~AResource() {};
	const QSharedPointer<T> & constData() const { return mData; }
};


#define RESOURCE_CACHE( ResourceDataType ) \
	template<> QHash< QString, QWeakPointer<ResourceDataType> > AResource<ResourceDataType>::cached \
		= QHash< QString, QWeakPointer<ResourceDataType> >()


#endif
