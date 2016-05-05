// Copyright (c) 2002 - 2014, Quentin S. Smith
// All Rights Reserved

#include <dxi/animation/Group.h>
#include <unify/Exception.h>
#include <algorithm>
#include <assert.h>

using namespace dxi;
using namespace animation;

Group::Group( SpriteManager * parent )
: m_parent( parent )
{
}

Group::~Group()
{
	// We do not own m_parent.
	m_parent = 0;
}

void Group::Add( const SequenceName & name, std::shared_ptr< Sequence > sequence )
{
	m_sequenceMap[ name ] = static_cast< unsigned int >( m_sequenceArray.size() );
	m_sequenceArray.push_back( sequence );
	assert( m_sequenceArray.size() == m_sequenceMap.size() );
}

bool Group::SequenceExists( const SequenceName & name ) const
{
	SequenceMap::const_iterator itrSequence = m_sequenceMap.find( name );
	return ( itrSequence == m_sequenceMap.end() ) ? false : true;
}

const std::shared_ptr< Sequence > & Group::FindSequence( const SequenceName & name ) const
{
	SequenceMap::const_iterator itrSequence = m_sequenceMap.find( name );
	if( itrSequence == m_sequenceMap.end() )
	{
		throw unify::Exception( "Sequence not found!" );
	}
	else
	{
		return m_sequenceArray[ itrSequence->second ];
	}
}

const std::shared_ptr< Sequence > & Group::GetSequence( unsigned int index ) const
{
	assert( index < m_sequenceArray.size() );
	if( index >= Count() )
	{
		throw unify::Exception( "Sequence index out of bounds!" );
	}

	return m_sequenceArray[ index ];
}

const SequenceName & Group::GetSequenceName( unsigned int index ) const
{
	assert( index < m_sequenceArray.size() );
	if( index >= Count() )
	{
		throw unify::Exception( "Sequence index out of bounds!" );
	}

	SequenceMap::const_iterator itrSequence = m_sequenceMap.begin();
	SequenceMap::const_iterator itrEnd = m_sequenceMap.end();
	for( itrSequence; itrSequence != itrEnd && index != itrSequence->second; ++itrSequence )
	{
		// empty.
	}
	return itrSequence->first;
}

unsigned int Group::GetSequenceIndex( const SequenceName & name ) const
{
	SequenceMap::const_iterator itrSequence = m_sequenceMap.find( name );
	if( itrSequence == m_sequenceMap.end() )
	{
		throw unify::Exception( "Sequence not found!" );
	}
	else
	{
		return itrSequence->second;
	}
}

unsigned int Group::Count() const
{
	return static_cast< unsigned int >( m_sequenceArray.size() );
}
