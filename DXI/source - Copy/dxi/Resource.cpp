// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#include <dxi/Resource.h>
#include <algorithm>

using namespace dxi;

Resource::Resource()
: m_created( false )
, m_valid( false )
{
}

Resource::~Resource()
{
}

bool Resource::Created() const
{
	return m_created;
}

bool Resource::Valid() const
{
	return m_created ? m_valid : false;
}

void Resource::AddOwner( const dxi::ResourceOwner * owner )
{
	if( std::find( m_owners.begin(), m_owners.end(), owner ) == m_owners.end() )
	{
		m_owners.push_back( owner );
	}
}

void Resource::RemoveOwner( const dxi::ResourceOwner * owner )
{
	m_owners.remove( owner );
}