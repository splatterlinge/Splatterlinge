/*
 * This file is part of Splatterlinge.
 *
 * Splatterlinge is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * Splatterlinge is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Splatterlinge. If not, see <http://www.gnu.org/licenses/>.
 */

#include "AudioSample.hpp"
#include "audioLoader/audioLoader.h"

#include <scene/object/AObject.hpp>

#include <QDebug>
#include <QDir>
#include <QVector3D>
#include <AL/al.h>
#include <float.h>


RESOURCE_CACHE( AudioSampleData );


AudioSampleData::AudioSampleData( QString name ) :
	AResourceData( name ),
	mName( name ),
	mBuffer( 0 )
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

	QDir soundDir( baseDirectory() );
	QStringList candidateNameFilter;
	candidateNameFilter << (mName+".*");
	soundDir.setNameFilters( candidateNameFilter );
	QStringList candidates = soundDir.entryList( candidateNameFilter, QDir::Files | QDir::Readable, QDir::Name );
	if( candidates.size() > 1 )
		qWarning() << "!" << this << "AudioSampleData" << uid() << "found multiple candidates with the same name: " << candidates;

	bool validFileLoaded = false;
	foreach( const QString & file, candidates )
	{
		if( audioLoader( (baseDirectory()+file).toLocal8Bit().constData(), &mBuffer, &mFrequency, &mFormat ) == 0 )
		{
			validFileLoaded = true;
			break;
		}
	}
	if( !validFileLoaded )
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
		qFatal( "Could not allocate sound source" );

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
	QVector3D velocity = ( position - mLastPosition ) / delta;
	setVelocity( velocity );
	mLastPosition = position;
}
