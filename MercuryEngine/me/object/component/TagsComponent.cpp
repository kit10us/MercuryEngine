// Copyright (c) 2002 - 2018, Quentin S. Smith
// All Rights Reserved

#include <me/object/component/TagsComponent.h>
#include <me/object/Object.h>

using namespace me;
using namespace object;
using namespace component;
using namespace render;

TagsComponent::TagsComponent( TagsComponent & component )
	: ObjectComponent( component )
	, m_tagValues{ component.m_tagValues }
	, m_tagLookup{ component.m_tagLookup }
{
}

TagsComponent::TagsComponent()
	: ObjectComponent( "Tags", false, false )
{
}

TagsComponent::~TagsComponent()
{
}


void TagsComponent::AddTag( std::string tag, std::string value )
{
	auto itr = m_tagLookup.find( tag );
	if( itr != m_tagLookup.end() )
	{
		m_tagValues[itr->second] = value;
	}
	else
	{
		m_tagLookup[tag] = m_tagValues.size();
		m_tagValues.push_back( value );
	}
}

bool TagsComponent::HasTag( std::string tag ) const
{
	return m_tagLookup.find( tag ) != m_tagLookup.end();
}

std::string TagsComponent::GetTagValue( std::string tag ) const
{
	auto itr = m_tagLookup.find( tag );
	if( itr != m_tagLookup.end() )
	{
		return m_tagValues[ itr->second ];
	}
	else
	{
		return std::string();
	}
}

IObjectComponent::ptr TagsComponent::Duplicate()
{
	auto duplicate = new TagsComponent( *this );
	return IObjectComponent::ptr( duplicate );
}

int TagsComponent::GetValueCount() const
{
	return ObjectComponent::GetValueCount() + m_tagValues.size();
}

bool TagsComponent::ValueExists( std::string name ) const
{
	if( ValueExists( name ) )
	{
		return true;
	}

	return HasTag( name );
}

std::string TagsComponent::GetValueName( int index ) const
{
	if( index < ObjectComponent::GetValueCount() )
	{
		return ObjectComponent::GetValueName( index );
	}

	if( index < GetValueCount() )
	{
		size_t tagIndex = index - ObjectComponent::GetValueCount();
		for( auto itr : m_tagLookup )
		{
			if( itr.second == tagIndex ) return itr.first;
		}
	}

	return std::string();
}

int TagsComponent::FindValueIndex( std::string name ) const
{
	if( ObjectComponent::ValueExists( name ) )
	{
		return ObjectComponent::FindValueIndex( name );
	}
	
	auto itr = m_tagLookup.find( name );
	if( itr != m_tagLookup.end() )
	{
		return ObjectComponent::GetValueCount() + itr->second;
	}

	return -1;
}

bool TagsComponent::SetValue( int index, std::string value )
{
	if( index < ObjectComponent::GetValueCount() )
	{
		return ObjectComponent::SetValue( index, value );
	}
	
	if( index < GetValueCount() )
	{
		size_t tagIndex = index - ObjectComponent::GetValueCount();
		m_tagValues[tagIndex] = value;
		return true;
	}

	return false;
}

std::string TagsComponent::GetValue( int index ) const
{
	if( index < ObjectComponent::GetValueCount() )
	{
		return ObjectComponent::GetValue( index );
	}

	if( index <= GetValueCount() )
	{
		size_t tagIndex = index - ObjectComponent::GetValueCount();
		return m_tagValues[tagIndex];
	}

	return std::string();
}