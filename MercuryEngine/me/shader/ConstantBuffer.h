// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#pragma once

#include <qxml/Element.h>
#include <memory>
#include <string>
#include <vector>

namespace me
{
	namespace shader
	{			  
		struct ConstantsType
		{
			enum TYPE {
				Matrix,
				Float1,
				Float2,
				Float3,
				Float4
			};

			static std::string ToString( TYPE value );
			static TYPE FromString( std::string value );
			static size_t SizeOf( TYPE value );
		};

		struct ConstantVariable
		{
			ConstantVariable()
				: hasDefault{ false }
			{
			}

			ConstantVariable( std::string name, ConstantsType::TYPE type, size_t count )
				: name( name )
				, type( type )
				, count( count )
			{
			}

			std::string name;
			ConstantsType::TYPE type;

			std::vector< float > default;
			bool hasDefault;

			size_t count;
			size_t offsetInBytes;
		};

		class ConstantBuffer
		{
		public:
			typedef std::shared_ptr< ConstantBuffer > ptr;

			ConstantBuffer( std::string name = std::string() );
			ConstantBuffer( const qxml::Element * node );

			/// <summary>
			/// Add a variable.
			/// </summary>
			/// <returns>
			/// Index into our variable table.
			/// </returns>
			size_t AddVariable( ConstantVariable variable );

			std::string GetName() const;

			size_t GetSizeInBytes() const;

			const std::vector< ConstantVariable > & GetVariables() const;

		private:
			std::string m_name;
			size_t m_sizeInBytes;
			std::vector< ConstantVariable > m_variables;
		};
	}
}