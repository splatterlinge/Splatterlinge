#ifndef RESOURCE_AUDIOSAMPLE_INCLUDED
#define RESOURCE_AUDIOSAMPLE_INCLUDED


#include "AResource.hpp"

#include <utility/alWrappers.hpp>

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

	float secOffset() const { float f; alGetSourcef( mSource, AL_SEC_OFFSET, &f ); return f; }
	bool isLooping() const { int i; alGetSourcei( mSource, AL_LOOPING, &i ); return i?true:false; }
	bool isPlaying() const { int i; alGetSourcei( mSource, AL_SOURCE_STATE , &i ); return i==AL_PLAYING; }
	float gain() const { float f; alGetSourcef( mSource, AL_GAIN, &f ); return f; }
	float pitch() const { float f; alGetSourcef( mSource, AL_PITCH, &f ); return f; }
	float referenceDistance() const { float f; alGetSourcef( mSource, AL_REFERENCE_DISTANCE, &f ); return f; }
	float maxDistance() const { float f; alGetSourcef( mSource, AL_MAX_DISTANCE, &f ); return f; }
	float rolloffFactor() const { float f; alGetSourcef( mSource, AL_ROLLOFF_FACTOR, &f ); return f; }
	const QVector3D position() const { QVector3D v; alGetSourcefv( mSource, AL_POSITION, reinterpret_cast<float*>(&v) ); return v; }
	const QVector3D velocity() const { QVector3D v; alGetSourcefv( mSource, AL_VELOCITY, reinterpret_cast<float*>(&v) ); return v; }
	const QVector3D direction() const { QVector3D v; alGetSourcefv( mSource, AL_DIRECTION, reinterpret_cast<float*>(&v) ); return v; }

	void setLooping( bool loop ) { if( mSource ) alSource( mSource, AL_LOOPING, loop?AL_TRUE:AL_FALSE ); }
	void setGain( float gain ) { if( mSource ) alSource( mSource, AL_GAIN, gain ); }
	void setPitch( float pitch ) { if( mSource ) alSource( mSource, AL_PITCH, pitch); }
	void setReferenceDistance( float ref ) { if( mSource ) alSource( mSource, AL_REFERENCE_DISTANCE, ref ); }
	void setMaxDistance( float maxDist ) { if( mSource ) alSource( mSource, AL_MAX_DISTANCE, maxDist ); }
	void setRolloffFactor( float rolloff ) { if( mSource ) alSource( mSource, AL_ROLLOFF_FACTOR, rolloff ); }
	void setPosition( const QVector3D & position ) { if( mSource ) alSource( mSource, AL_POSITION, position ); }
	void setVelocity( const QVector3D & velocity ) { if( mSource ) alSource( mSource, AL_VELOCITY, velocity ); }
	void setDirection( const QVector3D & direction ) { if( mSource ) alSource( mSource, AL_DIRECTION, direction ); }

	void rewind() { if( mSource ) alSourceRewind( mSource ); }
	void play() { if( mSource ) alSourcePlay( mSource ); }
	void stop() { if( mSource ) alSourceStop( mSource ); }


private:
	ALuint mSource;
};


#endif
