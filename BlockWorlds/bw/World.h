// Copyright (c) 2002 - 2011, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <memory>
#include <me/render/Geometry.h>

namespace bw
{
	class BlockDescription
	{
	public:
		typedef std::shared_ptr< BlockDescription > ptr;
		BlockDescription( std::string name, me::render::Geometry::ptr geometry );

		std::string GetName();

		me::render::Geometry::ptr GetGeometry();
		
	private:
		std::string m_name;
		me::render::Geometry::ptr m_geometry;
	};

	class BlockLibrary
	{
	public:
		BlockLibrary();
		size_t Add( BlockDescription::ptr blockDescription );

	private:
		std::vector< BlockDescription::ptr > m_library;
	};

	class Block
	{
	public:
		Block();

	private:
	};

	class WorldChunk
	{
	public:
		typedef std::shared_ptr< WorldChunk > ptr;

		WorldChunk( size_t size );

	private:
		size_t m_size;
	};

	class World
	{		  
	public:
		World( size_t chunkSize );

		BlockLibrary & GetBlockLibrary();

	private:
		BlockLibrary m_blockLibrary;
		size_t m_chunkSize;
		unify::V3< float > m_worldOffset;
		std::vector< WorldChunk::ptr > m_activeChunks;
		std::vector< WorldChunk::ptr > m_inactiveChunks; // To be stored.
	};
}

