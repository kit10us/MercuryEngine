// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <me/shader/ConstantBuffer.h>
#include <qxml/Element.h>
#include <memory>
#include <string>
#include <vector>
#include <map>
#include <limits>

namespace me
{
	namespace shader
	{			  
		struct Reference {
			Reference() : buffer{ SIZE_MAX }, index{ SIZE_MAX }, offsetInBytes{ SIZE_MAX } {}
			Reference( size_t buffer, size_t index, size_t offsetInBytes ) : buffer{ buffer }, index{ index }, offsetInBytes{ offsetInBytes } {}
			size_t buffer;
			size_t index;
			size_t offsetInBytes;

			bool IsSet() const { return index != SIZE_MAX; }
		};

		class ShaderConstants
		{
		public:
			typedef std::shared_ptr< ShaderConstants > ptr;

			ShaderConstants();
			ShaderConstants( const qxml::Element * node );

			void AddBuffer( ConstantBuffer::ptr buffer );

			const std::vector< ConstantBuffer::ptr > & GetBuffers() const;
			Reference FindDefinition( std::string name ) const;
			Reference GetWorld() const;
			Reference GetView() const;
			Reference GetProjection() const;
			bool HasDefaults( size_t buffer ) const;

		private:
			std::vector< ConstantBuffer::ptr > m_buffers;
			std::map< std::string, Reference > m_map;
			Reference m_world;
			Reference m_view;
			Reference m_projection;
			size_t m_hasDefaults;
		};
	}
}