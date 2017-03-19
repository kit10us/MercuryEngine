// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#include <bw/World.h>

using namespace me;
using namespace bw;

BlockDescription::BlockDescription( std::string name, Geometry::ptr geometry )
	: m_name( name )
	, m_geometry( geometry )
{
}

std::string BlockDescription::GetName()
{
	return m_name;
}

Geometry::ptr BlockDescription::GetGeometry()
{
	return m_geometry;
}

BlockLibrary::BlockLibrary()
	: m_library( 1000 )
{
}

size_t BlockLibrary::Add( BlockDescription::ptr blockDescription )
{
	m_library.push_back( blockDescription );
	return m_library.size();
}

Block::Block()
{
}

WorldChunk::WorldChunk( size_t size )
{
}



World::World( size_t chunkSize )
	: m_chunkSize{ chunkSize }
	, m_worldOffset{ 0.0f, 0.0f, 0.0f }
{
}

BlockLibrary & World::GetBlockLibrary()
{
	return m_blockLibrary;
}

