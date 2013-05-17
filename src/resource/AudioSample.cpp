#include "AudioSample.hpp"
#include "audioLoader/audioLoader.h"

#include <scene/object/AObject.hpp>
#include <utility/qtalwrappers.hpp>

#include <QDebug>
#include <QVector3D>
#include <AL/al.h>
#include <float.h>


RESOURCE_CACHE(AudioSampleData);


AudioSampleData::AudioSampleData( QString file ) :
	AResourceData( file ),
	mFile(file),
	mBuffer(0)
{
}


AudioSampleData::~AudioSampleData()
{
	if( loaded() )
	{
		qDebug() << "-" << this << "AudioSampleData" << uid();
		alDeleteBuffers( 1, &mBuffer );
	}
}


bool AudioSampleData::load()
{
	qDebug() << "+" << this << "AudioSampleData" << uid();

	if( audioLoader( mFile.toLocal8Bit().constData(), &mBuffer, &mFrequency, &mFormat ) != 0 )
		return false;

	if( mFormat == AL_FORMAT_STEREO8 || mFormat == AL_FORMAT_STEREO16 )
		qWarning() << "!" << this << "AudioSampleData" << uid() << "is a stereo file - positional audio disabled.";

	return AResourceData::load();
}


AudioSample::AudioSample( QString file ) : AResource()
{
	QSharedPointer<AudioSampleData> n( new AudioSampleData( file ) );
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
	alSource( mSource, AL_POSITION, mObjectHook->position() );
	alSource( mSource, AL_PITCH, mPitch);
	alSource( mSource, AL_REFERENCE_DISTANCE, mReferenceDistance );
	alSource( mSource, AL_ROLLOFF_FACTOR, mRolloffFactor );
	alSource( mSource, AL_GAIN, mGain);
	alSource( mSource, AL_LOOPING, mLooping);
}


void AudioSample::play()
{
	alSourcePlay( mSource );
}


void AudioSample::stop()
{
	alSourceStop( mSource );
}
