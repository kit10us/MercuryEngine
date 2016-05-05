// Copyright (c) 2002 - 2014, Quentin S. Smith
// All Rights Reserved

#include <unify/FrameSet.h>
#include <unify/Exception.h>
#include <stack>
#include <cassert>

using namespace unify;

FrameSet::FrameSet()
{
}

FrameSet::~FrameSet()
{
}

size_t FrameSet::Add( const Matrix & model, size_t parent, const std::string & name )
{
	// Assert valid connectivity.
	size_t index = m_connectivity.size();
	assert( parent <= index );

	m_models.push_back( model );
	m_connectivity.push_back( parent );
	if( ! name.empty() )
	{
		m_nameToIndex[ name ] = index;
		m_indexToName[ index ] = name;
	}

	// Build local matrix.
	if( parent == index ) // No parent.
	{
		m_locals.push_back( model );
	}
	else
	{
		m_locals.push_back( m_locals[ parent ] * model );
	}

	return index;
}

bool FrameSet::Exists( const std::string & name ) const
{
	std::map< std::string, size_t >::const_iterator itr = m_nameToIndex.find( name );
	bool found( itr == m_nameToIndex.end() );
	return found;
}

size_t FrameSet::Find( const std::string & name ) const
{
	std::map< std::string, size_t >::const_iterator itr = m_nameToIndex.find( name );
	if( itr == m_nameToIndex.end() )
	{
		throw unify::Exception( "Frame not found! (" + name + ")" );
	}
	else
	{
		return itr->second;
	}
}

size_t FrameSet::Count() const
{
	return m_connectivity.size();
}

std::string FrameSet::Name( size_t index ) const
{
	std::map< size_t, std::string >::const_iterator itr = m_indexToName.find( index );
	if( itr == m_indexToName.end() )
	{
		return "";
	}

	return itr->second;
}

const Matrix & FrameSet::Local( size_t index ) const
{
	assert( index < Count() );
	return m_locals[ index ];
}

const Matrix & FrameSet::Model( size_t index ) const
{
	assert( index < Count() );
	return m_models[ index ];
}

void FrameSet::ForEach( ForEachFunctor & functor ) const
{
	std::stack< size_t > depth;
	for( size_t index = 0, end = m_connectivity.size(); index != end; ++index )
	{
		// If our parent isn't immediately before us, then we need to regress.
		if( ! depth.empty() && m_connectivity[ index ] != index - 1 )
		{
			// Reduce our depth until our parent is on the top of the stack or we hit the bottom.
			while( ! depth.empty() && depth.top() != m_connectivity[ index ] )
			{
				depth.pop();
			}
		}

		functor( depth.size(), index );
		depth.push( index );
	}
}



FrameSetInstance::FrameSetInstance( const FrameSet & frameSet )
: m_frameSet( frameSet )
, m_models( frameSet.m_models )
, m_locals( frameSet.m_locals )
, m_dirty( false )
{
}

FrameSetInstance::FrameSetInstance( const FrameSetInstance & frameSetInstance )
: m_frameSet( frameSetInstance.m_frameSet )
, m_models( frameSetInstance.m_models )
, m_locals( frameSetInstance.m_locals )
, m_dirty( frameSetInstance.m_dirty )
{
}

FrameSetInstance::~FrameSetInstance()
{
}

FrameSetInstance & FrameSetInstance::operator=( const FrameSetInstance & frameSetInstance )
{
	m_models = frameSetInstance.m_models;
	m_locals = frameSetInstance.m_locals;
	m_dirty = frameSetInstance.m_dirty;
	return *this;
}

size_t FrameSetInstance::Count() const
{
	return m_frameSet.Count();
}

void FrameSetInstance::Reset()
{
	m_models = m_frameSet.m_models;
	m_locals = m_frameSet.m_locals;
	m_dirty = false;
}

bool FrameSetInstance::Exists( const std::string & name ) const
{
	return m_frameSet.Exists( name );
}

size_t FrameSetInstance::Find( const std::string & name ) const
{
	return m_frameSet.Find( name );
}

const Matrix & FrameSetInstance::Model( size_t index ) const
{
	assert( index < Count() );
	return m_models[ index ];
}

const Matrix & FrameSetInstance::Local( size_t index ) const
{
	assert( index < Count() );
	assert( ! m_dirty ); // Typically it is a defect to get a dirty local.
	return m_locals[ index ];
}

const Matrix & FrameSetInstance::OriginalModel( size_t index ) const
{
	return m_frameSet.Model( index );
}

const Matrix & FrameSetInstance::OriginalLocal( size_t index ) const
{
	return m_frameSet.Local( index );
}

void FrameSetInstance::Transform( size_t index, const Matrix & transform )
{
	assert( index < Count() );
	m_models[ index ] *= transform;
	m_dirty = true;
}

void FrameSetInstance::UpdateLocals()
{
	if( m_dirty )
	{
		for( size_t i = 0, end = Count(); i != end; ++i )
		{
			const size_t & parentIndex = m_frameSet.m_connectivity[ i ];
			if( i == parentIndex )
			{
				m_locals[ i ] = m_models[ i ];
			}
			else
			{
				const Matrix & parentLocal = m_locals[ parentIndex ];
				m_locals[ i ] = parentLocal * m_models[ i ];
			}
		}
		m_dirty = false;
	}
}
