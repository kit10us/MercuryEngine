// Copyright (c) 2002 - 2018, Kit10 Studios LLC
// All Rights Reserved

#pragma once

#include <me/render/ConstantVariable.h>
#include <qxml/Element.h>
#include <memory>
#include <map>

namespace me
{
	namespace render
	{			  
		struct Reference {
			Reference() : buffer{ SIZE_MAX }, index{ SIZE_MAX }, offsetInBytes{ SIZE_MAX } {}
			Reference( size_t buffer, size_t index, size_t offsetInBytes ) : buffer{ buffer }, index{ index }, offsetInBytes{ offsetInBytes } {}
			size_t buffer;
			size_t index;
			size_t offsetInBytes;

			bool IsSet() const { return index != SIZE_MAX; }
		};

		class ConstantTable
		{
		public:
			typedef std::shared_ptr< ConstantTable > ptr;

			ConstantTable();
			ConstantTable( const qxml::Element * node );

			/// <summary>
			/// Add a buffer.
			/// </summary>
			void AddBuffer( std::string name = "" );

			size_t BufferCount() const;

			/// <summary>
			/// Add a variable. 
			/// Returns the index into our variable table.
			/// </summary>
			size_t AddVariable( size_t bufferIndex, ConstantVariable variable );

			size_t GetSizeInBytes( size_t bufferIndex ) const;

			const std::vector< ConstantVariable > & GetVariables( size_t bufferIndex ) const;

			const ConstantVariable & GetVariable( const Reference & reference ) const;

			Reference FindDefinition( std::string name ) const;

			// TODO: Isolate this, perhaps to the shaders.

			Reference GetWorld() const;
			Reference GetView() const;
			Reference GetProjection() const;
			bool HasDefaults( size_t buffer ) const;

		private:
			std::vector< size_t > m_sizeInBytes;
			std::vector< std::string > m_name;
			std::vector< std::vector< ConstantVariable > > m_variables;
			std::map< std::string, Reference > m_map;
			Reference m_world;
			Reference m_view;
			Reference m_projection;
			size_t m_hasDefaults;
		};
	}
}