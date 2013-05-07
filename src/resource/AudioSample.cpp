#include "AudioSample.hpp"
#include "audioLoader/wav.h"

#include <scene/object/AObject.hpp>

#include <QDebug>
#include <QVector3D>
#include <AL/al.h>
#include <float.h>


RESOURCE_CACHE(AudioSampleData);


AudioSampleData::AudioSampleData( QString name ) :
	AResourceData( name ),
	mName(name),
	mBuffer(0)
{
}


AudioSampleData::~AudioSampleData()
{
	if( loaded() )
	{
		qDebug() << "-" << this << "AudioSampleData" << uid();
	}
}


bool AudioSampleData::load()
{
	qDebug() << "+" << this << "AudioSampleData" << uid();

	if( audioLoader_WAV( QString("./data/sound/"+mName+".wav").toLocal8Bit().constData(), &mBuffer, &mFrequency, &mFormat ) != 0 )
		return false;

	return AResourceData::load();
}


AudioSample::AudioSample( QString name ) : AResource()
{
	QSharedPointer<AudioSampleData> n( new AudioSampleData( name ) );
	cache( n );

	mSource = 0;
	mLooping = false;
	mGain = 1.0f;
	mPitch = 1.0f;
	mReferenceDistance = 1.0f;
	mMaxDistance = FLT_MAX;
	mRolloffFactor = 1.0f;

	alGenSources( 1, &mSource );
	alSourcei( mSource, AL_SOURCE_RELATIVE, AL_FALSE);
	alSourcei( mSource, AL_BUFFER, data()->buffer() );
}


AudioSample::~AudioSample()
{
	alDeleteSources( 1, &mSource );
}


void AudioSample::update()
{
	alSourcefv( mSource, AL_POSITION, reinterpret_cast<const ALfloat*>(&mObjectHook->position()) );
	alSourcef( mSource, AL_PITCH, mPitch);
	alSourcef( mSource, AL_REFERENCE_DISTANCE, mReferenceDistance );
	alSourcef( mSource, AL_ROLLOFF_FACTOR, mRolloffFactor );
	alSourcef( mSource, AL_GAIN, mGain);
	alSourcei( mSource, AL_LOOPING, mLooping);
}


void AudioSample::play()
{
	alSourcePlay( mSource );
}


void AudioSample::stop()
{
	alSourceStop( mSource );
}
