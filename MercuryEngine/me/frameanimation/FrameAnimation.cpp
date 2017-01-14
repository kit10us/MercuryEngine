// Copyright (c) 2003 - 2014, Quentin S. Smith
// All Rights Reserved

#include <me/frameanimation/FrameAnimation.h>
#include <cassert>

using namespace me;
using namespace frameanimation;

Animation::Animation( const std::string & name, unify::Seconds duration )
: m_name( name ), m_duration( duration )
{
}

Animation::~Animation()
{
}

const std::string & Animation::Name() const
{
	return m_name;
}

unify::Seconds Animation::Duration() const
{
	return m_duration;
}

bool Animation::Loops() const
{
	return m_loops;
}

void Animation::AddScaleKey( size_t boneIndex, const ScaleKey & key )
{
	auto && itr = m_boneAnimations.find( boneIndex );
	if( itr == m_boneAnimations.end() )
	{
		m_boneAnimations[ boneIndex ] = SRTTimeline();
	}
	SRTTimeline & boneTimeline = m_boneAnimations[ boneIndex ];

	boneTimeline.scale.push_back( key );
	boneTimeline.scale.sort();
}

void Animation::AddRotationKey( size_t boneIndex, const RotationKey & key )
{
	auto && itr = m_boneAnimations.find( boneIndex );
	if( itr == m_boneAnimations.end() )
	{
		m_boneAnimations[ boneIndex ] = SRTTimeline();
	}
	SRTTimeline & boneTimeline = m_boneAnimations[ boneIndex ];

	boneTimeline.rotation.push_back( key );
	boneTimeline.rotation.sort();
}

void Animation::AddTranslationKey( size_t boneIndex, const TranslationKey & key )
{
	auto && itr = m_boneAnimations.find( boneIndex );
	if( itr == m_boneAnimations.end() )
	{
		m_boneAnimations[ boneIndex ] = SRTTimeline();
	}
	SRTTimeline & boneTimeline = m_boneAnimations[ boneIndex ];

	boneTimeline.translation.push_back( key );
	boneTimeline.translation.sort();
}

void Animation::ApplyToFrames( float elapsedTime, unify::FrameSetInstance & frameSetInstance ) const
{
	while ( elapsedTime < 0.0f ) elapsedTime += Duration();
	while ( elapsedTime > Duration() ) elapsedTime -= Duration();

	// Apply animation
	for( auto && itr : m_boneAnimations )
	{
		size_t boneIndex = itr.first;
		assert( boneIndex < frameSetInstance.Count() );

		const ScaleKey::list & scaleKeys = itr.second.scale;
		const RotationKey::list & rotationKeys = itr.second.rotation;
		const TranslationKey::list & translationKeys = itr.second.translation;

		unify::V3< float > scale( InterpretValue< unify::V3< float > >( elapsedTime, scaleKeys, unify::V3< float >( 1.0f, 1.0f, 1.0f ) ) );
		unify::Quaternion rotation( InterpretValue< unify::Quaternion >( elapsedTime, rotationKeys, unify::QuaternionIdentity() ) );
		unify::V3< float > translation( InterpretValue< unify::V3< float > >( elapsedTime, translationKeys, unify::V3< float >( 0.0f, 0.0f, 0.0f ) ) );
		unify::Matrix matrix( rotation, translation ); // TODO: Quaternions
		frameSetInstance.Transform( boneIndex, matrix );
	}
}
