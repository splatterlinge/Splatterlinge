#ifndef RESOURCE_AUDIOSAMPLE_INCLUDED
#define RESOURCE_AUDIOSAMPLE_INCLUDED


#include "AResource.hpp"

#include <AL/al.h>
#include <QDebug>


class AObject;


class AudioSampleData : public AResourceData
{
public:
	AudioSampleData( QString file );
	virtual ~AudioSampleData();
	virtual bool load();
	virtual void unload();

	const QString & file() const { return mFile; }

	ALuint buffer() const { return mBuffer; }
	ALsizei frequency() const { return mFrequency; }
	ALenum format() const { return mFormat; }

private:
	QString mFile;
	ALuint mBuffer;
	ALsizei mFrequency;
	ALenum mFormat;
};


class AudioSample : public AResource<AudioSampleData>
{
public:
	AudioSample( QString file );
	virtual ~AudioSample();

	float gain() const { return mGain; }
	float pitch() const { return mPitch; }
	float referenceDistance() const { return mReferenceDistance; }
	float maxDistance() const { return mMaxDistance; }
	float rolloffFactor() const { return mRolloffFactor; }
	const AObject * objectHook() const { return mObjectHook; }

	void setLooping( bool loop ) { mLooping = loop?AL_TRUE:AL_FALSE; }
	void setGain( float gain ) { mGain = gain; }
	void setPitch( float pitch ) { mGain = pitch; }
	void setReferenceDistance( float ref ) { mReferenceDistance = ref; }
	void setMaxDistance( float maxDist ) { mReferenceDistance = maxDist; }
	void setRolloffFactor( float rolloff ) { mReferenceDistance = rolloff; }
	void setObjectHook( const AObject * object ) { mObjectHook = object; }

	void update();
	void play();
	void stop();

private:
	ALuint mSource;
	ALboolean mLooping;
	ALfloat mGain;
	ALfloat mPitch;
	ALfloat mReferenceDistance;
	ALfloat mMaxDistance;
	ALfloat mRolloffFactor;
	const AObject * mObjectHook;
};


#endif
