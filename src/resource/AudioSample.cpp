#include "AudioSample.hpp"
#include "audioLoader/audioLoader.h"

#include <scene/object/AObject.hpp>

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
    unload();
}


void AudioSampleData::unload()
{
    if( !loaded() )
        return;
    qDebug() << "-" << this << "AudioSampleData" << uid();

    alDeleteBuffers( 1, &mBuffer );

    AResourceData::unload();
}


bool AudioSampleData::load()
{
    unload();
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

    alGenSources( 1, &mSource );
    if( !mSource )
    {
        qFatal("Could not allocate sound source");
    }
    alSourcei( mSource, AL_SOURCE_RELATIVE, AL_FALSE );
    alSourcei( mSource, AL_BUFFER, data()->buffer() );

    setLooping( true );
    setGain( 1.0f );
    setPitch( 1.0f );
    setReferenceDistance( 1.0f );
    setMaxDistance( FLT_MAX );
    setRolloffFactor( 1.0f );
}


AudioSample::~AudioSample()
{
    if( !mSource )
        return;
    alDeleteSources( 1, &mSource );
}


void AudioSample::setPositionAutoVelocity( const QVector3D & position, const double & delta )
{
    setPosition( position );
    QVector3D velocity = (position - mLastPosition) / delta;
    setVelocity( velocity );
    mLastPosition = position;
}
