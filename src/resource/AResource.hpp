#ifndef RESOURCE_ARESOURCE_INCLUDED
#define RESOURCE_ARESOURCE_INCLUDED


#include <QHash>
#include <QWeakPointer>
#include <QString>


/// Abstract base class for common resource data
/**
 * This class defines a resource's data using a unique identifier.\n
 * The resource data can then be loaded to memory.
 */
class AResourceData
{
private:
	QString mUID;
	bool mLoaded;

public:
	/// Initializes a description to the data identified by given UID
	AResourceData( const QString & uid ) : mUID(uid), mLoaded(false) {}
	virtual ~AResourceData() = 0;

	/// Returns this data's unique identifier
	const QString & uid() const { return mUID; }

	/// Returns true if the data could be loaded successfully
	bool loaded() const { return mLoaded; }

	/// Loads the data to memory
	virtual bool load() { mLoaded = true; return true; }

	virtual void unload() { mLoaded = false; }

	virtual bool operator==( const AResourceData & rhs ) const { return mUID==rhs.mUID; }
	virtual bool operator!=( const AResourceData & rhs ) const { return !(*this==rhs); }
};


/// Abstract base class for accessing a resource
/**
 * This class represents a resource.
 * If multiple classes reference the same data,
 * this data is stored using the AResourceData class
 * and shared among all AResource classes.
 */
template< class T >
class AResource
{
private:
	static QHash< QString, QWeakPointer<T> > sCache;
	QSharedPointer<T> mData;

protected:
	/// Returns the pointer to this resource's data
	QSharedPointer<T> & data() { return mData; }

	/// This method should be called by the resource to bind the specified data
	/**
	 * Binds the given resource data.
	 * If the resource's data is already loaded,
	 * this resource is bound to the already loaded data.\n
	 * \warning Do not use the given data pointer after calling this method - use the data() method instead.
	 * Only data() will return a valid pointer to the resource's data in case the data is already cached.
	 * @param data The (unloaded) data that is used as a data descriptor.
	 */
	void cache( QSharedPointer<T> data )
	{
		if( sCache.contains(data->uid()) && !(sCache[data->uid()].isNull()) )
		{
			mData = sCache[data->uid()];
		} else {
			sCache[data->uid()] = data;
			mData = data;
			mData->load();
		}
	}

public:
	AResource() {};
	virtual ~AResource() {};

	/// Returns the pointer to this resource's data.
	const QSharedPointer<T> & constData() const { return mData; }

	static const QHash< QString, QWeakPointer<T> > & cache() { return sCache; }
};


#define RESOURCE_CACHE( ResourceDataType ) \
	template<> QHash< QString, QWeakPointer<ResourceDataType> > AResource<ResourceDataType>::sCache \
		= QHash< QString, QWeakPointer<ResourceDataType> >()


#endif
