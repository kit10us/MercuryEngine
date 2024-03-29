// Copyright (c) 2002 - 2018, Kit10 Studios LLC
// All Rights Reserved
#pragma once

#include <me/scene/ObjectAllocator.h>

using namespace me;
using namespace scene;

ObjectAllocator::ObjectAllocator( std::string typeName )
	: m_typeName{ typeName }
{
	AddInterface( "IObjectAllocator", this );
	AddInterface( "ObjectAllocator", this );
	AddInterface( typeName, this );
}

std::string ObjectAllocator::GetTypeName() const
{
	return m_typeName;
}

std::string ObjectAllocator::GetWhat() const
{
	return std::string();
}

me::IThing* ObjectAllocator::QueryInterface( std::string name )
{
	auto itr = m_interfaces.find( name );
	if ( itr == m_interfaces.end() )
	{
		return nullptr;
	}

	return itr->second;
}

void ObjectAllocator::AddInterface( std::string name, me::IThing* ptr )
{
	m_interfaces[ name ] = ptr;
}
